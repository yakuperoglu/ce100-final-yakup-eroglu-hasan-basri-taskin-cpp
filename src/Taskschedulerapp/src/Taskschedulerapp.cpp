/**
 * @file main.cpp
 * @brief Main file for the Task Scheduler application.
 *
 * This file contains the main function which initiates the Task Scheduler application by calling the main menu.
 * The application is designed to manage and schedule tasks efficiently using various algorithms.
 */

#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <stdexcept>
#include "Taskscheduler.h"

using namespace std;

/**
 * @brief The entry point of the Task Scheduler application.
 *
 * This function serves as the entry point for the Task Scheduler application.
 * It calls the mainMenu function to display the main menu of the application.
 *
 * @param in Input stream for the application, typically std::cin.
 * @param out Output stream for the application, typically std::cout.
 * @return int Returns 0 upon successful execution.
 *
 * @exception std::runtime_error Throws a runtime error if the mainMenu function encounters an issue.
 *
 * @note The mainMenu function is responsible for managing user interactions and executing the appropriate task scheduling algorithms.
 */
int main() {

	mainMenu(cin, cout);
	return 0;
}

