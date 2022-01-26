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

layout(location=0) in vec3 objCoords;
layout(location=1) in vec3 objNormals;
layout(location=2) in vec2 objTexCoords;
layout(location=3) in vec4 Coords;
layout(location=4) in vec2 TexCoords;
layout(location=5) in vec3 skyCoords;


uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform uint object;

uniform float waveTime;

out vec2 texCoordsExport;
out vec3 normalExport;
out vec3 SkytexCoordsExport;
out vec3 FragPos;

vec4 coords;

void main(void)
{   
    coords = vec4(objCoords, 1.0f);
    normalExport = objNormals;
    texCoordsExport = objTexCoords;
    if (object == WATER)
    {
        coords = vec4(objCoords, 1.0f);
        coords.y += 0.1f * (sin(coords.x + waveTime) + cos(coords.z + waveTime)) - 0.1f;
        texCoordsExport = vec2(objTexCoords.t + 0.1f, objTexCoords.s);
    }
    if (object == SKYBOX)
    {
        SkytexCoordsExport = skyCoords;
        coords = vec4(skyCoords, 1.0);
    }
    FragPos = vec3(modelMat * coords);
    gl_Position = projMat * viewMat * vec4(FragPos,1.0);
}
