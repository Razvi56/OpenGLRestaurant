//
// Created by Razvan on 1/28/2024.
//

#ifndef LAB8_BLINN_PHONG_MODELTRANSFORMS_H
#define LAB8_BLINN_PHONG_MODELTRANSFORMS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gps{

    class ModelTransforms {
    public:
        ModelTransforms(const glm::vec3 &translation, const glm::vec3 &rotation, const glm::vec3 &scaling);
        ModelTransforms() = default;

        const glm::vec3 &getTranslation() const;

        void setTranslation(const glm::vec3 &translation);

        const glm::vec3 &getRotation() const;

        void setRotation(const glm::vec3 &rotation);

        const glm::vec3 &getScaling() const;

        void setScaling(const glm::vec3 &scaling);

    private:
        glm::vec3 translation;
        glm::vec3 rotation;
        glm::vec3 scaling;
    };
}

#endif //LAB8_BLINN_PHONG_MODELTRANSFORMS_H
