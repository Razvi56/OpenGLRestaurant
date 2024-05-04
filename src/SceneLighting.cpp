//
// Created by Razvan on 1/28/2024.
//


#include "SceneLighting.h"

namespace gps{
    SceneLighting::SceneLighting(const DirLight &globalIllumination, const PointLight (&pointLights)[NR_OF_POINT_LIGHTS])
            : globalIllumination(
            globalIllumination) {
        for (int i = 0; i < NR_OF_POINT_LIGHTS; i++) {
            this->pointLights[i] = pointLights[i];
        }
    }

    void SceneLighting::updatePointLightPosition(int index, const glm::vec3 &newPosition) {
        pointLights[index].setPosition(newPosition);
    }

    const DirLight &SceneLighting::getGlobalIllumination() const {
        return globalIllumination;
    }

    void SceneLighting::setGlobalIllumination(const DirLight &globalIllumination) {
        SceneLighting::globalIllumination = globalIllumination;
    }

    const PointLight *SceneLighting::getPointLights() const {
        return pointLights;
    }
}