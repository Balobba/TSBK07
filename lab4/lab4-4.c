// Lab 4, terrain generation

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "loadobj.h"
#include "LoadTGA.h"

mat4 projectionMatrix;

Model* GenerateTerrain(TextureData *tex)
{
	int vertexCount = tex->width * tex->height;
	int triangleCount = (tex->width-1) * (tex->height-1) * 2;
	int x, z;

	GLfloat *vertexArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *normalArray = malloc(sizeof(GLfloat) * 3 * vertexCount);
	GLfloat *texCoordArray = malloc(sizeof(GLfloat) * 2 * vertexCount);
	GLuint *indexArray = malloc(sizeof(GLuint) * triangleCount*3);

	printf("bpp %d\n", tex->bpp);
	for (x = 0; x < tex->width; x++)
		for (z = 0; z < tex->height; z++)
		{
// Vertex array. You need to scale this properly
			vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
			vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 15.0;
			vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;

// Texture coordinates. You may want to scale them.
			texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
			texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
		}
	for (x = 0; x < tex->width-1; x++)
		for (z = 0; z < tex->height-1; z++)
		{
		// TriangleY1
			indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
		// TriangleY2
			indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
			indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
		}

	// Normal vectors. You need to calculate these.
		for (x = 0; x < tex->width; x++)
			for (z = 0; z < tex->height; z++)
			{
				if ((abs(x)>1) && (abs(z)>1)){
					vec3 v1 = {
						vertexArray[((x) + (z+1) * tex->width)*3 + 0],
						vertexArray[((x) + (z+1) * tex->width)*3 + 1],
						vertexArray[((x) + (z+1) * tex->width)*3 + 2]
					 			};
					vec3 v2 = {
						vertexArray[((x+1) + (z-1) * tex->width)*3 + 0],
						vertexArray[((x+1) + (z-1) * tex->width)*3 + 1],
						vertexArray[((x+1) + (z-1) * tex->width)*3 + 2]
					 			};
					vec3 v3  = {
						vertexArray[((x-1) + (z-1) * tex->width)*3 + 0],
						vertexArray[((x-1) + (z-1) * tex->width)*3 + 1],
						vertexArray[((x-1) + (z-1) * tex->width)*3 + 2]
					 			};
					vec3 normalVec = CalcNormalVector(v1,v2,v3);
					normalArray[(x + z * tex->width)*3 + 0] = normalVec.x;
					normalArray[(x + z * tex->width)*3 + 1] = normalVec.y;
					normalArray[(x + z * tex->width)*3 + 2] = normalVec.z;

				}else{
					normalArray[(x + z * tex->width)*3 + 0] = 0;
					normalArray[(x + z * tex->width)*3 + 1] = 1;
					normalArray[(x + z * tex->width)*3 + 2] = 0;
				}
			}

	// End of terrain generation

	// Create Model and upload to GPU:

	Model* model = LoadDataToModel(
			vertexArray,
			normalArray,
			texCoordArray,
			NULL,
			indexArray,
			vertexCount,
			triangleCount*3);

	return model;
}

