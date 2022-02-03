#version 420 core

#define TERRAIN 0
#define WATER 1
#define TREE 2
#define SKYBOX 3
#define CLOUDS 4

struct light
{
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   vec3 coords;
};
  
struct Material
{
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
};

in vec3 FragPos;
in vec3 normalExport;
in vec2 texCoordsExport;
in float height;

in float cloud_opacity;

in vec3 SkytexCoordsExport;

uniform uint object;
uniform vec3 viewDir;
uniform Material terrainMaterial;
uniform Material waterMaterial;
uniform light light0;
uniform sampler2D sandTex;
uniform sampler2D grassTex;
uniform sampler2D snowTex;
uniform sampler2D gravelTex;

uniform samplerCube skyboxTex;
vec4 skyboxTexColor;


vec3 normal, lightDirection;
vec4 MaterialDiffuse;

vec4 textureMap;

out vec4 colorsExport;

vec3 calculatelight(Material material,vec3 skin){

	// ambient
	vec3 ambient = light0.ambient * material.ambient;
  	
	// diffuse 
	vec3 norm = normalExport;
	vec3 lightDir = normalize(vec3(light0.coords) - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light0.diffuse * diff * material.diffuse;
    
	// specular
	vec3 specular;
	if (material.shininess != 0){
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		specular = material.specular * spec;
	}
	else{
		specular = vec3(0);
	}

	return (ambient + diffuse + specular) * skin;
}

vec4 result;

void main(void)
{
	if (object == TERRAIN){
		if(height < 30)
		{
			textureMap = texture(sandTex, texCoordsExport);
		}
		else if(height < 55)
		{
			float diff = (height - 30);
			if (diff < 10){ // a value between 0, 1
				diff *= 0.1;
				textureMap = (texture(grassTex, texCoordsExport) * diff) + (texture(sandTex, texCoordsExport) * (1-diff));
			}
			else{
				textureMap = texture(grassTex, texCoordsExport);
			}
		}
		else if(height < 65)
		{
			float diff = (height - 55);
			if (diff < 10){ // a value between 0, 1
				diff *= 0.1;
				textureMap = (texture(gravelTex, texCoordsExport) * diff) + (texture(grassTex, texCoordsExport) * (1-diff));
			}
			else{
				textureMap = texture(gravelTex, texCoordsExport);
			}
		}
		else 
		{
			float diff = (height - 65);
			if (diff < 10){ // a value between 0, 1
				diff *= 0.1;
				textureMap = (texture(snowTex, texCoordsExport) * diff) + (texture(gravelTex, texCoordsExport) * (1-diff));
			}
			else{
				textureMap = texture(snowTex, texCoordsExport);
			}
		}
		result = vec4(calculatelight(terrainMaterial,textureMap.rgb),1.0);
		colorsExport = result;
	}

	else if (object == WATER){
		vec3 R = reflect(viewDir, normalExport);
		textureMap = vec4(texture(skyboxTex, R).rgb + vec3(-0.1,-0.1,0.2), 1.0);
		result = vec4(calculatelight(waterMaterial,textureMap.rgb), 0.6);
		colorsExport =  result;
	}

	else if (object == TREE){
		result = vec4(0.6,0.445,0.327,1.0);
		result = vec4(calculatelight(terrainMaterial,result.rgb), 1.0);
		colorsExport =  result;
	}
	else if (object == SKYBOX) {
		textureMap = texture(skyboxTex, SkytexCoordsExport);
        colorsExport = textureMap;
    }
	else if (object == CLOUDS) {
		result = vec4(cloud_opacity * 0.8,cloud_opacity* 0.8,cloud_opacity* 0.8,cloud_opacity);
        colorsExport = result;
    }
}