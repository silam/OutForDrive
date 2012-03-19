/*
 Si Lam
 Assignment 2: Out for Drive

 **/

#include <GL/Angel.h>
#include <math.h>
#include "matrix_stack.h"
#include <assert.h>

#pragma comment(lib, "glew32.lib")


typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

//store window width and height
int ww=1500, wh=1100;

#define Cos(th) cos(M_PI/180*(th))
#define Sin(th) sin(M_PI/180*(th))

#define M_PI 3.14159265358979323846

// CAR
GLuint vCarAmbientDiffuseColor; //Ambient and Diffuse can be the same for the material
GLuint vCarSpecularColor;
GLuint vCarSpecularExponent;


// WHEEEL
GLuint vWheelSide1AmbientDiffuseColor; //Ambient and Diffuse can be the same for the material
GLuint vWheelSide1SpecularColor;
GLuint vWheelSide1SpecularExponent;

// wheel2
GLuint vWheelSide2AmbientDiffuseColor; //Ambient and Diffuse can be the same for the material
GLuint vWheelSide2SpecularColor;
GLuint vWheelSide2SpecularExponent;

// cylinder
GLuint vWheelCylinderAmbientDiffuseColor; //Ambient and Diffuse can be the same for the material
GLuint vWheelCylinderSpecularColor;
GLuint vWheelCylinderSpecularExponent;

//Some light properties
GLuint light_position;
GLuint light_color;
GLuint ambient_light;

//////////////////////////
GLboolean  pointCameraAt;
GLfloat lenszoom;
GLfloat dollyzoom;
GLfloat atX, atZ;

/////////////////////////
// simple object vertices
/////////////////////////
vec4 objVerts[36];
vec4 objColors[36];
GLuint objvao[1];
GLuint objvbo[2];

/////////////////////////
// the chase camera
/////////////////////////

vec4 viewportcamlookdirection = vec4(0, 0, 0.07f, 0);
vec4 viewpointcam;

vec4 * chasecamVers;
vec4 chasecamColors[400];
GLuint chasecamvao[1];
GLuint chasecamvbo[2];
int totalchasecamverts;
vec4 chasecamlookdirection;



GLint switchcamera;
GLint turnEyeRight;

//these are the extra variables needed for the Stage
GLfloat tx, ty, tz;
GLfloat rx, ry, rz;


//////////////////////////////////////////////////
// current position of the car
//////////////////////////////////////////////////
GLfloat currentX;
GLfloat currentZ;

//////////////////////////////////////////////////
//our modelview and perspective matrices
mat4 mv, p;
//////////////////////////////////////////////////

//and we'll need pointers to our shader variables
GLuint model_view;
GLuint projection;
GLuint vPosition;
GLuint vColor;
GLuint program;

/////////////////////////
// stage vertices
/////////////////////////
vec4 stageVerts[6];
vec4 stageColors[6];
GLuint stagevao[1];
GLuint stagevbo[2];

/////////////////////////
// car vertices
/////////////////////////
vec4 carVerts[36];
vec4 carColors[36];
GLuint carvao[1];
GLuint carvbo[2];
vec4   carNormals[36];

/////////////////////////
// wheel side vertices
/////////////////////////
vec4 wheelSide1Verts[75];
vec4 wheelSide1Normals[75];

vec4 wheelSide2Verts[75];
vec4 wheelSide2Normals[75];

vec4 wheelside1Colors[75];
vec4 wheelside2Colors[75];

GLuint wheelside1vao[1];
GLuint wheelside1vbo[2];

GLuint wheelside2vao[1];
GLuint wheelside2vbo[2];

GLfloat maxTurnWheel = 45.00f;

/////////////////////////
// wheel cylinder
/////////////////////////
vec4 wheelCylinderVers[414];
vec4 vWheelCylinderNormals[414];

vec4 wheelCylinderColors[500];

GLuint cylindervao[1];
GLuint cylindervbo[2];

/////////////////////////
// eye
/////////////////////////
vec4 eyeVerts[144];
vec4 eyeColors[144];
GLuint eyevao[1];
GLuint eyevbo[2];
GLfloat turnEyeAngle;

/////////////////////////
// the head
/////////////////////////
vec4 * headVers;
vec4 headColors[400];
GLuint headvao[1];
GLuint headvbo[2];
int totalheadverts;

//////////////////////////////////////////////////
// // turn the wheel left/right angle
//////////////////////////////////////////////////
GLfloat turnAngle;
GLfloat turnCarAngle = 0;

/////////////////////////
// move car
/////////////////////////
//GLfloat moveForward;
GLfloat moveStepZ = 0.0005;
GLfloat rollangle;

GLfloat moveStepX, vectorLen;

GLboolean  bmoveForward;

/////////////////////////
// stack
/////////////////////////
matrix_stack stack;

/////////////////////////////////////////
// generateObject
/////////////////////////////////////////

void generateSimpleObjects(){
	for(int i=0; i<6; i++){
		objColors[i] = vec4(i/6.0f, 1.0, 1.0, 1.0); //front
	}
	objVerts[0] = vec4(0.05f, -0.05f, 0.05f, 1.0);
	objVerts[1] = vec4(0.05f, 0.05f, 0.05f, 1.0);
	objVerts[2] = vec4(-0.05f, 0.05f, 0.05f, 1.0);
	objVerts[3] = vec4(-0.05f, 0.05f, 0.05f, 1.0);
	objVerts[4] = vec4(-0.05f, -0.05f, 0.05f, 1.0);
	objVerts[5] = vec4(0.05f, -0.05f, 0.05f, 1.0);
	

	for(int i=6; i<12; i++){
		objColors[i] = vec4(1.0, i/12.0f, 1.0, 1.0); //back
	}
	objVerts[6] = vec4(-0.05f, -0.05f, -0.05f, 1.0);
	objVerts[7] = vec4(-0.05f, 0.05f, -0.05f, 1.0);
	objVerts[8] = vec4(0.05f, 0.05f, -0.05f, 1.0);
	objVerts[9] = vec4(0.05f, 0.05f, -0.05f, 1.0);
	objVerts[10] = vec4(0.05f, -0.05f, -0.05f, 1.0);
	objVerts[11] = vec4(-0.05f, -0.05f, -0.05f, 1.0);

	for(int i=12; i<18; i++){
		objColors[i] = vec4(1.0, 1.0,i/18.0f, 1.0); //left
	}
	objVerts[12] = vec4(0.05f, 0.05f, 0.05f, 1.0);
	objVerts[13] = vec4(0.05f, -0.05f, 0.05f, 1.0);
	objVerts[14] = vec4(0.05f, -0.05f, -0.05f, 1.0);
	objVerts[15] = vec4(0.05f, -0.05f, -0.05f, 1.0);
	objVerts[16] = vec4(0.05f, 0.05f, -0.05f, 1.0);
	objVerts[17] = vec4(0.05f, 0.05f, 0.05f, 1.0);
	
	for(int i=18; i<24; i++){
		objColors[i] = vec4(i/24.0f, 0.0, 0.0, 1.0); //right
	}
	objVerts[18] = vec4(-0.05f, 0.05f, -0.05f, 1.0);
	objVerts[19] = vec4(-0.05f, -0.05f, -0.05f, 1.0);
	objVerts[20] = vec4(-0.05f, -0.05f, 0.05f, 1.0);
	objVerts[21] = vec4(-0.05f, -0.05f, 0.05f, 1.0);
	objVerts[22] = vec4(-0.05f, 0.05f, 0.05f, 1.0);
	objVerts[23] = vec4(-0.05f, 0.05f, -0.05f, 1.0);
	
	for(int i=24; i<30; i++){
		objColors[i] = vec4(0.0, 0.0, i/30.0f, 1.0); //top
	}
	objVerts[24] = vec4(0.05f, 0.05f, 0.05f, 1.0);
	objVerts[25] = vec4(0.05f, 0.05f, -0.05f, 1.0);
	objVerts[26] = vec4(-0.05f, 0.05f, -0.05f, 1.0);
	objVerts[27] = vec4(-0.05f, 0.05f, -0.05f, 1.0);
	objVerts[28] = vec4(-0.05f, 0.05f, 0.05f, 1.0);
	objVerts[29] = vec4(0.05f, 0.05f, 0.05f, 1.0);
	
	for(int i=30; i<36; i++){
		objColors[i] = vec4(0.0, 1.0, i/36.0f, 1.0); //bottom
	}
	objVerts[30] = vec4(0.05f, -0.05f, -0.05f, 1.0);
	objVerts[31] = vec4(0.05f, -0.05f, 0.05f, 1.0);
	objVerts[32] = vec4(-0.05f, -0.05f, 0.05f, 1.0);
	objVerts[33] = vec4(-0.05f, -0.05f, 0.05f, 1.0);
	objVerts[34] = vec4(-0.05f, -0.05f, -0.05f, 1.0);
	objVerts[35] = vec4(0.05f, -0.05f, -0.05f, 1.0);


	/////////////////////////////
	// Objects
	/////////////////////////////

	 // Create and initialize any buffer objects
	glBindVertexArray( objvao[0] );
	glGenBuffers( 2, &objvbo[0] );
    glBindBuffer( GL_ARRAY_BUFFER, objvbo[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(objVerts), objVerts, GL_STATIC_DRAW);
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//and now our colors for each vertex
	glBindBuffer( GL_ARRAY_BUFFER, objvbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(objColors), objColors, GL_STATIC_DRAW );
	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);


}

