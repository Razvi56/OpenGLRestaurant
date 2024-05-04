#version 410 core

in vec3 fNormal;
in vec4 fPosWorld;
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

uniform sampler2D depthMapTexture;
uniform samplerCube depthCubeMapTexture[2];

uniform PointLight pointLights[2];
uniform DirLight globalIllumination;
uniform mat4 view;
uniform float far_plane;

float shininess = 32.0f;

vec3 ambientResult;
vec3 diffuseResult;
vec3 specularResult;



float shadowCalculationPointLight(vec3 fragPosWorld, vec3 pointLightPosWorld, int index){
    vec3 fragToLight = fragPosWorld - pointLightPosWorld;
    float closestDepth = texture(depthCubeMapTexture[index], fragToLight).r;
    closestDepth*= far_plane;
    float currentDepth = length(fragToLight);
    if (currentDepth > far_plane){
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
    float closestDepth = texture(depthMapTexture, normalizedCoordinates.xy).r;
    float currentDepth = normalizedCoordinates.z;

    float bias = max(0.05f * (1.0f - dot(fNormal, globalIllumination.directionEye)), 0.005f);
    float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;
    return shadow;
}

/*calcPointLight(pointLights[i], normalEye, fPosEye.xyz, viewDirN, i)*/

vec3 calcPointLight(PointLight pointLight, vec3 normalEye, vec3 fPosEyeP, vec3 viewDirN, int index, vec3 fPosWorldP){
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

    float shadow = shadowCalculationPointLight(fPosWorldP, pointLight.position, index);

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




vec3 computeLightComponents(vec4 fPosEye)
{
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    //transform normal
    vec3 normalEye = normalize(fNormal);

    //compute view direction
    vec3 viewDirN = normalize(- fPosEye.xyz);

    result += calcDirLight(globalIllumination, normalEye, viewDirN);
    for (int i=0;i<2;i++){
        result += calcPointLight(pointLights[i], normalEye, fPosEye.xyz, viewDirN, i, fPosWorld.xyz);
    }
    return result;
}

float computeFog(vec4 fragmentPosEyeSpace)
{
    float fogDensity = 0.05f;
    float fragmentDistance = length(fragmentPosEyeSpace.xyz);
    float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2.0));

    return clamp(fogFactor, 0.0, 1.0);
}



void main()
{
    vec3 fogColor = vec3(0.7, 0.7, 0.7); // Light gray fog

    vec4 fPosEye = view * fPosWorld;
    vec3 result = computeLightComponents(fPosEye);


    vec3 color = min(result, 1.0f);

    float fogFactor = computeFog(fPosEye);
    vec3 finalColor = mix(fogColor, result, fogFactor);

    fColor = vec4(finalColor, 1.0f);
}
