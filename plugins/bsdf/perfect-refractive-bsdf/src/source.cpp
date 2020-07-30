// perfect refractive material

#include <monte-toad/core/accelerationstructure.hpp>
#include <monte-toad/core/any.hpp>
#include <monte-toad/core/geometry.hpp>
#include <monte-toad/core/integratordata.hpp>
#include <monte-toad/core/log.hpp>
#include <monte-toad/core/math.hpp>
#include <monte-toad/core/renderinfo.hpp>
#include <monte-toad/core/scene.hpp>
#include <monte-toad/core/spectrum.hpp>
#include <monte-toad/core/surfaceinfo.hpp>
#include <monte-toad/core/triangle.hpp>
#include <mt-plugin/plugin.hpp>

#include <imgui/imgui.hpp>

namespace {

struct MaterialInfo {
  glm::vec3 albedo;
};

} // -- namespace

extern "C" {

char const * PluginLabel() { return "perfect refractive mtl"; }
mt::PluginType PluginType() { return mt::PluginType::Bsdf; }

void Allocate(mt::core::Any & userdata) {
  if (userdata.data == nullptr) { free(userdata.data); }
  userdata.data = ::malloc(sizeof(MaterialInfo));
  *reinterpret_cast<MaterialInfo*>(userdata.data) = {};
}

glm::vec3 BsdfFs(
  mt::core::Any const & userdata, float const /*indexOfRefraction*/
, mt::core::SurfaceInfo const & /*surface*/
, glm::vec3 const &
) {
  auto & material = *reinterpret_cast<MaterialInfo const *>(userdata.data);
  return material.albedo;
}

float BsdfPdf(
  mt::core::Any const & /*userdata*/, float const /*indexOfRefraction*/
, mt::core::SurfaceInfo const & /*surface*/
, glm::vec3 const & /*wo*/
) {
  return 0.0f; // dirac delta
}

mt::core::BsdfSampleInfo BsdfSample(
  mt::core::Any const & userdata, float const indexOfRefraction
, mt::PluginInfoRandom const & /*random*/
, mt::core::SurfaceInfo const & surface
) {

  glm::vec3 wi = surface.incomingAngle;
  glm::vec3 normal = surface.normal;
  float eta = indexOfRefraction;
  // flip normal if surface is incorrect for refraction
  if (glm::dot(wi, surface.normal) < 0.0f) {
    normal = -surface.normal;
    // eta = 1.0f/eta;
  }

  glm::vec3 const wo = glm::normalize(glm::refract(wi, normal, eta));

  float pdf = 0.0f; // dirac delta
  glm::vec3 fs = BsdfFs(userdata, indexOfRefraction, surface, wo);
  return { wo, fs, pdf };
}

bool IsReflective() { return false; }
bool IsRefractive() { return true; }

bool IsEmitter(
  mt::core::Any const & /*self*/
, mt::core::Triangle const & /*triangle*/
) {
  return false;
}

void UiUpdate(
  mt::core::Any & userdata
, mt::core::RenderInfo & render
, mt::core::Scene & /*scene*/
) {
  auto & material = *reinterpret_cast<::MaterialInfo*>(userdata.data);

  ImGui::Separator();

  ImGui::Text("albedo");

  if (ImGui::ColorPicker3("##albedo", &material.albedo.x)) {
    render.ClearImageBuffers();
  }
}

} // -- end extern "C"