//
// Created by Razvan on 1/28/2024.
//

#ifndef LAB8_BLINN_PHONG_POINTLIGHT_H
#define LAB8_BLINN_PHONG_POINTLIGHT_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"

namespace gps {
    class PointLight {

    public:
        PointLight(const glm::vec3 &position, float constant, float linear, float quadratic, const glm::vec3 &ambient,
                   const glm::vec3 &diffuse, const glm::vec3 &specular, const gps::Shader &solidObjectsShader);

        PointLight() = default;

        static int getNrOfPointLights();

        static void setNrOfPointLights(int nrOfPointLights);

        const glm::vec3 &getPosition() const;

       void setPosition(const glm::vec3 &position);

        float getConstant() const;

        void setConstant(float constant);

        float getLinear() const;

        void setLinear(float linear);

        float getQuadratic() const;

        void setQuadratic(float quadratic);

        const glm::vec3 &getAmbient() const;

        void setAmbient(const glm::vec3 &ambient);

        const glm::vec3 &getDiffuse() const;

        void setDiffuse(const glm::vec3 &diffuse);

        const glm::vec3 &getSpecular() const;

        void setSpecular(const glm::vec3 &specular);

        const Shader &getSolidObjectsShader() const;

        void setSolidObjectsShader(const Shader &solidObjectsShader);

        GLuint getPositionLoc() const;

        void setPositionLoc(GLuint positionLoc);

        GLuint getConstantLoc() const;

        void setConstantLoc(GLuint constantLoc);

        GLuint getLinearLoc() const;

        void setLinearLoc(GLuint linearLoc);

        GLuint getQuadraticLoc() const;

        void setQuadraticLoc(GLuint quadraticLoc);

        GLuint getAmbientLoc() const;

        void setAmbientLoc(GLuint ambientLoc);

        GLuint getDiffuseLoc() const;

        void setDiffuseLoc(GLuint diffuseLoc);

        GLuint getSpecularLoc() const;

        void setSpecularLoc(GLuint specularLoc);


    private:
        static int nrOfPointLights;

        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        gps::Shader solidObjectsShader;

        GLuint positionLoc;
        GLuint constantLoc;
        GLuint linearLoc;
        GLuint quadraticLoc;
        GLuint ambientLoc;
        GLuint diffuseLoc;
        GLuint specularLoc;
    };
}

#endif //LAB8_BLINN_PHONG_POINTLIGHT_H
