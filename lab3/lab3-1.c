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

#define far 30.0

#define right 0.5

#define left -0.5

#define top 0.5

#define bottom -0.5

GLfloat projectionMatrix[] = {
	2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,

	0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,

	0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),

	0.0f, 0.0f, -1.0f, 0.0f };


		GLfloat a = M_PI/4; //I RADIANER!!!

		mat4 rot, trans, trans2, total,total2;
		mat4 lookAtMatrix;
		mat4 cameraRot;
		mat4 originalTotal2;

		mat4 originalLookat;


	// vertex array object
	unsigned int vertexArrayObjID;
	unsigned int bunnyVertexArrayObjID;
	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;
	unsigned int bunnyTexCoordBufferObjID;

	Model *walls;
	Model *roof;
	Model *balcony;
	Model *blades;

	GLuint myTex;

	// Reference to shader program
	GLuint program;
	void init(void)
	{
		walls = LoadModelPlus("windmill/windmill-walls.obj");
		roof = LoadModelPlus("windmill/windmill-roof.obj");
		balcony = LoadModelPlus("windmill/windmill-balcony.obj");
		blades = LoadModelPlus("windmill/blade.obj");
		cameraRot = Ry(0);

		//LoadTGATextureSimple("maskros512.tga", &myTex);

		trans = T(0, 0, 0); //T(1,2,3) original
		trans2 = T(0.0,9.2,5);
		//model-to-world matrix
		total = Mult(trans,Ry(-M_PI/2));
		total2 = Mult(trans2,Ry(M_PI/2));
		originalTotal2 = total2;

		//model-to-view matrix
		originalLookat = lookAt(0,20,22,0,0,0,0,1,0);
		lookAtMatrix  = originalLookat;

		dumpInfo();

		// GL inits
		glClearColor(0.2,0.2,0.5,0);
		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);
		printError("GL inits");

		// Load and compile shader
		program = loadShaders("lab3-1.vert", "lab3-1.frag");
		printError("init shader");

		glBindTexture(GL_TEXTURE_2D, myTex);
		glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
		glActiveTexture(GL_TEXTURE0);
		//glGenBuffers(1, &bunnyTexCoordBufferObjID);  //TEXTURE

		printError("init arrays");
	}

	void display(void)
	{
		printError("pre display");

		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
		//glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

		glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
		glUniformMatrix4fv(glGetUniformLocation(program, "lookieMat"), 1, GL_TRUE, lookAtMatrix.m);

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

		glutSwapBuffers();
	}

	void OnTimer(int value)
	{
		GLfloat t =(GLfloat)glutGet(GLUT_ELAPSED_TIME);
		cameraRot = Ry(t/1000);
		total2 = Mult(originalTotal2,Rx(t/1000));
		lookAtMatrix = Mult(originalLookat,cameraRot);
		glutPostRedisplay();
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
