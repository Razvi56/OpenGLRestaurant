//
// Created by Razvan on 1/28/2024.
//

#ifndef LAB8_BLINN_PHONG_TRANSFORMSANDLIGHTING_H
#define LAB8_BLINN_PHONG_TRANSFORMSANDLIGHTING_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"
#include "Shader.hpp"
#include "ModelTransforms.h"
#include "Constants.h"
#include "SceneLighting.h"
#include "Model3D.hpp"
#include "RenderModes.h"

namespace gps {

    class TransformsAndLighting {
    public:
        void renderScene(RenderMode renderMode) {

            switch (renderMode) {
                case RenderMode::SOLID:
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    break;
                case RenderMode::WIREFRAME:
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    break;
                case RenderMode::SMOOTH:
                    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                    break;
            }


            buildSolidsDepth2DMapTexture();
            for (int i = 0; i < NR_OF_POINT_LIGHTS; i++) {
                buildSolidsDepthCubeMapTexture(i);
            }
            drawSolidObjects();
            drawLights();
        }

        void updateSolidObjTranslation(glm::vec3 translationP, int index) {
            solidModelTransforms[index].setTranslation(translationP);
            recalculateSolidsModelAndNormal(index);
        }

        void updateLightObjTranslation(glm::vec3 translationP, int index) {
            pointLightModelTransforms[index].setTranslation(translationP);
            recalculateLightModel(index);
        }

        void updateSolidObjRotation(glm::vec3 rotationP, int index) {
            solidModelTransforms[index].setRotation(rotationP);
            recalculateSolidsModelAndNormal(index);
        }

        void updateLightObjRotation(glm::vec3 rotationP, int index) {
            pointLightModelTransforms[index].setRotation(rotationP);
            recalculateLightModel(index);
        }

        void updateSolidObjScaling(glm::vec3 scalingP, int index) {
            solidModelTransforms[index].setScaling(scalingP);
            recalculateSolidsModelAndNormal(index);
        }

        void updateLightObjScaling(glm::vec3 scalingP, int index) {
            pointLightModelTransforms[index].setScaling(scalingP);
            recalculateLightModel(index);
        }

        void updateSolidObjTransforms(glm::vec3 translationP, glm::vec3 rotationP, glm::vec3 scalingP, int index) {
            solidModelTransforms[index].setTranslation(translationP);
            solidModelTransforms[index].setRotation(rotationP);
            solidModelTransforms[index].setScaling(scalingP);

            recalculateSolidsModelAndNormal(index);
        }

        void updateLightObjTransforms(glm::vec3 translationP, glm::vec3 rotationP, glm::vec3 scalingP, int index) {
            pointLightModelTransforms[index].setTranslation(translationP);
            pointLightModelTransforms[index].setRotation(rotationP);
            pointLightModelTransforms[index].setScaling(scalingP);

            recalculateLightModel(index);
        }

        void moveCamera(gps::MOVE_DIRECTION direction) {
            camera.move(direction, cameraSpeed);
            view = camera.getViewMatrix();
            updateAfterChangeInView();
        }

        void rotateCamera(float pitch, float yaw) {
            camera.rotate(pitch, yaw);
            view = camera.getViewMatrix();
            updateAfterChangeInView();
        }

        void setCamera(glm::vec3 cameraPosition, glm::vec3 cameraFrontDirection) {
            this->camera.setCamera(cameraPosition,cameraFrontDirection,glm::vec3(0.0f,1.0f,0.0f));
            view = camera.getViewMatrix();
            updateAfterChangeInView();
        }

        glm::vec3 getCameraPosition(){
            return this->camera.getCameraPosition();
        }

        void updateProjection(glm::mat4 projection);

        void getSolidObjReadyFor2DDepthPass(int objIndex);

        void getPointLightInPlaceForCubeDepthPass(int lightIndex);

        void getSolidObjReadyForCubeDepthPass(int objIndex);

        void getSolidObjReadyForFinalPass(int objIndex);

        void getLightObjReadyForFinalPass(int lightIndex);

