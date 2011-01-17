
#include <vector>
#include <string>
#include <cstdlib>

#include "math3d/math3d.h"

#include "pbge/gfx/SceneGraph.h"

namespace pbge {
    SceneManager::SceneManager() {
        ambientLight = math3d::vector4(0,0,0,0);
    }

    void SceneManager::setAmbientLightColor(const math3d::vector4 & color) {
        ambientLight = color;
    }

    void SceneManager::setAmbientLightColor(const float & red, const float & green, const float & blue, const float & alpha) {
        ambientLight = math3d::vector4(red, green, blue, alpha);
    }

    const math3d::vector4 SceneManager::getAmbientLightColor() const {
        return ambientLight;
    }
}
