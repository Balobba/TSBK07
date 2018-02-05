// Lab 1-1.
// This is the same as the first simple example in the course book,
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#include "VectorUtils3.h"
#include <math.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
// Linking hint for Lightweight IDE
// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "loadobj.h"
#include "LoadTGA.h"

// Linking hint for Lightweight IDE
// uses framework Cocoa

// Globals
// Data would normally be read from files

#define near 1.0

#define far 60.0

#define right 0.5

#define left -0.5

#define top 0.5

#define bottom -0.5

GLfloat projectionMatrix[] = {
	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

	0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

	0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

	0.0f, 0.0f, -1.0f, 0.0f };

	GLfloat groundMatrix[] = {
		-10, 0, 10,
		-10, 0, 0,
		10, 0, 0,

		10, 0, 0,
		-10, 0, 10,
		10, 0, 10
	};

	GLfloat groundTextureCoord[]={
		0.0f,10.0f,
		0.0f,0.0f,
		10.0f,0.0f,
		10.0f,0.0f,
		0.0f,10.0f,
		10.0f,10.0f
	};

	GLfloat groundNormal[] = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,

		0, 1, 0,
		0, 1, 0,
		0, 1, 0
	};

	//Lights
	Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light

	{0.0f, 1.0f, 0.0f}, // Green light

	{0.0f, 0.0f, 1.0f}, // Blue light

	{1.0f, 1.0f, 1.0f} }; // White light


	GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};

	GLint isDirectional[] = {0,0,1,1};

	Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional

	{0.0f, 5.0f, 10.0f}, // Green light, positional

	{-1.0f, 0.0f, 0.0f}, // Blue light along X

	{0.0f, 0.0f, -1.0f} }; // White light along Z

	//Constants

	GLfloat a = M_PI/4; //I RADIANER!!!

	mat4 rot, trans, trans2, total, total2, totalbunny;
	mat4 lookAtMatrix;
	mat4 cameraRot, cameraPos;
	mat4 originalTotal2;
	mat4 groundTransform;

	mat4 originalLookat;


	// vertex array object
	unsigned int vertexArrayObjID;
	unsigned int vertexBufferObjID;
	unsigned int groundTexCoordBufferObjID;
	unsigned int groundNormalBufferObjID;

	Model *walls;
	Model *roof;
	Model *balcony;
	Model *blades;
	Model *bunny;
	Model *skybox;

	GLuint myTex;
	GLuint whiteTex;
	GLuint skyboxTex;

	// Reference to shader program
	GLuint program;
	GLuint groundShader;
	GLuint skyShader;
	void init(void)
	{
		walls = LoadModelPlus("windmill/windmill-walls.obj");
		roof = LoadModelPlus("windmill/windmill-roof.obj");
		balcony = LoadModelPlus("windmill/windmill-balcony.obj");
		blades = LoadModelPlus("windmill/blade.obj");
		bunny = LoadModelPlus("../lab2/bunny.obj");
		skybox = LoadModelPlus("skybox.obj");
		cameraRot = Ry(0);
		cameraPos = T(0,0,0);

		LoadTGATextureSimple("grass.tga", &myTex);
		LoadTGATextureSimple("SkyBox512.tga", &skyboxTex);

		trans = T(0, 0, 0); //T(1,2,3) original
		trans2 = T(0.0,9.2,5);
		//model-to-world matrix
		total = Mult(trans,Ry(-M_PI/2));
		total2 = Mult(S(0.8,0.8,0.8),Ry(M_PI/2));
		total2 = Mult(trans2,total2);
		originalTotal2 = total2;
		totalbunny = T(5,0.7,5);

		groundTransform = IdentityMatrix();
		groundTransform = Mult(S(2,1,2),groundTransform);
		groundTransform = Mult(T(0,0,-10),groundTransform);
		//model-to-view matrix
		originalLookat = lookAt(0,1,22,0,0,0,0,1,0);
		lookAtMatrix  = originalLookat;

		dumpInfo();

		// GL inits
		glClearColor(0.2,0.2,0.5,0);
		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);
		printError("GL inits");

		// Load and compile shader
		program = loadShaders("lab3-4.vert", "lab3-4.frag");
		groundShader = loadShaders("lab3-3-ground.vert", "lab3-3-ground.frag");
		skyShader = loadShaders("lab3-3-sky.vert", "lab3-3-sky.frag");
		printError("init shader");

		glUseProgram(program);
		glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
		glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
		glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
		glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);

		glUseProgram(skyShader);
		glActiveTexture(GL_TEXTURE1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, skyboxTex);
		glUniform1i(glGetUniformLocation(skyShader, "texUnit"), 1); // Texture unit 1

		glUseProgram(groundShader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, myTex);
		glUniform1i(glGetUniformLocation(groundShader, "texUnit"), 0); // Texture unit 0
		glGenBuffers(1, &groundTexCoordBufferObjID);  //TEXTURE




		// Allocate and activate Vertex Array Object
		glGenVertexArrays(1, &vertexArrayObjID);
		glBindVertexArray(vertexArrayObjID);
		// Allocate Vertex Buffer Objects
		glGenBuffers(1, &vertexBufferObjID);
		glGenBuffers(1, &groundNormalBufferObjID);

		// VBO for vertex data
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID);
		//glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, 3*6*sizeof(GLfloat), groundMatrix, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(groundShader, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(groundShader, "in_Position"));

		glBindBuffer(GL_ARRAY_BUFFER, groundTexCoordBufferObjID);
		glBufferData(GL_ARRAY_BUFFER,6*2*sizeof(GLfloat), groundTextureCoord, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(groundShader, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(groundShader, "inTexCoord"));

		glBindBuffer(GL_ARRAY_BUFFER, groundNormalBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, 3*6*sizeof(GLfloat), groundNormal, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(groundShader, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(groundShader, "in_Normal"));

		printError("init arrays");
	}

	void display(void)
	{
		printError("pre display");

		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(skyShader);
		glDisable(GL_DEPTH_TEST);

		glUniformMatrix4fv(glGetUniformLocation(skyShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
		glUniformMatrix4fv(glGetUniformLocation(skyShader, "lookieMat"), 1, GL_TRUE, lookAtMatrix.m);
		//glUniformMatrix4fv(glGetUniformLocation(skyShader, "mdlMatrix"), 1, GL_TRUE, totalSky.m);
		DrawModel(skybox,skyShader,"in_Position",NULL,"inTexCoord");


		glEnable(GL_DEPTH_TEST);


		//glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
		//glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);
		glUseProgram(groundShader);
		glUniformMatrix4fv(glGetUniformLocation(groundShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
		glUniformMatrix4fv(glGetUniformLocation(groundShader, "lookieMat"), 1, GL_TRUE, lookAtMatrix.m);
		//glActiveTexture(GL_TEXTURE0);
		glUniformMatrix4fv(glGetUniformLocation(groundShader, "mdlMatrix"), 1, GL_TRUE, groundTransform.m);
		glBindVertexArray(vertexArrayObjID);	// Select VAO
		glDrawArrays(GL_TRIANGLES, 0, 2*3);	// draw object


		glUseProgram(program);
		glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
		glUniformMatrix4fv(glGetUniformLocation(program, "lookieMat"), 1, GL_TRUE, lookAtMatrix.m);

		//glActiveTexture(GL_TEXTURE1);
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
		DrawModel(walls,program,"in_Position","in_Normal","inTexCoord");
		DrawModel(roof,program,"in_Position","in_Normal","inTexCoord");
		DrawModel(balcony,program,"in_Position","in_Normal","inTexCoord");

		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total2.m);
		DrawModel(blades,program,"in_Position","in_Normal","inTexCoord");
		//mat4 total3 = Mult(Mult(total2,T(0,-0.5,0)),Rx(M_PI/2));
		mat4 total3 = Mult(total2,Rx(M_PI/2));
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total3.m);
		DrawModel(blades,program,"in_Position","in_Normal","inTexCoord");
		//mat4 total4 = Mult(Mult(total2,T(0,-0.5,-0.5)),Rx(M_PI));
		mat4 total4 = Mult(total2,Rx(M_PI));
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total4.m);
		DrawModel(blades,program,"in_Position","in_Normal","inTexCoord");
		//mat4 total5 = Mult(Mult(total2,T(0,0,-0.5)),Rx(-M_PI/2));
		mat4 total5 = Mult(total2,Rx(-M_PI/2));
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total5.m);
		DrawModel(blades,program,"in_Position","in_Normal","inTexCoord");
		printError("display");

		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalbunny.m);
		DrawModel(bunny,program,"in_Position","in_Normal","inTexCoord");



		glutSwapBuffers();
	}
	void moveCamera(){
		if(glutKeyIsDown('w')){
			cameraRot = Mult(cameraRot,Rx(0.1));
		}
		if(glutKeyIsDown('s')){
			cameraRot = Mult(cameraRot,Rx(-0.1));
		}
		if(glutKeyIsDown('a')){
			cameraRot = Mult(cameraRot,Ry(-0.1));
		}
		if(glutKeyIsDown('d')){
			cameraRot = Mult(cameraRot,Ry(0.1));
		}
		if(glutKeyIsDown('i')){
			cameraPos = Mult(cameraPos,T(0,0,0.5));
		}
		if(glutKeyIsDown('k')){
			cameraPos = Mult(cameraPos,T(0,0,-0.5));
		}
		if(glutKeyIsDown('j')){
			cameraPos = Mult(cameraPos,T(0.5,0,0));
		}
		if(glutKeyIsDown('l')){
			cameraPos = Mult(cameraPos,T(-0.5,0,0));
		}
	}
	void OnTimer(int value)
	{
		GLfloat t =(GLfloat)glutGet(GLUT_ELAPSED_TIME);
		//cameraRot = Ry(t/1000);
		total2 = Mult(originalTotal2,Rx(t/1000));
		lookAtMatrix = Mult(originalLookat,cameraRot);
		lookAtMatrix = Mult(cameraPos, lookAtMatrix);
		glutPostRedisplay();
		moveCamera();

		glutTimerFunc(20, &OnTimer, value);
	}



	int main(int argc, char *argv[])
	{
		glutInit(&argc, argv);
		glutInitContextVersion(3, 2);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutCreateWindow ("GL3 white triangle example");
		glutDisplayFunc(display);
		init ();
		glutTimerFunc(20, &OnTimer, 0);
		glutMainLoop();
		return 0;
	}