        TransformsAndLighting(const ModelTransforms (&solidModelTransforms)[NR_OF_SOLID_OBJECTS],
                              const glm::mat4 &projection, const gps::Camera &camera,
                              const SceneLighting &sceneLighting,
                              const gps::Shader (&solidsShader), const gps::Shader (&pointLightsShader),
                              const gps::Shader (&depth2DMapShader), const gps::Shader (&depthCubeMapShader),
                              const Model3D (&solidObjectMeshMods)[NR_OF_SOLID_OBJECTS],
                              const Model3D (&lightObjectMeshMod),
                              const int &retina_width, const int &retina_height);

        const ModelTransforms *getSolidModelTransforms() const;

        const glm::mat4 *getSolidModels() const;

        const glm::mat3 *getSolidNormalMatrices() const;

        const ModelTransforms *getPointLightModelTransforms() const;

        const glm::mat4 *getPointLightModels() const;


        const glm::mat4 &getView() const;

        void setView(const glm::mat4 &view);

        const glm::mat4 &getProjection() const;

        void setProjection(const glm::mat4 &projection);

        const Camera &getCamera() const;

        void setCamera(const Camera &camera);

        const float getCameraSpeed() const;

        SceneLighting &getSceneLighting();

        void setSceneLighting(const SceneLighting &sceneLighting);

        const Shader &getSolidsShader() const;

        void setSolidsShader(const Shader &solidsShader);

        const Shader &getPointLightsShader() const;

        void setPointLightsShader(const Shader &pointLightsShader);

        const Shader &getDepth2DMapShader() const;

        void setDepth2DMapShader(const Shader &depth2DMapShader);

        const Shader &getDepthCubeMapShader() const;

        void setDepthCubeMapShader(const Shader &depthCubeMapShader);

        GLuint getSolidsViewLoc() const;

        void setSolidsViewLoc(GLuint solidsViewLoc);

        GLuint getSolidsProjectionLoc() const;

        void setSolidsProjectionLoc(GLuint solidsProjectionLoc);

        GLuint getSolidsModelLoc() const;

        void setSolidsModelLoc(GLuint solidsModelLoc);

        GLuint getSolidsNormalMatrixLoc() const;

        void setSolidsNormalMatrixLoc(GLuint solidsNormalMatrixLoc);

        GLuint getPointLightsViewLoc() const;

        void setPointLightsViewLoc(GLuint pointLightsViewLoc);

        GLuint getPointLightsProjectionLoc() const;

        void setPointLightsProjectionLoc(GLuint pointLightsProjectionLoc);

        GLuint getPointLightsModelLoc() const;

        void setPointLightsModelLoc(GLuint pointLightsModelLoc);

        GLuint getDepth2DLightSpaceTrMatrixDirLoc() const;

        void setDepth2DLightSpaceTrMatrixDirLoc(GLuint depth2DLightSpaceTrMatrixDirLoc);

        const float getDirNearPlane() const;

        const float getDirFarPlane() const;

        const float getPointNearPlane() const;

        const float getPointFarPlane() const;


    private:
        SceneLighting sceneLighting;

        const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
        int retina_width, retina_height;

        gps::Camera camera;
        const float cameraSpeed = 0.06f;

        gps::Shader solidsShader;
        gps::Shader pointLightsShader;
        gps::Shader depth2DMapShader;
        gps::Shader depthCubeMapShader;

        Model3D solidObjectMeshMods[NR_OF_SOLID_OBJECTS] = {};
        Model3D lightObjectMeshMod = {};

        GLuint depthCubeMapFBO[NR_OF_POINT_LIGHTS] = {};
        GLuint depth2DMapFBO = {};

        ModelTransforms solidModelTransforms[NR_OF_SOLID_OBJECTS] = {};

        ModelTransforms pointLightModelTransforms[NR_OF_POINT_LIGHTS] = {};

        const float dirNearPlane = 0.01f;
        const float dirFarPlane = 30.0f;

        const float pointNearPlane = 0.01f;
        ///const float pointFarPlane = 20.0f;/// ALSO SENT TO SOLIDS FRAGMENT SHADER!!!!!

