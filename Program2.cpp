/*
	Author: Saif Mahmud
	Date : 4 / 03 / 2023

	Purpose: Adding a test to the program of recursive ray tracer of my Program1 to check if the intersection point (of the floor) can see the light source or not. If not, mixing the local color with the shadow color.
*/


#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "plane.h"

namespace Program2 {

	using namespace std;

	// viewer
	static vector3 viewer(-40.0, 40.0, 120.0);

	// floor
	static vector3 floorNormal(0.0, 1.0, 0.0);
	static plane   floorDef(0.0, 4.0, 0.0, 0.0);
	static vector3 floorLimit1(-75.0, 0, -75.0);
	static vector3 floorLimit2(-75.0, 0, 75.0);
	static vector3 floorLimit3(75.0, 0, 75.0);
	static vector3 floorLimit4(75.0, 0, -75.0);

	// wall

	static vector3 wallNormal(0.0, 0.0, 1.0);
	static plane wall(0.0, 0.0, 4.0, 0.0);
	static vector3 wallLimit1(-25.0, 0.0, 0.0);
	static vector3 wallLimit2(-25.0, 50.0, 0.0);
	static vector3 wallLimit3(25.0, 50.0, 0.0);
	static vector3 wallLimit4(25.0, 0.0, 0.0);

	// colors
	static vector3 floorColor(0.65, 0.85, 0.95); // Darker Blue
	static vector3 wallColor(0.686, 0.796, 1.0); // Pastel Blue
	static vector3 border(0.0, 0.0, 0.0); 
	static GLfloat borderWidth = 1.0;

	// background Color
	static GLclampf red = 0.8;
	static GLclampf green = 0.8;
	static GLclampf blue = 0.8;
	static GLclampf alpha = 1.0; // Fully opaque

	// lighting position
	static vector3 lightPosition(25, 25, 100.0);

	// lighting elements
	static float ambient_coef = 0.3;
	static float diffuse_coef = 0.7;
	static float reflect_coef = 0.4;
	static float local_coef = 0.6;


