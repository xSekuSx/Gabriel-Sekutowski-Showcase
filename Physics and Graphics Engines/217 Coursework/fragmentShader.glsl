#version 430 core

#define DEBUG_SPHERE 0
#define DEBUG_AABB 1
#define DEBUG_PLANE 2
#define DEBUG_OBB 3
#define SKYBOX 4
#define MUSTANG 5
#define DEFAULT_CUBE 6
#define GROUND 7
#define WATER 8
#define RACETRACK 9
#define BACK_WHEELS 10
#define LEFT_WHEEL 11
#define RIGHT_WHEEL 12
#define CONE 13
#define ROAD_BLOCK 14

in vec3 FragPos;
in vec3 SkytexCoordsExport;
in vec3 normalExport;
in vec2 texCoordsExport;

struct Light
{
	vec4 ambCols;
	vec4 difCols;
	vec4 specCols;
	vec3 coords;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
};

struct Material
{
    vec4 ambRefl;
    vec4 difRefl;
    vec4 specRefl;
    vec4 emitCols;
    float shininess;
};

uniform Material Car;
uniform Material Rest;
uniform Light Lheadlight;
uniform Light Rheadlight;
uniform Light Sun;
uniform bool lights;
uniform vec3 viewPos;
uniform float Time;

uniform sampler2D grassTex;
uniform sampler2D waterTex;
uniform sampler2D raceTex;
uniform sampler2D mustangTex;
uniform sampler2D wheelTex;
uniform sampler2D coneTex;
uniform sampler2D road_blockTex;

uniform samplerCube skyboxTex;
uniform uint object;

vec4 colors;
out vec4 colorsOut;

vec4 fieldTexColor, waterTexColor, raceTexColor, mustangTexColor, wheelTexColor, skyboxTexColor, coneTexColor, road_blockTexColor;
vec3 normal, light0Direction, SpotLightDirection, LightDirection, SunDirection;
vec4 fAndBDif;

vec3 CalcDirLight(Light DirLight, vec3 normal, vec3 viewDir)
{
    //ambient
    vec4 ambient = DirLight.ambCols;
    //diffuse
    vec3 lightDir = normalize(-DirLight.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = diff * DirLight.difCols;
    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
    vec4 specular = spec * DirLight.specCols;

    vec4 result = ambient + diffuse + specular;
    return vec3(result);
}

vec3 CalcPointLight(Light PointLight, vec3 normal, vec3 FragPos, vec3 viewDir)
{
    //ambient
    vec4 ambient = PointLight.ambCols;
    //diffuse
    vec3 lightDir = normalize(PointLight.coords - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = diff * PointLight.difCols;
    //specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
    vec4 specular = spec * PointLight.specCols;

    vec4 result = ambient + diffuse + specular;

    return vec3(result);
}

vec3 CalcSpotLight(Light SpotLight, vec3 normal, vec3 FragPos, vec3 viewDir)
{
    //ambient
    vec3 ambient = vec3(SpotLight.ambCols);
    //diffuse
    vec3 lightDir = normalize(SpotLight.coords - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * vec3(SpotLight.difCols);
    //specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);
    vec3 specular = vec3(SpotLight.specCols) * spec;  

    float theta = dot(lightDir , normalize(-SpotLight.direction));
    float epsilon = (SpotLight.cutOff - SpotLight.outerCutOff);
    float intensity = clamp((theta - SpotLight.outerCutOff) / epsilon, 0.0, 1.0);

    float distance    = length(SpotLight.coords - FragPos);
    float attenuation = 1.0 / (SpotLight.constant + SpotLight.linear * distance + SpotLight.quadratic * (distance * distance)); 

    ambient *= intensity * attenuation;
    diffuse *= intensity  * attenuation;
    specular *= intensity  * attenuation;

    vec3 result = ambient + diffuse + specular;
    return result;
}

void main(void)
{  
    fieldTexColor = texture(grassTex, texCoordsExport);
    waterTexColor = texture(waterTex, texCoordsExport);
    raceTexColor = texture(raceTex, texCoordsExport);
    mustangTexColor = texture(mustangTex, texCoordsExport);
    wheelTexColor = texture(wheelTex, texCoordsExport);
    coneTexColor = texture(coneTex, texCoordsExport);
    road_blockTexColor = texture(road_blockTex, texCoordsExport);
    skyboxTexColor = texture(skyboxTex, SkytexCoordsExport);

    normal = normalize(normalExport);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(Sun, normal, viewDir);
    if (lights){
        result += CalcSpotLight(Lheadlight, normal, FragPos, viewDir);
        result += CalcSpotLight(Rheadlight, normal, FragPos, viewDir);
    }
    if (object == MUSTANG) {
        colorsOut = vec4(result,1.0) * (mustangTexColor) * vec4(length(Car.ambRefl), length(Car.difRefl), length(Car.specRefl), 2.0f)/2.0f;
    }
    if (object == DEFAULT_CUBE) {
        colorsOut =  vec4(result,1.0);
    }
    if (object == GROUND){
        colorsOut =  fieldTexColor* vec4(result,1.0) * vec4(length(Rest.ambRefl), length(Rest.difRefl), length(Rest.specRefl), 2.0f)/2.0f;
    }
    if (object == WATER) {
        colorsOut =  waterTexColor* vec4(result,1.0) * vec4(length(Rest.ambRefl), length(Rest.difRefl), length(Rest.specRefl), 2.0f)/2.0f;
        colorsOut.a = 0.7f;
    } 
    if (object == RACETRACK){
        colorsOut =  raceTexColor* vec4(result,1.0) * vec4(length(Rest.ambRefl), length(Rest.difRefl), length(Rest.specRefl), 2.0f)/2.0f;
    }
    if (object == BACK_WHEELS || object == LEFT_WHEEL || object == RIGHT_WHEEL){
        colorsOut =  wheelTexColor* vec4(result,1.0) * vec4(length(Rest.ambRefl), length(Rest.difRefl), length(Rest.specRefl), 2.0f)/2.0f;
    }
    if (object == CONE){
        colorsOut =  coneTexColor * vec4(result,1.0) * vec4(length(Rest.ambRefl), length(Rest.difRefl), length(Rest.specRefl), 2.0f)/2.0f;
    }
    if (object == ROAD_BLOCK){
        colorsOut =  road_blockTexColor * vec4(result,1.0) * vec4(length(Rest.ambRefl), length(Rest.difRefl), length(Rest.specRefl), 2.0f)/2.0f;
    }
    if (object == DEBUG_SPHERE){
        colorsOut =  vec4(1.0, 0.0, 1.0, 1.0);
    }
    if (object == DEBUG_AABB){
        colorsOut =  vec4(1.0, 0.0, 1.0, 1.0);
    }
    if (object == DEBUG_PLANE){
        colorsOut =  vec4(1.0, 0.0, 1.0, 1.0);
    }
    if (object == DEBUG_OBB){
        colorsOut =  vec4(1.0, 0.0, 1.0, 1.0);
    }
    if (object == SKYBOX) {
        colorsOut = vec4(vec3(skyboxTexColor) * Sun.ambCols[0],1.0f);
    }
}