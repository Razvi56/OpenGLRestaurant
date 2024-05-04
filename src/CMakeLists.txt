cmake_minimum_required(VERSION 3.27)
project(Lab8_Blinn_Phong)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(glew STATIC IMPORTED)
set_target_properties(glew PROPERTIES
        IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib/libglew32.a"
)

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/lib/libglfw3.a"
)

add_executable(Lab8_Blinn_Phong Camera.cpp main.cpp Mesh.cpp Model3D.cpp Shader.cpp stb_image.cpp tiny_obj_loader.cpp
        TransformsAndLighting.cpp
        ModelTransforms.cpp
        SceneLighting.cpp
        DirLight.cpp
        PointLight.cpp
        RenderModes.h)

target_link_options(Lab8_Blinn_Phong PRIVATE -static-libgcc -static-libstdc++ -static)

find_package(OpenGL REQUIRED)

target_link_libraries(Lab8_Blinn_Phong glew glfw OpenGL::GL)