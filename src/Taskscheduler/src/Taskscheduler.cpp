/**
 * @file main.cpp
 * @brief Main file for the Task Scheduler application.
 *
 * This file contains the necessary includes, global variables, and utility functions for the Task Scheduler application.
 * The application manages and schedules tasks using various algorithms and provides encryption using the AES algorithm.
 */

#include <stdexcept>
#include <iostream>
#include "Taskscheduler.h"
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>
#include <list>
#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>
#include <climits>
#include <set>
#include "../../aes/header/aes.h"

using namespace std;

/**
 * @brief Represents the currently logged-in user.
 */
User loggedUser;

/**
 * @brief Path to the binary file where user data is stored.
 */
const char* pathFileUsers = "Users.bin";

/**
 * @brief Path to the binary file where task data is stored.
 */
const char* pathFileTasks = "Tasks.bin";

/**
 * @brief Maximum number of tasks that can be managed by the application.
 */
const int MAX_TASKS = 100;

/**
 * @brief Adjacency list for storing task dependencies.
 */
list<int> adjLists[MAX_TASKS];

/**
 * @brief Map for storing Huffman codes.
 */
unordered_map<char, string> codes;

/**
 * @brief List of pairs representing the adjacency list for the graph.
 */
list<pair<int, int>> adj[MAX_TASKS];

/**
 * @brief AES encryption key.
 */