float calculateHeight(Model *m, TextureData *tex, float x, float z){
	//get the quad coordinates
	//int xFloored = floor(x+0.5);
	//int zFloored = floor(z+0.5);
	int xFloored = floor(x);
	int zFloored = floor(z);

	//float terrainY = m->vertexArray[((int)x+xFloored + (int)z+xFloored * tex->width)*3 + 1];
	//return terrainY;

	//vectors for plane equation
	vec3 v,u,n;

	vec3 vertexVec1;

	//unique vertex for the 2 triangles
	if((x-xFloored)+(z-zFloored) < 1){
		//in the right triangle
		float x00 = m->vertexArray[(xFloored + zFloored * tex->width)*3 + 0];
		float y00 = m->vertexArray[(xFloored + zFloored * tex->width)*3 + 1];
		float z00 = m->vertexArray[(xFloored + zFloored * tex->width)*3 + 2];

	 	vec3 tmpvertexVec1 = {x00,y00,z00};
		vertexVec1 = tmpvertexVec1;

	}else{
		float x11 = m->vertexArray[((xFloored+1) + (zFloored+1) * tex->width)*3 + 0];
		float y11 = m->vertexArray[((xFloored+1) + (zFloored+1) * tex->width)*3 + 1];
		float z11 = m->vertexArray[((xFloored+1) + (zFloored+1) * tex->width)*3 + 2];

		vec3 tmpvertexVec1 = {x11,y11,z11};
		vertexVec1 = tmpvertexVec1;
	}


	//shared vertices
	float x01 = m->vertexArray[(xFloored + (zFloored+1) * tex->width)*3 + 0];
	float y01 = m->vertexArray[(xFloored + (zFloored+1) * tex->width)*3 + 1];
	float z01 = m->vertexArray[(xFloored + (zFloored+1) * tex->width)*3 + 2];

	vec3 vertexVec2 = {x01,y01,z01};

	float x10 = m->vertexArray[((xFloored+1) + zFloored * tex->width)*3 + 0];
	float y10 = m->vertexArray[((xFloored+1) + zFloored * tex->width)*3 + 1];
	float z10 = m->vertexArray[((xFloored+1) + zFloored * tex->width)*3 + 2];

	vec3 vertexVec3 = {x10,y10,z10};

	u = VectorSub(vertexVec3,vertexVec2);
	v = VectorSub(vertexVec3,vertexVec1);

	n = CrossProduct(u,v);


	// Ax+By+Cz = -D
	float D = (n.x*vertexVec3.x + n.y*vertexVec3.y + n.z*vertexVec3.z);

	// y = (-D -Ax -Cz) / B
	float ySolved = (D - n.x*x - n.z*z) / n.y;
	return ySolved;
	//return (vertexVec1.y+vertexVec2.y+vertexVec3.y)/3;

	//float terrainX = vertexArray[(x+xFloored + z+xFloored * tex->width)*3 + 0];
	//float terrainY = vertexArray[(x+xFloored + z+xFloored * tex->width)*3 + 1];
	//float terrainZ = vertexArray[(x+xFloored + z+xFloored * tex->width)*3 + 2];


}

// vertex array object
Model *m, *m2, *tm;

Model *ball;
Model *ball2;
// Reference to shader program
GLuint program,ballShader;
GLuint tex1, tex2;
TextureData ttex; // terrain
GLuint textureConcrecte,textureSand;

GLfloat angleY;
GLfloat angleX ;

mat4 camTrans;
mat4 ballTrans;


void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 50.0);
	angleY= 0.0f;
	angleX = 0.0f;
	camTrans = T(0,0,0);

	// Load and compile shader
	program = loadShaders("terrain.vert", "terrain.frag");
	ballShader = loadShaders("ball.vert", "ball.frag");
	glUseProgram(program);
	printError("init shader");



	glUniformMatrix4fv(glGetUniformLocation(program, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	LoadTGATextureSimple("maskros512.tga", &tex1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex1);		// Bind Our Texture tex1
	glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0

	LoadTGATextureSimple("sand.tga", &textureSand);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureSand);		// Bind Our Texture textureConcrecte
	glUniform1i(glGetUniformLocation(program, "textureSand"), 1); // Texture unit 1

	LoadTGATextureSimple("conc.tga", &textureConcrecte);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureConcrecte);		// Bind Our Texture textureConcrecte
	glUniform1i(glGetUniformLocation(program, "textureConcrecte"), 2); // Texture unit 2 textureSand



// Load terrain data

	LoadTGATextureData("fft-terrain.tga", &ttex);
	tm = GenerateTerrain(&ttex);
	printError("init terrain");

	glUseProgram(ballShader);
	glUniformMatrix4fv(glGetUniformLocation(ballShader, "projMatrix"), 1, GL_TRUE, projectionMatrix.m);
	ball = LoadModelPlus("octagon.obj");
	ballTrans = T(0,0,0);


}