/////////////////////////////////////////
//generateEyes
/////////////////////////////////////////
void generateEyes()
{
		
	int point = 0;
	double angleincrement = 15;
	for ( double angle = 0; angle <= 360; angle += angleincrement)
	{
		eyeColors[point] = vec4(0.0f, 0.0f, 0.0f, 1.0);
		eyeVerts[point++] = vec4(0.0f,	0.0f, 0.0f, 1.0); //point 1
		
		eyeColors[point] = vec4(0.0f, 0.0f, 0.0f, 1.0);
		eyeVerts[point++] = vec4(cos(angle*M_PI/180), 0.0f, -sin(angle*M_PI/180), 1.0); //point 2
		
		eyeColors[point] = vec4(0.0f, 0.0f, 0.0f, 1.0);
		eyeVerts[point++] = vec4(cos((angle+angleincrement)*M_PI/180), 0.0f, -sin((angle+angleincrement)*M_PI/180), 1.0); //point 3
		

	}
}

/*
GLuint generateVertexArrayBufferData(GLuint vao[1], GLuint vbo[2], vec4 verts[], vec4 colors[])
{
	glGenVertexArrays( 1, &vao[0] );
	// Create and initialize any buffer objects
	glBindVertexArray( vao[0] );
	
	glGenBuffers( 2, &vbo[0] );
    glBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
    
	glBufferData( GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//and now our colors for each vertex
	glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW );
	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);

	return vao[0];
}
*/

