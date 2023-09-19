/*
	Author: Saif Mahmud
	Date : 4 / 02 / 2023
	Description : To develop a program based on the recursive ray tracing algorithm. The implemented recursive method should cast a ray from the ray source to the scene. The ray may either hit the wall (a non-reflective surface here) or hit the floor (a reflective surface here). If it hits the wall, the method can terminate the recursion and return the local color of the wall. If the ray hits the floor, the method should (recursively) invoke itself with the reflection ray, and calculate and return the mixed color as demonstrated in the original render() routine.
*/

#include "Program1.cpp"
#include "Program2.cpp"

int main(int argc, char** argv) {

	// ask user to select a program
	int programNumber = 0;
	std::cout << "Please select a program to run: " << std::endl;
	std::cout << "1. Program 1: Recursive Ray Tracing" << std::endl;
	std::cout << "2. Program 2: Recursive Ray Tracing With Shadow" << std::endl;

	std::cin >> programNumber;

	// run the selected program
	switch (programNumber) {
		case 1:
			return Program1::main(argc, argv);
		case 2:
			return Program2::main(argc, argv);
	}

	return EXIT_SUCCESS;
}