        GLuint solidsModelLoc = {};
        glm::mat4 solidsModels[NR_OF_SOLID_OBJECTS] = {};
        GLuint solidsNormalMatrixLoc = {};
        glm::mat3 solidsNormalMatrices[NR_OF_SOLID_OBJECTS] = {};
        GLuint solidsViewLoc = {};
        glm::mat4 view;
        GLuint solidsProjectionLoc = {};
        glm::mat4 projection;
        GLuint solidsLightSpaceDirTrMatrixLoc = {};
        glm::mat4 solidsLightSpaceDirTrMatrix;
        GLuint solidsDepth2DMapTextureLoc = {};
        GLuint solidsDepth2DMapTexture;
        GLuint solidsDepthCubeMapTextureLoc[NR_OF_POINT_LIGHTS] = {};
        GLuint solidsDepthCubeMapTexture[NR_OF_POINT_LIGHTS];
        GLuint solidsFarPlaneLoc = {};
        const float solidsFarPlane = 20.0f; ///far plane for point lights shadows

        GLuint pointLightsModelLoc = {};
        glm::mat4 pointLightModels[NR_OF_POINT_LIGHTS] = {};
        GLuint pointLightsViewLoc = {}; ///glm::mat4 view;
        GLuint pointLightsProjectionLoc = {}; ///glm::mat4 projection;

        GLuint depth2DModelLoc = {}; ///models sent at rendering first pass
        GLuint depth2DLightSpaceTrMatrixDirLoc = {}; /// glm::mat4 solidsLightSpaceDirTrMatrix;

        GLuint depthCubeModelLoc = {}; ///models sent at rendering first pass
        GLuint depthCubeLightPosLoc; ///light positions sent at rendering cube map pass
        GLuint depthCubeFar_planeLoc = {}; /// const float solidsFarPlane = 20.0f;
        GLuint depthCubeLightSpaceCubeTrMatricesLoc[6];
        glm::mat4 depthCubeLightSpaceCubeTrMatrices[NR_OF_POINT_LIGHTS][6];

        void helperConstructorInit2DFBO() {
            ///GLuint depth2DMapFBO = {};
            ///GLuint solidsDepth2DMapTexture;
            glGenFramebuffers(1, &depth2DMapFBO);
            glGenTextures(1, &solidsDepth2DMapTexture);
            glBindTexture(GL_TEXTURE_2D, solidsDepth2DMapTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                         SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

            glBindFramebuffer(GL_FRAMEBUFFER, depth2DMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, solidsDepth2DMapTexture,
                                   0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);///idk
        }

        void helperConstructorInitCubeFBO(int index) {
            ///GLuint depthCubeMapFBO[NR_OF_POINT_LIGHTS] = {};
            ///GLuint solidsDepthCubeMapTexture[NR_OF_POINT_LIGHTS];
            glGenFramebuffers(1, &depthCubeMapFBO[index]);

            glGenTextures(1, &solidsDepthCubeMapTexture[index]);
            glBindTexture(GL_TEXTURE_CUBE_MAP, solidsDepthCubeMapTexture[index]);
            for (unsigned int i = 0; i < 6; ++i)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                             SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glBindFramebuffer(GL_FRAMEBUFFER, depthCubeMapFBO[index]);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, solidsDepthCubeMapTexture[index], 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);///idk
        }

        void recalculateSolidsModelAndNormal(int index) {
            solidsModels[index] = glm::translate(solidModelTransforms[index].getTranslation()) *
                                  glm::rotate(solidModelTransforms[index].getRotation().z,
                                              glm::vec3(0.0f, 0.0f, 1.0f)) *
                                  glm::rotate(solidModelTransforms[index].getRotation().y,
                                              glm::vec3(0.0f, 1.0f, 0.0f)) *
                                  glm::rotate(solidModelTransforms[index].getRotation().x,
                                              glm::vec3(1.0f, 0.0f, 0.0f)) *
                                  glm::scale(solidModelTransforms[index].getScaling());
            solidsNormalMatrices[index] = glm::mat3(glm::inverseTranspose(view * solidsModels[index]));
        }