/////////////////////////////////////////
// generateWheelSides
/////////////////////////////////////////
void generateWheelSides()
{
	int side = 1; // the outer side of the wheel
	
	int point = 0;
	double angleincrement = 15;
	for ( double angle = 0; angle <= 360; angle += angleincrement)
	{
		wheelside1Colors[point] = vec4(1.0f, 1.0f, 1.0f, 1.0);
		point4 a = wheelSide1Verts[point++] = vec4(0.0f,		side*(-1.0f), 0.0f, 1.0); //point 1
		
		wheelside1Colors[point] = vec4(cos(angle*M_PI/180), -1.0f, -sin(angle*M_PI/180), 1.0); //point 2
		point4 b = wheelSide1Verts[point++] = vec4(cos(angle*M_PI/180), side*(-1.0f), -sin(angle*M_PI/180), 1.0); //point 2
		
		wheelside1Colors[point] = vec4(cos((angle+angleincrement)*M_PI/180), -1.0f, -sin((angle+angleincrement)*M_PI/180), 1.0); //point 3
		point4 c = wheelSide1Verts[point++] = vec4(cos((angle+angleincrement)*M_PI/180), side*(-1.0f), -sin((angle+angleincrement)*M_PI/180), 1.0); //point 3
		
		vec3 normal = normalize(cross(c-b, a -b));

		wheelSide1Normals[point-3] = normal;
		wheelSide1Normals[point-2] = normal;
		wheelSide1Normals[point-1] = normal;

	}

	side = -1; // the inner side of the wheel
	point = 0;
	angleincrement = 15;
	for ( double angle = 0; angle <= 360; angle += angleincrement)
	{
		point4 a = wheelSide2Verts[point++] = vec4(0.0f,		side*(-1.0f), 0.0f, 1.0); 
		

		point4 b = wheelSide2Verts[point++] = vec4(cos(angle*M_PI/180), side*(-1.0f), -sin(angle*M_PI/180), 1.0); 
		wheelside2Colors[point] = vec4(0.5f,0.5f,0.5f, 1.0); 

		point4 c = wheelSide2Verts[point++] = vec4(cos((angle+angleincrement)*M_PI/180), side*(-1.0f), -sin((angle+angleincrement)*M_PI/180), 1.0); //point 3
		wheelside2Colors[point] = vec4(0.5f ,0.5f,0.5f, 1.0); 

		vec3 normal = normalize(cross(c-b, a -b));

		wheelSide2Normals[point-3] = normal;
		wheelSide2Normals[point-2] = normal;
		wheelSide2Normals[point-1] = normal;
	}


	for ( int i = 0; i < 410 ; i++)
	{
		if ( i %9 )
			wheelCylinderColors[i] = vec4(1.0f ,0.7f,0.5f, 1.0); 
		else
			wheelCylinderColors[i] = vec4(0.0f ,0.0f,0.5f, 1.0); 
	}

	//
	// cylinder of the wheel
	int p = 0;
	for ( int i = 0; i < 410 ; i += 6)
	{
	
		point4 a = wheelCylinderVers[i] =   wheelSide1Verts[(p+1)%3==0?(p+1+1+1):(p+1)];
		point4 b = wheelCylinderVers[i+1] = wheelSide1Verts[(p+2)%3==0?(p+2+1+1):(p+2)];
		point4 c = wheelCylinderVers[i+2] = wheelSide2Verts[(p+1)%3==0?(p+1+1+1):(p+1)];

		vec3 normal = normalize(cross(c - b, a - b));
		vWheelCylinderNormals[i] = normal;
		vWheelCylinderNormals[i+1] = normal;
		vWheelCylinderNormals[i+2] = normal;

		a = wheelCylinderVers[i+3] = wheelSide1Verts[(p+2)%3==0?(p+2+1+1):(p+2)];
		b = wheelCylinderVers[i+4] = wheelSide2Verts[(p+2)%3==0?(p+2+1+1):(p+2)];
		c = wheelCylinderVers[i+5] = wheelSide2Verts[(p+1)%3==0?(p+1+1+1):(p+1)];
		normal = normalize(cross(c - b, a - b));
		vWheelCylinderNormals[i+3] = normal;
		vWheelCylinderNormals[i+4] = normal;
		vWheelCylinderNormals[i+5] = normal;


		p++;

	}


	///////////////////////////////////
	// wheel sides
	///////////////////////////////////

	vWheelSide1AmbientDiffuseColor = glGetAttribLocation(program, "vAmbientDiffuseColor");
	vWheelSide1SpecularColor = glGetAttribLocation(program, "vSpecularColor");
	vWheelSide1SpecularExponent = glGetAttribLocation(program, "vSpecularExponent");

	
	glGenVertexArrays( 1, &wheelside1vao[0] );
	// Create and initialize any buffer objects
	glBindVertexArray( wheelside1vao[0] );
	
	glGenBuffers( 2, &wheelside1vbo[0] );
    glBindBuffer( GL_ARRAY_BUFFER, wheelside1vbo[0] );
    
	glBufferData( GL_ARRAY_BUFFER, sizeof(wheelSide1Verts) + sizeof(wheelSide1Normals), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(wheelSide1Verts), wheelSide1Verts);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(wheelSide1Verts), sizeof(wheelSide1Normals), wheelSide1Normals);


	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(0));

	//and now our colors for each vertex
	/*glBindBuffer( GL_ARRAY_BUFFER, wheelside1vbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(wheelside1Colors), wheelside1Colors, GL_STATIC_DRAW );
	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	*/

	///
	// Initilize shader lighting parameters
	/*
	point4 light_position(0.0, 0.0,-1.0,0.0);
	color4 light_ambient(0.2, 0.2,0.2,1.0);
	color4 light_diffuse(1.0, 1.0,1.0,1.0);
	color4 light_specular(1.0, 1.0,1.0,1.0);


	color4 material_ambient(1.0, 0.5, 0.5, 1.0);
	color4 material_diffuse(1.0, 0.5, 1.0, 1.0);
	color4 material_specular(1.0, 0.0, 1.0, 1.0);
	float material_shininess = 50.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	*/

	//wheelside1vao[0] = generateVertexArrayBufferData(wheelside1vao, wheelside1vbo, wheelSide1Verts, wheelside1Colors);

	///////////////////////////
	// wheel side 2
	//////////////////////////

	vWheelSide2AmbientDiffuseColor = glGetAttribLocation(program, "vAmbientDiffuseColor");
	vWheelSide2SpecularColor = glGetAttribLocation(program, "vSpecularColor");
	vWheelSide2SpecularExponent = glGetAttribLocation(program, "vSpecularExponent");

	glGenVertexArrays( 1, &wheelside2vao[0] );
	// Create and initialize any buffer objects
	glBindVertexArray( wheelside2vao[0] );
	
	glGenBuffers( 2, &wheelside2vbo[0] );
    
	glBindBuffer( GL_ARRAY_BUFFER, wheelside2vbo[0] );
    
	glBufferData( GL_ARRAY_BUFFER, sizeof(wheelSide2Verts)+ sizeof(wheelSide2Normals), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(wheelSide2Verts), wheelSide2Verts);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(wheelSide2Verts), sizeof(wheelSide2Normals), wheelSide2Normals);



	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);


	vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(0));

	//and now our colors for each vertex
	/*glBindBuffer( GL_ARRAY_BUFFER, wheelside2vbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(wheelside2Colors), wheelside2Colors, GL_STATIC_DRAW );
	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);*/

	/*
	light_position = point4(0.0, 0.0,-1.0,0.0);
	light_ambient  = color4(0.2, 0.2,0.2,1.0);
	light_diffuse  = color4(1.0, 1.0,1.0,1.0);
	light_specular = color4(1.0, 1.0,1.0,1.0);


	material_ambient = color4(1.0, 0.5, 0.5, 1.0);
	material_diffuse = color4(1.0, 0.5, 1.0, 1.0);
	material_specular = color4(1.0, 0.0, 1.0, 1.0);
	material_shininess = 10.0;

	 ambient_product = light_ambient * material_ambient;
	 diffuse_product = light_diffuse * material_diffuse;
	 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	*/

	//////////////////////////
	// display cylinder
	///////////////////////////

	vWheelCylinderAmbientDiffuseColor = glGetAttribLocation(program, "vAmbientDiffuseColor");
	vWheelCylinderSpecularColor = glGetAttribLocation(program, "vSpecularColor");
	vWheelCylinderSpecularExponent = glGetAttribLocation(program, "vSpecularExponent");

	glGenVertexArrays( 1, &cylindervao[0] );
	// Create and initialize any buffer objects
	glBindVertexArray( cylindervao[0] );
	
	glGenBuffers( 2, &cylindervbo[0] );
    glBindBuffer( GL_ARRAY_BUFFER, cylindervbo[0] );
    
	glBufferData( GL_ARRAY_BUFFER, sizeof(wheelCylinderVers) + sizeof(vWheelCylinderNormals), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(wheelCylinderVers), wheelCylinderVers);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(wheelCylinderVers), sizeof(vWheelCylinderNormals), vWheelCylinderNormals);



	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//and now our colors for each vertex
	glBindBuffer( GL_ARRAY_BUFFER, cylindervbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(wheelCylinderColors), wheelCylinderColors, GL_STATIC_DRAW );
	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);


}
/////////////////////////////////////////
// generateStage
/////////////////////////////////////////
void generateStage(){
	for(int i=0; i<6; i++){
		stageColors[i] = vec4(0.0, 1.0, 0.0, 1.0); //bottom
	}
	stageVerts[0] = vec4(1.0f, -1.0f, -1.0f, 1.0);
	stageVerts[1] = vec4(1.0f, -1.0f, 1.0f, 1.0);
	stageVerts[2] = vec4(-1.0f, -1.0f, 1.0f, 1.0);
	stageVerts[3] = vec4(-1.0f, -1.0f, 1.0f, 1.0);
	stageVerts[4] = vec4(-1.0f, -1.0f, -1.0f, 1.0);
	stageVerts[5] = vec4(1.0f, -1.0f, -1.0f, 1.0);


	/////////////////////////////////
	// stage
	/////////////////////////////////
	// Create a vertex array object
    glGenVertexArrays( 1, &stagevao[0] );

    // Create and initialize any buffer objects
	glBindVertexArray( stagevao[0] );
	glGenBuffers( 2, &stagevbo[0] );
    glBindBuffer( GL_ARRAY_BUFFER, stagevbo[0] );
    glBufferData( GL_ARRAY_BUFFER, sizeof(stageVerts), stageVerts, GL_STATIC_DRAW);
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//and now our colors for each vertex
	glBindBuffer( GL_ARRAY_BUFFER, stagevbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(stageColors), stageColors, GL_STATIC_DRAW );
	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
}
/////////////////////////////////////////
// generateCar
/////////////////////////////////////////

