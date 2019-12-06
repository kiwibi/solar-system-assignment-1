// cube.cc

#include "cube.h"

cube::cube() {
   orbitDistance_ = 0;
   orbitSpeed_ = 0;
   rotationSpeed_ = 0;
   tilt_ = {0, 0};
   size_ = 0;
}

cube::cube(float orbitDistance, float orbitSpeed, float rotationSpeed, glm::vec2 tilt, float size){
   orbitDistance_ = orbitDistance;
   orbitSpeed_ = orbitSpeed;
   rotationSpeed_ = rotationSpeed;
   tilt_ = tilt;
   size_ = size;
}

bool cube::enter() {
   return true;
}
void cube::exit() {

}
bool cube::tick(int deltaTime) {
   return true;
}