        void recalculateLightModel(int index) {
            pointLightModels[index] = glm::translate(pointLightModelTransforms[index].getTranslation()) *
                                      glm::rotate(pointLightModelTransforms[index].getRotation().z,
                                                  glm::vec3(0.0f, 0.0f, 1.0f)) *
                                      glm::rotate(pointLightModelTransforms[index].getRotation().y,
                                                  glm::vec3(0.0f, 1.0f, 0.0f)) *
                                      glm::rotate(pointLightModelTransforms[index].getRotation().x,
                                                  glm::vec3(1.0f, 0.0f, 0.0f)) *
                                      glm::scale(pointLightModelTransforms[index].getScaling());
        }

        void updateAfterChangeInView();

        void constructorGetUniformLocationsForSolidsVertexShader() {
            ///The locations for the fragment shader lighting uniforms are obtained in the pointLight and dirLight constructors
            this->solidsShader.useShaderProgram();
            this->solidsModelLoc = glGetUniformLocation(this->solidsShader.shaderProgram, "model");
            this->solidsNormalMatrixLoc = glGetUniformLocation(this->solidsShader.shaderProgram, "normalMatrix");
            this->solidsViewLoc = glGetUniformLocation(this->solidsShader.shaderProgram, "view");
            this->solidsProjectionLoc = glGetUniformLocation(this->solidsShader.shaderProgram, "projection");
            this->solidsLightSpaceDirTrMatrixLoc = glGetUniformLocation(this->solidsShader.shaderProgram,
                                                                        "lSDTM");
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cerr << "OpenGL error after querying 'lightSpaceDirTrMatrix': " << err << std::endl;
            } else if (this->solidsLightSpaceDirTrMatrixLoc == -1) {
                std::cerr << "'lightSpaceDirTrMatrix' uniform location not found." << std::endl;
            }
        }

        void constructorGetUniformLocationsForSolidsFragmentShader() {
            this->solidsShader.useShaderProgram();
            this->solidsDepth2DMapTextureLoc = glGetUniformLocation(this->solidsShader.shaderProgram,
                                                                    "depthMapTexture");
            for (int i = 0; i < NR_OF_POINT_LIGHTS; i++) {
                this->solidsDepthCubeMapTextureLoc[i] =
                        glGetUniformLocation(this->solidsShader.shaderProgram,
                                             ("depthCubeMapTexture[" + std::to_string(i) + "]").c_str());
            }
            this->solidsFarPlaneLoc = glGetUniformLocation(this->solidsShader.shaderProgram, "far_plane");
        }

        void constructorGetUniformLocationsForPointLightsVertexShader() {
            ///fragment shader has no uniforms
            this->pointLightsShader.useShaderProgram();
            this->pointLightsModelLoc = glGetUniformLocation(this->pointLightsShader.shaderProgram, "model");
            this->pointLightsViewLoc = glGetUniformLocation(this->pointLightsShader.shaderProgram, "view");
            this->pointLightsProjectionLoc = glGetUniformLocation(this->pointLightsShader.shaderProgram, "projection");
        }

        void constructorGetUniformLocationsForDepth2DVertexShader() {
            this->depth2DMapShader.useShaderProgram();
            this->depth2DModelLoc = glGetUniformLocation(this->depth2DMapShader.shaderProgram, "model");
            this->depth2DLightSpaceTrMatrixDirLoc = glGetUniformLocation(this->depth2DMapShader.shaderProgram,
                                                                         "lightSpaceTrMatrixDir");
        }

        void constructorGetUniformLocationsForDepthCubeVertexShader() {
            this->depthCubeMapShader.useShaderProgram();
            this->depthCubeModelLoc = glGetUniformLocation(this->depthCubeMapShader.shaderProgram, "model");
        }

        void constructorGetUniformLocationsForDepthCubeGeometryShader() {
            this->depthCubeMapShader.useShaderProgram();
            for (int i = 0; i < 6; i++) {
                this->depthCubeLightSpaceCubeTrMatricesLoc[i] =
                        glGetUniformLocation(this->depthCubeMapShader.shaderProgram,
                                             ("shadowCubeMatrices[" + std::to_string(i) + "]").c_str());
            }
        }

