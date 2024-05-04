//
// Created by Razvan on 1/28/2024.
//

#ifndef LAB8_BLINN_PHONG_SCENELIGHTING_H
#define LAB8_BLINN_PHONG_SCENELIGHTING_H

#include "Constants.h"
#include "DirLight.h"
#include "PointLight.h"

namespace gps{
    class SceneLighting {

    public:
        SceneLighting(const DirLight &globalIllumination, const PointLight (&pointLights)[NR_OF_POINT_LIGHTS]);

        const DirLight &getGlobalIllumination() const;

        void setGlobalIllumination(const DirLight &globalIllumination);

        const PointLight *getPointLights() const;

        void updatePointLightPosition(int index, const glm::vec3 &newPosition);

    private:
        DirLight globalIllumination;
        PointLight pointLights[NR_OF_POINT_LIGHTS];
    };
}

#endif //LAB8_BLINN_PHONG_SCENELIGHTING_H
