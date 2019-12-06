//SQUARE_H_INCLUDED
#ifndef SQUARE_H_INCLUDED
#define SQUARE_H_INCLUDED

#include <neon_core.h>
#include "neon_graphics.h"

struct cube {
	struct vertex {
		float x_, y_, z_;
		float u_, v_;
	};
	cube();
   cube(float orbitDistance, float orbitSpeed, float rotationSpeed, glm::vec2 tilt, float size);
	virtual bool enter() final;
	virtual void exit() final;
	virtual bool tick(int deltaTime) final;

	neon::shader_program program_;
	neon::vertex_buffer vbo_;
	neon::vertex_format format_;
	neon::texture texture_;
	neon::sampler_state sampler_;

   cube* orbitPoint_;
   float orbitDistance_;
   float orbitSpeed_;
   float rotationSpeed_;
   glm::vec2 tilt;
   float size_;   // the size of a side of the square
};

#endif //SQUARE_H_INCLUDED