

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



