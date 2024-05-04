//
// Created by Razvan on 1/28/2024.

#include "TransformsAndLighting.h"

namespace gps {

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void TransformsAndLighting::updateAfterChangeInView() {
        for (int i = 0; i < NR_OF_SOLID_OBJECTS; i++) {
            solidsNormalMatrices[i] = glm::mat3(glm::inverseTranspose(view * solidsModels[i]));
        }

        solidsShader.useShaderProgram();
        glUniformMatrix4fv(solidsViewLoc, 1, GL_FALSE, glm::value_ptr(view));

        pointLightsShader.useShaderProgram();
        glUniformMatrix4fv(pointLightsViewLoc, 1, GL_FALSE, glm::value_ptr(view));

        solidsShader.useShaderProgram();
        glUniform3fv(sceneLighting.getGlobalIllumination().getDirectionLoc(), 1, glm::value_ptr(
                glm::inverseTranspose(glm::mat3(view)) * sceneLighting.getGlobalIllumination().getDirection()));
    }

    void TransformsAndLighting::updateProjection(glm::mat4 projection) {
        this->projection = projection;

        solidsShader.useShaderProgram();
        glUniformMatrix4fv(this->solidsProjectionLoc, 1, GL_FALSE, glm::value_ptr(this->projection));

        pointLightsShader.useShaderProgram();
        glUniformMatrix4fv(this->pointLightsProjectionLoc, 1, GL_FALSE, glm::value_ptr(this->projection));
    }

    void TransformsAndLighting::getSolidObjReadyFor2DDepthPass(int objIndex) {
        depth2DMapShader.useShaderProgram();
        glUniformMatrix4fv(depth2DModelLoc, 1, GL_FALSE, glm::value_ptr(solidsModels[objIndex]));
    }

    void TransformsAndLighting::getPointLightInPlaceForCubeDepthPass(int lightIndex) {
        depthCubeMapShader.useShaderProgram();
        constructorComputeDepthCubeLightSpaceCubeTrMatrices(lightIndex);
        glUniform3fv(depthCubeLightPosLoc, 1, glm::value_ptr(sceneLighting.getPointLights()[lightIndex].getPosition()));
        for (int i = 0; i < 6; i++) {
            glUniformMatrix4fv(this->depthCubeLightSpaceCubeTrMatricesLoc[i], 1, GL_FALSE,
                               glm::value_ptr(this->depthCubeLightSpaceCubeTrMatrices[lightIndex][i]));
        }
        solidsShader.useShaderProgram();
        glUniform3fv(sceneLighting.getPointLights()[lightIndex].getPositionLoc(), 1,
                     glm::value_ptr(sceneLighting.getPointLights()[lightIndex].getPosition()));
    }

    void TransformsAndLighting::getSolidObjReadyForCubeDepthPass(int objIndex) {
        depthCubeMapShader.useShaderProgram();
        glUniformMatrix4fv(depthCubeModelLoc, 1, GL_FALSE, glm::value_ptr(solidsModels[objIndex]));
    }

    void TransformsAndLighting::getSolidObjReadyForFinalPass(int objIndex) {
        solidsShader.useShaderProgram();
        glUniformMatrix4fv(solidsModelLoc, 1, GL_FALSE, glm::value_ptr(solidsModels[objIndex]));
        glUniformMatrix3fv(solidsNormalMatrixLoc, 1, GL_FALSE,
                           glm::value_ptr(this->solidsNormalMatrices[objIndex]));
    }

    void TransformsAndLighting::getLightObjReadyForFinalPass(int lightIndex) {
        pointLightsShader.useShaderProgram();
        pointLightModelTransforms[lightIndex].setTranslation(sceneLighting.getPointLights()[lightIndex].getPosition());
        recalculateLightModel(lightIndex);
        glUniformMatrix4fv(pointLightsModelLoc, 1, GL_FALSE, glm::value_ptr(pointLightModels[lightIndex]));
    }