        void constructorGetUniformLocationsForDepthCubeFragmentShader() {
            this->depthCubeMapShader.useShaderProgram();
            this->depthCubeFar_planeLoc = glGetUniformLocation(this->depthCubeMapShader.shaderProgram, "far_plane");
            this->depthCubeLightPosLoc = glGetUniformLocation(this->depthCubeMapShader.shaderProgram, "lightPos");
        }

        void constructorSendUniformsToSolidsVertexShader() {
            ///The locations for the fragment shader lighting uniforms are obtained in the pointLight and dirLight constructors
            this->solidsShader.useShaderProgram();
            glUniformMatrix4fv(this->solidsViewLoc, 1, GL_FALSE, glm::value_ptr(this->view));
            glUniformMatrix4fv(this->solidsProjectionLoc, 1, GL_FALSE, glm::value_ptr(this->projection));
            /*glUniformMatrix4fv(this->solidsLightSpaceDirTrMatrixLoc, 1, GL_FALSE, ///need to send it at each frame
                               glm::value_ptr(this->solidsLightSpaceDirTrMatrix));*/
        }


        void constructorSendUniformsToSolidsFragmentShader() {
            ///view has been sent, depth textures will be sent at every frame
            ///we need to send lighting and far_plane
            ///lighting:
            this->solidsShader.useShaderProgram();
            glUniformMatrix4fv(this->solidsViewLoc, 1, GL_FALSE, glm::value_ptr(this->view));
            glUniformMatrix4fv(this->solidsProjectionLoc, 1, GL_FALSE, glm::value_ptr(this->projection));
            glUniformMatrix4fv(this->solidsLightSpaceDirTrMatrixLoc, 1, GL_FALSE,
                               glm::value_ptr(this->solidsLightSpaceDirTrMatrix));

            glUniform3fv(sceneLighting.getGlobalIllumination().getAmbientLoc(), 1,
                         glm::value_ptr(sceneLighting.getGlobalIllumination().getAmbient()));
            glUniform3fv(sceneLighting.getGlobalIllumination().getDiffuseLoc(), 1,
                         glm::value_ptr(sceneLighting.getGlobalIllumination().getDiffuse()));
            glUniform3fv(sceneLighting.getGlobalIllumination().getSpecularLoc(), 1,
                         glm::value_ptr(sceneLighting.getGlobalIllumination().getSpecular()));
            glUniform3fv(sceneLighting.getGlobalIllumination().getDirectionLoc(), 1, glm::value_ptr(
                    glm::inverseTranspose(glm::mat3(this->view)) *
                    sceneLighting.getGlobalIllumination().getDirection()));

            for (int i = 0; i < NR_OF_POINT_LIGHTS; i++) {
                glUniform3fv(sceneLighting.getPointLights()[i].getAmbientLoc(), 1,
                             glm::value_ptr(sceneLighting.getPointLights()[i].getAmbient()));
                glUniform3fv(sceneLighting.getPointLights()[i].getDiffuseLoc(), 1,
                             glm::value_ptr(sceneLighting.getPointLights()[i].getDiffuse()));
                glUniform3fv(sceneLighting.getPointLights()[i].getSpecularLoc(), 1,
                             glm::value_ptr(sceneLighting.getPointLights()[i].getSpecular()));
                glUniform1f(sceneLighting.getPointLights()[i].getConstantLoc(),
                            sceneLighting.getPointLights()[i].getConstant());
                glUniform1f(sceneLighting.getPointLights()[i].getLinearLoc(),
                            sceneLighting.getPointLights()[i].getLinear());
                glUniform1f(sceneLighting.getPointLights()[i].getQuadraticLoc(),
                            sceneLighting.getPointLights()[i].getQuadratic());
                glUniform3fv(sceneLighting.getPointLights()[i].getPositionLoc(), 1,
                             glm::value_ptr(sceneLighting.getPointLights()[i].getPosition()));
            }
            ///far_plane:
            glUniform1f(solidsFarPlaneLoc, solidsFarPlane);
        }

