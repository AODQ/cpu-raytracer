#pragma once

#include <glm/glm.hpp>

#include <string>
#include <vector>

struct Triangle {
  // TODO I wish i could just do Triangle() = default, but it does not construct
  // properly for some reason
  Triangle(
    glm::vec3 a_, glm::vec3 b_, glm::vec3 c_,
    glm::vec3 n0_, glm::vec3 n1_, glm::vec3 n2_
  ) : v0(a_), v1(b_), v2(c_), n0(n0_), n1(n1_), n2(n2_)
  {}

  ~Triangle() {}

  glm::vec3 v0, v1, v2;
  glm::vec3 n0, n1, n2;
};

struct Scene {
  Scene() = default;

  static Scene Construct(std::string const & filename);

  std::vector<Triangle> scene;

  glm::vec3 bboxMin, bboxMax;
};

Triangle const * Raycast(
  Scene const & model
, glm::vec3 ori
, glm::vec3 dir
, float & distance
, glm::vec2 & uv
);