    TransformsAndLighting::TransformsAndLighting(const ModelTransforms (&solidModelTransforms)[NR_OF_SOLID_OBJECTS],
                                                 const glm::mat4 &projection, const gps::Camera &camera,
                                                 const SceneLighting &sceneLighting,
                                                 const gps::Shader (&solidsShader),
                                                 const gps::Shader (&pointLightsShader),
                                                 const gps::Shader (&depth2DMapShader),
                                                 const gps::Shader (&depthCubeMapShader),
                                                 const Model3D (&solidObjectMeshMods)[NR_OF_SOLID_OBJECTS],
                                                 const Model3D (&lightObjectMeshMod),
                                                 const int &retina_width, const int &retina_height)
            :
            projection(projection), camera(camera), sceneLighting(sceneLighting),
            solidsShader(solidsShader), pointLightsShader(pointLightsShader),
            depth2DMapShader(depth2DMapShader), depthCubeMapShader(depthCubeMapShader),
            retina_width(retina_width), retina_height(retina_height), lightObjectMeshMod(lightObjectMeshMod) {
        ///calculate view
        this->view = (this->camera).getViewMatrix();

        for (int i = 0; i < NR_OF_SOLID_OBJECTS; i++) {
            this->solidObjectMeshMods[i] = solidObjectMeshMods[i];
        }

        constructorUpdateTransformsSolidsAndLights(solidModelTransforms);
        constructorComputeSolidsLightSpaceDirTrMatrix();
        for (int i = 0; i < NR_OF_POINT_LIGHTS; i++) {
            constructorComputeDepthCubeLightSpaceCubeTrMatrices(i);
        }
        constructorInitFBOs();

        ///get the locations of all uniforms for all shaders (other than the ones that we have already got in 
        /// the constructors for lights)
        constructorGetUniformLocationsForSolidsVertexShader();
        ///uniform locations for lights are obtained from light constructors
        ///uniform locations for the depth textures and far plane must be obtained from here
        constructorGetUniformLocationsForSolidsFragmentShader();
        constructorGetUniformLocationsForPointLightsVertexShader();///no uniforms in fragment shader
        constructorGetUniformLocationsForDepth2DVertexShader();///no uniforms in fragment shader
        constructorGetUniformLocationsForDepthCubeVertexShader();
        constructorGetUniformLocationsForDepthCubeGeometryShader();
        constructorGetUniformLocationsForDepthCubeFragmentShader();

        ///sendToShaders
        constructorSendUniformsToSolidsVertexShader();
        constructorSendUniformsToSolidsFragmentShader();
        constructorSendUniformsToPointLightsVertexShader();
        constructorSendUniformsToDepth2DVertexShader();
        constructorSendUniformsToDepthCubeGeometryShader();
        constructorSendUniformsToDepthCubeFragmentShader();
    }

    const ModelTransforms *TransformsAndLighting::getSolidModelTransforms() const {
        return solidModelTransforms;
    }

    const glm::mat4 *TransformsAndLighting::getSolidModels() const {
        return solidsModels;
    }

    const glm::mat3 *TransformsAndLighting::getSolidNormalMatrices() const {
        return solidsNormalMatrices;
    }

    const ModelTransforms *TransformsAndLighting::getPointLightModelTransforms() const {
        return pointLightModelTransforms;
    }

    const glm::mat4 *TransformsAndLighting::getPointLightModels() const {
        return pointLightModels;
    }


    const glm::mat4 &TransformsAndLighting::getView() const {
        return view;
    }

    void TransformsAndLighting::setView(const glm::mat4 &view) {
        TransformsAndLighting::view = view;
    }

    const glm::mat4 &TransformsAndLighting::getProjection() const {
        return projection;
    }

    void TransformsAndLighting::setProjection(const glm::mat4 &projection) {
        TransformsAndLighting::projection = projection;
    }

    const Camera &TransformsAndLighting::getCamera() const {
        return camera;
    }

    void TransformsAndLighting::setCamera(const Camera &camera) {
        TransformsAndLighting::camera = camera;
    }

    const float TransformsAndLighting::getCameraSpeed() const {
        return cameraSpeed;
    }

    SceneLighting &TransformsAndLighting::getSceneLighting(){
        return sceneLighting;
    }

    void TransformsAndLighting::setSceneLighting(const SceneLighting &sceneLighting) {
        TransformsAndLighting::sceneLighting = sceneLighting;
    }

    const Shader &TransformsAndLighting::getSolidsShader() const {
        return solidsShader;
    }

