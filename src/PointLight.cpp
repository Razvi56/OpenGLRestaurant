//
// Created by Razvan on 1/28/2024.
//

#include "PointLight.h"

namespace gps{
    int PointLight::nrOfPointLights = 0;

    PointLight::PointLight(const glm::vec3 &position, float constant, float linear, float quadratic, const glm::vec3 &ambient,
               const glm::vec3 &diffuse, const glm::vec3 &specular, const gps::Shader &solidObjectsShader) : position(
            position), constant(constant),
                                                                                                             linear(linear),
                                                                                                             quadratic(
                                                                                                                     quadratic),
                                                                                                             ambient(ambient),
                                                                                                             diffuse(diffuse),
                                                                                                             specular(
                                                                                                                     specular),
                                                                                                             solidObjectsShader(
                                                                                                                     solidObjectsShader) {
        this->solidObjectsShader.useShaderProgram();
        ambientLoc = glGetUniformLocation(solidObjectsShader.shaderProgram,
                                          ("pointLights[" + std::to_string(nrOfPointLights) + "].ambient").c_str());
        diffuseLoc = glGetUniformLocation(solidObjectsShader.shaderProgram,
                                          ("pointLights[" + std::to_string(nrOfPointLights) + "].diffuse").c_str());
        specularLoc = glGetUniformLocation(solidObjectsShader.shaderProgram,
                                           ("pointLights[" + std::to_string(nrOfPointLights) + "].specular").c_str());
        constantLoc = glGetUniformLocation(solidObjectsShader.shaderProgram,
                                           ("pointLights[" + std::to_string(nrOfPointLights) + "].constant").c_str());
        linearLoc = glGetUniformLocation(solidObjectsShader.shaderProgram,
                                         ("pointLights[" + std::to_string(nrOfPointLights) + "].linear").c_str());
        quadraticLoc = glGetUniformLocation(solidObjectsShader.shaderProgram,
                                            ("pointLights[" + std::to_string(nrOfPointLights) + "].quadratic").c_str());
        positionLoc = glGetUniformLocation(solidObjectsShader.shaderProgram,
                                           ("pointLights[" + std::to_string(nrOfPointLights) + "].position").c_str());
        ++nrOfPointLights;
    }

    int PointLight::getNrOfPointLights() {
        return nrOfPointLights;
    }

    void PointLight::setNrOfPointLights(int nrOfPointLights) {
        PointLight::nrOfPointLights = nrOfPointLights;
    }

    const glm::vec3 &PointLight::getPosition() const {
        return position;
    }

    void PointLight::setPosition(const glm::vec3 &position) {
        PointLight::position = position;
    }

    float PointLight::getConstant() const {
        return constant;
    }

    void PointLight::setConstant(float constant) {
        PointLight::constant = constant;
    }

    float PointLight::getLinear() const {
        return linear;
    }

    void PointLight::setLinear(float linear) {
        PointLight::linear = linear;
    }

    float PointLight::getQuadratic() const {
        return quadratic;
    }

    void PointLight::setQuadratic(float quadratic) {
        PointLight::quadratic = quadratic;
    }

    const glm::vec3 &PointLight::getAmbient() const {
        return ambient;
    }

    void PointLight::setAmbient(const glm::vec3 &ambient) {
        PointLight::ambient = ambient;
    }

    const glm::vec3 &PointLight::getDiffuse() const {
        return diffuse;
    }

    void PointLight::setDiffuse(const glm::vec3 &diffuse) {
        PointLight::diffuse = diffuse;
    }

    const glm::vec3 &PointLight::getSpecular() const {
        return specular;
    }

    void PointLight::setSpecular(const glm::vec3 &specular) {
        PointLight::specular = specular;
    }

    const Shader &PointLight::getSolidObjectsShader() const {
        return solidObjectsShader;
    }

    void PointLight::setSolidObjectsShader(const Shader &solidObjectsShader) {
        PointLight::solidObjectsShader = solidObjectsShader;
    }

    GLuint PointLight::getPositionLoc() const {
        return positionLoc;
    }

    void PointLight::setPositionLoc(GLuint positionLoc) {
        PointLight::positionLoc = positionLoc;
    }

    GLuint PointLight::getConstantLoc() const {
        return constantLoc;
    }

    void PointLight::setConstantLoc(GLuint constantLoc) {
        PointLight::constantLoc = constantLoc;
    }

    GLuint PointLight::getLinearLoc() const {
        return linearLoc;
    }

    void PointLight::setLinearLoc(GLuint linearLoc) {
        PointLight::linearLoc = linearLoc;
    }

    GLuint PointLight::getQuadraticLoc() const {
        return quadraticLoc;
    }

    void PointLight::setQuadraticLoc(GLuint quadraticLoc) {
        PointLight::quadraticLoc = quadraticLoc;
    }

    GLuint PointLight::getAmbientLoc() const {
        return ambientLoc;
    }

    void PointLight::setAmbientLoc(GLuint ambientLoc) {
        PointLight::ambientLoc = ambientLoc;
    }

    GLuint PointLight::getDiffuseLoc() const {
        return diffuseLoc;
    }

    void PointLight::setDiffuseLoc(GLuint diffuseLoc) {
        PointLight::diffuseLoc = diffuseLoc;
    }

    GLuint PointLight::getSpecularLoc() const {
        return specularLoc;
    }

    void PointLight::setSpecularLoc(GLuint specularLoc) {
        PointLight::specularLoc = specularLoc;
    }
}