	// calculate local color
	// local color is intensity * base color
	static vector3 localColor(vector3 intersect, vector3 baseColor, vector3 normal) {

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
	static vector3 recursive_ray_tracing_algorithm(vector3 intersect, int currDepth) {

		// Maximum recursion depth
		int max_depth = 3;

		// initialize output
		vector3 outputColor = vector3(0.0, 0.0, 0.0);

		//Calculate incident ray
		vector3 incidentRay = viewer.subtract(intersect);

		//Calculate reflective ray
		vector3 reflect = incidentRay.reflect(floorNormal);

		vector3 sm = wall.intersect(intersect, intersect.add(reflect));

		//Calculate the surface color of the local object
		vector3 floorLocalColor = localColor(intersect, floorColor, floorNormal);
		vector3 wallLocalColor = localColor(intersect, wallColor, wallNormal);

		vector3 colorToBeMixed = outputColor;

		// to handle shadow
		vector3 st = wall.intersect(intersect, lightPosition);

		// base case - If termination condition is met; termination conditions: maximum recursive calls
		if (currDepth > max_depth) {
			outputColor = floorLocalColor;
		}

		// if it is the non reflected main wall
		else if (intersect.z == 0 && intersect.x >= wallLimit1.x && intersect.x <= wallLimit4.x && intersect.y >= wallLimit1.y && intersect.y <= wallLimit2.y) {
			outputColor = wallLocalColor;
		}

		// if it is the shadow part on the floor,
		else if ((st.x >= wallLimit1.x) && (st.x <= wallLimit4.x) && (st.y >= wallLimit1.y) && (st.y <= wallLimit2.y)) {
			outputColor = floorColor.scalar(ambient_coef); // shadow color (ambient color)
		}

		// else it is on the floor without the shawdow part
		else {
			//If the object at the intersection point is reflective, Call Recursive Ray Tracer to get reflective color
			vector3 returnedRayColor = recursive_ray_tracing_algorithm(intersect, currDepth + 1);

			// determining which color to mix on the floor, is it red or gray.

			if ((sm.x >= wallLimit1.x) && (sm.x <= wallLimit4.x) && (sm.y >= wallLimit1.y) && (sm.y <= wallLimit2.y)) {
				// test for inclusion of wall part on the floor
				colorToBeMixed = wallLocalColor;
			}
			else {
				colorToBeMixed = returnedRayColor;
			}

			// now mix the reflected color
			outputColor = floorLocalColor.scalar(local_coef).add(colorToBeMixed.scalar(reflect_coef));
		}
		return outputColor;
	}

	// draw the wall - non-reflective surface
	static void drawWall() {
		// render the wall - non-reflective surface
		for (int m = -25; m < 25; m++) {
			for (int n = 0; n < 50; n++) {

				vector3 intersect = vector3(m, n, 0);
				vector3 color = recursive_ray_tracing_algorithm(intersect, 0);
				glColor3f(color.x, color.y, color.z);
				glRecti(m, n, m + 1, n + 1);

			}
		}

		// Draw black border around the wall
		glColor3f(border.x, border.y, border.z);
		glLineWidth(borderWidth); // Set line width for the border

		// Draw the border as a line loop on the wall border
		glBegin(GL_LINE_LOOP);
		glVertex3i(-25, -1, 0); // Bottom left corner
		glVertex3i(25, -1, 0);  // Bottom right corner
		glVertex3i(25, 50, 0);  // Top right corner
		glVertex3i(-25, 50, 0); // Top left corner
		glEnd();
	}

	// draw the floor - reflective surface
	static void drawFloor() {
		float gridSize = 0.5;
		for (float i = -75.0; i < 75.0; i = i + gridSize) {
			for (float j = -75.0; j < 75.0; j = j + gridSize) {


				vector3 intersect = vector3(i, 0, j);
				vector3 color = recursive_ray_tracing_algorithm(intersect, 0);	// local color of the floor

				// set the color of the floor got from the recursive function
				glColor3f(color.x, color.y, color.z);

				// Floor _ use the small rectangles method 
				glBegin(GL_POLYGON);
				glVertex3f(i, 0.0, j);
				glVertex3f(i + gridSize, 0.0, j);
				glVertex3f(i + gridSize, 0.0, j + gridSize);
				glVertex3f(i, 0.0, j + gridSize);
				glEnd();
			}
		}
	}

	// initialize
	static void initialize() {
		// set background color
		glClearColor(red, green, blue, alpha);
		// Enable multisampling for anti-aliasing
		glEnable(GL_LINE_SMOOTH);  //enable line anti-aliasing
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_SMOOTH);

		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  //set the quality of anti-aliasing
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);  //set the quality of anti-aliasing
	}


	// renders the scene
	static void render() {

		// render the reflective floor
		drawFloor();

		// render the non reflective wall
		drawWall();
	}


	// display registry, clears the color and depth buffers, sets camera position and orientation and calls the render function 
	static void display(void) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity(); // reset the modelview matrix

		gluLookAt(viewer.x, viewer.y, viewer.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // set the camera position and orientation

		render(); // render the scene

		glutSwapBuffers(); //Swap the front and back buffers
	}



	// reshape registry
	// called when window is resized to change the viewport

	static void reshape(int w, int h) {
		glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the client window area
		glMatrixMode(GL_PROJECTION); // set the projection matrix mode
		glLoadIdentity(); // reset the projection matri
		glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 200.0); // set the projection frustum
		glMatrixMode(GL_MODELVIEW); // set the modelview matrix mode

	}



	// main program 

	static int main(int argc, char** argv)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
		glutInitWindowSize(500, 500);
		glutInitWindowPosition(100, 100);

		int windowHandle = glutCreateWindow("Ray Tracing Algorithm With Shadow");
		glutSetWindow(windowHandle);

		glutDisplayFunc(display);
		glutReshapeFunc(reshape);

		initialize();
		glutMainLoop();

		return EXIT_SUCCESS;

	}
}