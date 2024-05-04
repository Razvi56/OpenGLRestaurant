//
// Created by Razvan on 1/28/2024.
//

#ifndef LAB8_BLINN_PHONG_DIRLIGHT_H
#define LAB8_BLINN_PHONG_DIRLIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"


namespace gps{
    class DirLight { ///only one instance of this makes sense

    public:
        DirLight(const glm::vec3 &direction, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                 const gps::Shader &solidObjectsShader);

        const glm::vec3 &getDirection() const;

        void setDirection(const glm::vec3 &direction);

        const glm::vec3 &getAmbient() const;

        void setAmbient(const glm::vec3 &ambient);

        const glm::vec3 &getDiffuse() const;

        void setDiffuse(const glm::vec3 &diffuse);

        const glm::vec3 &getSpecular() const;

        void setSpecular(const glm::vec3 &specular);

        const Shader &getSolidObjectsShader() const;

        void setSolidObjectsShader(const Shader &solidObjectsShader);

        GLuint getDirectionLoc() const;

        void setDirectionLoc(GLuint directionLoc);

        GLuint getAmbientLoc() const;

        void setAmbientLoc(GLuint ambientLoc);

        GLuint getDiffuseLoc() const;

        void setDiffuseLoc(GLuint diffuseLoc);

        GLuint getSpecularLoc() const;

        void setSpecularLoc(GLuint specularLoc);

    private:
        glm::vec3 direction;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        gps::Shader solidObjectsShader;

        GLuint directionLoc;
        GLuint ambientLoc;
        GLuint diffuseLoc;
        GLuint specularLoc;
    };
}


#endif //LAB8_BLINN_PHONG_DIRLIGHT_H
