/*
	Author: Saif Mahmud
	Student ID: 3433058
	Date : 4 / 01 / 2023
	Course: COMP 390
	TME = 4
	Program : 1
	Purpose: To develop a program based on the recursive algorithm in unit 10 section 2 objective 3, implementing a recursive routine in the program of unit 11 section 2 objective 1 to produce a similar reflective effect.

	Rewriting the method render() of the program to implement the recursive ray tracing algorithm. The implemented recursive method should cast a ray from the ray source to the scene. The ray may either hit the wall (a non-reflective surface here) or hit the floor (a reflective surface here). If it hits the wall, the method can terminate the recursion and return the local color of the wall. If the ray hits the floor, the method should (recursively) invoke itself with the reflection ray, and calculate and return the mixed color as demonstrated in the original render() routine.
*/


#include <iostream>
#include <math.h>
#include "gl/glut.h"
#include "plane.h"

using namespace std;

// viewer
vector3 viewer(-40.0, 40.0, 120.0);

// floor
vector3 floorNormal(0.0, 1.0, 0.0);
plane   floorDef(0.0, 4.0, 0.0, 0.0);
vector3 floorLimit1(-75.0, 0, -75.0);
vector3 floorLimit2(-75.0, 0, 75.0);
vector3 floorLimit3(75.0, 0, 75.0);
vector3 floorLimit4(75.0, 0, -75.0);

// wall

vector3 wallNormal(0.0, 0.0, 1.0);
plane   wall(0.0, 0.0, 4.0, 0.0);
vector3 wallLimit1(-25.0, 0.0, 0.0);
vector3 wallLimit2(-25.0, 50.0, 0.0);
vector3 wallLimit3(25.0, 50.0, 0.0);
vector3 wallLimit4(25.0, 0.0, 0.0);

// colors
vector3 grey(0.75, 0.75, 0.75);
vector3 lightRed(0.75, 0.1, 0.1);

// lighting position
vector3 lightPosition(25, 25, 100.0);

// lighting elements
float ambient_coef = 0.3;
float diffuse_coef = 0.7;
float reflect_coef = 0.4;
float local_coef = 0.6;

// initialize
void initialize() {
	// set background color
	glClearColor(0.5, 0.7, 0.5, 0.0);
}

// calculate local color
// local color is intensity * base color
vector3 localColor(vector3 intersect, vector3 baseColor, vector3 normal) {

	// calculate unit vector

	vector3 origin = lightPosition.subtract(intersect);
	vector3 unitVec = origin.normalize();

	// calculate dot product
	float dotProd = unitVec.dot(normal);

	// calculate intensity
	float ambientContr = ambient_coef;
	float diffuseContr = diffuse_coef * dotProd;
	float intensity = ambientContr + diffuseContr;

	if (intensity > 1.0) intensity = 1.0;

	float r = intensity * baseColor.x;
	float g = intensity * baseColor.y;
	float b = intensity * baseColor.z;
	
	return vector3(r, g, b);
}



/**
The implemented recursive method should cast a ray from the ray source to the scene.The ray may either hit the wall(a non - reflective surface here) or hit the floor(a reflective surface here).If it hits the wall, the method can terminate the recursion and return the local color of the wall.If the ray hits the floor, the method should(recursively) invoke itself with the reflection ray, and calculate and return the mixed color.
*/
vector3 recursive_ray_tracing_algorithm(vector3 intersect, int currDepth) {

	// Maximum recursion depth
	int max_depth = 10;

	// initialize output
	vector3 outputColor = vector3(0.0, 0.0, 0.0);

	//Calculate incident ray
	vector3 incidentRay = viewer.subtract(intersect);

	//Calculate reflective ray
	vector3 reflect = incidentRay.reflect(floorNormal);

	//Calculate local color	// the surface color of the local object
	vector3 floorLocalColor = localColor(intersect, grey, floorNormal);
	
	// base case - If termination condition is met; termination conditions: maximum recursive calls
	if (currDepth > max_depth) {
		outputColor = floorLocalColor;
	} 
	else if (intersect.z == 0 && intersect.x >= wallLimit1.x && intersect.x <= wallLimit4.x && intersect.y >= wallLimit1.y && intersect.y <= wallLimit2.y) {
		// the main wall. 
		outputColor = localColor(intersect, lightRed, wallNormal);
	}	
	else {
		vector3 sm = wall.intersect(intersect, intersect.add(reflect));
		if ((sm.x >= wallLimit1.x) && (sm.x <= wallLimit4.x) && (sm.y >= wallLimit1.y) && (sm.y <= wallLimit2.y)) {
			// test for inclusion of wall part
			vector3 wallLocalColor = localColor(intersect, lightRed, wallNormal);
			outputColor = floorLocalColor.scalar(local_coef).add(wallLocalColor.scalar(reflect_coef));
		}
		else {
			//If the object at the intersection point is reflective, Call Recursive Ray Tracer to get reflective color
			vector3 reflectionColor = recursive_ray_tracing_algorithm(intersect, currDepth + 1);
			outputColor = floorLocalColor.scalar(local_coef).add(reflectionColor.scalar(reflect_coef));
		}
	}
	return outputColor;
}

// draw the wall - non-reflective surface
void drawWall() {
	// render the wall - non-reflective surface
	for (int m = -25; m < 25; m++) {
		for (int n = 0; n < 50; n++) {

			vector3 intersect = vector3(m, n, 0);
			vector3 color = recursive_ray_tracing_algorithm(intersect, 0);
			glColor3f(color.x, color.y, color.z);
			glRecti(m, n, m + 1, n + 1);

		}
	}
}
// renders the scene

void render() {

	// render the floor
	for (int i = -75; i < 75; i++) {
		for (int j = -75; j < 75; j++) {

			vector3 intersect = vector3(i, 0, j);
			vector3 color = recursive_ray_tracing_algorithm(intersect, 0);	// local color of the floor

			// set the color of the floor got from the recursive function
			glColor3f(color.x, color.y, color.z);

			// Floor _ use the small rectangles method 
			glBegin(GL_POLYGON);
				glVertex3i(i, 0, j);
				glVertex3i(i + 1, 0, j);
				glVertex3i(i + 1, 0, j + 1);
				glVertex3i(i, 0, j + 1);
			glEnd();
		}
	}

	// render the non reflective wall
	drawWall();
}


// display registry, clears the color and depth buffers, sets camera position and orientation and calls the render function 
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity(); // reset the modelview matrix

	gluLookAt(viewer.x, viewer.y, viewer.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // set the camera position and orientation

	render(); // render the scene

	glutSwapBuffers(); //Swap the front and back buffers
}



// reshape registry
// called when window is resized to change the viewport

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the client window area
	glMatrixMode(GL_PROJECTION); // set the projection matrix mode
	glLoadIdentity(); // reset the projection matri
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 200.0); // set the projection frustum
	glMatrixMode(GL_MODELVIEW); // set the modelview matrix mode

}



// main program 

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	int windowHandle = glutCreateWindow("TME 4 _ Q1");
	glutSetWindow(windowHandle);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	initialize();
	glutMainLoop();

}