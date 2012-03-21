#version 150

in vec4 vPosition;
in vec4 vAmbientDiffuseColor;
in vec3 vNormal; //must be in position 3 for teapot
in vec4 vSpecularColor;
in float vSpecularExponent;
in int   vlightsourcein;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 ambient_light;
uniform vec4 light_position;
uniform vec4 light_color;

uniform vec4 fdiffuse;
uniform vec4 fambient;
uniform vec4 fnormal;
uniform vec4 flightDir;
uniform vec4 fhalfVec;
uniform float fdist;

float dist;
out vec4 color;
flat out int   vlightsourceout;

out vec4 AmbientDiffuseColor;
out vec4 SpecularColor;
out float SpecularExponent;
out vec3 position;
out vec3 vN;

void
main()
{
   
    vlightsourceout = vlightsourcein;
	if ( vlightsourcein == 0 )
	{
		vec4 veyepos = model_view*vPosition;
		vec4 normal = vec4(vNormal, 0);
		vec3 L = normalize( light_position.xyz - veyepos.xyz);
		vec3 N = normalize((model_view * normal).xyz);
		vec3 V = normalize(-veyepos.xyz);
		vec3 H = normalize(L+V);
	
		gl_Position = projection*veyepos;
		vec4 ambient = ambient_light*vAmbientDiffuseColor;
	
		vec4 diffuse = light_color * vAmbientDiffuseColor * max(0.0,dot(L, N));
	
		vec4 specular = light_color * vSpecularColor * pow(max(0.0, dot(N, H)), vSpecularExponent);

		if(dot( L, N) < 0){
			specular = vec4(0, 0, 0, 1);
		}
	
		color = ambient + diffuse + specular;
	}
	else
	{
		vec4 vNormal = vec4(vNormal, 0.0);
		AmbientDiffuseColor = vAmbientDiffuseColor;
		SpecularColor = vSpecularColor;
		SpecularExponent = vSpecularExponent;
	
		vec4 veyepos = model_view*vPosition;
	


		vN = normalize(model_view * vNormal).xyz;
	
		//spec = vec4(1,1,1,1);
		position = veyepos.xyz;
		gl_Position = projection * veyepos;

	}
}
