#include <cr/cr.h>

#include <monte-toad/scene.hpp>
#include <monte-toad/log.hpp>

#include <mt-plugin/plugin.hpp>

namespace {
glm::vec3 LookAt(glm::vec3 dir, glm::vec2 uv, glm::vec3 up, float fovRadians) {
  glm::vec3
    ww = glm::normalize(dir),
    uu = glm::normalize(glm::cross(ww, up)),
    vv = glm::normalize(glm::cross(uu, ww));
  return
    glm::normalize(
      glm::mat3(uu, vv, ww)
    * glm::vec3(uv.x, uv.y, fovRadians)
    );
}

std::tuple<glm::vec3 /*ori*/, glm::vec3 /*dir*/> Dispatch(
  mt::PluginInfoRandom const & random
, mt::RenderInfo const & renderInfo
, glm::vec2 const & uv
) {
  std::tuple<glm::vec3, glm::vec3> results;
  std::get<0>(results) = renderInfo.cameraOrigin;
  std::get<1>(results) =
    ::LookAt(
      renderInfo.cameraTarget - renderInfo.cameraOrigin
    , uv
    , renderInfo.cameraUpAxis
    , renderInfo.cameraFieldOfView
    );
  return results;
}
}

CR_EXPORT int cr_main(struct cr_plugin * ctx, enum cr_op operation) {
  // return immediately if an update, this won't do anything
  if (operation == CR_STEP) { return 0; }

  if (!ctx) {
    spdlog::error("error loading depth integrator plugin, no context\n");
    return -1;
  }
  if (!ctx->userdata) {
    spdlog::error("error loading depth integrator plugin, no userdata\n");
    return -1;
  }

  auto & camera = *reinterpret_cast<mt::PluginInfoCamera*>(ctx->userdata);

  switch (operation) {
    case CR_LOAD:
      camera.Dispatch = &::Dispatch;
    break;
    case CR_UNLOAD: break;
    case CR_STEP: break;
    case CR_CLOSE: break;
  }

  spdlog::info("depth integrator plugin successfully loaded");

  return 0;
}