void display(void)
{
	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	mat4 total, modelView, camMatrix;
	printError("pre display");

	glUseProgram(program);

	// Build matrix

	vec3 cam = {0, 10, 0};
	vec3 lookAtPoint = {1, 0, 0};
	camMatrix = lookAt(cam.x, cam.y, cam.z,
				lookAtPoint.x, lookAtPoint.y, lookAtPoint.z,
				0.0, 1.0, 0.0);
	camMatrix = Mult(Ry(angleY),camMatrix);
	camMatrix = Mult(Rx(angleX),camMatrix);
	camMatrix = Mult(camTrans,camMatrix);
	modelView = IdentityMatrix();
	total = Mult(camMatrix, modelView);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, total.m);
	glUniformMatrix4fv(glGetUniformLocation(program, "camMatrix"), 1, GL_TRUE, camMatrix.m);
	DrawModel(tm, program, "inPosition", "inNormal", "inTexCoord");


	glUseProgram(ballShader);
	float ballTransX = ballTrans.m[3];
	float ballTransZ = ballTrans.m[11];
	float ballTransY = calculateHeight(tm,&ttex,ballTransX,ballTransZ);
	ballTrans = Mult(T(0.01,0,0.01),ballTrans);
	ballTrans.m[7] = ballTransY;
	mat4 ballMat = Mult(camMatrix,ballTrans);
	glUniformMatrix4fv(glGetUniformLocation(ballShader, "mdlMatrix"), 1, GL_TRUE, ballMat.m);
	glUniformMatrix4fv(glGetUniformLocation(ballShader, "camMatrix"), 1, GL_TRUE, camMatrix.m);
	DrawModel(ball,ballShader,"inPosition","inNormal",NULL);


	printError("display 2");

	glutSwapBuffers();
}

void moveCamera(){
	if(glutKeyIsDown('w')){
		//camMatrix = Mult(Rx(0.1),camMatrix);
		angleX -= 0.01;
	}
	if(glutKeyIsDown('s')){
		//camMatrix = Mult(Rx(-0.1),camMatrix);
		angleX += 0.01;
	}
	if(glutKeyIsDown('a')){
		//camMatrix = Mult(Ry(-0.1),camMatrix);
		angleY-= 0.01;
	}
	if(glutKeyIsDown('d')){
		//camMatrix = Mult(Ry(0.1),camMatrix,);
		angleY+= 0.01;
	}
	if(glutKeyIsDown('i')){
		//cameraPos = Mult(cameraPos,T(0,0,0.5));
		camTrans = Mult(T(0,0,0.3),camTrans);
	}
	if(glutKeyIsDown('k')){
		//cameraPos = Mult(cameraPos,T(0,0,-0.5));
		camTrans = Mult(T(0,0,-0.3),camTrans);
	}
	if(glutKeyIsDown('j')){
		//cameraPos = Mult(cameraPos,T(0.5,0,0));
		camTrans = Mult(T(0.3,0,0),camTrans);
	}
	if(glutKeyIsDown('l')){
		//cameraPos = Mult(cameraPos,T(-0.5,0,0));
		camTrans = Mult(T(-0.3,0,0),camTrans);
	}
	if(glutKeyIsDown('r')){
		//cameraPos = Mult(cameraPos,T(-0.5,0,0));
		camTrans = T(0,0,0);
		angleY = 0.0f;
		angleX = 0.0f;
	}
}

void timer(int i)
{
	glutTimerFunc(20, &timer, i);
	glutPostRedisplay();
	moveCamera();

}

void mouse(int x, int y)
{
	printf("%d %d\n", x, y);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitContextVersion(3, 2);
	glutInitWindowSize (600, 600);
	glutCreateWindow ("TSBK07 Lab 4");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &timer, 0);

	glutPassiveMotionFunc(mouse);

	glutMainLoop();
	exit(0);
}
