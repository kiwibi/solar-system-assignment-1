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
   cube(float orbitDistance, float orbitSpeed, float rotationSpeed, glm::vec2 tilt, glm::vec3 pos, float size, const char* texFilePath, neon::shader_program* shader_program);
	virtual bool enter(const char* texFilePath) final;
	virtual void exit() final;
	virtual bool tick(float deltaTime) final;

	neon::shader_program* program_;
	neon::vertex_buffer vbo_;
	neon::vertex_format format_;
	neon::texture texture_;

   cube* orbitPoint_;
   float orbitDistance_;
   float orbit_; // How far around the planetoid has gone around it's axis.
   float orbitSpeed_;
   float rotation_;
   float rotationSpeed_;
   glm::vec2 tilt_;
   glm::vec3 pos_;
   float size_;   // the size of a side of the square
};

#endif //SQUARE_H_INCLUDED