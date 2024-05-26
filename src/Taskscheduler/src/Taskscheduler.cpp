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


