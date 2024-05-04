//
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC

#include <glew.h>

#endif

#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "Shader.hpp"
//#include "Model3D.hpp"
//#include "Camera.hpp"
#include "TransformsAndLighting.h"

#include <iostream>


int glWindowWidth = 2560;
int glWindowHeight = 1440;
int retina_width, retina_height;
int movableLightIndex = 0;
GLFWwindow *glWindow = NULL;


bool pressedKeys[1024];

gps::Model3D myModel;
gps::Model3D groundModel;
gps::Model3D cubeModel;
gps::Model3D restaurantModel;

GLuint depthCubeMapFBO[2] = {};
GLuint depthCubeMapTexture[2] = {};
GLuint depthMapFBO;
GLuint depthMapTexture;
glm::mat4 shadowTransforms[2][6] = {};

std::vector<glm::vec3> squarePoints = {
        glm::vec3(-3.0f, 0.5f, 3.0f), // Point A
        glm::vec3(3.0f, 0.5f, 3.0f),  // Point B
        glm::vec3(3.0f, 0.5f, -3.0f), // Point C
        glm::vec3(-3.0f, 0.5f, -3.0f) // Point D
};

bool isAnimationActive = false;
bool animationStarted = false;
int currentSegmentIndex = 0; // Start at the first segment (A to B)
float tCam = 0.0f; // Interpolation factor
float speedCam = 0.005f; // Speed of the animation

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

///

RenderMode currentRenderMode = RenderMode::SOLID;
///

GLenum glCheckError_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow *window, int width, int height) {
    fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
    glViewport(0, 0, width, height);
    gps::TransformsAndLighting *transformsAndLightingP = static_cast<gps::TransformsAndLighting *>(glfwGetWindowUserPointer(
            window));
    transformsAndLightingP->updateProjection(glm::perspective(glm::radians(45.0f),
                                                              (float) width / (float) height, 0.1f, 1000.0f));
    glWindowWidth = width;
    glWindowHeight = height;
}

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            pressedKeys[key] = true;
        else if (action == GLFW_RELEASE)
            pressedKeys[key] = false;
    }
}

double lastX = glWindowWidth / 2, lastY = glWindowHeight / 2;
bool firstMouse = true;

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = lastX - xpos;
    double yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.002f;

    gps::TransformsAndLighting *transformsAndLightingP = static_cast<gps::TransformsAndLighting *>(glfwGetWindowUserPointer(
            window));
    transformsAndLightingP->rotateCamera(yoffset * sensitivity, xoffset * sensitivity);
}

void processMovement(gps::TransformsAndLighting &tAL) {
    /*if (pressedKeys[GLFW_KEY_Q]) {
        for (int i = 1; i < 3; i++) {
            tAL.updateObjRotation(
                    glm::vec3(tAL.modelTransforms[i].rotation.x, tAL.modelTransforms[i].rotation.y - glm::radians(1.0f),
                              tAL.modelTransforms[i].rotation.z), i);
        }
    }

    if (pressedKeys[GLFW_KEY_E]) {
        for (int i = 1; i < 3; i++) {
            tAL.updateObjRotation(
                    glm::vec3(tAL.modelTransforms[i].rotation.x, tAL.modelTransforms[i].rotation.y + glm::radians(1.0f),
                              tAL.modelTransforms[i].rotation.z), i);
        }
    }*/

    if (pressedKeys[GLFW_KEY_W] && !isAnimationActive) {
        tAL.moveCamera(gps::MOVE_FORWARD);
    }

    if (pressedKeys[GLFW_KEY_S] && !isAnimationActive) {
        tAL.moveCamera(gps::MOVE_BACKWARD);
    }

    if (pressedKeys[GLFW_KEY_A] && !isAnimationActive) {
        tAL.moveCamera(gps::MOVE_LEFT);
    }

    if (pressedKeys[GLFW_KEY_D] && !isAnimationActive) {
        tAL.moveCamera(gps::MOVE_RIGHT);
    }

    if (pressedKeys[GLFW_KEY_1]) {
        currentRenderMode = RenderMode::SOLID;
    }
    if (pressedKeys[GLFW_KEY_2]) {
        currentRenderMode = RenderMode::WIREFRAME;
    }
    if (pressedKeys[GLFW_KEY_3]) {
        currentRenderMode = RenderMode::SMOOTH;
    }

    if (pressedKeys[GLFW_KEY_0]) {
        if(!isAnimationActive){
            isAnimationActive = true;
            animationStarted = true;
        }
    }
}


