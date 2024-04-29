#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

using namespace std;

void clearScreen();

bool enterToContunie(istream& in, ostream& out);

bool handleInputError(istream& in, ostream& out);

int getInput(istream& in);

bool printMainMenu(ostream& out);


int mainMenu(istream& in, std::ostream& out);

#endif // TASKSCHEDULER_H