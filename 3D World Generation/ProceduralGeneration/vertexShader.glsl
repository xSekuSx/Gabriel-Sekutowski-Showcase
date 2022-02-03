#version 420 core

#define TERRAIN 0
#define WATER 1
#define TREE 2
#define SKYBOX 3
#define CLOUDS 4

layout(location=0) in vec4 objCoords;
layout(location=1) in vec3 objNormals;
layout(location=2) in vec2 objTexCoords;
layout(location=3) in vec3 skyCoords;

uniform vec4 globAmb;
uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform mat4 modelMat;

uniform uint object;
uniform float waveTime;
uniform float cloudTime;
vec4 coords;

out vec2 texCoordsExport;
out vec3 normalExport;
out float height;
out vec3 FragPos;
out vec3 SkytexCoordsExport;
out float cloud_opacity;

void main(void)
{
	height = objCoords.y;
	texCoordsExport = objTexCoords;
	coords = objCoords;
	if (object == WATER){
		coords.y += 0.25 * (sin((waveTime + coords.x)*0.25) + cos((waveTime + coords.z)*0.1));
		normalExport = normalize(vec3(coords.x, -1/(0.125 * cos(0.25 * (waveTime + coords.x))), coords.z));
		FragPos = vec3(coords);
		gl_Position = projMat * modelViewMat * coords;
		//vec3 tx = vec3(coords.x, 0.25 * cos(0.5 * (waveTime + coords.x)), coords.z);
		//vec3 tz = vec3(coords.x, -0.05 * sin(0.1 * (waveTime +coords.z)), coords.z);
	}
	else if (object == SKYBOX){
		FragPos = skyCoords;
		SkytexCoordsExport = skyCoords;
		coords = vec4(skyCoords, 1.0);
		gl_Position = projMat * modelViewMat * coords;
	}
	else if (object == CLOUDS){
		FragPos = vec3(coords);
		cloud_opacity = objNormals.x + cloudTime;
		if (cloud_opacity > 1){
			cloud_opacity -= 1;
		}

		normalExport = vec3(0,objNormals.y, objNormals.z);
		gl_Position = projMat * modelViewMat * coords;
	}
	else{
		FragPos = vec3(coords);
		normalExport = objNormals; 
		gl_Position = projMat * modelViewMat * coords;
	}
}