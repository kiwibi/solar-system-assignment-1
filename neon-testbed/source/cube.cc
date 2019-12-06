// cube.cc

#include "cube.h"

cube::cube(float orbitDistance, float orbitSpeed, float rotationSpeed, glm::vec2 tilt, glm::vec3 pos, float size, const char* texFilePath){
   orbitPoint_ = nullptr;
   orbitDistance_ = orbitDistance;
   orbit_ = 0;
   orbitSpeed_ = orbitSpeed;
   rotationSpeed_ = rotationSpeed;
   rotation_ = 0;
   tilt_ = tilt;
   size_ = size;

   enter(texFilePath);
}

bool cube::enter(const char* texFilePath) {

   vertex vertices[] =
   {
      //side 1, triangle 1
      { -size_,  -size_,   size_,   0.0f,0.0f},
      { -size_,   size_,   size_,   0.0f,1.0f},
      {  size_,   size_,   size_,   1.0f,1.0f},
      //side 1, triangle 2
      { -size_,  -size_,   size_,   0.0f,0.0f},
      {  size_,   size_,   size_,   1.0f,1.0f},
      {  size_,  -size_,   size_,   1.0f,0.0f},


      //side 2, triangle 1
      { -size_,  -size_,  -size_,   0.0f,0.0f},
      { -size_,   size_,  -size_,   1.0f,0.0f},
      { -size_,   size_,   size_,   1.0f,1.0f},
      //side 2, triangle 2
      { -size_,  -size_,  -size_,   0.0f,0.0f},
      { -size_,   size_,   size_,   1.0f,1.0f},
      { -size_,  -size_,   size_,   0.0f,1.0f},


      //side 3, triangle 1
      {  size_,  -size_,  -size_,   1.0f,0.0f},
      {  size_,   size_,  -size_,   1.0f,1.0f},
      { -size_,   size_,  -size_,   0.0f,1.0f},
      //side 3, triangle 2
      {  size_,  -size_,  -size_,   1.0f,0.0f},
      { -size_,   size_,  -size_,   0.0f,1.0f},
      { -size_,  -size_,  -size_,   0.0f,0.0f},


      //side 4, triangle 1
      {  size_,  -size_,   size_,   0.0f,1.0f},
      {  size_,   size_,   size_,   1.0f,1.0f},
      {  size_,   size_,  -size_,   1.0f,1.0f},
      //side 4, triangle 2
      {  size_,  -size_,   size_,   0.0f,1.0f},
      {  size_,   size_,  -size_,   1.0f,0.0f},
      {  size_,  -size_,  -size_,   0.0f,0.0f},


      //side 5, triangle 1
      { -size_,   size_,   size_,   0.0f,1.0f},
      { -size_,   size_,  -size_,   0.0f,0.0f},
      {  size_,   size_,  -size_,   1.0f,0.0f},
      //side 5, triangle 2
      { -size_,   size_,   size_,   0.0f,1.0f},
      {  size_,   size_,  -size_,   1.0f,0.0f},
      {  size_,   size_,   size_,   1.0f,1.0f},


      //side 6, triangle 1
      { -size_,  -size_,  -size_,   0.0f,0.0f},
      { -size_,  -size_,   size_,   0.0f,1.0f},
      {  size_,  -size_,   size_,   1.0f,1.0f},
      //side 6, triangle 2
      { -size_,  -size_,  -size_,   0.0f,0.0f},
      {  size_,  -size_,   size_,   1.0f,1.0f},
      {  size_,  -size_,  -size_,   1.0f,0.0f},
   };

   GLuint vao = 0;	//vertex array object
   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   if (!vbo_.create(sizeof(vertices), vertices)) {
      return false;
   }

   if (!program_.create("assets/vertex_shader.txt",
                        "assets/fragment_shader.txt"))
   {
      return false;
   }

   format_.add_attribute(0, 3, GL_FLOAT, false);

   format_.add_attribute(2, 2, GL_FLOAT, false);

   if (!texture_.create(texFilePath)) {
      return false;
   }

   if (!sampler_.create(GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE)) {
      return false;
   }

   //	note: uniforms
   glm::mat4 world = glm::translate(glm::mat4(1.0f),
                                    glm::vec3(0.0f, 0.0f, -10.0f));
   program_.bind();
   program_.set_uniform_mat4("world", world);

   GLenum error = glGetError();
   if (error != GL_NO_ERROR) {
      //CBT from wikipedia
   }

   return true;
}
void cube::exit() {

}
bool cube::tick(float deltaTime) {
   rotation_ += deltaTime / rotationSpeed_;

   // Position logic
   if (orbitPoint_ != nullptr){
      orbitPoint_->pos_;

   }

   glm::mat4 world = glm::translate(glm::mat4(1.0f),
                                    pos_);
   world = glm::rotate(world,
                       rotation_,
                       glm::vec3(0.5f, 0.7f, 0.0f));

   program_.bind();
   program_.set_uniform_mat4("world", world);
   vbo_.bind();
   format_.bind();
   texture_.bind();
   sampler_.bind();

   glDrawArrays(GL_TRIANGLES, 0, 36);

   return true;
}