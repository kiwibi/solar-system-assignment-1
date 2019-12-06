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
	   if (!program_.create("assets/vertex_shader.txt",
							"assets/fragment_shader.txt"))
	   {
		   return false;
	   }
      
      if (!sampler_.create(GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE)) {
			return false;
		}

      bodies_[0] = new cube(0.0f, 0.0f, 2.0f, glm::vec2(0.8f, 0.8f), glm::vec3(0.0f, 0.0f, -50.0f), 5.0f, "assets/sun.png", &program_);

	   //	note: uniforms
		glm::mat4 projection_ = glm::perspective(glm::radians(45.0f), 
															  16.0f / 9.0f,
															  0.5f,
															  100.0f);
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


     bodies_[0]->tick(dt.as_milliseconds());

	  /*glEnable(GL_BLEND);
	  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);*/

      return true;
   }
} // !neon
