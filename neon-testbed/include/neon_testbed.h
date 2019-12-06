// neon_testbed.h

#ifndef NEON_TESTBED_H_INCLUDED
#define NEON_TESTBED_H_INCLUDED

#include <neon_core.h>
#include <neon_opengl.h>

#include "neon_graphics.h"
#include "cube.h"

namespace neon {
	struct vertex {
		float x_, y_,z_;
		uint32 color_;
		float u_, v_;
	};
   struct testbed : application {
      testbed();
      virtual bool enter() final;
      virtual void exit() final;
      virtual bool tick(const time &dt) final;

	  shader_program program_;
	  vertex_buffer vbo_;
	  vertex_format format_;
	  texture texture_;
	  sampler_state sampler_;

	  float rotation_;

     cube * bodies_[11];
   };
} // !neon

#endif // !NEON_TESTBED_H_INCLUDED