        void constructorSendUniformsToPointLightsVertexShader() {
            ///fragment shader has no uniforms
            this->pointLightsShader.useShaderProgram();
            glUniformMatrix4fv(this->pointLightsViewLoc, 1, GL_FALSE, glm::value_ptr(this->view));
            glUniformMatrix4fv(this->pointLightsProjectionLoc, 1, GL_FALSE, glm::value_ptr(this->projection));
        }

        void constructorSendUniformsToDepth2DVertexShader() {
            this->depth2DMapShader.useShaderProgram();
            ///lightSpaceTrMatrix ar trebui updatat o data cu schimbarea directiei luminiii, daca va fi sa schimb directia luminii
            glUniformMatrix4fv(this->depth2DLightSpaceTrMatrixDirLoc, 1, GL_FALSE,
                               glm::value_ptr(this->solidsLightSpaceDirTrMatrix));
        }


        void constructorSendUniformsToDepthCubeGeometryShader() {
            this->depthCubeMapShader.useShaderProgram();
            /*for (int i = 0; i < 6; i++) {              I don't send this at the beginning because I need to send it at each frame
                glUniformMatrix4fv(this->depthCubeLightSpaceCubeTrMatricesLoc[i], 1, GL_FALSE,
                                   glm::value_ptr(this->depthCubeLightSpaceCubeTrMatrices[i]));
            }*/
        }

        void constructorSendUniformsToDepthCubeFragmentShader() {
            this->depthCubeMapShader.useShaderProgram();
            glUniform1f(depthCubeFar_planeLoc, solidsFarPlane);
            this->depthCubeFar_planeLoc = glGetUniformLocation(this->depthCubeMapShader.shaderProgram, "far_plane");
        }

        //////////////////////////////////////////////////////////////////////////////////////////////Other constructor helpers
        void constructorUpdateTransformsSolidsAndLights(const ModelTransforms (&solidModelTransforms)[NR_OF_SOLID_OBJECTS]) {
            for (int i = 0; i < NR_OF_SOLID_OBJECTS; i++) {
                updateSolidObjTransforms(solidModelTransforms[i].getTranslation(),
                                         solidModelTransforms[i].getRotation(),
                                         solidModelTransforms[i].getScaling(), i);
            }
            // calculate models for pointLight objects
            for (int i = 0; i < NR_OF_POINT_LIGHTS; i++) {
                updateLightObjTransforms(sceneLighting.getPointLights()[i].getPosition(),
                                         glm::vec3(0.0f),
                                         glm::vec3(1.0f / 100.0f), i);
            }
        }

        void constructorComputeSolidsLightSpaceDirTrMatrix() {
            glm::vec3 lightDir = getSceneLighting().getGlobalIllumination().getDirection();
            glm::mat4 lightView = glm::lookAt(lightDir, glm::vec3(5.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            const GLfloat near_plane = getDirNearPlane(), far_plane = getDirFarPlane();
            glm::mat4 lightProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near_plane, far_plane);
            glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;
            this->solidsLightSpaceDirTrMatrix = lightSpaceTrMatrix;
        }

        void constructorComputeDepthCubeLightSpaceCubeTrMatrices(int pointLightIndex) {
            float aspect = (float) this->SHADOW_WIDTH / (float) this->SHADOW_HEIGHT;
            glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, this->pointNearPlane,
                                                    this->solidsFarPlane);
            glm::vec3 lightPos = this->sceneLighting.getPointLights()[pointLightIndex].getPosition();
            depthCubeLightSpaceCubeTrMatrices[pointLightIndex][0] =
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
            depthCubeLightSpaceCubeTrMatrices[pointLightIndex][1] =
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
            depthCubeLightSpaceCubeTrMatrices[pointLightIndex][2] =
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
            depthCubeLightSpaceCubeTrMatrices[pointLightIndex][3] =
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
            depthCubeLightSpaceCubeTrMatrices[pointLightIndex][4] =
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
            depthCubeLightSpaceCubeTrMatrices[pointLightIndex][5] =
                    shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));
        }

        void constructorInitFBOs() {
            helperConstructorInit2DFBO();
            for (int i = 0; i < 2; i++) {
                helperConstructorInitCubeFBO(i);
            }
        }

        ///rendering-----------<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
