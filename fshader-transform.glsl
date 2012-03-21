#version 150 

flat in  int   vlightsourceout;
in  vec4 color;
out vec4 fColor;
in vec3 position;
in vec3 vN;

in vec4 AmbientDiffuseColor;
in vec4 SpecularColor;
in float SpecularExponent;

uniform vec4 light_position;
uniform vec4 light_color;
uniform vec4 ambient_light;



void main() 
{ 
	if ( vlightsourceout == 0 )
	{
		fColor = color;
	}
	else
	{
		float att,spotEffect;
		vec3 L = normalize( light_position.xyz - position.xyz);
		vec3 E = normalize(-position.xyz);
		vec3 N = normalize(vN);

		vec3 H = normalize(L+E);
		vec4 amb = AmbientDiffuseColor * ambient_light;
		vec4 diff = max(dot(L,N), 0.0) * AmbientDiffuseColor * light_color;
		vec4 spec = pow( max (dot(N,H), 0.0), SpecularExponent) *  SpecularColor * light_color;

		if(dot(L,N) > 0.0){

			//spotEffect = dot(normalize(gl_LightSource[0].spotDirection), normalize(-L));
		
		}
		
	
		fColor = amb + diff + spec;
	
	}
} 

