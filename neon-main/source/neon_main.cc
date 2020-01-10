// neon_main.cc

#include <neon_core.h>

#include <Windows.h>
#include <windowsx.h>

struct input_state {
   int x_;
   int y_;
   int delta_;
   struct {
      bool current_;
      bool previous_;
   } buttons_[neon::mouse_button::MOUSE_BUTTON_COUNT]{};
   struct {
      bool current_;
      bool previous_;
   } keys_[neon::keycode::KEYCODE_COUNT]{};
};

static WPARAM
win32_convert_extended(WPARAM wParam, LPARAM lParam) {
   WPARAM result = wParam;
   UINT scancode = (lParam & 0x00ff0000) >> 16;
   bool extended = (lParam & 0x01000000) != 0;

   switch (wParam) {
      case VK_SHIFT:
         result = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
         break;
      case VK_CONTROL:
         result = extended ? VK_RCONTROL : VK_LCONTROL;
         break;
      case VK_MENU:
         result = extended ? VK_RMENU : VK_LMENU;
         break;
   }

   return result;
}

static LRESULT CALLBACK
win32_main_proc(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
   input_state *is = (input_state *)GetWindowLongPtrA(window, GWLP_USERDATA);
   if (!is) { return DefWindowProcA(window, message, wParam, lParam); }

   switch (message) {
      case WM_MOUSEMOVE:
      {
         is->x_ = GET_X_LPARAM(lParam);
         is->y_ = GET_Y_LPARAM(lParam);
      } break;

      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      {
         is->buttons_[neon::MOUSE_BUTTON_LEFT].current_ = message == WM_LBUTTONDOWN;
      } break;

      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      {
         is->buttons_[neon::MOUSE_BUTTON_RIGHT].current_ = message == WM_RBUTTONDOWN;
      } break;

      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      {
         is->buttons_[neon::MOUSE_BUTTON_MIDDLE].current_ = message == WM_MBUTTONDOWN;
      } break;

      case WM_MOUSEWHEEL:
      {
         is->delta_ = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
      } break;

      case WM_KEYDOWN:
      case WM_KEYUP:
      {
         wParam = win32_convert_extended(wParam, lParam);
         int index = (int)wParam;
         bool down = message == WM_KEYDOWN;
         if (index <= 0xff) {
            is->keys_[index].current_ = down;
         }
      } break;

      case WM_CLOSE:
      {
         PostQuitMessage(0);
      } break;

      default:
      {
         return DefWindowProcA(window, message, wParam, lParam);
      } break;
   }

   return 0;
}

static void
input_state_process(input_state &is, neon::keyboard &kb) {
   for (int index = 0; index < neon::keycode::KEYCODE_COUNT; index++) {
      kb.keys_[index].pressed_ = false;
      kb.keys_[index].released_ = false;

      if (is.keys_[index].current_ != is.keys_[index].previous_) {
         if (is.keys_[index].current_) {
            kb.keys_[index].down_ = true;
            kb.keys_[index].pressed_ = true;
         }
         else {
            kb.keys_[index].down_ = false;
            kb.keys_[index].released_ = true;
         }

         is.keys_[index].previous_ = is.keys_[index].current_;
      }
   }
}

static void
input_state_process(input_state &is, neon::mouse &mb) {
   mb.x_ = is.x_;
   mb.y_ = is.y_;

   mb.delta_ = is.delta_;
   is.delta_ = 0;

   for (int index = 0; index < neon::mouse_button::MOUSE_BUTTON_COUNT; index++) {
      mb.buttons_[index].pressed_ = false;
      mb.buttons_[index].released_ = false;

      if (is.buttons_[index].current_ != is.buttons_[index].previous_) {
         if (is.buttons_[index].current_) {
            mb.buttons_[index].down_ = true;
            mb.buttons_[index].pressed_ = true;
         }
         else {
            mb.buttons_[index].down_ = false;
            mb.buttons_[index].released_ = true;
         }

         is.buttons_[index].previous_ = is.buttons_[index].current_;
      }
   }
}

static bool
win32_register_class(const char *ident) {
   WNDCLASSA wc = {};
   wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
   wc.lpfnWndProc = win32_main_proc;
   wc.hInstance = GetModuleHandle(NULL);
   wc.lpszClassName = ident;
   wc.hCursor = LoadCursor(NULL, IDC_ARROW);
   wc.hbrBackground = CreateSolidBrush(0x00000000);
   //wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(101));
   if (!RegisterClassA(&wc)) {
      return false;
   }

   return true;
}