bool initOpenGLWindow() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //window scaling for HiDPI displays
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    //for sRBG framebuffer
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

    //for antialising
    glfwWindowHint(GLFW_SAMPLES, 4);

    glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
    if (!glWindow) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }

    glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
    glfwSetKeyCallback(glWindow, keyboardCallback);
    glfwSetCursorPosCallback(glWindow, mouseCallback);
    //glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(glWindow);

    glfwSwapInterval(1);

#if not defined (__APPLE__)
    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
#endif

    // get version info
    const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte *version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    //for RETINA display
    glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

    return true;
}

void initOpenGLState() {
    glClearColor(0.3f, 0.3f, 0.3f, 1.0);
    glViewport(0, 0, retina_width, retina_height);

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

    glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {
    myModel.LoadModel("objects/nanosuit/nanosuit.obj", "objects/nanosuit/");
    groundModel.LoadModel("objects/ground/ground.obj", "objects/ground/");
    cubeModel.LoadModel("objects/cube/cube.obj", "objects/cube/");
    restaurantModel.LoadModel("objects/restaurant/Resto_Upload.obj", "objects/restaurant/");
}


void cleanup() {

    glfwDestroyWindow(glWindow);
    //close GL context and any other GLFW resources
    glfwTerminate();
}


float updateT(float t, bool movingTowardsB, float speed){
    if (movingTowardsB) {
        t += speed;
        if (t >= 1.0f) {
            t = 1.0f;
            movingTowardsB = false;
        }
    } else {
        t -= speed;
        if (t <= 0.0f) {
            t = 0.0f;
            movingTowardsB = true;
        }
    }
    return t;
}

glm::vec3 moveObjectAlongSquare(glm::vec3 objectPosition) {
    // Calculate the current segment's start and end points
    glm::vec3 startPoint = squarePoints[currentSegmentIndex];
    glm::vec3 endPoint = squarePoints[(currentSegmentIndex + 1) % squarePoints.size()];

    // Update the interpolation factor based on the speed and delta time
    tCam += speedCam;

    // If we reach the end of the segment, reset t and move to the next segment
    if (tCam >= 1.0f) {
        tCam = 0.0f; // Reset interpolation factor
        currentSegmentIndex = (currentSegmentIndex + 1) % squarePoints.size();
    }

    // Update the object's position using linear interpolation
    return glm::mix(startPoint, endPoint, tCam);
}

int main(int argc, const char *argv[]) {

    if (!initOpenGLWindow()) {
        glfwTerminate();
        return 1;
    }

    glfwSetKeyCallback(glWindow, keyboardCallback);
    glfwSetCursorPosCallback(glWindow, mouseCallback);
    glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    initOpenGLState();
    initObjects();

    gps::Shader solidsShader;
    gps::Shader pointLightsShader;
    gps::Shader depth2DMapShader;
    gps::Shader depthCubeMapShader;

    GLint success;
    GLchar infoLog[512];

    
    
    pointLightsShader.loadShader("shaders/lightCube.vert","shaders/lightCube.glsl", "shaders/lightCube.frag");
//    pointLightsShader.useShaderProgram();
//    GLint numActiveUniforms = 0;
//    glGetProgramiv(pointLightsShader.shaderProgram, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
//
//    GLint maxUniformNameLength = 0;
//    glGetProgramiv(pointLightsShader.shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
//    std::vector<GLchar> uniformNameData(maxUniformNameLength);
//
//    for (GLint uniform = 0; uniform < numActiveUniforms; ++uniform) {
//        GLint arraySize = 0;
//        GLenum type = 0;
//        GLsizei actualLength = 0;
//        glGetActiveUniform(
//                pointLightsShader.shaderProgram, uniform, uniformNameData.size(),
//                &actualLength, &arraySize, &type, &uniformNameData[0]
//        );
//        std::string uniformName(uniformNameData.begin(), uniformNameData.begin() + actualLength);
//        GLint location = glGetUniformLocation(pointLightsShader.shaderProgram, uniformName.c_str());
//
//        std::cout << "Uniform #" << uniform << " Type: " << type
//                  << " Name: " << uniformName << " Location: " << location << std::endl;
//    }


    depth2DMapShader.loadShader("shaders/depthDir.vert", "shaders/depthDir.frag");
/*    depth2DMapShader.useShaderProgram();
    GLint numActiveUniforms = 0;
    glGetProgramiv(depth2DMapShader.shaderProgram, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

    GLint maxUniformNameLength = 0;
    glGetProgramiv(depth2DMapShader.shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
    std::vector<GLchar> uniformNameData(maxUniformNameLength);

    for (GLint uniform = 0; uniform < numActiveUniforms; ++uniform) {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        glGetActiveUniform(
                depth2DMapShader.shaderProgram, uniform, uniformNameData.size(),
                &actualLength, &arraySize, &type, &uniformNameData[0]
        );
        std::string uniformName(uniformNameData.begin(), uniformNameData.begin() + actualLength);
        GLint location = glGetUniformLocation(depth2DMapShader.shaderProgram, uniformName.c_str());

        std::cout << "Uniform #" << uniform << " Type: " << type
                  << " Name: " << uniformName << " Location: " << location << std::endl;
    }*/


    depthCubeMapShader.loadShader("shaders/depth.vert", "shaders/depth.glsl", "shaders/depth.frag");
/*    depthCubeMapShader.useShaderProgram();
    GLint numActiveUniforms = 0;
    glGetProgramiv(depthCubeMapShader.shaderProgram, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

    GLint maxUniformNameLength = 0;
    glGetProgramiv(depthCubeMapShader.shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
    std::vector<GLchar> uniformNameData(maxUniformNameLength);

    for (GLint uniform = 0; uniform < numActiveUniforms; ++uniform) {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        glGetActiveUniform(
                depthCubeMapShader.shaderProgram, uniform, uniformNameData.size(),
                &actualLength, &arraySize, &type, &uniformNameData[0]
        );
        std::string uniformName(uniformNameData.begin(), uniformNameData.begin() + actualLength);
        GLint location = glGetUniformLocation(depthCubeMapShader.shaderProgram, uniformName.c_str());

        std::cout << "Uniform #" << uniform << " Type: " << type
                  << " Name: " << uniformName << " Location: " << location << std::endl;
    }*/


    solidsShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
    solidsShader.useShaderProgram();
    GLint numActiveUniforms = 0;
    glGetProgramiv(solidsShader.shaderProgram, GL_ACTIVE_UNIFORMS, &numActiveUniforms);

    GLint maxUniformNameLength = 0;
    glGetProgramiv(solidsShader.shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength);
    std::vector<GLchar> uniformNameData(maxUniformNameLength);

    for (GLint uniform = 0; uniform < numActiveUniforms; ++uniform) {
        GLint arraySize = 0;
        GLenum type = 0;
        GLsizei actualLength = 0;
        glGetActiveUniform(
                solidsShader.shaderProgram, uniform, uniformNameData.size(),
                &actualLength, &arraySize, &type, &uniformNameData[0]
        );
        std::string uniformName(uniformNameData.begin(), uniformNameData.begin() + actualLength);
        GLint location = glGetUniformLocation(solidsShader.shaderProgram, uniformName.c_str());

        std::cout << "Uniform #" << uniform << " Type: " << type
                  << " Name: " << uniformName << " Location: " << location << std::endl;
    }

    ///!!!!!!!!!!!!!!!!!!!!!!!!!The constructors of the following objects only work after shader initialization because they initialize uniform locations

    glm::vec3 pointA1 = glm::vec3(-3.0f, 0.5f, 1.5f); // Example point A
    glm::vec3 pointB1 = glm::vec3(3.0f, 0.5f, 1.5f); // Example point B

    glm::vec3 pointA2 = glm::vec3(3.0f, 0.5f, 0.0f); // Example point A
    glm::vec3 pointB2 = glm::vec3(-3.0f, 0.5f, 0.0f); // Example point B

    gps::PointLight pointLight1 = gps::PointLight(pointA1, 1.0f, 0.22, 0.2,
                                        glm::vec3(0.3f,0.14f,0.05f),
                                        glm::vec3(3.0f,1.68,0.6),
                                        glm::vec3(5.0f, 4.0,3.0),
                                        solidsShader);

    gps::PointLight pointLight2 = gps::PointLight(pointA2, 1.0f, 0.22, 0.2,
                                        glm::vec3(0.04f, 0.08f, 0.16f),
                                        glm::vec3(0.0f, 1.77f, 3.0f),
                                        glm::vec3(2.5f,4.1f,5.0f),
                                        solidsShader);


    gps::DirLight globalIllumination = gps::DirLight(glm::vec3(0.0f, 1.0f, 1.0f),
                                                     glm::vec3(0.00f),
                                                     glm::vec3(0.00f),
                                                     glm::vec3(0.00f),
                                           solidsShader);

    gps::SceneLighting sceneLighting = gps::SceneLighting(globalIllumination, {pointLight1, pointLight2});


    gps::ModelTransforms solidModelTransforms[NR_OF_SOLID_OBJECTS] = {gps::ModelTransforms(glm::vec3(0.0f, -0.99f, 0.0f),
                                                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                                                      glm::vec3(1.0f, 1.0f, 1.0f)),///restaurant
                                                                 gps::ModelTransforms(glm::vec3(0.0f, -1.0f, 0.0f),
                                                                      glm::vec3(0.0f, 0.0f, 0.0f),
                                                                      glm::vec3(1.0f, 1.0f, 1.0f)),/**ground*/};



    gps::Model3D solidObjectMeshMods[NR_OF_SOLID_OBJECTS] = {restaurantModel, groundModel};
    gps::Model3D lightObjectMeshMod = cubeModel;


    gps::TransformsAndLighting transformsAndLighting =
            gps::TransformsAndLighting(
                    solidModelTransforms,
                    glm::perspective(glm::radians(45.0f), (float) glWindowWidth / (float) glWindowHeight,
                                     0.1f, 1000.0f),
                    gps::Camera(glm::vec3(0.0f, 0.0f, 2.5f),
                                glm::vec3(0.0f, 0.0f, -10.0f),
                                glm::vec3(0.0f, 1.0f, 0.0f)),
                    sceneLighting,
                    solidsShader, pointLightsShader, depth2DMapShader, depthCubeMapShader,
                    solidObjectMeshMods, lightObjectMeshMod,
                    retina_width, retina_height
                    );
    glfwSetWindowUserPointer(glWindow, &transformsAndLighting);


    ///initUniforms(geometryTransform, sceneLighting); ??Not needed



    float speed = 0.002f;
    float t = 0.0f;
    bool movingTowardsB = true;

    float t1 = 0.0f;
    float speed1 = 0.002f;
    bool movingTowardsB1 = true;

    GLuint unloc = glGetUniformLocation(pointLightsShader.shaderProgram, "fact");


    while (!glfwWindowShouldClose(glWindow)) {
        if (movingTowardsB) {
            t += speed;
            if (t >= 1.0f) {
                t = 1.0f;
                movingTowardsB = false;
            }
        } else {
            t -= speed;
            if (t <= 0.0f) {
                t = 0.0f;
                movingTowardsB = true;
            }
        }

        if (movingTowardsB1) {
            t1 += speed1;
            if (t1 > 0.3f) {
                t1 = 0.3f;
                movingTowardsB1 = false;
            }
        } else {
            t1 -= speed1;
            if (t1 < 0.0f) {
                t1 = 0.0f;
                movingTowardsB1 = true;
            }
        }

        pointLightsShader.useShaderProgram();
        glUniform1f(unloc, t1);

        if(animationStarted){
            transformsAndLighting.setCamera(squarePoints[0],glm::vec3(0,0,1));

            tCam = 0;
            currentSegmentIndex = 0;
            animationStarted = false;
        }

        if(isAnimationActive){
            transformsAndLighting.setCamera(moveObjectAlongSquare(transformsAndLighting.getCameraPosition()),glm::vec3(0,0,1));
            /*if(glm::distance(transformsAndLighting.getCameraPosition(),squarePoints[3]) < 0.1){
                isAnimationActive = false;
            }*/
        }

        glm::vec3 newPosition1 = glm::mix(pointA1, pointB1, t);
        glm::vec3 newPosition2 = glm::mix(pointA2, pointB2, t);

        transformsAndLighting.getSceneLighting().updatePointLightPosition(0,newPosition1);
        transformsAndLighting.getSceneLighting().updatePointLightPosition(1,newPosition2);

        processMovement(transformsAndLighting);
        transformsAndLighting.renderScene(currentRenderMode);

        glfwPollEvents();
        glfwSwapBuffers(glWindow);
    }

    cleanup();

    return 0;
}
