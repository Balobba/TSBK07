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


	GLfloat myMatrix[] = {    1.0f, 0.0f, 0.0f, 0.5f,

		0.0f, 1.0f, 0.0f, 0.0f,

		0.0f, 0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 0.0f, 1.0f };

		GLfloat a = M_PI/4; //I RADIANER!!!

		mat4 rot, trans, total;
		/*GLfloat rotationMatrix[] = {
		cos(a), -sin(a), 0.0f, 0.0f,
		sin(a), cos(a), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};*/

	GLfloat rotationMatrix[16];
	GLfloat rotationMatrix2[16];

	// vertex array object
	unsigned int vertexArrayObjID;
	unsigned int bunnyVertexArrayObjID;
	unsigned int bunnyVertexBufferObjID;
	unsigned int bunnyIndexBufferObjID;
	unsigned int bunnyNormalBufferObjID;
	unsigned int bunnyTexCoordBufferObjID;

	Model *m;
	GLuint myTex;

	void SetRotationMatrix(GLfloat a, GLfloat *m)
	{
		m[0] = cos(a); m[1] = -sin(a); m[2] = 0; m[3] = 0.0;
		m[4] = sin(a); m[5] = cos(a); m[6] = 0; m[7] = 0.0;
		m[8] = 0; m[9] = 0; m[10] = 1.0; m[11] = 0.0;
		m[12] = 0.0; m[13] = 0.0; m[14] = 0.0; m[15] = 1.0;
	}

	void SetRotationMatrixAroundX(GLfloat a, GLfloat *m)
	{
		m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0.0;
		m[4] = 0; m[5] = cos(a); m[6] = -sin(a); m[7] = 0.0;
		m[8] = 0; m[9] = sin(a); m[10] = cos(a); m[11] = 0.0;
		m[12] = 0.0; m[13] = 0.0; m[14] = 0.0; m[15] = 1.0;
	}
	// Reference to shader program
	GLuint program;
	void init(void)
	{
		m = LoadModel("bunnyplus.obj");

		LoadTGATextureSimple("maskros512.tga", &myTex);

		trans = T(0, 0, -2); //T(1,2,3) original
		rot = Ry(a);
		total = Mult(trans,rot);
		// vertex buffer object, used for uploading the geometry
		//unsigned int vertexBufferObjID;
		//unsigned int vertexColorBufferObjID;

		mat4 lookAtMatrix  = lookAt(0,3,3,
																0,0,0,
																0,1,0);



			SetRotationMatrixAroundX(a,rotationMatrix);
			SetRotationMatrix(a,rotationMatrix2);

			/*rotationMatrix	= {
			cos(a), -sin(a), 0.0f, 0.0f,
			sin(a), cos(a), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};*/
		dumpInfo();

		// GL inits
		glClearColor(0.2,0.2,0.5,0);
		//glDisable(GL_DEPTH_TEST);
		glEnable(GL_DEPTH_TEST);
		printError("GL inits");

		// Load and compile shader
		program = loadShaders("lab2-4.vert", "lab2-4.frag");
		printError("init shader");


		glBindTexture(GL_TEXTURE_2D, myTex);
		glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
		glActiveTexture(GL_TEXTURE0);

		// Upload geometry to the GPU:
		glGenVertexArrays(1, &bunnyVertexArrayObjID);
		glGenBuffers(1, &bunnyVertexBufferObjID);
		glGenBuffers(1, &bunnyIndexBufferObjID);
		glGenBuffers(1, &bunnyNormalBufferObjID);
		glGenBuffers(1, &bunnyTexCoordBufferObjID);  //TEXTURE

		glBindVertexArray(bunnyVertexArrayObjID);

		// VBO for vertex data
		glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
		glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));

		// VBO for normal data
		glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
		glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
		/*
		glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));*/


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,  m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);



		glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
		glUniformMatrix4fv(glGetUniformLocation(program, "lookieMat"), 1, GL_TRUE, lookAtMatrix.m);

		if (m->texCoordArray != NULL)
		{
			glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
			glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
			glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
		}

		// End of upload of geometry

		printError("init arrays");
	}

	void display(void)
	{
		printError("pre display");

		// clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBindVertexArray(vertexArrayObjID);	// Select VAO
		//glDrawArrays(GL_TRIANGLES, 0, 12*3);	// draw object

		glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
		glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

		glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
		glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
		glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix"), 1, GL_TRUE, rotationMatrix);
		glUniformMatrix4fv(glGetUniformLocation(program, "rotationMatrix2"), 1, GL_TRUE, rotationMatrix2);

		printError("display");

		glutSwapBuffers();
	}

	void OnTimer(int value)
	{
		GLfloat t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
		a += (t/100000);
		SetRotationMatrixAroundX(t/10000,rotationMatrix);
		SetRotationMatrix(t/10000,rotationMatrix2);
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