static HWND
win32_create_window(int width, int height, const char *ident) {
   DWORD ws = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);
   RECT wr = { 0, 0, width, height };
   if (!AdjustWindowRect(&wr, ws, 0)) {
      return nullptr;
   }

   int x = CW_USEDEFAULT;
   int y = CW_USEDEFAULT;
   HWND window = CreateWindowA(ident,
                               "rainbow",
                               ws,
                               x,
                               y,
                               wr.right - wr.left,
                               wr.bottom - wr.top,
                               0, 0,
                               GetModuleHandle(NULL),
                               0);
   return window;
}

static HGLRC
win32_create_opengl_context(HWND window) {
   PIXELFORMATDESCRIPTOR pfd = {};
   pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
   pfd.nVersion = 1;
   pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
   pfd.iPixelType = PFD_TYPE_RGBA;
   pfd.cColorBits = 32;
   pfd.iLayerType = PFD_MAIN_PLANE;

   HDC device = GetDC(window);
   int pixel_format_index = ChoosePixelFormat(device, &pfd);
   SetPixelFormat(device, pixel_format_index, &pfd);

   HGLRC context = wglCreateContext(device);
   if (!context) {
      return nullptr;
   }

   if (!wglMakeCurrent(device, context)) {
      wglDeleteContext(context);
      return nullptr;
   }

   return context;
}

// note: simplify resizing and setting title of window
static HWND g_window = nullptr;
HWND win32_get_window_handle() {
   return g_window;
}

static void
win32_resize_window(int width, int height) {
   HWND window = g_window;
   RECT window_rect = { 0 };
   GetWindowRect(window, &window_rect);

   int window_x = window_rect.left;
   int window_y = window_rect.top;

   window_rect = {};
   window_rect.right = width;
   window_rect.bottom = height;
   DWORD style = GetWindowStyle(window);
   AdjustWindowRect(&window_rect, style, 0);

   int window_width = window_rect.right - window_rect.left;
   int window_height = window_rect.bottom - window_rect.top;
   SetWindowPos(window,
                NULL,
                window_x,
                window_y,
                window_width,
                window_height,
                SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
}

static void
win32_set_title(const char *title) {
   SetWindowTextA(g_window, title);
}

extern bool win32_create_core_opengl_context(void *window_handle, int major, int minor);

int WINAPI WinMain(_In_ HINSTANCE instance, 
                   _In_opt_ HINSTANCE prev_instance, 
                   _In_ LPSTR cmd_line, 
                   _In_ int cmd_show) 
{
   const int width = 1280;
   const int height = 1280;
   neon::string title = "neon";

   const char *ident = "neonTestClassName";
   win32_register_class(ident);
   HWND window = win32_create_window(width, height, ident);
   if (!window) {
      return -1;
   }

   g_window = window;
   HDC device = GetDC(window);
   /*HGLRC context = win32_create_opengl_context(window);
   if (!context) {
      return -1;
   }*/
   
   if (!win32_create_core_opengl_context(window, 3, 3)) {
      return -1;
   }

   input_state is = {};
   SetWindowLongPtrA(window, GWLP_USERDATA, (LONG_PTR)&is);

   //if (!opengl_load_extensions()) {
   //   return -1;
   //}
   //opengl_default_settings();
   //opengl_projection(width, height);
   ShowWindow(window, SW_NORMAL);

   int req_width = width;
   int req_height = height;
   auto app = neon::application::create(req_width, req_height, title);
   if (!app->init()) {
      return -1;
   }

   win32_set_title(title.c_str());
   if (req_width != width || req_height != height) {
      win32_resize_window(req_width, req_height);
   }

   bool running = true;
   while (running) {
      MSG msg = {};
      while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
         if (msg.message == WM_QUIT) {
            running = false;
         }

         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }

      input_state_process(is, app->keyboard_);
      input_state_process(is, app->mouse_);

      // todo: frame limit?
      if (!app->frame()) {
         running = false;
      }

      SwapBuffers(device);
      Sleep(16);
   }

   app->exit();
   delete app;

   return 0;
}