/*        void TransformsAndLighting::getSolidObjReadyFor2DDepthPass(int objIndex){
            depth2DMapShader.useShaderProgram();
            glUniformMatrix4fv(depth2DModelLoc, 1, GL_FALSE, glm::value_ptr(solidsModels[objIndex]));
        }

        void TransformsAndLighting::getPointLightInPlaceForCubeDepthPass(int lightIndex){
            depthCubeMapShader.useShaderProgram();
            glUniform3fv(depthCubeLightPosLoc,1,glm::value_ptr(sceneLighting.getPointLights()[lightIndex].getPosition()));
            for (int i = 0; i < 6; i++) {
                glUniformMatrix4fv(this->depthCubeLightSpaceCubeTrMatricesLoc[i], 1, GL_FALSE,
                                   glm::value_ptr(this->depthCubeLightSpaceCubeTrMatrices[lightIndex][i]));
            }
        }

        void TransformsAndLighting::getSolidObjReadyForCubeDepthPass(int objIndex){
            depthCubeMapShader.useShaderProgram();
            glUniformMatrix4fv(depthCubeModelLoc, 1, GL_FALSE, glm::value_ptr(solidsModels[objIndex]));
        }

        void TransformsAndLighting::getSolidObjReadyForFinalPass(int objIndex){
            solidsShader.useShaderProgram();
            glUniformMatrix4fv(solidsModelLoc, 1, GL_FALSE, glm::value_ptr(solidsModels[objIndex]));
            glUniformMatrix3fv(solidsNormalMatrixLoc, 1, GL_FALSE,
                               glm::value_ptr(this->solidsNormalMatrices[objIndex]));
        }

        void TransformsAndLighting::getLightObjReadyForFinalPass(int lightIndex){
            pointLightsShader.useShaderProgram();
            glUniformMatrix4fv(pointLightsModelLoc, 1, GL_FALSE, glm::value_ptr(pointLightModels[lightIndex]));
        }*/


        void buildSolidsDepth2DMapTexture() {
            depth2DMapShader.useShaderProgram(); ///lightSpaceTrMatrix will be updated from the outside if the light dir changes
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, this->depth2DMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            for (int i = 0; i < NR_OF_SOLID_OBJECTS; i++) {
                getSolidObjReadyFor2DDepthPass(i);
                solidObjectMeshMods[i].Draw(depth2DMapShader);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void buildSolidsDepthCubeMapTexture(int lightIndex) {
            depthCubeMapShader.useShaderProgram();
            ///functiile ajutatoare
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, this->depthCubeMapFBO[lightIndex]);
            glClear(GL_DEPTH_BUFFER_BIT);
            getPointLightInPlaceForCubeDepthPass(lightIndex);

            for (int i = 0; i < NR_OF_SOLID_OBJECTS; i++) {
                getSolidObjReadyForCubeDepthPass(i);
                solidObjectMeshMods[i].Draw(depthCubeMapShader);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void drawSolidObjects() {
            glViewport(0, 0, retina_width, retina_height);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            solidsShader.useShaderProgram();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, solidsDepth2DMapTexture);
            glUniform1i(solidsDepth2DMapTextureLoc, 0);

            for (int i = 0; i < 2; i++) {
                glActiveTexture(GL_TEXTURE1 + i);
                glBindTexture(GL_TEXTURE_CUBE_MAP, solidsDepthCubeMapTexture[i]);
                glUniform1i(solidsDepthCubeMapTextureLoc[i], i + 1);
            }

            for (int i = 0; i < NR_OF_SOLID_OBJECTS; i++) {
                getSolidObjReadyForFinalPass(i);
                solidObjectMeshMods[i].Draw(solidsShader);
            }
        }

        void drawLights() {
            pointLightsShader.useShaderProgram();
            for (int i = 0; i < NR_OF_POINT_LIGHTS; i++) {
                getLightObjReadyForFinalPass(i);
                lightObjectMeshMod.Draw(pointLightsShader);
            }
        }
    };
}

#endif //LAB8_BLINN_PHONG_TRANSFORMSANDLIGHTING_H
