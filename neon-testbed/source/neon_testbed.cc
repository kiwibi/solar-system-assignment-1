// neon_testbed.cc

#include "neon_testbed.h"
#include <cassert>
#include <random>

namespace neon {
   // note: application factory
   application *application::create(int &width, int &height, string &title) {
      width = 1280; height = 1280;
      title = "neon-testbed";
      return new testbed;
   }

   namespace opengl {
	   GLuint create_shader(GLenum type, const char* source) {
		   GLuint id = glCreateShader(type);
		   glShaderSource(id, 1, &source, nullptr);
		   glCompileShader(id);

		   return id;
	   }

	   GLuint create_program(GLuint vid, GLuint fid){
		   GLuint id = glCreateProgram();
		   glAttachShader(id, vid);
		   glAttachShader(id, fid);
		   glLinkProgram(id);

		   GLint status = 0;
		   glGetProgramiv(id, GL_LINK_STATUS, &status);
		   if (status == GL_FALSE) {
			   char vsh_err[1024] = {};
			   char fsh_err[1024] = {};
			   char sh_err[1024] = {};

			   glGetShaderInfoLog(vid, sizeof(vsh_err), nullptr, vsh_err);
			   glGetShaderInfoLog(fid, sizeof(fsh_err), nullptr, fsh_err);
			   glGetProgramInfoLog(id, sizeof(sh_err), nullptr, sh_err);

			   assert(false);

			   return 0;
		   }

		   glDeleteShader(vid);
		   glDeleteShader(fid);

		   return id;
	   }
   }	//	!opengl

   // note: derived application class
   testbed::testbed()
		:	rotation_(0)
   {	  
   }

   bool testbed::enter() {
	   if (!program_.create("assets/vertex_shader.txt",
							"assets/fragment_shader.txt"))
	   {
		   return false;
	   }
      
      if (!sampler_.create(GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE)) {
			return false;
		}

      glm::vec3 pos = { 0.0f, 0.0f, -90.0f };

      srand(0);

     bodies_[0] = new cube(0.0f, 0.0f, 2.0f, glm::vec2(0.2f, 0.8f), pos, 10.0f, "assets/sun.png", &program_); //sun
     float multiplier = 10.0f;
	  bodies_[1] = new cube(17.74f, 4.15f , 0.006f, glm::vec2(0.0f, 1.0f), pos, 0.38f, "assets/mercury.png", &program_); // mercury
	  bodies_[2] = new cube(24.46f, 1.63f , 0.009f, glm::vec2(0.9f, 0.1f), pos, 0.95f, "assets/venus.png", &program_); // venus
	  bodies_[3] = new cube(30.0f, 1.0f  , 1.0f  , glm::vec2(0.7f, 0.2f), pos, 1.0f, "assets/earth.png", &program_); // earth
	  bodies_[4] = new cube(40.4f, 0.53f , 0.971f, glm::vec2(0.7f, 0.2f), pos, 0.53f, "assets/mars.png", &program_); // mars
	  bodies_[5] = new cube(74.0f, 0.08f , 2.42f , glm::vec2(0.1f, 0.9f), pos, 11.11f, "assets/jupiter.png", &program_); // jupiter
	  bodies_[6] = new cube(121.6f, multiplier * 0.03f , 2.25f , glm::vec2(0.7f, 0.2f), pos, 9.45f, "assets/saturn.png", &program_); // saturn
	  bodies_[7] = new cube(144.0f, multiplier * 0.012f, 1.39f , glm::vec2(0.6f, 0.5f), pos, 4.01f, "assets/uranus.png", &program_); // uranus
	  bodies_[8] = new cube(161.0f, multiplier * 0.006f, 1.49f , glm::vec2(0.7f, 0.3f), pos, 3.88f, "assets/neptune.png", &program_); // neptune
	  bodies_[9] = new cube(189.6f, multiplier * 0.004f, 0.16f , glm::vec2(0.8f, 0.2f), pos, 0.19f, "assets/pluto.png", &program_); // pluto

	  bodies_[10] = new cube(2.0514f, 13.37f, 0.03f, glm::vec2(0.7f, 0.2f), pos, 0.27f, "assets/moon.png", &program_);//moon

	  for (int i = 1; i < 10; i++)
	  {
		bodies_[i]->orbitPoint_ = bodies_[0];
	  }
	  bodies_[10]->orbitPoint_ = bodies_[3];

	   //	note: uniforms
		/*glm::mat4 projection_ = glm::perspective(glm::radians(90.0f), 
															  16.0f / 9.0f,
															  0.5f,
															  1000.0f);*/
      glm::mat4 projection_ = glm::orthoRH(-200.0f, 200.0f, 200.0f, -200.0f, 0.1f, 1000.0f);

		program_.bind();
		program_.set_uniform_mat4("projection", glm::mat4(projection_));
	   
	   GLenum error = glGetError();	
	   if (error != GL_NO_ERROR) {
		   //CBT from wikipedia
	   }

      return true;
   }

   void testbed::exit() {
   }

   bool testbed::tick(const time &dt) {
     if (keyboard_.is_pressed(KEYCODE_ESCAPE)) {
        return false;
     }
      
     glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     program_.bind();
     sampler_.bind();

	  glEnable(GL_DEPTH_TEST);

	  glEnable(GL_CULL_FACE);
	  glCullFace(GL_BACK);
	  glFrontFace(GL_CW);

	  for (int i = 0; i < 11; i++)
	  {
		  bodies_[i]->tick(dt.as_milliseconds());
	  }

	  /*glEnable(GL_BLEND);
	  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);*/

      return true;
   }
} // !neon
