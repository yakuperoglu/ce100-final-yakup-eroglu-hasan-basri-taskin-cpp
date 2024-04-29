#include <stdexcept>
#include <iostream>
#include "Taskscheduler.h"

using namespace std;

void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

bool enterToContunie(istream& in, ostream& out) {
	out << "\nPress any key to continue";
	in.ignore(numeric_limits<streamsize>::max(), '\n');
	in.get();
	return true;
}

bool handleInputError(istream& in, ostream& out) {
	in.clear();
	in.ignore(numeric_limits<streamsize>::max(), '\n');
	out << "Invalid input. Please enter a number.\n";
	return false;
}

int getInput(istream& in) {
	int choice;
	in >> choice;
	return choice;
}

bool printMainMenu(ostream& out) {
	clearScreen();
	out << "1. Login\n";
	out << "2. Register\n";
	out << "3. Guest Mode\n";
	out << "4. Exit\n";
	return true;
}


int mainMenu(istream& in, ostream& out) {
	int choice;

	while (true) {
		printMainMenu(out);
		choice = getInput(in);

		if (in.fail()) {
			handleInputError(in, out);
			continue;
		}

		switch (choice) {
		case 1:
			clearScreen();
			//loginUser(in, out);
			break;

		case 2:
			clearScreen();
			//registerUser(in, out);
			break;

		case 3:
			clearScreen();
			out << "Guest Operations\n";
			//guestOperation(in, out);
			break;

		case 4:
			clearScreen();
			out << "ExitProgram\n";
			return 0;

		default:
			out << "Invalid choice";
			break;
		}
	}
}// this function provides redirects in MainMenu.