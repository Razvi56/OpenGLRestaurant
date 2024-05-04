//
// Created by Razvan on 1/28/2024.
//

#include "DirLight.h"

namespace gps{
    DirLight::DirLight(const glm::vec3 &direction, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
             const gps::Shader &solidObjectsShader)
            : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular),
              solidObjectsShader(solidObjectsShader) {
        this->solidObjectsShader.useShaderProgram();
        directionLoc = glGetUniformLocation(this->solidObjectsShader.shaderProgram, "globalIllumination.directionEye");
        ambientLoc = glGetUniformLocation(this->solidObjectsShader.shaderProgram, "globalIllumination.ambient");
        diffuseLoc = glGetUniformLocation(this->solidObjectsShader.shaderProgram, "globalIllumination.diffuse");
        specularLoc = glGetUniformLocation(this->solidObjectsShader.shaderProgram, "globalIllumination.specular");
    }

    const glm::vec3 &DirLight::getDirection() const {
        return direction;
    }

    void DirLight::setDirection(const glm::vec3 &direction) {
        DirLight::direction = direction;
    }

    const glm::vec3 &DirLight::getAmbient() const {
        return ambient;
    }

    void DirLight::setAmbient(const glm::vec3 &ambient) {
        DirLight::ambient = ambient;
    }

    const glm::vec3 &DirLight::getDiffuse() const {
        return diffuse;
    }

    void DirLight::setDiffuse(const glm::vec3 &diffuse) {
        DirLight::diffuse = diffuse;
    }

    const glm::vec3 &DirLight::getSpecular() const {
        return specular;
    }

    void DirLight::setSpecular(const glm::vec3 &specular) {
        DirLight::specular = specular;
    }

    const Shader &DirLight::getSolidObjectsShader() const {
        return solidObjectsShader;
    }

    void DirLight::setSolidObjectsShader(const Shader &solidObjectsShader) {
        DirLight::solidObjectsShader = solidObjectsShader;
    }

    GLuint DirLight::getDirectionLoc() const {
        return directionLoc;
    }

    void DirLight::setDirectionLoc(GLuint directionLoc) {
        DirLight::directionLoc = directionLoc;
    }

    GLuint DirLight::getAmbientLoc() const {
        return ambientLoc;
    }

    void DirLight::setAmbientLoc(GLuint ambientLoc) {
        DirLight::ambientLoc = ambientLoc;
    }

    GLuint DirLight::getDiffuseLoc() const {
        return diffuseLoc;
    }

    void DirLight::setDiffuseLoc(GLuint diffuseLoc) {
        DirLight::diffuseLoc = diffuseLoc;
    }

    GLuint DirLight::getSpecularLoc() const {
        return specularLoc;
    }

    void DirLight::setSpecularLoc(GLuint specularLoc) {
        DirLight::specularLoc = specularLoc;
    }
}