const uint8_t AES_KEY[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

/**
 * @brief AES initialization vector.
 */
const uint8_t AES_IV[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

//TOOLS

/**
 * @brief Clears the console screen.
 *
 * This function clears the console screen by calling the appropriate system command based on the operating system.
 */
void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

/**
 * @brief Prompts the user to press enter to continue.
 *
 * This function outputs a message prompting the user to press enter to continue and waits for the user to press enter.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the prompt message.
 * @return true Always returns true.
 */
bool enterToContinue(istream& in, ostream& out) {
	out << "\nPress enter to continue";
	while (in.get() != '\n');
	return true;
}

/**
 * @brief Handles input errors.
 *
 * This function clears the input stream in case of an error and displays an error message to the user.
 *
 * @param in Input stream to be cleared.
 * @param out Output stream for displaying the error message.
 * @return true Always returns true.
 */
bool handleInputError(istream& in, ostream& out) {
	int c;
	while ((c = in.get()) != '\n' && c != EOF);
	out << "Invalid input. Please enter a number." << std::endl;
	return true;
}
/**
 * @brief Reads an integer input from the input stream.
 *
 * This function reads a line from the input stream and attempts to convert it to an integer.
 * If the input is not a valid integer, it returns an error code.
 *
 * @param in Input stream to read from.
 * @return int The integer input if valid, -1 if the input stream fails, -2 if the input is not a valid integer.
 */
int getInput(istream& in) {
	string line;
	int choice;

	if (!getline(in, line)) {
		return -1;
	}

	istringstream iss(line);
	if (!(iss >> choice)) {
		return -2;
	}

	return choice;
}

/**
 * @brief Generates a new user ID.
 *
 * This function generates a new user ID based on the maximum existing user ID in the array of users.
 * It uses a max heap to find the maximum ID efficiently.
 *
 * @param users Array of User objects.
 * @param userCount The number of users in the array.
 * @return int A new unique user ID.
 */
int getNewUserId(User users[], int userCount) {
	if (userCount == 0)
		return 1;

	buildMaxHeap(users, userCount);

	int maxId = users[0].id;

	return maxId + 1;
}

/**
 * @brief Generates a new task ID.
 *
 * This function generates a new task ID based on the maximum existing task ID in the binary file.
 * It reads the tasks from the binary file and finds the maximum ID.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @return int A new unique task ID.
 */
int getNewTaskId(const char* pathFileTasks) {
	FILE* file = fopen(pathFileTasks, "rb");
	if (!file) {
		return 1;
	}

	Task task;
	int maxId = 0;

	while (fread(&task, sizeof(Task), 1, file)) {
		if (task.id > maxId) {
			maxId = task.id;
		}
	}

	fclose(file);

	return maxId + 1;
}


//TOOLS



//PRINT MENUS

/**
 * @brief Prints the guest menu.
 *
 * This function displays the guest menu options to the output stream.
 *
 * @param out Output stream for displaying the menu.
 * @return bool Always returns true.
 */
bool printGuestMenu(ostream& out) {
	out << "1. View Categories\n";
	out << "2. Exit";
	return true;
}

/**
 * @brief Prints the main menu.
 *
 * This function clears the screen and displays the main menu options to the output stream.
 *
 * @param out Output stream for displaying the menu.
 * @return bool Always returns true.
 */
bool printMainMenu(ostream& out) {
	clearScreen();
	out << "1. Login\n";
	out << "2. Register\n";
	out << "3. Guest Mode\n";
	out << "4. Exit\n";
	return true;
}

/**
 * @brief Prints the user menu.
 *
 * This function clears the screen and displays the user menu options to the output stream.
 * It includes options for creating tasks, setting deadlines, managing reminders, task prioritization, and flow algorithms.
 *
 * @param out Output stream for displaying the menu.
 * @return bool Always returns true.
 */
bool printUserMenu(ostream& out) {
	clearScreen();
	out << "Welcome " << loggedUser.name << " " << loggedUser.surname << " Please Enter Your Choice\n";
	out << "1. Create Task\n";
	out << "2. Deadline Settings\n";
	out << "3. Reminder System\n";
	out << "4. Task Prioritization\n";
	out << "5. Flow Algorithms\n";  // New menu option
	out << "6. Exit\n";
	return true;
}

/**
 * @brief Prints the create task menu.
 *
 * This function clears the screen and displays the create task menu options to the output stream.
 * It includes options for adding, categorizing, viewing tasks, viewing similar tasks, task allegiances, SCC analysis, and Huffman encoding.
 *
 * @param out Output stream for displaying the menu.
 * @return bool Always returns true.
 */
bool printCreateTaskMenu(ostream& out) {
	clearScreen();
	out << "1. Add Task\n";
	out << "2. Categorize Tasks\n";
	out << "3. View Tasks\n";
	out << "4. View Similar Tasks\n";
	out << "5. Task Allegiances\n";
	out << "6. Analyze SCC\n";
	out << "7. Encode With Huffman\n";
	out << "8. Exit\n";
	return true;
}

/**
 * @brief Prints the deadline settings menu.
 *
 * This function clears the screen and displays the deadline settings menu options to the output stream.
 * It includes options for assigning and viewing deadlines.
 *
 * @param out Output stream for displaying the menu.
 * @return bool Always returns true.
 */
bool printDeadlineSettingsMenu(ostream& out) {
	clearScreen();
	out << "1. Assign Deadline\n";
	out << "2. View Deadlines\n";
	out << "3. Exit\n";
	return true;
}

/**
 * @brief Prints the reminder system menu.
 *
 * This function clears the screen and displays the reminder system menu options to the output stream.
 * It includes options for setting reminders and managing notification settings.
 *
 * @param out Output stream for displaying the menu.
 * @return bool Always returns true.
 */
bool printReminderSystemMenu(ostream& out) {
	clearScreen();
	out << "1. Set Reminders\n";
	out << "2. Notification Settings\n";
	out << "3. Exit\n";
	return true;
}

/**
 * @brief Prints the task prioritization menu.
 *
 * This function clears the screen and displays the task prioritization menu options to the output stream.
 * It includes options for marking task importance, reordering tasks, calculating MST for task dependencies, and calculating the shortest path.
 *
 * @param out Output stream for displaying the menu.
 * @return bool Always returns true.
 */
bool printTaskPrioritizationMenu(ostream& out) {
	clearScreen();
	out << "1. Mark Task Importance\n";
	out << "2. Reorder Task\n";
	out << "3. Calculate MST for Task Dependencies\n";
	out << "4. Calculate Shortest Path\n";
	out << "5. Exit\n";
	return true;
}

/**
 * @brief Prints the flow algorithms menu.
 *
 * This function clears the screen and displays the flow algorithms menu options to the output stream.
 * It includes options for choosing between the Ford-Fulkerson and Edmonds-Karp algorithms.
 *
 * @param out Output stream for displaying the menu.
 * @return bool Always returns true.
 */
bool printFlowAlgorithmsMenu(ostream& out) {
	clearScreen();
	out << "Choose flow algorithm:\n";
	out << "1. Ford-Fulkerson Algorithm\n";
	out << "2. Edmonds-Karp Algorithm\n";
	out << "3. Exit\n";
	return true;
}


//PRINT MENUS



//MENUS

/**
 * @brief Displays and handles the create task menu.
 *
 * This function displays the create task menu and processes user input to perform various task-related operations.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 0 when the user chooses to exit the menu.
 */
int createTaskMenu(istream& in, ostream& out) {
	int choice;

	while (true) {
		printCreateTaskMenu(out);
		choice = getInput(in);

		if (in.fail()) { handleInputError(in, out); continue; }

		switch (choice) {
		case 1:
			addTaskMenu(pathFileTasks, in, out);
			break;
		case 2:
			categorizeTask(pathFileTasks, in, out);
			break;
		case 3:
			viewTask(pathFileTasks, in, out);
			break;
		case 4:
			similarTasks(pathFileTasks, in, out);
			break;
		case 5:
			allegiances(pathFileTasks, in, out);
			break;
		case 6:
			analyzeSCC(pathFileTasks, in, out);
			break;
		case 7:
			huffmanEncodingTaskMenu(pathFileTasks, in, out);
			break;
		case 8:
			return 0;
		default:
			out << "\nInvalid choice. Please try again.\n";
			enterToContinue(in, out);
			break;
		}
	}
}

/**
 * @brief Displays and handles the deadline settings menu.
 *
 * This function displays the deadline settings menu and processes user input to assign or view deadlines.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 0 when the user chooses to exit the menu.
 */
int deadlineSettingsMenu(istream& in, ostream& out) {
	int choice;

	while (true) {
		printDeadlineSettingsMenu(out);
		choice = getInput(in);

		if (in.fail()) { handleInputError(in, out); continue; }

		switch (choice) {
		case 1:
			assignDeadline(pathFileTasks, in, out);
			break;
		case 2:
			viewDeadlines(pathFileTasks, in, out);
			break;
		case 3:
			return 0;
		default:
			out << "\nInvalid choice. Please try again.\n";
			enterToContinue(in, out);
			break;
		}
	}
}

/**
 * @brief Displays and handles the reminder system menu.
 *
 * This function displays the reminder system menu and processes user input to set reminders or manage notification settings.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 0 when the user chooses to exit the menu.
 */
int reminderSystemMenu(istream& in, ostream& out) {
	int choice;

	while (true) {
		printReminderSystemMenu(out);
		choice = getInput(in);

		if (in.fail()) { handleInputError(in, out); continue; }

		switch (choice) {
		case 1:
			setReminders(in, out);
			break;
		case 2:
			notificationSettings(in, out);
			break;
		case 3:
			return 0;
		default:
			out << "\nInvalid choice. Please try again.\n";
			enterToContinue(in, out);
			break;
		}
	}
}

/**
 * @brief Displays and handles the task prioritization menu.
 *
 * This function displays the task prioritization menu and processes user input to mark task importance, reorder tasks, calculate MST, or find the shortest path.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 0 when the user chooses to exit the menu.
 */
int taskPrioritizationMenu(istream& in, ostream& out) {
	int choice;

	while (true) {
		printTaskPrioritizationMenu(out);
		choice = getInput(in);

		if (in.fail()) { handleInputError(in, out); continue; }

		switch (choice) {
		case 1:
			markTaskImportance(pathFileTasks, in, out);
			break;
		case 2:
			reorderTask(pathFileTasks, in, out);
			break;
		case 3:
			calculateMST(in, out);
			break;
		case 4:
			shortestPath(in, out);
			break;
		case 5:
			return 0;
		default:
			out << "\nInvalid choice. Please try again.\n";
			enterToContinue(in, out);
			break;
		}
	}
}

/**
 * @brief Displays and handles the flow algorithms menu.
 *
 * This function displays the flow algorithms menu and processes user input to choose between the Ford-Fulkerson and Edmonds-Karp algorithms for calculating maximum flow.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 0 when the user chooses to exit the menu.
 */
int flowAlgorithmsMenu(istream& in, ostream& out) {
	int choice;
	int source, sink;
	int V = MAX_TASKS;
	int** graph = new int* [V];
	for (int i = 0; i < V; i++) {
		graph[i] = new int[V]();
	}

	while (true) {
		printFlowAlgorithmsMenu(out);
		choice = getInput(in);

		if (in.fail()) { handleInputError(in, out); continue; }

		if (choice == 3) {
			for (int i = 0; i < V; i++) {
				delete[] graph[i];
			}
			delete[] graph;
			return 0;
		}

		viewTaskForFunc(pathFileTasks, in, out);
		out << "\nEnter the source task ID:\n";
		source = getInput(in);
		out << "Enter the sink task ID:\n";
		sink = getInput(in);

		if (source < 0 || source >= V || sink < 0 || sink >= V) {
			out << "Invalid task ID.\n";
			enterToContinue(in, out);
			continue;
		}

		loadTasksAndDependencies(pathFileTasks);

		int maxFlow = 0;
		if (choice == 1) {
			maxFlow = fordFulkerson(graph, source, sink, V);
		}
		else if (choice == 2) {
			maxFlow = edmondsKarp(graph, source, sink, V);
		}
		else {
			out << "Invalid choice.\n";
			enterToContinue(in, out);
			continue;
		}

		out << "Maximum flow from task " << source << " to task " << sink << " is: " << maxFlow << endl;
		enterToContinue(in, out);
	}
}


//MENUS

/**
 * @brief Displays and handles viewing tasks.
 *
 * This function clears the screen and displays all tasks owned by the logged-in user.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the tasks and messages.
 * @return bool Always returns true.
 */
bool viewTask(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task* tasks = NULL;
	int taskCount = loadOwnedTasks(pathFileTasks, &tasks, loggedUser.id);

	if (taskCount <= 0) {
		out << "No Task Created.\n";
	}
	else {
		for (int i = 0; i < taskCount; i++) {
			out << "ID: " << tasks[i].id << ", Name: " << tasks[i].name
				<< ", Description: " << tasks[i].description << ", Category: "
				<< tasks[i].category << ", Deadline: " << tasks[i].deadLine << endl;
		}
	}

	enterToContinue(in, out);
	free(tasks);
	return true;
}

/**
 * @brief Displays tasks for function usage.
 *
 * This function clears the screen and displays all tasks owned by the logged-in user without waiting for user input.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the tasks and messages.
 * @return bool Always returns true.
 */
bool viewTaskForFunc(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task* tasks = NULL;
	int taskCount = loadOwnedTasks(pathFileTasks, &tasks, loggedUser.id);

	if (taskCount <= 0) {
		out << "No Task Created.\n";
	}
	else {
		for (int i = 0; i < taskCount; i++) {
			out << "ID: " << tasks[i].id << ", Name: " << tasks[i].name
				<< ", Description: " << tasks[i].description << ", Category: "
				<< tasks[i].category << ", Deadline: " << tasks[i].deadLine << endl;
		}
	}

	free(tasks);
	return true;
}

/**
 * @brief Displays tasks with deadlines.
 *
 * This function clears the screen and displays all tasks owned by the logged-in user that have deadlines assigned.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the tasks and messages.
 * @return bool Always returns true.
 */
bool viewDeadlines(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task* tasks = nullptr;
	int taskCount = loadOwnedTasks(pathFileTasks, &tasks, loggedUser.id);

	if (taskCount <= 0) {
		out << "No Task Created or Assigned to You." << endl;
		enterToContinue(in, out);
		free(tasks);
		return false;
	}

	bool hasDeadlines = false;
	for (int i = 0; i < taskCount; ++i) {
		if (tasks[i].isDeadlined) {
			out << "ID: " << tasks[i].id << ", Name: " << tasks[i].name
				<< ", Description: " << tasks[i].description << ", Category: "
				<< tasks[i].category << ", Deadline: " << tasks[i].deadLine << endl;
			hasDeadlines = true;
		}
	}

	if (!hasDeadlines) {
		out << "No Task with Deadline Assigned to You." << endl;
	}

	enterToContinue(in, out);
	free(tasks);
	return true;
}

/**
 * @brief Loads all tasks from the binary file.
 *
 * This function loads all tasks from the specified binary file and stores them in the provided array.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param tasks Pointer to an array of Task objects.
 * @return int The number of tasks loaded.
 */
int loadTasks(const char* pathFileTasks, Task** tasks) {
	FILE* file = fopen(pathFileTasks, "rb");
	if (!file) {
		printf("Failed to open file\n");
		return -1;
	}

	Task temp;
	int count = 0;
	while (fread(&temp, sizeof(Task), 1, file) == 1) {
		*tasks = (Task*)realloc(*tasks, (count + 1) * sizeof(Task));
		(*tasks)[count] = temp;
		count++;
	}
	fclose(file);
	return count;
}

/**
 * @brief Loads tasks owned by a specific user from the binary file.
 *
 * This function loads tasks owned by the specified user from the binary file and stores them in the provided array.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param tasks Pointer to an array of Task objects.
 * @param userId The ID of the user whose tasks are to be loaded.
 * @return int The number of tasks loaded.
 */
int loadOwnedTasks(const char* pathFileTasks, Task** tasks, int userId) {
	FILE* file = fopen(pathFileTasks, "rb");
	if (!file) {
		printf("Failed to open file\n");
		return -1;
	}

	Task temp;
	int count = 0;
	while (fread(&temp, sizeof(Task), 1, file) == 1) {
		if (temp.owner.id == userId) {
			*tasks = (Task*)realloc(*tasks, (count + 1) * sizeof(Task));
			(*tasks)[count] = temp;
			count++;
		}
	}
	fclose(file);
	return count;
}

/**
 * @brief Adds a new task to the binary file.
 *
 * This function appends a new task to the specified binary file.
 *
 * @param newTask Pointer to the Task object to be added.
 * @param pathFileTasks Path to the binary file containing tasks.
 * @return int Returns 1 if the task is added successfully, otherwise 0.
 */
int addTask(const Task* newTask, const char* pathFileTasks) {
	FILE* file = fopen(pathFileTasks, "ab");
	if (!file) {
		return 0;
	}

	fwrite(newTask, sizeof(Task), 1, file);
	fclose(file);
	return 1;
}

/**
 * @brief Displays and handles the add task menu.
 *
 * This function displays the add task menu and processes user input to add a new task.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 1 if the task is added successfully, otherwise 0.
 */
int addTaskMenu(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task newTask;

	out << "Enter Task Name: ";
	in.getline(newTask.name, sizeof(newTask.name));

	out << "Enter Task Description: ";
	in.getline(newTask.description, sizeof(newTask.description));

	newTask.owner = loggedUser;
	newTask.id = getNewTaskId(pathFileTasks);
	newTask.isCategorized = false;
	newTask.isDeadlined = false;
	newTask.impid = 0;
	newTask.category[0] = '\0';
	newTask.deadLine[0] = '\0';
	newTask.numDependencies = 0;

	out << "Enter number of dependencies for this task: ";
	int numDeps;
	in >> numDeps;
	in.ignore();
	newTask.numDependencies = numDeps;

	for (int i = 0; i < numDeps; i++) {
		out << "Enter dependency task ID #" << (i + 1) << ": ";
		in >> newTask.dependencies[i];
		in.ignore();
	}

	if (!addTask(&newTask, pathFileTasks)) {
		out << "Failed to add Task.\n";
		enterToContinue(in, out);
		return 0;
	}

	out << "Task added successfully.\n";
	enterToContinue(in, out);
	return 1;
}

/**
 * @brief Displays and handles the categorize task menu.
 *
 * This function displays the categorize task menu and processes user input to categorize tasks.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 1 if the task is categorized successfully, otherwise 0.
 */
int categorizeTask(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task* tasks = nullptr;
	int taskCount = loadOwnedTasks(pathFileTasks, &tasks, loggedUser.id);
	int uncategorizedTaskCount = 0;

	for (int i = 0; i < taskCount; ++i) {
		if (!tasks[i].isCategorized) {
			out << "ID: " << tasks[i].id << ", Name: " << tasks[i].name
				<< ", Description: " << tasks[i].description << endl;
			uncategorizedTaskCount++;
		}
	}

	if (taskCount <= 0 || uncategorizedTaskCount == 0) {
		out << "No tasks available to categorize." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	out << "\nEnter the ID of the task you want to categorize: ";
	int selectedTaskId = getInput(in);

	Task* selectedTask = nullptr;
	for (int i = 0; i < taskCount; ++i) {
		if (tasks[i].id == selectedTaskId && !tasks[i].isCategorized) {
			selectedTask = &tasks[i];
			break;
		}
	}

	if (!selectedTask) {
		out << "Invalid task ID. Please try again." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	out << "Select a category for the task:" << endl;
	out << "1. Work" << endl;
	out << "2. Sport" << endl;
	out << "3. Diet" << endl;
	out << "4. Study" << endl;

	int categoryChoice = getInput(in);

	switch (categoryChoice) {
	case 1:
		strcpy(selectedTask->category, "Work");
		break;
	case 2:
		strcpy(selectedTask->category, "Sport");
		break;
	case 3:
		strcpy(selectedTask->category, "Diet");
		break;
	case 4:
		strcpy(selectedTask->category, "Study");
		break;
	default:
		out << "Invalid category choice. Please try again." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	selectedTask->isCategorized = true;

	FILE* file = fopen(pathFileTasks, "wb");

	for (int i = 0; i < taskCount; ++i) {
		fwrite(&tasks[i], sizeof(Task), 1, file);
	}

	fclose(file);
	out << "Task categorized successfully." << endl;
	enterToContinue(in, out);
	free(tasks);
	return 1;
}

/**
 * @brief Displays and handles the assign deadline menu.
 *
 * This function displays the assign deadline menu and processes user input to assign deadlines to tasks.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 1 if the deadline is assigned successfully, otherwise 0.
 */
int assignDeadline(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task* tasks = nullptr;
	int taskCount = loadOwnedTasks(pathFileTasks, &tasks, loggedUser.id);
	int unDeadlinedTaskCount = 0;

	for (int i = 0; i < taskCount; ++i) {
		if (!tasks[i].isDeadlined) {
			out << "ID: " << tasks[i].id << ", Name: " << tasks[i].name
				<< ", Description: " << tasks[i].description << endl;
			unDeadlinedTaskCount++;
		}
	}

	if (taskCount <= 0 || unDeadlinedTaskCount == 0) {
		out << "No Tasks Available To Add Deadline." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	out << "\nEnter The ID Of The Task You Want To Add Deadline: ";
	int selectedTaskId = getInput(in);

	Task* selectedTask = nullptr;
	for (int i = 0; i < taskCount; ++i) {
		if (tasks[i].id == selectedTaskId && !tasks[i].isDeadlined) {
			selectedTask = &tasks[i];
			break;
		}
	}

	if (!selectedTask) {
		out << "Invalid Task ID. Please Try again." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	out << "Enter Deadline Day (1-31): ";
	int day = getInput(in);
	out << "Enter Deadline Month (1-12): ";
	int month = getInput(in);
	out << "Enter Deadline Year: ";
	int year = getInput(in);

	if (day < 1 || day > 31 || month < 1 || month > 12 || year < 0) {
		out << "Invalid date. Please enter valid day, month, and year." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	stringstream deadlineStream;
	deadlineStream << day << "/" << month << "/" << year;
	string deadlineStr = deadlineStream.str();

	strncpy(selectedTask->deadLine, deadlineStr.c_str(), sizeof(selectedTask->deadLine) - 1);
	selectedTask->deadLine[sizeof(selectedTask->deadLine) - 1] = '\0';

	selectedTask->isDeadlined = true;

	FILE* file = fopen(pathFileTasks, "wb");
	for (int i = 0; i < taskCount; ++i) {
		fwrite(&tasks[i], sizeof(Task), 1, file);
	}
	fclose(file);

	out << "Deadline Added Successfully." << endl;
	enterToContinue(in, out);
	free(tasks);
	return 1;
}

/**
 * @brief Sets a reminder.
 *
 * This function sets a reminder for a specified duration.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying messages.
 * @return int Always returns 0.
 */
int setReminders(istream& in, ostream& out) {
	int seconds, minutes, hours, days;

	out << "Enter the reminder duration:" << endl;
	out << "Seconds: ";
	seconds = getInput(in);
	out << "Minutes: ";
	minutes = getInput(in);
	out << "Hours: ";
	hours = getInput(in);
	out << "Days: ";
	days = getInput(in);

	int totalSeconds = seconds + minutes * 60 + hours * 3600 + days * 86400;

	if (totalSeconds <= 0) {
		out << "Invalid duration. Please enter a positive duration." << endl;
		enterToContinue(in, out);
		return 0;
	}

	out << "Setting reminder for " << totalSeconds << " seconds..." << endl;
	this_thread::sleep_for(chrono::seconds(totalSeconds));
	out << "Time's up! Reminder triggered after " << totalSeconds << " seconds." << endl;

	enterToContinue(in, out);
	return 0;
}

/**
 * @brief Manages notification settings.
 *
 * This function displays and processes user input for managing notification settings.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying messages.
 * @return int Always returns 0.
 */
int notificationSettings(istream& in, ostream& out) {
	int choice;

	out << "Select notification method:" << endl;
	out << "1. SMS" << endl;
	out << "2. E-Mail" << endl;
	out << "3. Notification" << endl;
	out << "Enter your choice: ";
	choice = getInput(in);

	switch (choice) {
	case 1:
		out << "Your reminder has been set to SMS." << endl;
		enterToContinue(in, out);
		break;
	case 2:
		out << "Your reminder has been set to E-Mail." << endl;
		enterToContinue(in, out);
		break;
	case 3:
		out << "Your reminder has been set to Notification." << endl;
		enterToContinue(in, out);
		break;
	default:
		out << "Invalid choice. Please try again." << endl;
		enterToContinue(in, out);
		break;
	}

	return 0;
}

/**
 * @brief Marks the importance of a task.
 *
 * This function displays tasks and allows the user to mark the importance of a selected task.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 1 if the importance is marked successfully, otherwise 0.
 */
int markTaskImportance(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task* tasks = nullptr;
	int taskCount = loadOwnedTasks(pathFileTasks, &tasks, loggedUser.id);
	bool taskAlreadyMarked = false;
	int unMarkedTaskCount = 0;

	for (int i = 0; i < taskCount; ++i) {
		if (tasks[i].impid == 0 && !taskAlreadyMarked) {
			out << "ID: " << tasks[i].id << ", Name: " << tasks[i].name
				<< ", Description: " << tasks[i].description << endl;
			unMarkedTaskCount++;
		}
	}

	if (taskCount <= 0 || unMarkedTaskCount == 0) {
		out << "No tasks available to mark importance." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}
	out << "Select the task to mark importance by entering its ID:" << endl;

	int selectedTaskId;
	selectedTaskId = getInput(in);

	Task* selectedTask = nullptr;
	for (int i = 0; i < taskCount; ++i) {
		if (tasks[i].id == selectedTaskId && tasks[i].impid == 0) {
			selectedTask = &tasks[i];
			taskAlreadyMarked = true;
			break;
		}
	}

	if (!selectedTask) {
		out << "Invalid task ID. Please try again." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	out << "Enter the importance ID for the task: ";
	int importanceId;
	importanceId = getInput(in);

	if (importanceId < 0) {
		out << "Invalid input" << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	selectedTask->impid = importanceId;

	FILE* file = fopen(pathFileTasks, "wb");
	if (!file) {
		out << "Failed to open file for writing." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	for (int i = 0; i < taskCount; ++i) {
		if (tasks[i].id == selectedTaskId) {
			fwrite(selectedTask, sizeof(Task), 1, file);
		}
		else {
			fwrite(&tasks[i], sizeof(Task), 1, file);
		}
	}
	fclose(file);

	out << "Task importance marked successfully." << endl;
	enterToContinue(in, out);
	free(tasks);
	return 1;
}

/**
 * @brief Reorders tasks based on importance.
 *
 * This function displays tasks and allows the user to reorder a selected task by changing its importance ID.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 1 if the task is reordered successfully, otherwise 0.
 */
int reorderTask(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task* tasks = nullptr;
	int taskCount = loadOwnedTasks(pathFileTasks, &tasks, loggedUser.id);
	int reorderedTaskCount = 0;

	out << "Tasks with importance ID:" << endl;
	for (int i = 0; i < taskCount; ++i) {
		if (tasks[i].impid != 0) {
			out << "ID: " << tasks[i].id << ", Importance ID: " << tasks[i].impid
				<< ", Name: " << tasks[i].name << ", Description: " << tasks[i].description
				<< ", Category: " << tasks[i].category << ", Deadline: " << tasks[i].deadLine << endl;
			reorderedTaskCount++;
		}
	}

	if (taskCount <= 0 || reorderedTaskCount == 0) {
		out << "No tasks available to reorder." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	out << "Enter the id of the task to reorder :" << endl;
	int selectedTaskId;
	selectedTaskId = getInput(in);

	Task* selectedTask = nullptr;
	for (int i = 0; i < taskCount; ++i) {
		if (tasks[i].id == selectedTaskId && tasks[i].impid != 0) {
			selectedTask = &tasks[i];
			break;
		}
	}

	if (!selectedTask) {
		out << "Invalid task ID. Please try again." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	out << "Enter the new importance ID for the task: ";
	int newImportanceId;
	newImportanceId = getInput(in);

	selectedTask->impid = newImportanceId;

	FILE* file = fopen(pathFileTasks, "wb");
	for (int i = 0; i < taskCount; ++i) {
		fwrite(&tasks[i], sizeof(Task), 1, file);
	}
	fclose(file);

	out << "Task reordered successfully." << endl;
	enterToContinue(in, out);
	free(tasks);
	return 1;
}

/**
 * @brief Finds and displays the most similar tasks based on their descriptions.
 *
 * This function compares tasks based on their descriptions and finds the two most similar tasks using the longest common subsequence algorithm.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the results.
 * @return bool Always returns true.
 */
bool similarTasks(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task* tasks = nullptr;
	int taskCount = loadOwnedTasks(pathFileTasks, &tasks, loggedUser.id);

	if (taskCount < 2) {
		out << "Insufficient number of tasks, at least two tasks are required." << endl;
		return false;
	}

	int maxLcs = 0;
	int task1Index = -1, task2Index = -1;

	for (int i = 0; i < taskCount - 1; i++) {
		for (int j = i + 1; j < taskCount; j++) {
			int lcsLength = longestCommonSubsequence(tasks[i].description, tasks[j].description);
			if (lcsLength > maxLcs) {
				maxLcs = lcsLength;
				task1Index = i;
				task2Index = j;
			}
		}
	}

	if (task1Index != -1 && task2Index != -1) {
		out << "The two most similar tasks are:" << endl;
		out << "Task 1: " << tasks[task1Index].description << endl;
		out << "Task 2: " << tasks[task2Index].description << endl;
		enterToContinue(in, out);
	}
	else {
		out << "No similar task found." << endl;
		enterToContinue(in, out);
	}
	free(tasks);
	return true;
}

/**
 * @brief Displays and handles the task allegiances menu.
 *
 * This function displays the task allegiances menu and processes user input to perform BFS or DFS on the tasks.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return bool Always returns true.
 */
bool allegiances(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task* tasks = nullptr;
	int taskCount = loadOwnedTasks(pathFileTasks, &tasks, loggedUser.id);
	int choice, startVertex;

	if (taskCount <= 0) {
		out << "No Tasks Available." << endl;
		enterToContinue(in, out);
		free(tasks);
		return 0;
	}

	out << "Choose algorithm:\n 1. BFS\n 2. DFS\n";
	choice = getInput(in);

	viewTaskForFunc(pathFileTasks, in, out);

	out << "\nEnter the starting task ID:\n";
	startVertex = getInput(in);

	if (startVertex > taskCount) {
		out << "There is no " << startVertex << ". Task";
		enterToContinue(in, out);
		return 0;
	}

	int result = 0;
	switch (choice) {
	case 1:
		result = BFS(startVertex);
		break;
	case 2:
		result = DFS(startVertex);
		break;
	default:
		out << "Wrong Input ";
		enterToContinue(in, out);
		return 0;
	}

	out << "Number of tasks connected to task " << startVertex << " is: " << result << endl;
	enterToContinue(in, out);
	return true;
}

/**
 * @brief Calculates and displays the minimum spanning tree (MST) of the tasks.
 *
 * This function displays the MST menu and processes user input to calculate the MST using Prim's or Kruskal's algorithm.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the results.
 * @return bool Always returns true.
 */
bool calculateMST(istream& in, ostream& out) {
	viewTaskForFunc(pathFileTasks, in, out);
	out << "\nEnter the starting task ID:\n";
	int startVertex = getInput(in);

	if (startVertex >= MAX_TASKS || startVertex < 0) {
		out << "Invalid task ID." << endl;
		enterToContinue(in, out);
		return false;
	}

	loadTasksAndDependencies(pathFileTasks);

	out << "Choose MST algorithm:\n 1. Prim's Algorithm\n 2. Kruskal's Algorithm\n";
	int mstChoice = getInput(in);

	if (mstChoice == 1) {
		primMST(startVertex, out);
	}
	else if (mstChoice == 2) {
		kruskalMST(MAX_TASKS, out);
	}
	else {
		out << "Invalid choice." << endl;
	}

	enterToContinue(in, out);
	return true;
}

/**
 * @brief Calculates and displays the shortest path between tasks.
 *
 * This function displays the shortest path menu and processes user input to calculate the shortest path using Dijkstra's or Bellman-Ford algorithm.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the results.
 * @return bool Always returns true.
 */
bool shortestPath(istream& in, ostream& out) {
	viewTaskForFunc(pathFileTasks, in, out);
	out << "\nEnter the starting task ID:\n";
	int startVertex = getInput(in);

	if (startVertex < 0 || startVertex >= MAX_TASKS) {
		out << "Invalid task ID.\n";
		enterToContinue(in, out);
		return false;
	}

	out << "Choose shortest path algorithm:\n 1. Dijkstra\n 2. Bellman-Ford\n";
	int choice = getInput(in);

	switch (choice) {
	case 1:
		calculateShortestPath(startVertex, out);
		break;
	case 2:
		calculateBellmanFord(startVertex, MAX_TASKS, out);
		break;
	default:
		out << "Invalid choice.\n";
		enterToContinue(in, out);
		return false;
	}

	enterToContinue(in, out);
	return true;
}

//Algorithms


/**
 * @brief Loads tasks and their dependencies from the binary file.
 *
 * This function reads tasks from the specified binary file and adds edges with weights to the adjacency list.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @return bool Returns true if tasks are loaded successfully, otherwise false.
 */
bool loadTasksAndDependencies(const char* pathFileTasks) {
	FILE* file = fopen(pathFileTasks, "rb");
	if (!file) {
		cout << "Failed to open task file." << endl;
		return false;
	}

	Task task;
	while (fread(&task, sizeof(Task), 1, file)) {
		for (int i = 0; i < task.numDependencies; i++) {
			int depId = task.dependencies[i];
			int weight = task.id + depId;
			addEdgeWithWeight(task.id, depId, weight);
		}
	}
	fclose(file);
	return true;
}

/**
 * @brief Performs Breadth-First Search (BFS) on the task graph.
 *
 * This function performs BFS starting from the specified vertex and returns the number of connected tasks.
 *
 * @param startVertex The starting vertex for BFS.
 * @return int The number of tasks connected to the starting vertex.
 */
int BFS(int startVertex) {
	bool visited[MAX_TASKS] = { false };
	queue<int> queue;
	int connectedCount = 0;

	visited[startVertex] = true;
	queue.push(startVertex);

	while (!queue.empty()) {
		int current = queue.front();
		queue.pop();
		connectedCount++;

		for (const auto& adjPair : adj[current]) {
			int adj = adjPair.first;
			if (!visited[adj]) {
				visited[adj] = true;
				queue.push(adj);
			}
		}
	}

	return connectedCount;
}

/**
 * @brief Utility function for Depth-First Search (DFS).
 *
 * This function recursively performs DFS starting from the specified vertex and returns the number of connected tasks.
 *
 * @param vertex The current vertex in DFS.
 * @param visited Array to track visited vertices.
 * @return int The number of tasks connected to the current vertex.
 */
int DFSUtil(int vertex, bool visited[]) {
	visited[vertex] = true;
	int connectedCount = 1;

	for (const auto& adjPair : adj[vertex]) {
		int adj = adjPair.first;
		if (!visited[adj]) {
			connectedCount += DFSUtil(adj, visited);
		}
	}

	return connectedCount;
}

/**
 * @brief Performs Depth-First Search (DFS) on the task graph.
 *
 * This function performs DFS starting from the specified vertex and returns the number of connected tasks.
 *
 * @param startVertex The starting vertex for DFS.
 * @return int The number of tasks connected to the starting vertex.
 */
int DFS(int startVertex) {
	bool visited[MAX_TASKS] = { false };
	return DFSUtil(startVertex, visited);
}

/**
 * @brief Heapifies a subtree with the root at the given index.
 *
 * This function heapifies a subtree in a max heap with the root at the specified index.
 *
 * @param users Array of User objects.
 * @param n The size of the heap.
 * @param i The root index of the subtree.
 */
void heapify(User users[], int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && users[left].id > users[largest].id)
		largest = left;

	if (right < n && users[right].id > users[largest].id)
		largest = right;

	if (largest != i) {
		User swap = users[i];
		users[i] = users[largest];
		users[largest] = swap;

		heapify(users, n, largest);
	}
}

/**
 * @brief Builds a max heap from the given array of users.
 *
 * This function builds a max heap from the specified array of User objects.
 *
 * @param users Array of User objects.
 * @param n The size of the array.
 */
void buildMaxHeap(User users[], int n) {
	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(users, n, i);
}

/**
 * @brief Finds the length of the longest common subsequence between two strings.
 *
 * This function calculates the length of the longest common subsequence between two given strings.
 *
 * @param text1 The first string.
 * @param text2 The second string.
 * @return int The length of the longest common subsequence.
 */
int longestCommonSubsequence(const char* text1, const char* text2) {
	int m = strlen(text1);
	int n = strlen(text2);
	int dp[101][101] = { 0 };

	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			if (text1[i - 1] == text2[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1] + 1;
			}
			else {
				dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
			}
		}
	}

	return dp[m][n];
}

/**
 * @brief Builds a Huffman tree from the given data.
 *
 * This function constructs a Huffman tree based on the frequency of characters in the given data and stores the Huffman codes.
 *
 * @param pathFileTasks Path to the binary file containing tasks (unused in this function).
 * @param data The input data to build the Huffman tree from.
 * @param size The size of the input data.
 */
void buildHuffmanTree(const char* pathFileTasks, const char* data, int size) {
	unordered_map<char, int> freq;
	for (int i = 0; i < size; ++i)
		freq[data[i]]++;

	priority_queue<HuffmanNode*, vector<HuffmanNode*>, compare> minHeap;

	for (auto pair : freq)
		minHeap.push(new HuffmanNode(pair.first, pair.second));

	while (minHeap.size() != 1) {
		HuffmanNode* left = minHeap.top();
		minHeap.pop();
		HuffmanNode* right = minHeap.top();
		minHeap.pop();

		HuffmanNode* top = new HuffmanNode('$', left->freq + right->freq);
		top->left = left;
		top->right = right;
		minHeap.push(top);
	}

	storeCodes(minHeap.top(), "");
}

/**
 * @brief Stores Huffman codes for the given Huffman tree.
 *
 * This function traverses the Huffman tree and stores the Huffman codes for each character.
 *
 * @param root The root of the Huffman tree.
 * @param str The current Huffman code as a string.
 */
void storeCodes(HuffmanNode* root, string str) {
	if (root == nullptr)
		return;
	if (root->data != '$')
		codes[root->data] = str;
	storeCodes(root->left, str + "0");
	storeCodes(root->right, str + "1");
}

/**
 * @brief Prints the stored Huffman codes.
 *
 * This function prints the Huffman codes for each character to the output stream.
 *
 * @param out Output stream for displaying the Huffman codes.
 */
void printCodes(ostream& out) {
	for (auto pair : codes) {
		out << pair.first << ": " << pair.second << "\n";
	}
}

/**
 * @brief Encodes the given data using the stored Huffman codes.
 *
 * This function encodes the input data based on the previously stored Huffman codes.
 *
 * @param data The input data to encode.
 * @param size The size of the input data.
 * @return string The encoded string.
 */
string encode(const char* data, int size) {
	string encodedString = "";
	for (int i = 0; i < size; ++i) {
		encodedString += codes[data[i]];
	}
	return encodedString;
}

/**
 * @brief Displays and handles the Huffman encoding task menu.
 *
 * This function displays the Huffman encoding task menu, processes user input, and encodes the selected task description using Huffman coding.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 */
void huffmanEncodingTaskMenu(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task* tasks = nullptr;
	int taskCount = loadOwnedTasks(pathFileTasks, &tasks, loggedUser.id);

	if (taskCount <= 0) {
		out << "No tasks available to encode." << endl;
		enterToContinue(in, out);
		free(tasks);
		return;
	}

	out << "Select a task to encode by entering its ID:" << endl;
	for (int i = 0; i < taskCount; i++) {
		out << "ID: " << tasks[i].id << " - " << tasks[i].name << endl;
	}

	int selectedTaskId = getInput(in);
	Task* selectedTask = nullptr;
	for (int i = 0; i < taskCount; ++i) {
		if (tasks[i].id == selectedTaskId) {
			selectedTask = &tasks[i];
			break;
		}
	}

	if (!selectedTask) {
		out << "Invalid task ID. Please try again." << endl;
		enterToContinue(in, out);
		free(tasks);
		return;
	}

	const char* taskDescription = selectedTask->description;
	buildHuffmanTree(pathFileTasks, taskDescription, strlen(taskDescription));
	string encoded = encode(taskDescription, strlen(taskDescription));

	printCodes(out);
	out << "\nEncoded description for \"" << selectedTask->name << "\": " << encoded << endl;
	enterToContinue(in, out);
	free(tasks);
}

/**
 * @brief Builds the transpose of a graph.
 *
 * This function constructs the transpose of a given graph, where all edges are reversed.
 *
 * @param V Number of vertices in the graph.
 * @param adjLists Adjacency list of the original graph.
 * @param transpose Adjacency list of the transpose graph.
 */
void buildTranspose(int V, list<int>* adjLists, list<int>* transpose) {
	for (int v = 0; v < V; v++) {
		for (int u : adjLists[v]) {
			transpose[u].push_back(v);
		}
	}
}

/**
 * @brief Utility function for Depth-First Search (DFS) in SCCs.
 *
 * This function performs DFS on a graph, used to fill the stack with vertices in the order of completion times.
 *
 * @param v Current vertex in DFS.
 * @param visited Array to track visited vertices.
 * @param Stack Stack to store the vertices by finish time.
 * @param graph Adjacency list of the graph.
 */
void DFSUtilSCCs(int v, bool visited[], stack<int>& Stack, list<int>* graph) {
	visited[v] = true;

	for (int i : graph[v]) {
		if (!visited[i]) {
			DFSUtilSCCs(i, visited, Stack, graph);
		}
	}
	Stack.push(v);
}

/**
 * @brief Finds and prints all Strongly Connected Components (SCCs) in a graph.
 *
 * This function finds and prints all SCCs in a graph using Kosaraju's algorithm.
 *
 * @param V Number of vertices in the graph.
 * @param adjLists Adjacency list of the original graph.
 * @param out Output stream for displaying the SCCs.
 */
void findSCCs(int V, list<int>* adjLists, ostream& out) {
	stack<int> Stack;

	bool* visited = new bool[V];
	memset(visited, 0, V * sizeof(bool));
	for (int i = 0; i < V; ++i) {
		if (!visited[i]) {
			DFSUtilSCCs(i, visited, Stack, adjLists);
		}
	}

	list<int>* transpose = new list<int>[V];
	buildTranspose(V, adjLists, transpose);

	memset(visited, 0, V * sizeof(bool));
	int numSCC = 0;
	while (!Stack.empty()) {
		int v = Stack.top();
		Stack.pop();

		if (!visited[v]) {
			stack<int> componentStack;
			DFSUtilSCCs(v, visited, componentStack, transpose);
			numSCC++;
			out << "SCC #" << numSCC << ": ";
			while (!componentStack.empty()) {
				out << componentStack.top() << " ";
				componentStack.pop();
			}
			out << "\n";
		}
	}

	delete[] visited;
	delete[] transpose;
}

/**
 * @brief Analyzes and prints Strongly Connected Components (SCCs) of tasks.
 *
 * This function loads tasks, builds the task graph, and finds SCCs.
 *
 * @param pathFileTasks Path to the binary file containing tasks.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the SCCs.
 * @return bool Returns true if SCCs are analyzed successfully, otherwise false.
 */
bool analyzeSCC(const char* pathFileTasks, istream& in, ostream& out) {
	clearScreen();
	Task* tasks = nullptr;
	int taskCount = loadOwnedTasks(pathFileTasks, &tasks, loggedUser.id);
	loadTasksAndDependencies(pathFileTasks);

	if (taskCount <= 0) {
		out << "There is no task." << endl;
		enterToContinue(in, out);
		free(tasks);
		return false;
	}

	int V = taskCount;
	findSCCs(V, adjLists, out);
	enterToContinue(in, out);
	return true;
}

/**
 * @brief Adds an edge with a specified weight to the adjacency list.
 *
 * This function adds a directed edge with a specified weight to the adjacency list of the graph.
 *
 * @param u Source vertex of the edge.
 * @param v Destination vertex of the edge.
 * @param weight Weight of the edge.
 */
void addEdgeWithWeight(int u, int v, int weight) {
	adj[u].emplace_back(v, weight);
	adj[v].emplace_back(u, weight);
}

/**
 * @brief Finds and prints the Minimum Spanning Tree (MST) using Prim's algorithm.
 *
 * This function finds and prints the MST of the graph starting from the given vertex using Prim's algorithm.
 *
 * @param startVertex The starting vertex for Prim's algorithm.
 * @param out Output stream for displaying the MST.
 */
void primMST(int startVertex, ostream& out) {
	int key[MAX_TASKS];
	int parent[MAX_TASKS];
	bool inMST[MAX_TASKS];

	for (int i = 0; i < MAX_TASKS; i++) {
		key[i] = INT_MAX;
		parent[i] = -1;
		inMST[i] = false;
	}

	set<pair<int, int>> pq;
	pq.insert(make_pair(0, startVertex));
	key[startVertex] = 0;

	while (!pq.empty()) {
		int u = pq.begin()->second;
		pq.erase(pq.begin());

		inMST[u] = true;

		for (auto x : adj[u]) {
			int v = x.first;
			int weight = x.second;

			if (!inMST[v] && key[v] > weight) {
				if (key[v] != INT_MAX) {
					pq.erase(pq.find(make_pair(key[v], v)));
				}

				key[v] = weight;
				pq.insert(make_pair(key[v], v));
				parent[v] = u;
			}
		}
	}

	out << "Edge \tWeight\n";
	for (int i = 1; i < MAX_TASKS; ++i) {
		if (parent[i] != -1) {
			out << parent[i] << " - " << i << "\t" << key[i] << " \n";
		}
	}
}

/**
 * @brief Compares two edges based on their weight.
 *
 * This function compares two edges and returns true if the first edge has a smaller weight than the second edge.
 *
 * @param a The first edge.
 * @param b The second edge.
 * @return bool Returns true if the first edge has a smaller weight, otherwise false.
 */
bool compareEdges(const Edge& a, const Edge& b) {
	return a.weight < b.weight;
}

/**
 * @brief Finds the root of a set in the union-find data structure.
 *
 * This function finds the root of the set containing the given element.
 *
 * @param parent The array representing the union-find data structure.
 * @param i The element whose root is to be found.
 * @return int The root of the set containing the element.
 */
int find(int parent[], int i) {
	if (parent[i] == -1) {
		return i;
	}
	return find(parent, parent[i]);
}

/**
 * @brief Unites two sets in the union-find data structure.
 *
 * This function unites the sets containing the two given elements.
 *
 * @param parent The array representing the union-find data structure.
 * @param x The first element.
 * @param y The second element.
 */
void unionSets(int parent[], int x, int y) {
	int xset = find(parent, x);
	int yset = find(parent, y);
	if (xset != yset) {
		parent[xset] = yset;
	}
}

/**
 * @brief Finds and prints the Minimum Spanning Tree (MST) using Kruskal's algorithm.
 *
 * This function finds and prints the MST of the graph using Kruskal's algorithm.
 *
 * @param V Number of vertices in the graph.
 * @param out Output stream for displaying the MST.
 */
void kruskalMST(int V, ostream& out) {
	Edge edges[MAX_TASKS * MAX_TASKS];
	int edgeCount = 0;

	for (int u = 0; u < V; u++) {
		for (list<pair<int, int>>::iterator it = adj[u].begin(); it != adj[u].end(); ++it) {
			edges[edgeCount++] = { u, it->first, it->second };
		}
	}

	for (int i = 0; i < edgeCount - 1; i++) {
		for (int j = 0; j < edgeCount - i - 1; j++) {
			if (compareEdges(edges[j + 1], edges[j])) {
				Edge temp = edges[j];
				edges[j] = edges[j + 1];
				edges[j + 1] = temp;
			}
		}
	}

	Edge result[MAX_TASKS];
	int resultIndex = 0;
	int parent[MAX_TASKS];
	memset(parent, -1, sizeof(parent));

	for (int i = 0; i < edgeCount; i++) {
		int x = find(parent, edges[i].src);
		int y = find(parent, edges[i].dest);

		if (x != y) {
			result[resultIndex++] = edges[i];
			unionSets(parent, x, y);
		}
	}

	out << "Edge \tWeight\n";
	for (int i = 0; i < resultIndex; i++) {
		out << result[i].src << " - " << result[i].dest << "\t" << result[i].weight << " \n";
	}
}

/**
 * @brief Calculates and prints the shortest path from the start vertex using Dijkstra's algorithm.
 *
 * This function calculates and prints the shortest path from the start vertex to all other vertices using Dijkstra's algorithm.
 *
 * @param startVertex The starting vertex.
 * @param out Output stream for displaying the shortest paths.
 */
void calculateShortestPath(int startVertex, ostream& out) {
	int dist[MAX_TASKS];
	bool sptSet[MAX_TASKS];

	for (int i = 0; i < MAX_TASKS; i++) {
		dist[i] = INT_MAX;
		sptSet[i] = false;
	}

	dist[startVertex] = 0;

	for (int count = 0; count < MAX_TASKS - 1; count++) {
		int u = -1;

		for (int i = 0; i < MAX_TASKS; i++) {
			if (!sptSet[i] && (u == -1 || dist[i] < dist[u])) {
				u = i;
			}
		}

		if (u == -1 || dist[u] == INT_MAX) {
			break;
		}

		sptSet[u] = true;

		for (const auto& adjPair : adj[u]) {
			int v = adjPair.first;
			int weight = adjPair.second;

			if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
				dist[v] = dist[u] + weight;
			}
		}
	}

	out << "Vertex\tDistance from Source\n";
	for (int i = 0; i < MAX_TASKS; i++) {
		if (dist[i] != INT_MAX) {
			out << i << "\t" << dist[i] << "\n";
		}
	}
}

/**
 * @brief Calculates and prints the shortest path from the start vertex using Bellman-Ford algorithm.
 *
 * This function calculates and prints the shortest path from the start vertex to all other vertices using the Bellman-Ford algorithm.
 *
 * @param startVertex The starting vertex.
 * @param V Number of vertices in the graph.
 * @param out Output stream for displaying the shortest paths.
 */
void calculateBellmanFord(int startVertex, int V, ostream& out) {
	int dist[MAX_TASKS];

	for (int i = 0; i < V; i++) {
		dist[i] = INT_MAX;
	}

	dist[startVertex] = 0;

	for (int i = 1; i <= V - 1; i++) {
		for (int u = 0; u < V; u++) {
			for (auto adjPair : adj[u]) {
				int v = adjPair.first;
				int weight = adjPair.second;
				if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
					dist[v] = dist[u] + weight;
				}
			}
		}
	}

	for (int u = 0; u < V; u++) {
		for (auto adjPair : adj[u]) {
			int v = adjPair.first;
			int weight = adjPair.second;
			if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
				out << "Graph contains negative weight cycle" << endl;
				return;
			}
		}
	}

	out << "Vertex\tDistance from Source\n";
	for (int i = 0; i < V; i++) {
		if (dist[i] != INT_MAX) {
			out << i << "\t" << dist[i] << "\n";
		}
	}
}

/**
 * @brief Performs Breadth-First Search (BFS) in a residual graph.
 *
 * This function performs BFS in the residual graph to find an augmenting path from the source to the sink.
 *
 * @param residualGraph The residual graph.
 * @param source The source vertex.
 * @param sink The sink vertex.
 * @param parent Array to store the path.
 * @param V Number of vertices in the graph.
 * @return bool Returns true if there is an augmenting path, otherwise false.
 */
bool bfs(int** residualGraph, int source, int sink, int parent[], int V) {
	bool* visited = new bool[V]();
	std::queue<int> queue;
	queue.push(source);
	visited[source] = true;
	parent[source] = -1;

	while (!queue.empty()) {
		int u = queue.front();
		queue.pop();

		for (int v = 0; v < V; v++) {
			if (!visited[v] && residualGraph[u][v] > 0) {
				queue.push(v);
				parent[v] = u;
				visited[v] = true;
				if (v == sink) {
					delete[] visited;
					return true;
				}
			}
		}
	}

	delete[] visited;
	return false;
}

/**
 * @brief Finds the maximum flow in a graph using the Edmonds-Karp algorithm.
 *
 * This function finds the maximum flow from the source to the sink in the given graph using the Edmonds-Karp algorithm.
 *
 * @param graph The capacity graph.
 * @param source The source vertex.
 * @param sink The sink vertex.
 * @param V Number of vertices in the graph.
 * @return int The maximum flow from the source to the sink.
 */
int edmondsKarp(int** graph, int source, int sink, int V) {
	int** residualGraph = new int* [V];
	for (int i = 0; i < V; i++) {
		residualGraph[i] = new int[V];
		for (int j = 0; j < V; j++) {
			residualGraph[i][j] = graph[i][j];
		}
	}

	int* parent = new int[V];
	int maxFlow = 0;

	while (bfs(residualGraph, source, sink, parent, V)) {
		int pathFlow = INT_MAX;
		for (int v = sink; v != source; v = parent[v]) {
			int u = parent[v];
			pathFlow = std::min(pathFlow, residualGraph[u][v]);
		}

		for (int v = sink; v != source; v = parent[v]) {
			int u = parent[v];
			residualGraph[u][v] -= pathFlow;
			residualGraph[v][u] += pathFlow;
		}

		maxFlow += pathFlow;
	}

	for (int i = 0; i < V; i++) {
		delete[] residualGraph[i];
	}
	delete[] residualGraph;
	delete[] parent;

	return maxFlow;
}

/**
 * @brief Performs Depth-First Search (DFS) in a residual graph.
 *
 * This function performs DFS in the residual graph to find an augmenting path from the source to the sink.
 *
 * @param residualGraph The residual graph.
 * @param source The source vertex.
 * @param sink The sink vertex.
 * @param parent Array to store the path.
 * @param V Number of vertices in the graph.
 * @return bool Returns true if there is an augmenting path, otherwise false.
 */
bool dfs(int** residualGraph, int source, int sink, int parent[], int V) {
	bool* visited = new bool[V]();
	std::stack<int> stack;
	stack.push(source);
	visited[source] = true;
	parent[source] = -1;

	while (!stack.empty()) {
		int u = stack.top();
		stack.pop();

		for (int v = 0; v < V; v++) {
			if (!visited[v] && residualGraph[u][v] > 0) {
				stack.push(v);
				parent[v] = u;
				visited[v] = true;
				if (v == sink) {
					delete[] visited;
					return true;
				}
			}
		}
	}

	delete[] visited;
	return false;
}

/**
 * @brief Finds the maximum flow in a graph using the Ford-Fulkerson algorithm.
 *
 * This function finds the maximum flow from the source to the sink in the given graph using the Ford-Fulkerson algorithm.
 *
 * @param graph The capacity graph.
 * @param source The source vertex.
 * @param sink The sink vertex.
 * @param V Number of vertices in the graph.
 * @return int The maximum flow from the source to the sink.
 */
int fordFulkerson(int** graph, int source, int sink, int V) {
	int** residualGraph = new int* [V];
	for (int i = 0; i < V; i++) {
		residualGraph[i] = new int[V];
		for (int j = 0; j < V; j++) {
			residualGraph[i][j] = graph[i][j];
		}
	}

	int* parent = new int[V];
	int maxFlow = 0;

	while (dfs(residualGraph, source, sink, parent, V)) {
		int pathFlow = INT_MAX;
		for (int v = sink; v != source; v = parent[v]) {
			int u = parent[v];
			pathFlow = std::min(pathFlow, residualGraph[u][v]);
		}

		for (int v = sink; v != source; v = parent[v]) {
			int u = parent[v];
			residualGraph[u][v] -= pathFlow;
			residualGraph[v][u] += pathFlow;
		}

		maxFlow += pathFlow;
	}

	for (int i = 0; i < V; i++) {
		delete[] residualGraph[i];
	}
	delete[] residualGraph;
	delete[] parent;

	return maxFlow;
}

/*
void encryptData(uint8_t* data, size_t length) {
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, AES_KEY, AES_IV);
	AES_CBC_encrypt_buffer(&ctx, data, length);
}

void decryptData(uint8_t* data, size_t length) {
	struct AES_ctx ctx;
	AES_init_ctx_iv(&ctx, AES_KEY, AES_IV);
	AES_CBC_decrypt_buffer(&ctx, data, length);
}

*/

//Algorithms


//LOGIN REGISTER

/**
 * @brief Logs in a user by verifying email and password.
 *
 * This function opens the user file, reads registered users, and checks if the provided email and password match any user.
 *
 * @param loginUser The user attempting to log in.
 * @param pathFileUsers Path to the binary file containing user data.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying messages.
 * @return int Returns 1 if login is successful, otherwise 0.
 */
int loginUser(User loginUser, const char* pathFileUsers, istream& in, ostream& out) {
	ifstream file(pathFileUsers, ios::binary);
	if (!file.is_open()) {
		out << "Failed to open user file." << endl;
		return 0;
	}

	int userCount = 0;
	file.read(reinterpret_cast<char*>(&userCount), sizeof(int));
	if (userCount == 0) {
		out << "No users registered." << endl;
		file.close();
		enterToContinue(in, out);
		return 0;
	}

	User userFromFile;
	for (int i = 0; i < userCount; i++) {
		file.read(reinterpret_cast<char*>(&userFromFile), sizeof(User));
		if (strcmp(userFromFile.email, loginUser.email) == 0 && strcmp(userFromFile.password, loginUser.password) == 0) {
			out << "Login successful. Welcome " << endl;
			file.close();
			enterToContinue(in, out);
			loggedUser = userFromFile;
			return 1;
		}
	}

	out << "Incorrect email or password." << endl;
	file.close();
	enterToContinue(in, out);
	return 0;
}

/**
 * @brief Displays and handles the login user menu.
 *
 * This function displays the login menu and processes user input to log in a user.
 *
 * @param pathFileUsers Path to the binary file containing user data.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 1 if login is successful, otherwise 0.
 */
int loginUserMenu(const char* pathFileUsers, istream& in, ostream& out) {
	clearScreen();
	User loginUsers;

	out << "Enter email: ";
	in.getline(loginUsers.email, sizeof(loginUsers.email));

	out << "Enter password: ";
	in.getline(loginUsers.password, sizeof(loginUsers.password));

	return loginUser(loginUsers, pathFileUsers, in, out);
}

/**
 * @brief Registers a new user by adding them to the user file.
 *
 * This function opens the user file, checks if the email is already registered, and adds the new user to the file.
 *
 * @param user The new user to be registered.
 * @param pathFileUser Path to the binary file containing user data.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying messages.
 * @return int Returns 1 if registration is successful, otherwise 0.
 */
int registerUser(User user, const char* pathFileUser, istream& in, ostream& out) {
	fstream file(pathFileUser, ios::binary | ios::in | ios::out);
	int userCount = 0;
	User* users = nullptr;

	if (file.is_open()) {
		file.read(reinterpret_cast<char*>(&userCount), sizeof(int));
		if (userCount > 0) {
			users = new User[userCount];
			file.read(reinterpret_cast<char*>(users), sizeof(User) * userCount);

			for (int i = 0; i < userCount; ++i) {
				if (strcmp(users[i].email, user.email) == 0) {
					out << "User already exists." << endl;
					file.close();
					delete[] users;
					enterToContinue(in, out);
					return 0;
				}
			}
		}
		file.seekg(0, ios::beg);
	}
	else {
		file.open(pathFileUser, ios::binary | ios::out);
	}

	user.id = getNewUserId(users, userCount);
	userCount++;
	User* updatedUsers = new User[userCount];
	if (users) {
		copy(users, users + (userCount - 1), updatedUsers);
		delete[] users;
	}
	updatedUsers[userCount - 1] = user;

	file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
	file.write(reinterpret_cast<const char*>(updatedUsers), sizeof(User) * userCount);

	out << "User registered successfully " << endl;

	delete[] updatedUsers;
	file.close();
	enterToContinue(in, out);
	return 1;
}

/**
 * @brief Displays and handles the register user menu.
 *
 * This function displays the register menu and processes user input to register a new user.
 *
 * @param pathFileUsers Path to the binary file containing user data.
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Returns 1 if registration is successful, otherwise 0.
 */
int registerUserMenu(const char* pathFileUsers, istream& in, ostream& out) {
	clearScreen();
	User newUser;

	out << "Enter Name: ";
	in.getline(newUser.name, sizeof(newUser.name));

	out << "Enter Surname: ";
	in.getline(newUser.surname, sizeof(newUser.surname));

	out << "Enter email: ";
	in.getline(newUser.email, sizeof(newUser.email));

	out << "Enter password: ";
	in.getline(newUser.password, sizeof(newUser.password));

	if (registerUser(newUser, pathFileUsers, in, out)) {
		return 1;
	}
	else {
		out << "Registration failed." << endl;
	}

	enterToContinue(in, out);
	return 1;
}


//LOGIN REGISTER

/**
 * @brief Handles guest operations menu.
 *
 * This function displays the guest menu, processes user input, and performs actions based on the guest's choice.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Always returns 0 to indicate the guest operation is complete.
 */
int guestOperation(istream& in, ostream& out) {
	int choice;

	while (true) {
		printGuestMenu(out);
		choice = getInput(in);

		if (in.fail()) {
			handleInputError(in, out);
			continue;
		}

		switch (choice) {
		case 1:
			clearScreen();
			out << "1. Work" << endl;
			out << "2. Sport" << endl;
			out << "3. Diet" << endl;
			out << "4. Study" << endl;
			enterToContinue(in, out);
			break;

		case 2:
			return 0;

		default:
			out << "\nInvalid choice. Please try again.\n";
			enterToContinue(in, out);
			break;
		}
	}
}

/**
 * @brief Handles user operations menu.
 *
 * This function displays the user menu, processes user input, and performs actions based on the user's choice.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Always returns 0 to indicate the user operation is complete.
 */
int userOperations(istream& in, ostream& out) {
	int choice;

	while (true) {
		printUserMenu(out);
		choice = getInput(in);

		if (in.fail()) {
			handleInputError(in, out);
			continue;
		}

		switch (choice) {
		case 1:
			createTaskMenu(in, out);
			break;

		case 2:
			deadlineSettingsMenu(in, out);
			break;

		case 3:
			reminderSystemMenu(in, out);
			break;

		case 4:
			taskPrioritizationMenu(in, out);
			break;

		case 5:
			flowAlgorithmsMenu(in, out);
			break;

		case 6:
			return 0;

		default:
			out << "\nInvalid choice. Please try again.\n";
			enterToContinue(in, out);
			break;
		}
	}
}

/**
 * @brief Handles the main menu.
 *
 * This function displays the main menu, processes user input, and performs actions based on the user's choice.
 *
 * @param in Input stream for reading user input.
 * @param out Output stream for displaying the menu and messages.
 * @return int Always returns 0 to indicate the main menu operation is complete.
 */
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
			if (loginUserMenu(pathFileUsers, in, out)) {
				userOperations(in, out);
			}
			break;

		case 2:
			clearScreen();
			registerUserMenu(pathFileUsers, in, out);
			break;

		case 3:
			clearScreen();
			out << "Guest Operations\n";
			guestOperation(in, out);
			break;

		case 4:
			clearScreen();
			out << "Exit Program\n";
			return 0;

		default:
			out << "\nInvalid choice. Please try again.\n";
			enterToContinue(in, out);
			break;
		}
	}
}