    void TransformsAndLighting::setSolidsShader(const Shader &solidsShader) {
        TransformsAndLighting::solidsShader = solidsShader;
    }

    const Shader &TransformsAndLighting::getPointLightsShader() const {
        return pointLightsShader;
    }

    void TransformsAndLighting::setPointLightsShader(const Shader &pointLightsShader) {
        TransformsAndLighting::pointLightsShader = pointLightsShader;
    }

    const Shader &TransformsAndLighting::getDepth2DMapShader() const {
        return depth2DMapShader;
    }

    void TransformsAndLighting::setDepth2DMapShader(const Shader &depth2DMapShader) {
        TransformsAndLighting::depth2DMapShader = depth2DMapShader;
    }

    const Shader &TransformsAndLighting::getDepthCubeMapShader() const {
        return depthCubeMapShader;
    }

    void TransformsAndLighting::setDepthCubeMapShader(const Shader &depthCubeMapShader) {
        TransformsAndLighting::depthCubeMapShader = depthCubeMapShader;
    }

    GLuint TransformsAndLighting::getSolidsViewLoc() const {
        return solidsViewLoc;
    }

    void TransformsAndLighting::setSolidsViewLoc(GLuint solidsViewLoc) {
        TransformsAndLighting::solidsViewLoc = solidsViewLoc;
    }

    GLuint TransformsAndLighting::getSolidsProjectionLoc() const {
        return solidsProjectionLoc;
    }

    void TransformsAndLighting::setSolidsProjectionLoc(GLuint solidsProjectionLoc) {
        TransformsAndLighting::solidsProjectionLoc = solidsProjectionLoc;
    }

    GLuint TransformsAndLighting::getSolidsModelLoc() const {
        return solidsModelLoc;
    }

    void TransformsAndLighting::setSolidsModelLoc(GLuint solidsModelLoc) {
        TransformsAndLighting::solidsModelLoc = solidsModelLoc;
    }

    GLuint TransformsAndLighting::getSolidsNormalMatrixLoc() const {
        return solidsNormalMatrixLoc;
    }

    void TransformsAndLighting::setSolidsNormalMatrixLoc(GLuint solidsNormalMatrixLoc) {
        TransformsAndLighting::solidsNormalMatrixLoc = solidsNormalMatrixLoc;
    }

    GLuint TransformsAndLighting::getPointLightsViewLoc() const {
        return pointLightsViewLoc;
    }

    void TransformsAndLighting::setPointLightsViewLoc(GLuint pointLightsViewLoc) {
        TransformsAndLighting::pointLightsViewLoc = pointLightsViewLoc;
    }

    GLuint TransformsAndLighting::getPointLightsProjectionLoc() const {
        return pointLightsProjectionLoc;
    }

    void TransformsAndLighting::setPointLightsProjectionLoc(GLuint pointLightsProjectionLoc) {
        TransformsAndLighting::pointLightsProjectionLoc = pointLightsProjectionLoc;
    }

    GLuint TransformsAndLighting::getPointLightsModelLoc() const {
        return pointLightsModelLoc;
    }

    void TransformsAndLighting::setPointLightsModelLoc(GLuint pointLightsModelLoc) {
        TransformsAndLighting::pointLightsModelLoc = pointLightsModelLoc;
    }

    GLuint TransformsAndLighting::getDepth2DLightSpaceTrMatrixDirLoc() const {
        return depth2DLightSpaceTrMatrixDirLoc;
    }

    void TransformsAndLighting::setDepth2DLightSpaceTrMatrixDirLoc(GLuint depth2DLightSpaceTrMatrixDirLoc) {
        TransformsAndLighting::depth2DLightSpaceTrMatrixDirLoc = depth2DLightSpaceTrMatrixDirLoc;
    }

    const float TransformsAndLighting::getDirNearPlane() const {
        return dirNearPlane;
    }

    const float TransformsAndLighting::getDirFarPlane() const {
        return dirFarPlane;
    }

    const float TransformsAndLighting::getPointNearPlane() const {
        return pointNearPlane;
    }

    const float TransformsAndLighting::getPointFarPlane() const {
        return solidsFarPlane;
    }

}
//
