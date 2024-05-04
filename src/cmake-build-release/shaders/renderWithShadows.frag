#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec4 fragPosLightSpace;

out vec4 fColor;

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 directionEye;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SceneLighting {
    DirLight globalIllumination;
    PointLight pointLights[2];
};

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

uniform sampler2D depth2DMapTexture;
uniform samplerCube depthCubeMapTexture[2];

uniform SceneLighting sceneLighting;
uniform mat4 view;
uniform float far_planePoint;

float shininess = 32.0f;

vec3 ambientResult;
vec3 diffuseResult;
vec3 specularResult;

float shadowCalculationPointLight(vec3 fragPosEye, vec3 pointLightPosEye, int index){
    vec3 fragToLight = fragPosEye - pointLightPosEye;
    float closestDepth = texture(depthCubeMapTexture[index], fragToLight).r;
    closestDepth*= far_planePoint;
    float currentDepth = length(fragToLight);
    if (currentDepth > far_planePoint){
        return 0.0f;
    }
    float bias = 0.05;
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

float shadowCalculationDirLight(){
    vec3 normalizedCoordinates = fragPosLightSpace.xyz / fragPosLightSpace.w;
    normalizedCoordinates = normalizedCoordinates * 0.5 + 0.5;
    if (normalizedCoordinates.z > 1.0f){
        return 0.0f;
    }
    float closestDepth = texture(depth2DMapTexture, normalizedCoordinates.xy).r;
    float currentDepth = normalizedCoordinates.z;

    float bias = max(0.05f * (1.0f - dot(fNormal, sceneLighting.globalIllumination.directionEye)), 0.005f);
    float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
    return shadow;
}

vec3 calcPointLight(PointLight pointLight, vec3 normalEye, vec3 fPosEyeP, vec3 viewDirN, int index){
    vec3 lightPosEye = vec3(view * vec4(pointLight.position, 1.0));
    vec3 lightDirEye = normalize(lightPosEye - fPosEyeP.xyz);
    float dist = distance(lightPosEye, fPosEyeP.xyz);
    float att = 1.0 / (pointLight.constant + pointLight.linear * dist+ pointLight.quadratic * dist * dist);

    vec3 halfVector = normalize(viewDirN + lightDirEye);

    float diffCoef = max(dot(normalEye, lightDirEye), 0.0f);
    float specCoeff = pow(max(dot(normalEye, halfVector), 0.0f), shininess);

    vec3 totalAmbientThisLight = pointLight.ambient * texture(diffuseTexture, fTexCoords).rgb * att;
    vec3 totalDiffuseThisLight = pointLight.diffuse * diffCoef * texture(diffuseTexture, fTexCoords).rgb * att;
    vec3 totalSpecularThisLight = pointLight.specular * specCoeff * texture(diffuseTexture, fTexCoords).rgb * att;

    float shadow = shadowCalculationPointLight(fPosEyeP, lightPosEye, index);

    return (totalAmbientThisLight + (1.0f - shadow) * (totalDiffuseThisLight + totalSpecularThisLight));
}

vec3 calcDirLight(DirLight dirLight, vec3 normalEye, vec3 viewDirN){
    vec3 lightDirN = normalize(dirLight.directionEye);
    vec3 halfVector = normalize(viewDirN + lightDirN);
    float diffCoef = max(dot(normalEye, lightDirN), 0.0);
    float specCoeff = pow(max(dot(normalEye, halfVector), 0.0), shininess);

    vec3 totalAmbientThisLight = dirLight.ambient * texture(diffuseTexture, fTexCoords).rgb;
    vec3 totalDiffuseThisLight = dirLight.diffuse * diffCoef * texture(diffuseTexture, fTexCoords).rgb;
    vec3 totalSpecularThisLight = dirLight.specular * specCoeff * texture(specularTexture, fTexCoords).rgb;

    float shadow = shadowCalculationDirLight();

    return (totalAmbientThisLight + (1.0f - shadow) * (totalDiffuseThisLight + totalSpecularThisLight));
}

vec3 computeLightComponents()
{
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    //transform normal
    vec3 normalEye = normalize(fNormal);

    //compute view direction
    vec3 viewDirN = normalize(- fPosEye.xyz);

    //result += calcDirLight(sceneLighting.globalIllumination, normalEye, viewDirN);
    for (int i=0;i<2;i++){
        result += calcPointLight(sceneLighting.pointLights[i], normalEye, fPosEye.xyz, viewDirN, i);
    }
    return result;
}


void main()
{
    vec3 result = computeLightComponents();

    vec3 color = min(result, 1.0f);

    fColor = vec4(color, 1.0f);
}