void generateCar(){

	int index = 0;
	//for(int i=0; i<6; i++){
	//	carColors[i] = vec4(0.0, 1.0, 1.0, 1.0); //front
	//}
	//
	carVerts[0] = vec4( 0.05f, -0.05f, 0.05f, 1.0);
	carVerts[1] = vec4( 0.05f,  0.05f, 0.05f, 1.0);
	carVerts[2] = vec4(-0.05f,  0.05f, 0.05f, 1.0);

	// calculate normal vectore for car
	vec3 normal = normalize(cross(carVerts[2] - carVerts[1], carVerts[0] - carVerts[1]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 


	carVerts[3] = vec4(-0.05f,  0.05f, 0.05f, 1.0);
	carVerts[4] = vec4(-0.05f, -0.05f, 0.05f, 1.0);
	carVerts[5] = vec4( 0.05f, -0.05f, 0.05f, 1.0);
	
	// calculate normal vectore for car
	//normal = normalize(cross( carVerts[5] - carVerts[4], carVerts[3] - carVerts[4]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 


	for(int i=6; i<12; i++){
		carColors[i] = vec4(1.0, 0.0, 1.0, 1.0); //back
	}
	carVerts[6] = vec4(-0.05f, -0.05f, -0.05f, 1.0);
	carVerts[7] = vec4(-0.05f, 0.05f, -0.05f, 1.0);
	carVerts[8] = vec4(0.05f, 0.05f, -0.05f, 1.0);


	// calculate normal vectore for car
	normal = normalize(cross(carVerts[7] - carVerts[6], carVerts[8] - carVerts[7]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 

	carVerts[9] = vec4(0.05f, 0.05f, -0.05f, 1.0);
	carVerts[10] = vec4(0.05f, -0.05f, -0.05f, 1.0);
	carVerts[11] = vec4(-0.05f, -0.05f, -0.05f, 1.0);

	// calculate normal vectore for car
	normal = normalize(cross(carVerts[10] - carVerts[9], carVerts[11] - carVerts[10]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 


	for(int i=12; i<18; i++){
		carColors[i] = vec4(1.0, 1.0, 0.0, 1.0); //left
	}
	carVerts[12] = vec4(0.05f, 0.05f, 0.05f, 1.0);
	carVerts[13] = vec4(0.05f, -0.05f, 0.05f, 1.0);
	carVerts[14] = vec4(0.05f, -0.05f, -0.05f, 1.0);

	// calculate normal vectore for car
	normal = normalize(cross(carVerts[13] - carVerts[12], carVerts[14] - carVerts[13]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 

	carVerts[15] = vec4(0.05f, -0.05f, -0.05f, 1.0);
	carVerts[16] = vec4(0.05f, 0.05f, -0.05f, 1.0);
	carVerts[17] = vec4(0.05f, 0.05f, 0.05f, 1.0);
	

	// calculate normal vectore for car
	normal = normalize(cross(carVerts[16] - carVerts[15], carVerts[17] - carVerts[16]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 


	for(int i=18; i<24; i++){
		carColors[i] = vec4(1.0, 0.0, 0.0, 1.0); //right
	}
	carVerts[18] = vec4(-0.05f, 0.05f, -0.05f, 1.0);
	carVerts[19] = vec4(-0.05f, -0.05f, -0.05f, 1.0);
	carVerts[20] = vec4(-0.05f, -0.05f, 0.05f, 1.0);

	// calculate normal vectore for car
	normal = normalize(cross(carVerts[19] - carVerts[18], carVerts[20] - carVerts[19]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 


	carVerts[21] = vec4(-0.05f, -0.05f, 0.05f, 1.0);
	carVerts[22] = vec4(-0.05f, 0.05f, 0.05f, 1.0);
	carVerts[23] = vec4(-0.05f, 0.05f, -0.05f, 1.0);
	
	// calculate normal vectore for car
	normal = normalize(cross(carVerts[22] - carVerts[21], carVerts[23] - carVerts[22]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 


	for(int i=24; i<30; i++){
		carColors[i] = vec4(0.0, 0.0, 1.0, 1.0); //top
	}
	carVerts[24] = vec4(0.05f, 0.05f, 0.05f, 1.0);
	carVerts[25] = vec4(0.05f, 0.05f, -0.05f, 1.0);
	carVerts[26] = vec4(-0.05f, 0.05f, -0.05f, 1.0);

	// calculate normal vectore for car
	normal = normalize(cross(carVerts[25] - carVerts[24], carVerts[26] - carVerts[25]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 

	carVerts[27] = vec4(-0.05f, 0.05f, -0.05f, 1.0);
	carVerts[28] = vec4(-0.05f, 0.05f, 0.05f, 1.0);
	carVerts[29] = vec4(0.05f, 0.05f, 0.05f, 1.0);
	
	// calculate normal vectore for car
	normal = normalize(cross(carVerts[29] - carVerts[28], carVerts[28] - carVerts[27]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal;

	for(int i=30; i<36; i++){
		carColors[i] = vec4(0.0, 1.0, 0.0, 1.0); //bottom
	}
	carVerts[30] = vec4(0.05f, -0.05f, -0.05f, 1.0);
	carVerts[31] = vec4(0.05f, -0.05f, 0.05f, 1.0);
	carVerts[32] = vec4(-0.05f, -0.05f, 0.05f, 1.0);

	// calculate normal vectore for car
	normal = normalize(cross(carVerts[31] - carVerts[30], carVerts[32] - carVerts[31]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal;

	carVerts[33] = vec4(-0.05f, -0.05f, 0.05f, 1.0);
	carVerts[34] = vec4(-0.05f, -0.05f, -0.05f, 1.0);
	carVerts[35] = vec4(0.05f, -0.05f, -0.05f, 1.0);

	// calculate normal vectore for car
	normal = normalize(cross(carVerts[34] - carVerts[33], carVerts[35] - carVerts[34]));
	carNormals[index++] = normal; 
	carNormals[index++] = normal; 
	carNormals[index++] = normal;

	/////////////////////////////
	// Car
	/////////////////////////////

	////// Assignment 3
	//glBindVertexArray( carvao[0] );
	//glGenBuffers( 2, &carvbo[0] );
 //   glBindBuffer( GL_ARRAY_BUFFER, carvbo[0] );
 //   glBufferData( GL_ARRAY_BUFFER, sizeof(carVerts), carVerts, GL_STATIC_DRAW);
	//vPosition = glGetAttribLocation(program, "vPosition");
	//glEnableVertexAttribArray(vPosition);
	//glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	////and now our colors for each vertex
	//glBindBuffer( GL_ARRAY_BUFFER, carvbo[1] );
	//glBufferData( GL_ARRAY_BUFFER, sizeof(carColors), carColors, GL_STATIC_DRAW );
	//vColor = glGetAttribLocation(program, "vColor");
	//glEnableVertexAttribArray(vColor);
	//glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);


	////grab pointers for our modelview and perspecive uniform matrices
	model_view = glGetUniformLocation(program, "model_view");
	projection = glGetUniformLocation(program, "projection");

	////// Assigment 4 


	vCarAmbientDiffuseColor = glGetAttribLocation(program, "vAmbientDiffuseColor");
	vCarSpecularColor = glGetAttribLocation(program, "vSpecularColor");
	vCarSpecularExponent = glGetAttribLocation(program, "vSpecularExponent");

	light_position = glGetUniformLocation(program, "light_position");
	light_color = glGetUniformLocation(program, "light_color");
	ambient_light = glGetUniformLocation(program, "ambient_light");


	 // Create and initialize any buffer objects
	glBindVertexArray( carvao[0] );
	glGenBuffers( 2, &carvbo[0] );
    glBindBuffer( GL_ARRAY_BUFFER, carvbo[0] );

	//glBufferData( GL_ARRAY_BUFFER, spherevertcount*sizeof(vec4), sphere_verts, GL_STATIC_DRAW);
	glBufferData( GL_ARRAY_BUFFER, sizeof(carVerts) + sizeof(carNormals), NULL, GL_STATIC_DRAW);
	
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(carVerts), carVerts);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(carVerts), sizeof(carNormals), carNormals);

	// set up vertex array
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	//and now our colors for each vertex
	//glBindBuffer( GL_ARRAY_BUFFER, carvbo[1] );

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(0));


	///
	// Initilize shader lighting parameters
	/*
	point4 light_position(0.0, 0.0,-1.0,0.0);
	color4 light_ambient(0.2, 0.2,0.2,1.0);
	color4 light_diffuse(1.0, 1.0,1.0,1.0);
	color4 light_specular(1.0, 1.0,1.0,1.0);


	color4 material_ambient(1.0, 0.0, 0.5, 1.0);
	color4 material_diffuse(1.0, 1.0, 1.0, 1.0);
	color4 material_specular(1.0, 0.0, 1.0, 1.0);
	float material_shininess = 100.0;

	color4 ambient_product = light_ambient * material_ambient;
	color4 diffuse_product = light_diffuse * material_diffuse;
	color4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);
	*/

	


	//and now our colors for each vertex
	/*glBindBuffer( GL_ARRAY_BUFFER, carvbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(carColors), carColors, GL_STATIC_DRAW );
	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);*/


	
	
	glShadeModel(GL_FLAT);
}
/////////////////////////////////////////
// DrawTriagle
/////////////////////////////////////////
void DrawTriagle(GLuint vao[], int count)
{
	glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
	glUniformMatrix4fv(projection, 1, GL_TRUE, p);
	glBindVertexArray( vao[0] );
	glDrawArrays( GL_TRIANGLES, 0, count );
}
/////////////////////////////////////////
// DrawWheels
/////////////////////////////////////////
void DrawWheels(GLuint wheelside1vao[1],
				  GLuint wheelside2vao[2],
				  GLuint cylindervao[1], 
				  GLsizei sidecount, GLsizei cylindercount)
{
	glVertexAttrib4fv(vWheelSide1AmbientDiffuseColor, vec4(1, .5, 0, 1));
	glVertexAttrib4fv(vWheelSide1SpecularColor, vec4(1, 1, 1, 1));
	glVertexAttrib1f(vWheelSide1SpecularExponent, 10);

	DrawTriagle(wheelside1vao, sidecount);

	glVertexAttrib4fv(vWheelSide2AmbientDiffuseColor, vec4(1, .5, 0, 1));
	glVertexAttrib4fv(vWheelSide2SpecularColor, vec4(1, 1, 1, 1));
	glVertexAttrib1f(vWheelSide2SpecularExponent, 10);

	DrawTriagle(wheelside2vao, sidecount);
	
	
	// draw cylinder

	glVertexAttrib4fv(vWheelCylinderAmbientDiffuseColor, vec4(0.5, .5, 0.5, 1));
	glVertexAttrib4fv(vWheelCylinderSpecularColor, vec4(1, 0.5, 1, 1));
	glVertexAttrib1f(vWheelCylinderSpecularExponent, 5);

	DrawTriagle(cylindervao, cylindercount);

}
/////////////////////////////////////////
// displayWheels
/////////////////////////////////////////
void displayWheels()
{

	/////////////////////
	// First wheel : driver wheel
	////////////////////
	stack.push(mv);

	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);
		
		
	mv = mv * Translate(0.04, -0.97f, 0.0405); //0.065); // 0.04);
	mv = mv * RotateZ(90);
	mv = mv * RotateX(turnAngle);
    mv = mv * RotateY(-rollangle);
	mv = mv * Scale(0.025,0.007,0.025);

	glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);

    
	//glUniform4fv(ambient_light, 1, vec4(0.2, 0.2, 0.2, 1));
	//glUniform4fv(light_color, 1, vec4(1, 1, 1, 1));
	//glUniform4fv(light_position, 1, vec4(50,50, 50, 1)); //mv*vec4(50,50, 50, 1));

	
	DrawWheels(wheelside1vao,wheelside2vao,cylindervao, 75, 414);

	
	mv = stack.pop();

	/////////////////////////////////
	// seconf wheel: passenger wheel
	/////////////////////////////////
	stack.push(mv);


	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);
	
	
	mv = mv * Translate(-0.04, -0.97f, 0.0405);
	mv = mv * RotateZ(-90);
	mv = mv * RotateX(-turnAngle);
	mv = mv * RotateY(rollangle);
	mv = mv * Scale(0.025,0.007,0.025);

	DrawWheels(wheelside1vao,wheelside2vao,cylindervao, 75, 414);

	mv = stack.pop();

	/////////////////////////////////
	// third wheel
	/////////////////////////////////
	stack.push(mv);
	
	
	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);
	
		
	mv = mv * Translate(0.04, -0.97f, -0.0595); //-0.06 + 0.0005);
	mv = mv * RotateZ(90);
	
	mv = mv * RotateY(-rollangle);
	mv = mv * Scale(0.025,0.007,0.025);

	DrawWheels(wheelside1vao,wheelside2vao,cylindervao, 75, 414);
	
	mv = stack.pop();

	/////////////////////////////////
	// fourth wheel
	/////////////////////////////////
	stack.push(mv);
		
	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);
	
	
	mv = mv * Translate(-0.04, -0.97f, -0.0595);
	mv = mv * RotateZ(-90);
	
	mv = mv * RotateY(rollangle);
	mv = mv * Scale(0.025,0.007,0.025);
		
	DrawWheels(wheelside1vao,wheelside2vao,cylindervao, 75, 414);
		 

	mv = stack.pop();

}

/////////////////////////////////////////
// displayStage
/////////////////////////////////////////
void displayStage(void)
{
  
	DrawTriagle(stagevao, 36);
	   
}

void displayObjects(void)
{
	stack.push(mv);
		
	mv = mv * Translate(-0.8, -0.9, 0.8); 
	
	mv = mv * Scale(2,2,3 );
	
	DrawTriagle(objvao, 36);

	mv = stack.pop();


	//////////////
	///////////////
	stack.push(mv);

	
	mv = mv * Translate(0.8, -0.90, -0.8); 
		
	mv = mv * Scale(0.6,0.1,0.2);
	
	DrawTriagle(chasecamvao, totalchasecamverts);

	mv = stack.pop();

	//////////////
	///////////////
	stack.push(mv);

	
	mv = mv * Translate(-0.8, -0.90, -0.8); 
		
	mv = mv * Scale(0.1,0.5,0.1);
	
	DrawTriagle(chasecamvao, totalchasecamverts);

	mv = stack.pop();


	//////////////
	///////////////
	stack.push(mv);

	
	mv = mv * Translate(0.8, -0.90, 0.8); 
		
	mv = mv * Scale(0.15,0.1,0.2);
	
	DrawTriagle(chasecamvao, totalchasecamverts);

	mv = stack.pop();

}
/////////////////////////////////////////
// displayCar
/////////////////////////////////////////
void displayCar(void)
{
	stack.push(mv);
		
	
	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);
	
	mv = mv * Translate(0, -0.93, 0.0005); //0.025); // 0.05
	//mv = mv * Scale(1.0,0.5,3);
	mv = mv * Scale(0.5,0.25,1.5 );
	
	glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);

    
	glUniform4fv(ambient_light, 1, vec4(0.2, 0.2, 0.2, 1));
	glUniform4fv(light_color, 1, vec4(1, 1, 1, 1));
	glUniform4fv(light_position, 1, vec4(50,50, 50, 1)); //mv*vec4(50,50, 50, 1));

	glVertexAttrib4fv(vCarAmbientDiffuseColor, vec4(0, .5, 0, 1));
	glVertexAttrib4fv(vCarSpecularColor, vec4(1, 1, 1, 1));
	glVertexAttrib1f(vCarSpecularExponent, 10);


	DrawTriagle(carvao, 36);

	mv = stack.pop();
   
}

/////////////////////////////////////////
// generateHead
/////////////////////////////////////////
void generateChaseCamera()
{
	
	int subdiv = 10;
	int radius = 1;

	float step = (360.0/subdiv)*(M_PI/180.0);

	totalchasecamverts = ceil(subdiv/2.0)*subdiv * 6;

	if(chasecamVers){
		delete[] chasecamVers;
	}

	chasecamVers = new vec4[totalchasecamverts];

	int k = 0;
	for(float i = -M_PI/2; i<=M_PI/2; i+=step){
		for(float j = -M_PI; j<=M_PI; j+=step){
			//triangle 1
			chasecamVers[k]=   vec4(radius*sin(j)*cos(i), radius*cos(j)*cos(i), radius*sin(i), 1.0);
			chasecamColors[k] = vec4(1.0, 0.0, 1.0, 1.0);
			k++;
	
			chasecamVers[k]=   vec4(radius*sin(j)*cos(i+step), radius*cos(j)*cos(i+step), radius*sin(i+step), 1.0);
			chasecamColors[k] = vec4(1.0, 0.0, 1.0, 1.0);
			k++;
			
			chasecamVers[k]=   vec4(radius*sin((j+step))*cos((i+step)), radius*cos(j+step)*cos(i+step), radius*sin(i+step), 1.0);
			chasecamColors[k] = vec4(1.0, 0.0, 1.0, 1.0);
			k++;

			//triangle 2
			chasecamVers[k]=   vec4(radius*sin((j+step))*cos((i+step)), radius*cos(j+step)*cos(i+step), radius*sin(i+step), 1.0);
			chasecamColors[k] = vec4(1.0, 1.0, 1.0, 1.0);
			k++;

			chasecamVers[k]=   vec4(radius*sin(j+step)*cos(i), radius*cos(j+step)*cos(i), radius*sin(i), 1.0);
			chasecamColors[k] = vec4(1.0, 0.0, 1.0, 1.0);
			k++;

			chasecamVers[k]=   vec4(radius*sin(j)*cos(i), radius*cos(j)*cos(i), radius*sin(i), 1.0);
			chasecamColors[k] = vec4(1.0, 1.0, 1.0, 1.0);
			k++;
		}
	}


	///////////////////////////////
	// HEAD
	///////////////////////////////
	glGenVertexArrays( 1, &chasecamvao[0] );
	// Create and initialize any buffer objects
	glBindVertexArray( chasecamvao[0] );
	
	glGenBuffers( 2, &chasecamvbo[0] );
    glBindBuffer( GL_ARRAY_BUFFER, chasecamvbo[0] );
    
	glBufferData( GL_ARRAY_BUFFER, totalchasecamverts*sizeof(vec4), chasecamVers, GL_STATIC_DRAW);
	// glBufferData( GL_ARRAY_BUFFER, sizeof(headVers), headVers, GL_STATIC_DRAW);
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//and now our colors for each vertex
	glBindBuffer( GL_ARRAY_BUFFER, chasecamvbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(chasecamColors), chasecamColors, GL_STATIC_DRAW );
	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);

	
}
/////////////////////////////////////////
// generateHead
/////////////////////////////////////////
void generateHead()
{
	generateEyes();

	int subdiv = 10;
	int radius = 1;

	float step = (360.0/subdiv)*(M_PI/180.0);

	totalheadverts = ceil(subdiv/2.0)*subdiv * 6;

	if(headVers){
		delete[] headVers;
	}
	headVers = new vec4[totalheadverts];

	int k = 0;
	for(float i = -M_PI/2; i<=M_PI/2; i+=step){
		for(float j = -M_PI; j<=M_PI; j+=step){
			//triangle 1
			headVers[k]=   vec4(radius*sin(j)*cos(i), radius*cos(j)*cos(i), radius*sin(i), 1.0);
			headColors[k] = vec4(1.0, 1.0, 1.0, 1.0);
			k++;
	
			headVers[k]=   vec4(radius*sin(j)*cos(i+step), radius*cos(j)*cos(i+step), radius*sin(i+step), 1.0);
			headColors[k] = vec4(1.0, 1.0, 1.0, 1.0);
			k++;
			
			headVers[k]=   vec4(radius*sin((j+step))*cos((i+step)), radius*cos(j+step)*cos(i+step), radius*sin(i+step), 1.0);
			headColors[k] = vec4(1.0, 1.0, 1.0, 1.0);
			k++;

			//triangle 2
			headVers[k]=   vec4(radius*sin((j+step))*cos((i+step)), radius*cos(j+step)*cos(i+step), radius*sin(i+step), 1.0);
			headColors[k] = vec4(1.0, 1.0, 1.0, 1.0);
			k++;

			headVers[k]=   vec4(radius*sin(j+step)*cos(i), radius*cos(j+step)*cos(i), radius*sin(i), 1.0);
			headColors[k] = vec4(1.0, 1.0, 1.0, 1.0);
			k++;

			headVers[k]=   vec4(radius*sin(j)*cos(i), radius*cos(j)*cos(i), radius*sin(i), 1.0);
			headColors[k] = vec4(1.0, 1.0, 1.0, 1.0);
			k++;
		}
	}


	///////////////////////////////
	// HEAD
	///////////////////////////////
	glGenVertexArrays( 1, &headvao[0] );
	// Create and initialize any buffer objects
	glBindVertexArray( headvao[0] );
	
	glGenBuffers( 2, &headvbo[0] );
    glBindBuffer( GL_ARRAY_BUFFER, headvbo[0] );
    
	glBufferData( GL_ARRAY_BUFFER, totalheadverts*sizeof(vec4), headVers, GL_STATIC_DRAW);
	// glBufferData( GL_ARRAY_BUFFER, sizeof(headVers), headVers, GL_STATIC_DRAW);
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//and now our colors for each vertex
	glBindBuffer( GL_ARRAY_BUFFER, headvbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(headColors), headColors, GL_STATIC_DRAW );
	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);

	
	///////////////////////////////
	// EYES
	///////////////////////////////
	glGenVertexArrays( 1, &eyevao[0] );
	// Create and initialize any buffer objects
	glBindVertexArray( eyevao[0] );
	
	glGenBuffers( 2, &eyevbo[0] );
    glBindBuffer( GL_ARRAY_BUFFER, eyevbo[0] );
    
	glBufferData( GL_ARRAY_BUFFER, sizeof(eyeVerts), eyeVerts, GL_STATIC_DRAW);
	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//and now our colors for each vertex
	glBindBuffer( GL_ARRAY_BUFFER,eyevbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(eyeColors), eyeColors, GL_STATIC_DRAW );
	vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	
}

void displayViewPointCamera()
{
	stack.push(mv);

	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);

	mv = mv * Translate(0, -0.90, 0.05); // 0.05
	
	mv = mv * RotateY(turnEyeAngle); // rotate VoitPoint camera even head is just a white sphere
	
	mv = mv * Scale(0.02,0.02,0.02);
	mv = mv * RotateY(180);

	DrawTriagle(chasecamvao, totalchasecamverts);

	mv = stack.pop();
}
/////////////////////////////////////////
// displayChase cam
/////////////////////////////////////////
void displayChaseCamera()
{
	stack.push(mv);

	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);

	mv = mv * Translate(0, -0.85, -0.02); // 0.05
	
	mv = mv * Scale(0.02,0.02,0.02);
	
	DrawTriagle(chasecamvao, totalchasecamverts);

	mv = stack.pop();

	///////////////////////////
	//////////////////////////
	stack.push(mv);

	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);

	mv = mv * Translate(0, -0.90, -0.02); // 0.05
	
	mv = mv * Scale(0.003,0.04,0.003);
	
	DrawTriagle(chasecamvao, totalchasecamverts);

	mv = stack.pop();


}
/////////////////////////////////////////
// displayHead
/////////////////////////////////////////
void displayHead()
{
	
	stack.push(mv);

	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);

	mv = mv * Translate(0, -0.88, 0.01); // 0.05
	mv = mv * RotateY(turnEyeAngle); // rotate head even head is just a white sphere
	mv = mv * Scale(0.01,0.01,0.01);
	
	DrawTriagle(headvao, totalheadverts);

	mv = stack.pop();


	// driver body
	stack.push(mv);

	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);

	mv = mv * Translate(0, -0.90, 0.01); // 0.05
	mv = mv * RotateY(turnEyeAngle); // rotate head even head is just a white sphere
	mv = mv * Scale(0.03,0.02,0.005);
	
	DrawTriagle(headvao, totalheadverts);

	mv = stack.pop();

	//////////////////////////////////////////
	// EYE
	//////////////////////////////////////////
	

	// left eye
	stack.push(mv);
	
	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);

	//mv = mv * Translate(0, 0, 0.05);
	
	mv = mv * Translate(0, 0.034, 0.015);
	mv = mv * RotateY(turnEyeAngle);
	mv = mv * Translate(0.006, -0.905, 0.01); // -0.905
	//mv = mv * Translate(0.006, -0.905, 0.07);
	mv = mv * RotateX(90);
	mv = mv * Scale(0.005f,0.005f,0.005f);

	DrawTriagle(eyevao, 144);

	mv = stack.pop();

	// right eye
	stack.push(mv);
	
	
	mv = mv * Translate(currentX, 0, currentZ);
	mv = mv * RotateY(turnCarAngle);
		
	//mv = mv * Translate(0, 0, 0.05);
	mv = mv * Translate(0, 0.034, 0.015);
	mv = mv * RotateY(turnEyeAngle);
	mv = mv * Translate(-0.006, -0.905, 0.01);
	mv = mv * RotateX(90);
	mv = mv * Scale(0.005f,0.005f,0.005f);

	DrawTriagle(eyevao, 144);

	mv = stack.pop();


}

/////////////////////////////////////////
// display
/////////////////////////////////////////
void display()
{
	 /*clear all pixels*/
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	
	
	if ( switchcamera == 0 ) // static cam
	{
		mv = LookAt(vec4(0, 0, dollyzoom, 1.0), vec4(atX, 0, atZ, 1.0), vec4(0, 1, 0, 0.0));

		mv = mv * Translate(tx, ty, tz);

		mv = mv *RotateX(rx);
		mv = mv * RotateY(ry);
		mv = mv * RotateZ(rz);

		//glUniformMatrix4fv(model_view, 1, GL_TRUE, mv);
	}
	else if (switchcamera == 1) // chase cam
	{
		
		mv = LookAt(vec4(currentX, -0.91, currentZ, 1.0), // eye
			       vec4(chasecamlookdirection.x*10, -0.91, chasecamlookdirection.z*10, 1.0), // at
				   vec4(0, 1, 0, 0.0));    // up
	}
	else // viewport cam
	{
		vec4 viewpoint = vec4(0,0,1,0);
		vec4 view = vec4(0,0,1,0);

		if ( turnEyeRight != 0)
		{
			
			viewpoint = RotateY(turnEyeAngle) * viewpointcam;

			view = viewpoint;
			
		}
		else
		{
			view = viewpointcam;
		}
		

		mv = LookAt(vec4(currentX, -0.90, currentZ, 1.0), // eye
			       vec4(view.x*10, -0.90, view.z*10, 1.0), // at
				   vec4(0, 1, 0, 0.0));    // up
	}
	
	
	
	displayStage();

	displayCar();

	displayWheels();
	
	displayHead();

	//displayObjects();

	//displayChaseCamera();
	//displayViewPointCamera();

	glFlush();
    /*start processing buffered OpenGL routines*/
    glutSwapBuffers();


}

/////////////////////////////////////////
// myIdle
/////////////////////////////////////////
void myIdle()
{

	GLint moveF = 1;
	if ( bmoveForward == true )
	{
		moveF = 1;
	}
	else
	{
		moveF = -1;
	}

	

	chasecamlookdirection = RotateY((moveF)*0.2*(turnAngle)/maxTurnWheel) * chasecamlookdirection;
	viewpointcam = RotateY((moveF)*0.2*(turnAngle)/maxTurnWheel) * viewpointcam;

		
	if ( pointCameraAt == true ) // point the stage
	{
		atX = 0;
		atZ = 0;
	}
	else // point at the car
	{
		atX = currentX;
		atZ = currentZ;
	}

	if ( currentX > -1 && currentX < 1 &&
		 currentZ > -1 && currentZ < 1 ) 
	{
		if ( bmoveForward == true )
		{
		
			if ( turnAngle < 0)
			{
				turnCarAngle -= 0.2*abs(turnAngle)/maxTurnWheel;
			}
			else if ( turnAngle > 0)
			{
				turnCarAngle += 0.2*abs(turnAngle)/maxTurnWheel;
			}
			
	
			moveStepX = vectorLen * sin(turnCarAngle*M_PI/180);
			moveStepZ  = vectorLen * cos(turnCarAngle*M_PI/180);

			currentX = currentX + moveStepX;
			currentZ = currentZ + moveStepZ;

			if ( currentX < -1.0 )
			{
				currentX = -1.00f;
			}
			else if ( currentX > 1.0 )
			{
				currentX = 1.00f;
			}
			else if ( currentZ > 1.0 )
			{
				currentZ = 1.00f;
			}
			else if ( currentZ < -1.0 )
			{
				currentZ = -1.00f;
			}
			else
			{
			
			}

			rollangle += (vectorLen * 360)/(0.05*M_PI);
		}
		else
		{
			if ( turnAngle < 0)
			{
				turnCarAngle += 0.2*abs(turnAngle)/maxTurnWheel;;
			}
			else if ( turnAngle > 0)
			{
				turnCarAngle -= 0.2*abs(turnAngle)/maxTurnWheel;;
			}
		
			moveStepX = vectorLen * sin(turnCarAngle*M_PI/180);
			moveStepZ  = vectorLen * cos(turnCarAngle*M_PI/180);

			currentX = currentX - moveStepX;
			currentZ = currentZ - moveStepZ;

			if ( currentX < -1.0 )
			{
				currentX = -1.00f;
			}
			else if ( currentX > 1.0 )
			{
				currentX = 1.00f;
			}
			else if ( currentZ > 1.0 )
			{
				currentZ = 1.00f;
			}
			else if ( currentZ < -1.0 )
			{
				currentZ = -1.00f;
			}
		

			rollangle -= (vectorLen * 360)/(0.05*M_PI);
		}

		glutPostRedisplay();
	}
} 

/////////////////////////////////////////
//
/////////////////////////////////////////
void reshape(int width, int height){
	ww= width;
	wh = height;

	//field of view angle, aspect ratio, closest distance from camera to object, largest distanec from camera to object
	p = Perspective(lenszoom, (float)width/(float)height, 1.0, 100.0);
	//p = Perspective(20, (float)width/(float)height, 1, 100.0);

	glViewport( 0, 0, width, height );
}

/////////////////////////////////////////
// Keyboard
/////////////////////////////////////////
void Keyboard(unsigned char key, int x, int y) {
	/*exit when the escape key is pressed*/
	if (key == 27)
		exit(0);

	if (key == 'r' || key == 'R' )
	{
		 ///////////////////
		  // assighment 3
		  ///////////////////
		  atX = atZ = 0;
		  dollyzoom = 20.0f;
		  lenszoom = 45.0;

		  reshape(ww, wh);
	}
	else if (key == 'c' || key == 'C' )
	{
		if ( switchcamera == 0 )
		{
			p = Perspective(45.0, (float)ww/(float)wh, 1.0, 100.0);
			switchcamera = 1;
		}
		else if ( switchcamera == 1 )
		{
			p = Perspective(45.0, (float)ww/(float)wh, 1.0, 100.0);
			switchcamera = 2;
		}
		else if ( switchcamera == 2 )
		{
			reshape(ww, wh);
			switchcamera = 0;
		}

	}
	else if (key == 'f' || key == 'F' ) // toggle b/t look at origin or current car postion
	{
		if ( pointCameraAt == true )
		{
			pointCameraAt = false;
			
		}
		else
		{
			pointCameraAt = true;
			
		}
	}
	else if (key == 's' || key == 'S' ) // to zoom out
	{


		if ( switchcamera == 0)
		{
			lenszoom += 1;
			reshape(ww, wh);
		}
		else
			p = Perspective(45.0, (float)ww/(float)wh, 1.0, 100.0);
			
	}
	else if (key == 'a' || key == 'A' ) // to zoom in
	{
		
		if ( switchcamera == 0)
		{
			lenszoom -= 1;
			reshape(ww, wh);
		}
		else
			p = Perspective(45.0, (float)ww/(float)wh, 1.0, 100.0);
	}
	else if (key == 'w' || key == 'W' ) // dolly in
	{
		dollyzoom += 1;
	}
	else if (key == 'q' || key == 'Q' ) // to dolly out in
	{
		dollyzoom -= 1;
	}
	if ( key == 'b' || key == 'B' ) // to start the car
	{
		glutIdleFunc(myIdle);
	}

	if ( key == 32 ) // to stop car moving
	{
		glutIdleFunc(NULL);
	}
	else if(key == 'z')
	{
		turnEyeRight = 1;
		turnEyeAngle += 1;
	}
	else if(key == 'x')
	{
		turnEyeRight = -1;
		turnEyeAngle -= 1;
	}
	else if(key == 'i')
	{
		rx += 5;
		if(rx > 360)
			rx -= 360;
	}
	else if(key == 'j'){
		rx -= 5;
		if(rx < 0)
			rx += 360;
	}
	else if(key == 'o'){
		ry += 5;
		if(ry > 360)
			ry -= 360;
	}
	else if(key == 'k'){
		ry -= 5;
		if(ry < 0)
			ry += 360;
	}else if(key == 'p'){
		rz += 5;
		if(rz > 360)
			rz -= 360;
	}else if(key == 'l'){
		rz -= 5;
		if(rz < 0)
			rz += 360;
	}

	printf("rx = %f\n", rx);
	printf("ry = %f\n", ry);
	printf("rz = %f\n", rz);


	printf("tx = %f\n", tx);
	printf("ty = %f\n", ty);
	printf("tz = %f\n", tz);

	//don't forget to request a new frame since parameters have changed
	glutPostRedisplay();

}



/////////////////////////////////////////
// init
/////////////////////////////////////////
void init() {

  /*select clearing (background) color*/
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
    
  ///////////////////
  // assighment 3
  ///////////////////
  atX = atZ = 0;
  dollyzoom = 20.0f;
  lenszoom = 45.0;
  // TRUE: point camear at the center, 
  // False: point at the car
  pointCameraAt = true; 

  chasecamlookdirection		= vec4(0.0, 0.0 , 0.07,0);
  viewportcamlookdirection  = vec4(0.0, 0.0 , 0.07,0);
  viewpointcam = vec4(0.0, 0.0 , 0.07,0);

  turnEyeRight = 0;

  // 0: static camera
  // 1: chase camera
  // 2 : viewpoit camera

  switchcamera = 0; //
  ///////////////////
  // assighment 2
  ///////////////////
  bmoveForward = true;
  moveStepZ = 0.0005;
  currentX = currentZ = 0;
  turnCarAngle = 0;
  turnAngle = 0;
  rollangle = 0;
  turnEyeAngle = 0;

  // Load shaders and use the resulting shader program
    program = InitShader( "vshader-transform.glsl", "fshader-transform.glsl" );
    glUseProgram( program );

//grab pointers for our modelview and perspecive uniform matrices
	model_view = glGetUniformLocation(program, "model_view");
	projection = glGetUniformLocation(program, "projection");

  //populate our arrays
  generateStage();
  
  generateCar();

  generateWheelSides();
  
  generateHead();

  //generateSimpleObjects();

  generateChaseCamera();

  //set up transformation defaults
  //start with no translation or rotation
  tx = ty = tz = rx = ry = rz = 0;

  // initialize stage
  rx = 45;
  ty = 0.8;
  tz = 19.2;

  // caculate movestepX and vector len for moving car
  moveStepX = tan(M_PI/180) * moveStepZ;
  vectorLen = sqrt(moveStepX*moveStepX + moveStepZ*moveStepZ);


    //Only draw the things in the front layer
	glEnable(GL_DEPTH_TEST);
}


/////////////////////////////////////////
// special keys
/////////////////////////////////////////
void special(int key, int x, int y){

	// restart the game
	if (key == GLUT_KEY_F2)
	{
		init();
		glutIdleFunc(NULL);
	}
	else if(key == GLUT_KEY_UP) // car move forward
	{
		bmoveForward = true;
				
		if ( currentZ >= 1 )
		{
			currentZ = 0.999;
		}
		else if ( currentZ <= -1 )
		{
			currentZ = -0.999;
		}

		if ( currentX >= 1 )
		{
			currentX = 0.999;
		}
		else if ( currentX <= -1 )
		{
			currentX = -0.999;
		}

	}
	else if(key == GLUT_KEY_DOWN) // car moves backward
	{
		bmoveForward = false;
		if ( currentZ >= 1 )
		{
			currentZ = 0.999;
		}
		else if ( currentZ <= -1 )
		{
			currentZ = -0.999;
		}

		if ( currentX >= 1 )
		{
			currentX = 0.999;
		}
		else if ( currentX <= -1 )
		{
			currentX = -0.999;
		}
	}
	else if(key == GLUT_KEY_LEFT) // move wheels to left
	{
		if ( turnAngle >= maxTurnWheel )
		{
			turnAngle = maxTurnWheel;
		}
		else
		{
			turnAngle += 1;
		}
		
	}
	else if(key == GLUT_KEY_RIGHT) // move wheels to right
	{
	
		if ( turnAngle <= -maxTurnWheel )
		{
			turnAngle = -maxTurnWheel;
		}
		else
		{
			turnAngle -= 1;
		}

		
	}
	
	printf("=============================\n");
	printf("moveStepX : %f\n", moveStepX);
	printf("moveStepZ : %f\n", moveStepZ);
	printf("vectorLen : %f\n", vectorLen);
	printf("currentX : %f\n", currentX);
	printf("currentZ : %f\n", currentZ);
	printf("turnCarAngle : %f\n", turnCarAngle);
	printf("turnAngle : %f\n", turnAngle);
 	

	//don't forget to request a new frame since parameters have changed
	glutPostRedisplay();
}




/////////////////////////////////////////
// main
/////////////////////////////////////////
int main(int argc, char **argv)
{
  /*set up window for display*/
  glutInit(&argc, argv);
  glutInitWindowPosition(0, 0); 
  glutInitWindowSize(ww, wh);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("Out For Drive");  

  glewExperimental = GL_TRUE;

  glewInit();
  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(Keyboard);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  

  glutMainLoop();
  return 0;
}