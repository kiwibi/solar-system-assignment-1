//SQUARE_H_INCLUDED
#ifndef SQUARE_H_INCLUDED
#define SQUARE_H_INCLUDED

#include <neon_core.h>
#include "neon_graphics.h"

struct square {
	struct vertex {
		float x_, y_, z_;
		float u_, v_;
	};
	square();
	virtual bool enter() final;
	virtual void exit() final;
	virtual bool tick() final;

	neon::shader_program program_;
	neon::vertex_buffer vbo_;
	neon::vertex_format format_;
	neon::texture texture_;
	neon::sampler_state sampler_;
};

#endif //SQUARE_H_INCLUDED