#pragma once

#include <glm/glm.hpp>

#include <bvh/vector.hpp>

namespace glm {
constexpr static float Pi     = 3.141592653589793f;
constexpr static float InvPi  = 0.318309886183791f;
constexpr static float Tau    = 6.283185307179586f;
constexpr static float InvTau = 0.159154943091895f;
float sqr(float i);
}

#define SQR(X) ((X)*(X))

bvh::Vector3<float> ToBvh(glm::vec3 vertex);
glm::vec3 ToGlm(bvh::Vector3<float> vertex);

template <typename U>
U BarycentricInterpolation(
  U const & v0, U const & v1, U const & v2
, glm::vec2 const & uv
) {
  return v0 + uv.x*(v1 - v0) + uv.y*(v2 - v0);
}
