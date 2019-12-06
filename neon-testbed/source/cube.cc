// cube.cc

#include "cube.h"

cube::cube(float orbitDistance, float orbitSpeed, float rotationSpeed, glm::vec2 tilt, glm::vec3 pos, float size, const char* texFilePath, neon::shader_program* shader_program){
   orbitPoint_ = nullptr;
   orbitDistance_ = orbitDistance;
   orbit_ = 0;
   orbitSpeed_ = orbitSpeed;
   rotationSpeed_ = rotationSpeed;
   rotation_ = 0;
   tilt_ = tilt;
   size_ = size;
   pos_ = pos;

   program_ = shader_program;
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
      {  size_,   size_,  -size_,   1.0f,0.0f},
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

   format_.add_attribute(0, 3, GL_FLOAT, false);
   format_.add_attribute(2, 2, GL_FLOAT, false);

   if (!texture_.create(texFilePath)) {
      return false;
   }
   glm::mat4 world = glm::translate(glm::mat4(1.0f),
                                    glm::vec3(0.0f, 0.0f, -50.0f));


   program_->set_uniform_mat4("world", world);
   program_->set_uniform_vec4("mod_color", glm::vec4(1, 1, 1, 1));

   GLenum error = glGetError();
   if (error != GL_NO_ERROR) {
      //CBT from wikipedia
   }

   return true;
}
void cube::exit() {

}
bool cube::tick(float deltaTime) {
   rotation_ += deltaTime * rotationSpeed_ / 1000;

   // Position logic
   if (orbitPoint_ != nullptr){
      orbit_ += orbitSpeed_ * deltaTime /1000; // Orbit = radians
      
      pos_.x = orbitPoint_->pos_.x + glm::cos(orbit_) * orbitDistance_;
      pos_.y = orbitPoint_->pos_.y +  glm::sin(orbit_) * orbitDistance_;
   }

   glm::mat4 world = glm::translate(glm::mat4(1.0f),
                                    pos_);
   world = glm::rotate(world,
                       rotation_,
                       glm::vec3(tilt_.x, tilt_.y, 0.0f));

   program_->bind();
   program_->set_uniform_mat4("world", world);
   vbo_.bind();
   format_.bind();
   texture_.bind();

   glDrawArrays(GL_TRIANGLES, 0, 36);

   return true;
}