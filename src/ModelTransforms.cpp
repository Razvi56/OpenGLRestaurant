//
// Created by Razvan on 1/28/2024.
//

#include "ModelTransforms.h"

namespace gps{
    ModelTransforms::ModelTransforms(const glm::vec3 &translation, const glm::vec3 &rotation, const glm::vec3 &scaling) : translation(
            translation), rotation(rotation), scaling(scaling) {}

    const glm::vec3 &ModelTransforms::getTranslation() const {
        return translation;
    }

    void ModelTransforms::setTranslation(const glm::vec3 &translation) {
        ModelTransforms::translation = translation;
    }

    const glm::vec3 &ModelTransforms::getRotation() const {
        return rotation;
    }

    void ModelTransforms::setRotation(const glm::vec3 &rotation) {
        ModelTransforms::rotation = rotation;
    }

    const glm::vec3 &ModelTransforms::getScaling() const {
        return scaling;
    }

    void ModelTransforms::setScaling(const glm::vec3 &scaling) {
        ModelTransforms::scaling = scaling;
    }
}