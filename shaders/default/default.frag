#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;
in vec4 FragPosLightSpace;
in vec3 Tangent;
in vec3 Bitangent;
in mat3 TBN;

struct Material {
  sampler2D diffuse;
  sampler2D normal;
  sampler2D specular;
  float shininess;
};

struct PointLight {
  vec3 position;
  float constant;
  float linear;
  float quadratic;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec4 color;
  float intensity;
  bool enabled;
  samplerCube shadowMap;
  float farPlane;
};

struct DirectionalLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec4 color;
  float intensity;
  bool enabled;
  mat4 lightSpaceMatrix;
  sampler2D shadowMap;
};

#define MAX_POINT_LIGHTS 5
#define MAX_DIRECTIONAL_LIGHTS 5

uniform int numberOfPointLights;
uniform int numberOfDirectionalLights;
uniform vec3 camPos;
uniform bool lightsEnabled;
uniform Material material;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];

float DirectLightShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap) {
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;

  if(projCoords.z > 1.0)
    return 1.0;

  float bias = 0.010;
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

  for(int x = -1; x <= 1; ++x) {
    for(int y = -1; y <= 1; ++y) {
      float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
      shadow += (projCoords.z - bias > pcfDepth) ? 1.0 : 0.0;
    }
  }
  shadow /= 9.0;

  return shadow;
}

float PointLightShadowCalculation(PointLight light, vec3 fragPos) {
    vec3 fragToLight = fragPos - light.position;
    float currentDepth = length(fragToLight);
    vec3 lightSpaceCoords = normalize(fragToLight);

    float bias = 0.05;
    float shadow = 0.0;
    float samples = 0.0;

    int sampleSize = 2;
    for (int x = -sampleSize; x <= sampleSize; ++x) {
        for (int y = -sampleSize; y <= sampleSize; ++y) {
            for (int z = -sampleSize; z <= sampleSize; ++z) {
                vec3 offset = vec3(x, y, z) * 0.1;
                float closestDepth = texture(light.shadowMap, lightSpaceCoords + offset).r;
                closestDepth *= light.farPlane;
                shadow += (currentDepth - bias > closestDepth) ? 1.0 : 0.0;
                samples++;
            }
        }
    }
    shadow /= samples;
    
    return shadow;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  vec3 ambient = light.ambient;
  vec3 diffuse = light.diffuse * diff;
  vec3 specular = light.specular * spec;

  float shadow = PointLightShadowCalculation(light, fragPos);

  diffuse *= (1.0 - shadow);
  specular *= (1.0 - shadow);

  ambient *= attenuation * light.color.rgb * light.intensity * vec3(texture(material.diffuse, TexCoord));
  diffuse *= attenuation * light.color.rgb * light.intensity * vec3(texture(material.diffuse, TexCoord));
  specular *= attenuation * light.color.rgb * light.intensity * vec3(texture(material.specular, TexCoord));

  return ambient + diffuse + specular;
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

  vec3 ambient = light.ambient;
  vec3 diffuse = light.diffuse * diff;
  vec3 specular = light.specular * spec;

  float shadow = DirectLightShadowCalculation(light.lightSpaceMatrix * vec4(fragPos, 1.0), light.shadowMap);

  diffuse *= (1.0 - shadow);
  specular *= (1.0 - shadow);
  
  ambient *= light.color.rgb * light.intensity * vec3(texture(material.diffuse, TexCoord));
  diffuse *= light.color.rgb * light.intensity * vec3(texture(material.diffuse, TexCoord));
  specular *= light.color.rgb * light.intensity * vec3(texture(material.specular, TexCoord));

  return ambient + diffuse; /*+ specular;*/
}

void main() {
  vec3 normal = normalize(Normal);
  vec3 viewDir = normalize(camPos - FragPos);

  vec3 result = vec3(0.0);

  if(lightsEnabled) {
    for(int i = 0; i < numberOfPointLights; ++i) {
      if(!pointLights[i].enabled) {
        continue;
      }
      result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
    }

    for(int i = 0; i < numberOfDirectionalLights; ++i) {
      if(!directionalLights[i].enabled) {
        continue;
      }
      result += CalcDirectionalLight(directionalLights[i], normal, FragPos, viewDir);
    }
  } else {
    result = vec3(texture(material.diffuse, TexCoord));
  }

  FragColor = vec4(result, 1.0);
}
