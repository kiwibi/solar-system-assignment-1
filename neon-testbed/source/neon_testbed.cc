// neon_testbed.cc

#include "neon_testbed.h"
#include <cassert>

namespace neon {
   // note: application factory
   application *application::create(int &width, int &height, string &title) {
      width = 1280; height = 720;
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
	   GLuint vao = 0;	//vertex array object
	   glGenVertexArrays(1, &vao);
	   glBindVertexArray(vao);

		//glEnable(GL_TEXTURE_2D);
	   //	Big triangles
	   vertex vertices[] =
	   {
		   //triangle 1 side 1
		   {  1.0f,  1.0f, 1.0f,			0xff0000ff,		1.0f, 1.0f},
		   {  1.0f, -1.0f, 1.0f,			0xff00ff00,		1.0f, 0.0f},
		   { -1.0f, -1.0f, 1.0f,			0xffff0000,		0.0f, 0.0f},
			//triangle 1 side 1
			{  1.0f,  1.0f, 1.0f,			0xff0000ff,		1.0f, 1.0f},
			{  -1.0f, -1.0f, 1.0f,			0xff00ff00,		0.0f, 0.0f},
			{ -1.0f, 1.0f, 1.0f,				0xffff0000,		0.0f, 1.0f},

		   //triangle 2 side 2
			{  1.0f,  1.0f, 1.0f,			0xff0000ff,		1.0f, 1.0f},
			{  1.0f, -1.0f, -1.0f,			0xffff0000,		0.0f, 0.0f},
			{  1.0f, -1.0f, 1.0f,			0xff00ff00,		0.0f, 1.0f},
			//triangle 2 side 2
			{  1.0f,  1.0f, 1.0f,			0xff0000ff,		1.0f, 1.0f},
			{  1.0f, 1.0f, -1.0f,			0xff00ff00,		1.0f, 0.0f},
			{ 1.0f, -1.0f, -1.0f,			0xffff0000,		0.0f, 0.0f},

			//triangle 3 side 2
			{  1.0f,  1.0f, -1.0f,			0xff0000ff,		1.0f, 1.0f},
			{  -1.0f, -1.0f, -1.0f,			0xffff0000,		0.0f, 0.0f},
			{  1.0f, -1.0f, -1.0f,			0xff00ff00,		1.0f, 0.0f},
			//triangle 3 side 2
			{  1.0f,  1.0f, -1.0f,			0xff0000ff,		1.0f, 1.0f},
			{  -1.0f, 1.0f, -1.0f,			0xff00ff00,		0.0f, 1.0f},
			{ -1.0f, -1.0f, -1.0f,			0xffff0000,		0.0f, 0.0f},

			//triangle 4 side 2
			{  -1.0f,  1.0f, -1.0f,			0xff0000ff,		1.0f, 0.0f},
			{  -1.0f, -1.0f, 1.0f,			0xffff0000,		0.0f, 1.0f},
			{  -1.0f, -1.0f, -1.0f,			0xff00ff00,		0.0f, 0.0f},
			//triangle 4 side 2
			{  -1.0f,  1.0f, -1.0f,			0xff0000ff,		1.0f, 0.0f},
			{  -1.0f, 1.0f, 1.0f,			0xff00ff00,		1.0f, 1.0f},
			{ -1.0f, -1.0f, 1.0f,			0xffff0000,		0.0f, 1.0f},

			// Triangle 1, side 5
			{ -1.0f ,  1.0f,  1.0f,    0xff0000ff,     0.0f, 1.0f  },
			{  1.0f ,  1.0f, -1.0f,    0xffff0000,     1.0f, 0.0f  },
			{  1.0f ,  1.0f,  1.0f,    0xff00ff00,     1.0f, 1.0f  },
			// Triangle 2, side 5      
			{ -1.0f ,  1.0f, -1.0f,    0xff4b3319,     0.0f, 0.0f  },
			{  1.0f ,  1.0f, -1.0f,    0xff4b3319,     1.0f, 0.0f  },
			{ -1.0f ,  1.0f,  1.0f,    0xff4b3319,     0.0f, 1.0f  },

			// Triangle 1, side 6
			{ -1.0f , -1.0f,  1.0f,    0xff0000ff,     0.0f, 1.0f  },
			{  1.0f , -1.0f,  1.0f,    0xff00ff00,     1.0f, 1.0f  },
			{  1.0f , -1.0f, -1.0f,    0xffff0000,     1.0f, 0.0f  },
			// Triangle 2, side 6      
			{ -1.0f , -1.0f, -1.0f,    0xff4b3319,     0.0f, 0.0f  },
			{ -1.0f , -1.0f,  1.0f,    0xff4b3319,     0.0f, 1.0f  },
			{  1.0f , -1.0f, -1.0f,    0xff4b3319,     1.0f, 0.0f  },



	   };

	   if (!vbo_.create(sizeof(vertices), vertices)) {
		   return false;
	   }

	   if (!program_.create("assets/vertex_shader.txt",
							"assets/fragment_shader.txt"))
	   {
		   return false;
	   }
	   
	   format_.add_attribute(0, 3, GL_FLOAT, false);
	   format_.add_attribute(1, 4, GL_UNSIGNED_BYTE, true);
		format_.add_attribute(2, 2, GL_FLOAT, false);

		if (!texture_.create("assets/bumblebee.jpg")) {
			return false;
		}

		if (!sampler_.create(GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE)) {
			return false;
		}

	   //	note: uniforms
		glm::mat4 projection_ = glm::perspective(glm::radians(45.0f), 
															  16.0f / 9.0f,
															  0.5f,
															  100.0f);

		glm::mat4 world = glm::translate(glm::mat4(1.0f),
													glm::vec3(0.0f, 0.0f, -10.0f));
		program_.bind();
		program_.set_uniform_mat4("projection", glm::mat4(projection_));
		program_.set_uniform_mat4("world", world);
	   program_.set_uniform_vec4("mod_color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	   
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

		rotation_ += dt.as_seconds() / 2;
		glm::mat4 world = glm::translate(glm::mat4(1.0f),
													glm::vec3(0.0f, 0.0f, -10.0f));
		world = glm::rotate(world,
								  rotation_,
								  glm::vec3(0.5f, 0.7f, 0.0f));

      glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  program_.bind();
	  program_.set_uniform_mat4("world", world);
	  vbo_.bind();
	  format_.bind();
	  texture_.bind();
	  sampler_.bind();
	  glEnable(GL_DEPTH_TEST);

	  glEnable(GL_CULL_FACE);
	  glCullFace(GL_BACK);
	  glFrontFace(GL_CW);

	  /*glEnable(GL_BLEND);
	  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);*/
	  glDrawArrays(GL_TRIANGLES, 0, 99);

      return true;
   }
} // !neon
