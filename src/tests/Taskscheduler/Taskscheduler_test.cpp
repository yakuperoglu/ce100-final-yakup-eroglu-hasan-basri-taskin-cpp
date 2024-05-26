#define ENABLE_TASKSCHEDULER_TEST 
#include "gtest/gtest.h"
#include <fstream>
#include <cstring>
#include "../../taskscheduler/header/taskscheduler.h"  

extern User loggedUser;

class TaskschedulerTest : public ::testing::Test {
protected:
	std::ostringstream out;
	std::istringstream in;
	int** graph;
	int V;
	void SetUp() override {
		out.str("");
		out.clear();

		loggedUser.id = 1;
		strcpy(loggedUser.name, "TestName");
		strcpy(loggedUser.surname, "TestSurname");
		strcpy(loggedUser.email, "test@example.com");
		strcpy(loggedUser.password, "password");

		V = 6;
		graph = new int* [V];
		for (int i = 0; i < V; i++) {
			graph[i] = new int[V];
			memset(graph[i], 0, V * sizeof(int));
		}

		graph[0][1] = 16;
		graph[0][2] = 13;
		graph[1][2] = 10;
		graph[1][3] = 12;
		graph[2][1] = 4;
		graph[2][4] = 14;
		graph[3][2] = 9;
		graph[3][5] = 20;
		graph[4][3] = 7;
		graph[4][5] = 4;
	}

	void TearDown() override {
		for (int i = 0; i < V; i++) {
			delete[] graph[i];
		}
		delete[] graph;
	}

	void simulateUserInput(const std::string& input) {
		in.str(input);
		in.clear();
	}
};


TEST_F(TaskschedulerTest, EnterToContinue_Success) {
	simulateUserInput("\n");
	EXPECT_TRUE(enterToContinue(in, out));
}

TEST_F(TaskschedulerTest, handleInputError_Success) {
	simulateUserInput("\n");
	EXPECT_TRUE(handleInputError(in, out));
}

TEST_F(TaskschedulerTest, getInput_Success) {
	simulateUserInput("4\n");
	EXPECT_TRUE(getInput(in));
	std::string expectedOutput = "";
	EXPECT_EQ(expectedOutput, out.str());
}

TEST_F(TaskschedulerTest, getInput_NotSuccess) {
	simulateUserInput("asd\n");
	EXPECT_TRUE(getInput(in));
	std::string expectedOutput = "";
	EXPECT_EQ(expectedOutput, out.str());
}

TEST_F(TaskschedulerTest, getInput_GetsLineFailure) {
	in.setstate(std::ios::failbit);
	EXPECT_EQ(getInput(in), -1);
}

TEST_F(TaskschedulerTest, getNewUserId_EmptyList) {
	User users[1];
	int userCount = 0;
	EXPECT_EQ(getNewUserId(users, userCount), 1);
}

TEST_F(TaskschedulerTest, getNewUserId_NonEmptyList) {
	User users[3] = { {1}, {2}, {3} };
	int userCount = 3;
	EXPECT_EQ(getNewUserId(users, userCount), 4);
}

TEST_F(TaskschedulerTest, getNewTaskId_FileNotExists) {
	const char* pathFileTasks = "non_existent_file.dat";
	EXPECT_EQ(getNewTaskId(pathFileTasks), 1);
}

TEST_F(TaskschedulerTest, getNewTaskId_FileExists) {
	const char* pathFileTasks = "tasks.bin";

	FILE* file = fopen(pathFileTasks, "wb");
	Task tasks[3] = { {1}, {2}, {3} };
	fwrite(tasks, sizeof(Task), 3, file);
	fclose(file);

	EXPECT_EQ(getNewTaskId(pathFileTasks), 4);

	remove(pathFileTasks);
}


TEST_F(TaskschedulerTest, printGuestMenu_Success) {
	EXPECT_TRUE(printGuestMenu(out));
}

TEST_F(TaskschedulerTest, printMainMenu_Success) {
	EXPECT_TRUE(printMainMenu(out));
}

TEST_F(TaskschedulerTest, printUserMenu_Success) {
	EXPECT_TRUE(printUserMenu(out));
}

TEST_F(TaskschedulerTest, printCreateTaskMenu_Success) {
	EXPECT_TRUE(printCreateTaskMenu(out));
}

TEST_F(TaskschedulerTest, printDeadlineSettingsMenu_Success) {
	EXPECT_TRUE(printDeadlineSettingsMenu(out));
}

TEST_F(TaskschedulerTest, printReminderSystemMenu_Success) {
	EXPECT_TRUE(printReminderSystemMenu(out));
}

TEST_F(TaskschedulerTest, printTaskPrioritizationMenu_Success) {
	EXPECT_TRUE(printTaskPrioritizationMenu(out));
}

TEST_F(TaskschedulerTest, printFlowAlgorithmsMenu_Success) {
	EXPECT_TRUE(printFlowAlgorithmsMenu(out));
}


TEST_F(TaskschedulerTest, createTaskMenu_Success) {
	simulateUserInput("8\n");
	EXPECT_EQ(createTaskMenu(in, out), 0);
}

TEST_F(TaskschedulerTest, createTaskMenu_InvalidChoices) {
	simulateUserInput("invalid\n\n9\n\n8\n6\n4\n");
	EXPECT_FALSE(createTaskMenu(in, out));
}

TEST_F(TaskschedulerTest, createTaskMenu_AddTask) {
	simulateUserInput("1\nasd\nasd\n2\n3\n1\n\n8\n6\n4\n");
	EXPECT_FALSE(createTaskMenu(in, out));
}

TEST_F(TaskschedulerTest, createTaskMenu_CatagorizeTask_NoTasks) {
	simulateUserInput("2\n\n\n8\n6\n4\n");
	EXPECT_FALSE(createTaskMenu(in, out));
}

TEST_F(TaskschedulerTest, createTaskMenu_ViewTask_NoTasks) {
	simulateUserInput("3\n\n8\n6\n4\n");
	EXPECT_FALSE(createTaskMenu(in, out));
}

TEST_F(TaskschedulerTest, createTaskMenu_SimilarTasks_NoTasks) {
	simulateUserInput("4\n8\n6\n4\n");
	EXPECT_FALSE(createTaskMenu(in, out));
}

TEST_F(TaskschedulerTest, createTaskMenu_Allegiances_NoTasks) {
	simulateUserInput("5\n\n8\n6\n8\n6\n8\n4\n");
	EXPECT_FALSE(createTaskMenu(in, out));
}

TEST_F(TaskschedulerTest, createTaskMenu_analyzeSCC_NoTasks) {
	simulateUserInput("6\n\n8\n6\n4\n");
	EXPECT_FALSE(createTaskMenu(in, out));
}

TEST_F(TaskschedulerTest, createTaskMenu_HuffmanEncode_NoTasks) {
	simulateUserInput("7\n\n8\n8\n6\n4\n");
	EXPECT_FALSE(createTaskMenu(in, out));
}


TEST_F(TaskschedulerTest, deadlineSettingsMenu_Success) {
	simulateUserInput("3\n");
	EXPECT_EQ(deadlineSettingsMenu(in, out), 0);
}

TEST_F(TaskschedulerTest, deadlineSettingsMenu_InvalidChoices) {
	simulateUserInput("invalid\n\n4\n\n3\n6\n4\n");
	EXPECT_FALSE(deadlineSettingsMenu(in, out));
}

TEST_F(TaskschedulerTest, deadlineSettingsMenu_AssignDeadline) {
	simulateUserInput("1\n\n\n3\n6\n4\n");
	EXPECT_FALSE(deadlineSettingsMenu(in, out));
}

TEST_F(TaskschedulerTest, deadlineSettingsMenu_ViewDeadlines) {
	simulateUserInput("2\n\n3\n6\n4\n");
	EXPECT_FALSE(deadlineSettingsMenu(in, out));
}


TEST_F(TaskschedulerTest, reminderSystemMenu_Success) {
	simulateUserInput("3\n");
	EXPECT_EQ(reminderSystemMenu(in, out), 0);
}

TEST_F(TaskschedulerTest, reminderSystemMenu_InvalidChoices) {
	simulateUserInput("invalid\n\n4\n\n3\n6\n4\n");
	EXPECT_FALSE(reminderSystemMenu(in, out));
}

TEST_F(TaskschedulerTest, reminderSystemMenu_SetReminders1) {
	simulateUserInput("1\n0\n0\n0\n0\n\n3\n6\n4\n");
	EXPECT_FALSE(reminderSystemMenu(in, out));
}


TEST_F(TaskschedulerTest, reminderSystemMenu_SetReminders2) {
	simulateUserInput("1\n1\n0\n0\n0\n\n3\n6\n4\n");
	EXPECT_FALSE(reminderSystemMenu(in, out));
}

TEST_F(TaskschedulerTest, reminderSystemMenu_NotificationSettings1) {
	simulateUserInput("2\n1\n\n3\n6\n4\n");
	EXPECT_FALSE(reminderSystemMenu(in, out));
}

TEST_F(TaskschedulerTest, reminderSystemMenu_NotificationSettings2) {
	simulateUserInput("2\n2\n\n3\n6\n4\n");
	EXPECT_FALSE(reminderSystemMenu(in, out));
}

TEST_F(TaskschedulerTest, reminderSystemMenu_NotificationSettings3) {
	simulateUserInput("2\n3\n\n3\n6\n4\n");
	EXPECT_FALSE(reminderSystemMenu(in, out));
}

TEST_F(TaskschedulerTest, reminderSystemMenu_NotificationSettings4) {
	simulateUserInput("2\n4\n\n3\n6\n4\n");
	EXPECT_FALSE(reminderSystemMenu(in, out));
}

TEST_F(TaskschedulerTest, taskPrioritizationMenu_Success) {
	simulateUserInput("5\n");
	EXPECT_EQ(taskPrioritizationMenu(in, out), 0);
}

TEST_F(TaskschedulerTest, taskPrioritizationMenu_InvalidChoices) {
	simulateUserInput("invalid\n\n6\n\n5\n6\n4\n");
	EXPECT_FALSE(taskPrioritizationMenu(in, out));
}

TEST_F(TaskschedulerTest, taskPrioritizationMenu_MarkTaskImportance) {
	simulateUserInput("1\n\n5\n5\n6\n4\n");
	EXPECT_FALSE(taskPrioritizationMenu(in, out));
}

TEST_F(TaskschedulerTest, taskPrioritizationMenu_ReorderTask) {
	simulateUserInput("2\n\n5\n6\n4\n");
	EXPECT_FALSE(taskPrioritizationMenu(in, out));
}

TEST_F(TaskschedulerTest, taskPrioritizationMenu_CalculateMST) {
	simulateUserInput("3\n1\n1\n\n5\n6\n4\n");
	EXPECT_FALSE(taskPrioritizationMenu(in, out));
}

TEST_F(TaskschedulerTest, taskPrioritizationMenu_shortestPath) {
	simulateUserInput("4\n1\n1\n\n5\n6\n4\n");
	EXPECT_FALSE(taskPrioritizationMenu(in, out));
}


TEST_F(TaskschedulerTest, flowAlgorithmsMenu_Success) {
	simulateUserInput("3\n");
	EXPECT_EQ(flowAlgorithmsMenu(in, out), 0);
}

TEST_F(TaskschedulerTest, flowAlgorithmsMenu_InvalidChoices) {
	simulateUserInput("invalid\n\n4\n\n3\n6\n4\n");
	EXPECT_FALSE(flowAlgorithmsMenu(in, out));
}

TEST_F(TaskschedulerTest, flowAlgorithmsMenu_FordFulkerson) {
	simulateUserInput("1\n1\n1\n\n3\n6\n4\n");
	EXPECT_FALSE(flowAlgorithmsMenu(in, out));
}

TEST_F(TaskschedulerTest, flowAlgorithmsMenu_EdmondsKarp) {
	simulateUserInput("2\n1\n1\n\n3\n6\n4\n");
	EXPECT_FALSE(flowAlgorithmsMenu(in, out));
}

TEST_F(TaskschedulerTest, flowAlgorithmsMenu_None) {
	simulateUserInput("4\n1\n1\n\n3\n6\n4\n");
	EXPECT_FALSE(flowAlgorithmsMenu(in, out));
}


TEST_F(TaskschedulerTest, viewTaskForFunc_NoTasks) {
	const char* pathFileTasks = "empty_tasks.bin";

	FILE* file = fopen(pathFileTasks, "wb");
	fclose(file);
	simulateUserInput("\n");
	EXPECT_TRUE(viewTaskForFunc(pathFileTasks, in, out));


	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, viewTask_NoTasks) {
	const char* pathFileTasks = "empty_tasks.bin";

	FILE* file = fopen(pathFileTasks, "wb");
	fclose(file);
	simulateUserInput("\n");
	EXPECT_TRUE(viewTask(pathFileTasks, in, out));


	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, viewTask_WithTasks) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasks[1] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "Deadline 1", "Category 1", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasks, sizeof(Task), 1, file);
	fclose(file);

	simulateUserInput("\n");
	EXPECT_TRUE(viewTask(pathFileTasks, in, out));

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, viewDeadlines_NoTasks) {
	const char* pathFileTasks = "empty_tasks.bin";

	FILE* file = fopen(pathFileTasks, "wb");
	fclose(file);

	simulateUserInput("\n");
	EXPECT_FALSE(viewDeadlines(pathFileTasks, in, out));

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, viewDeadlines_WithTasksAndDeadlines) {
	const char* pathFileTasks = "tasks_with_deadlines.bin";
	Task tasks[1] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "01/01/2024", "Category 1", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasks, sizeof(Task), 1, file);
	fclose(file);

	simulateUserInput("\n");
	EXPECT_TRUE(viewDeadlines(pathFileTasks, in, out));

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, viewDeadlines_WithTasksWithoutDeadlines) {
	const char* pathFileTasks = "tasks_without_deadlines.bin";
	Task tasks[1] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "Category 1", true, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasks, sizeof(Task), 1, file);
	fclose(file);

	simulateUserInput("\n");
	EXPECT_TRUE(viewDeadlines(pathFileTasks, in, out));


	remove(pathFileTasks);
}


TEST_F(TaskschedulerTest, loadTasks_FileNotFound) {
	const char* pathFileTasks = "non_existent_tasks.bin";
	Task* tasks = nullptr;

	remove(pathFileTasks);

	int result = loadTasks(pathFileTasks, &tasks);
	EXPECT_EQ(result, -1);
}

TEST_F(TaskschedulerTest, loadTasks_EmptyFile) {
	const char* pathFileTasks = "empty_tasks.bin";
	Task* tasks = nullptr;

	FILE* file = fopen(pathFileTasks, "wb");
	fclose(file);

	int result = loadTasks(pathFileTasks, &tasks);
	EXPECT_EQ(result, 0);


	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, loadTasks_WithTasks) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "01/01/2024", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "02/01/2024", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	Task* tasks = nullptr;
	int result = loadTasks(pathFileTasks, &tasks);
	EXPECT_EQ(result, 2);

	EXPECT_EQ(tasks[0].id, 1);
	EXPECT_STREQ(tasks[0].name, "Task 1");
	EXPECT_STREQ(tasks[0].description, "Description 1");
	EXPECT_STREQ(tasks[0].deadLine, "01/01/2024");

	EXPECT_EQ(tasks[1].id, 2);
	EXPECT_STREQ(tasks[1].name, "Task 2");
	EXPECT_STREQ(tasks[1].description, "Description 2");
	EXPECT_STREQ(tasks[1].deadLine, "02/01/2024");

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, loadOwnedTasks_FileNotFound) {
	const char* pathFileTasks = "non_existent_tasks.bin";
	Task* tasks = nullptr;

	remove(pathFileTasks);

	int result = loadOwnedTasks(pathFileTasks, &tasks, 2);
	EXPECT_EQ(result, -1);

}


TEST_F(TaskschedulerTest, categorizeTask_NoTasks) {
	const char* pathFileTasks = "empty_tasks.bin";

	FILE* file = fopen(pathFileTasks, "wb");
	fclose(file);

	std::stringstream input("\n");
	std::stringstream output;

	EXPECT_EQ(categorizeTask(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, categorizeTask_WithUncategorizedTasks1) {
	const char* pathFileTasks = "tasks_with_uncategorized.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "", false, false, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "", "", false, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n1\n\n");
	std::stringstream output;

	EXPECT_EQ(categorizeTask(pathFileTasks, input, output), 1);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, categorizeTask_WithUncategorizedTasks2) {
	const char* pathFileTasks = "tasks_with_uncategorized.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "", false, false, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "", "", false, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n2\n\n");
	std::stringstream output;

	EXPECT_EQ(categorizeTask(pathFileTasks, input, output), 1);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, categorizeTask_WithUncategorizedTasks3) {
	const char* pathFileTasks = "tasks_with_uncategorized.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "", false, false, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "", "", false, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n3\n\n");
	std::stringstream output;

	EXPECT_EQ(categorizeTask(pathFileTasks, input, output), 1);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, categorizeTask_WithUncategorizedTasks4) {
	const char* pathFileTasks = "tasks_with_uncategorized.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "", false, false, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "", "", false, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n4\n\n");
	std::stringstream output;

	EXPECT_EQ(categorizeTask(pathFileTasks, input, output), 1);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, categorizeTask_WithUncategorizedTasks5) {
	const char* pathFileTasks = "tasks_with_uncategorized.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "", false, false, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "", "", false, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n5\n\n");
	std::stringstream output;

	EXPECT_EQ(categorizeTask(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, categorizeTask_InvalidTaskID) {
	const char* pathFileTasks = "tasks_with_uncategorized.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "", false, false, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "", "", false, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("3\n1\n");
	std::stringstream output;

	EXPECT_EQ(categorizeTask(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}


TEST_F(TaskschedulerTest, assignDeadline_NoTasks) {
	const char* pathFileTasks = "empty_tasks.bin";

	FILE* file = fopen(pathFileTasks, "wb");
	fclose(file);

	std::stringstream input("\n");
	std::stringstream output;

	EXPECT_EQ(assignDeadline(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, assignDeadline_WithUncategorizedTasks) {
	const char* pathFileTasks = "tasks_with_uncategorized.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "", false, false, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "", "", false, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n15\n12\n2024\n\n");
	std::stringstream output;

	EXPECT_EQ(assignDeadline(pathFileTasks, input, output), 1);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, assignDeadline_InvalidDate) {
	const char* pathFileTasks = "tasks_with_uncategorized.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "", false, false, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "", "", false, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n32\n12\n2024\n\n");
	std::stringstream output;

	EXPECT_EQ(assignDeadline(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}


TEST_F(TaskschedulerTest, markTaskImportance_NoTasks) {
	const char* pathFileTasks = "empty_tasks.bin";

	FILE* file = fopen(pathFileTasks, "wb");
	fclose(file);

	std::stringstream input("\n");
	std::stringstream output;

	EXPECT_EQ(markTaskImportance(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, markTaskImportance_WithTasks) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "", false, false, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "", "", false, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n3\n\n");
	std::stringstream output;

	EXPECT_EQ(markTaskImportance(pathFileTasks, input, output), 1);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, markTaskImportance_InvalidImportanceId) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "", false, false, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "", "", false, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n-1\n\n");
	std::stringstream output;

	EXPECT_EQ(markTaskImportance(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, markTaskImportance_InvalidTaskId) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "", "", false, false, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "", "", false, false, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("3\n3\n\n");
	std::stringstream output;

	EXPECT_EQ(markTaskImportance(pathFileTasks, input, output), 0);


	remove(pathFileTasks);
}


TEST_F(TaskschedulerTest, reorderTask_NoTasks) {
	const char* pathFileTasks = "empty_tasks.bin";

	FILE* file = fopen(pathFileTasks, "wb");
	fclose(file);

	std::stringstream input("\n");
	std::stringstream output;

	EXPECT_EQ(reorderTask(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, reorderTask_WithTasks) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 2, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 1, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n3\n\n");
	std::stringstream output;

	EXPECT_EQ(reorderTask(pathFileTasks, input, output), 1);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, reorderTask_InvalidTaskId) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 2, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 1, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("3\n6\n\n");
	std::stringstream output;

	EXPECT_EQ(reorderTask(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}


TEST_F(TaskschedulerTest, similarTasks_NotEnoughTasks) {
	const char* pathFileTasks = "few_tasks.bin";
	Task tasksToWrite[1] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 1, file);
	fclose(file);

	std::stringstream input("\n");
	std::stringstream output;

	EXPECT_FALSE(similarTasks(pathFileTasks, input, output));

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, similarTasks_FoundSimilarTasks) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[3] = {
		{1, 0, loggedUser, "Task 1", "Similar description", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Similar description", "2024-01-02", "Category 2", true, true, {}, 0},
		{3, 0, loggedUser, "Task 3", "Different description", "2024-01-03", "Category 3", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 3, file);
	fclose(file);

	std::stringstream input("\n");
	std::stringstream output;

	EXPECT_TRUE(similarTasks(pathFileTasks, input, output));

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, similarTasks_NoSimilarTasks) {
	const char* pathFileTasks = "tasks_with_no_similarities.bin";
	Task tasksToWrite[3] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0},
		{3, 0, loggedUser, "Task 3", "Description 3", "2024-01-03", "Category 3", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 3, file);
	fclose(file);

	std::stringstream input("\n");
	std::stringstream output;

	EXPECT_TRUE(similarTasks(pathFileTasks, input, output));

	remove(pathFileTasks);
}


TEST_F(TaskschedulerTest, allegiances_NoTasks) {
	const char* pathFileTasks = "empty_tasks.bin";

	FILE* file = fopen(pathFileTasks, "wb");
	fclose(file);

	std::stringstream input("\n");
	std::stringstream output;

	EXPECT_EQ(allegiances(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, allegiances_BFS) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n1\n\n");
	std::stringstream output;

	EXPECT_TRUE(allegiances(pathFileTasks, input, output));

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, allegiances_DFS) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("2\n1\n\n");
	std::stringstream output;

	EXPECT_TRUE(allegiances(pathFileTasks, input, output));

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, allegiances_InvalidStartID) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n3\n\n");
	std::stringstream output;

	EXPECT_EQ(allegiances(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, allegiances_WrongAlgorithmInput) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("3\n1\n\n");
	std::stringstream output;

	EXPECT_EQ(allegiances(pathFileTasks, input, output), 0);

	remove(pathFileTasks);
}


TEST_F(TaskschedulerTest, calculateMST_InvalidStartID) {
	std::stringstream input("-1\n\n");
	std::stringstream output;

	EXPECT_FALSE(calculateMST(input, output));

}

TEST_F(TaskschedulerTest, calculateMST_InvalidAlgorithmChoice) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n3\n\n");
	std::stringstream output;

	EXPECT_TRUE(calculateMST(input, output));

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, calculateMST_ValidKruskalChoice) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n2\n\n");
	std::stringstream output;

	EXPECT_TRUE(calculateMST(input, output));

	remove(pathFileTasks);
}


TEST_F(TaskschedulerTest, shortestPath_InvalidStartID) {
	std::stringstream input("-1\n\n");
	std::stringstream output;

	EXPECT_FALSE(shortestPath(input, output));

}

TEST_F(TaskschedulerTest, shortestPath_InvalidAlgorithmChoice) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n3\n\n");
	std::stringstream output;

	EXPECT_FALSE(shortestPath(input, output));

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, shortestPath_ValidBellmanFordChoice) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n2\n\n");
	std::stringstream output;

	EXPECT_TRUE(shortestPath(input, output));

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, loadTasksAndDependencies_FileNotOpened) {
	const char* invalidPath = "invalid_tasks.bin";

	bool result = loadTasksAndDependencies(invalidPath);

	EXPECT_FALSE(result);

}


TEST_F(TaskschedulerTest, huffmanEncodingTaskMenu_NoTasks) {
	const char* pathFileTasks = "empty_tasks.bin";

	FILE* file = fopen(pathFileTasks, "wb");
	fclose(file);

	std::stringstream input("\n");
	std::stringstream output;

	huffmanEncodingTaskMenu(pathFileTasks, input, output);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, huffmanEncodingTaskMenu_InvalidTaskId) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "Description 1", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Description 2", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("3\n\n");
	std::stringstream output;

	huffmanEncodingTaskMenu(pathFileTasks, input, output);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, huffmanEncodingTaskMenu_ValidTaskId) {
	const char* pathFileTasks = "tasks_with_entries.bin";
	Task tasksToWrite[2] = {
		{1, 0, loggedUser, "Task 1", "This is a sample task description.", "2024-01-01", "Category 1", true, true, {}, 0},
		{2, 0, loggedUser, "Task 2", "Another task description.", "2024-01-02", "Category 2", true, true, {}, 0}
	};

	FILE* file = fopen(pathFileTasks, "wb");
	fwrite(tasksToWrite, sizeof(Task), 2, file);
	fclose(file);

	std::stringstream input("1\n\n");
	std::stringstream output;

	huffmanEncodingTaskMenu(pathFileTasks, input, output);

	remove(pathFileTasks);
}

TEST_F(TaskschedulerTest, analyzeSCC_NoTasks) {
	const char* pathFileTasks = "empty_tasks.bin";

	FILE* file = fopen(pathFileTasks, "wb");
	fclose(file);

	std::stringstream input("\n");
	std::stringstream output;

	bool result = analyzeSCC(pathFileTasks, input, output);

	EXPECT_FALSE(result);

	remove(pathFileTasks);
}


TEST_F(TaskschedulerTest, GuestOperation1) {
	simulateUserInput("8\n\n\n1\n2\n3\n");
	EXPECT_EQ(guestOperation(in, out), 0);
}

TEST_F(TaskschedulerTest, GuestOperation2) {
	simulateUserInput("1\n\n2\n3\n");
	EXPECT_EQ(guestOperation(in, out), 0);
}


TEST_F(TaskschedulerTest, MainMenu1) {
	simulateUserInput("asd\n\n3\n2\n4\n");
	EXPECT_EQ(mainMenu(in, out), 0);
}

TEST_F(TaskschedulerTest, MainMenu2) {
	simulateUserInput("1\nrs\na\n4\n");
	EXPECT_EQ(mainMenu(in, out), 0);
}


TEST_F(TaskschedulerTest, LoginUser_Success) {
	const char* pathFileUsers = "test_users.bin";
	User usersToWrite[2] = {
		{1, "TestName1", "TestSurname1", "test1@example.com", "password1"},
		{2, "TestName2", "TestSurname2", "test2@example.com", "password2"}
	};

	std::ofstream file(pathFileUsers, std::ios::binary);
	int userCount = 2;
	file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
	file.write(reinterpret_cast<const char*>(&usersToWrite), sizeof(usersToWrite));
	file.close();

	simulateUserInput("\n");

	User loginUser = { 2, "TestName2", "TestSurname2", "test2@example.com", "password2" };
	EXPECT_EQ(::loginUser(loginUser, pathFileUsers, in, out), 1);

	remove(pathFileUsers);
}

TEST_F(TaskschedulerTest, LoginUser_Failure) {
	const char* pathFileUsers = "test_users.bin";
	User usersToWrite[2] = {
		{1, "TestName1", "TestSurname1", "test1@example.com", "password1"},
		{2, "TestName2", "TestSurname2", "test2@example.com", "password2"}
	};

	std::ofstream file(pathFileUsers, std::ios::binary);
	int userCount = 2;
	file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
	file.write(reinterpret_cast<const char*>(&usersToWrite), sizeof(usersToWrite));
	file.close();

	simulateUserInput("\n");

	User loginUser = { 2, "TestName2", "TestSurname2", "test2@example.com", "wrongpassword" };
	EXPECT_EQ(::loginUser(loginUser, pathFileUsers, in, out), 0);

	remove(pathFileUsers);
}

TEST_F(TaskschedulerTest, loginUserMenu_Success) {
	const char* pathFileUsers = "test_users.bin";
	User usersToWrite[2] = {
		{1, "TestName1", "TestSurname1", "test1@example.com", "password1"},
		{2, "TestName2", "TestSurname2", "test2@example.com", "password2"}
	};

	std::ofstream file(pathFileUsers, std::ios::binary);
	int userCount = 2;
	file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
	file.write(reinterpret_cast<const char*>(usersToWrite), sizeof(usersToWrite));
	file.close();

	simulateUserInput("test2@example.com\npassword2\n\n");

	EXPECT_EQ(::loginUserMenu(pathFileUsers, in, out), 1);

	remove(pathFileUsers);
}

TEST_F(TaskschedulerTest, loginUserMenu_Failure) {
	const char* pathFileUsers = "test_users.bin";
	User usersToWrite[2] = {
		{1, "TestName1", "TestSurname1", "test1@example.com", "password1"},
		{2, "TestName2", "TestSurname2", "test2@example.com", "password2"}
	};

	std::ofstream file(pathFileUsers, std::ios::binary);
	int userCount = 2;
	file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
	file.write(reinterpret_cast<const char*>(usersToWrite), sizeof(usersToWrite));
	file.close();

	simulateUserInput("test2@example.com\nwrongpassword\n\n");

	EXPECT_EQ(::loginUserMenu(pathFileUsers, in, out), 0);

	remove(pathFileUsers);
}

TEST_F(TaskschedulerTest, registerUser_Success) {
	const char* pathFileUsers = "test_users.bin";
	User newUser = { 0, "NewName", "NewSurname", "new@example.com", "newpassword" };

	std::ofstream file(pathFileUsers, std::ios::binary);
	int userCount = 0;
	file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
	file.close();

	simulateUserInput("\n\n");

	EXPECT_EQ(::registerUser(newUser, pathFileUsers, in, out), 1);

	remove(pathFileUsers);
}

TEST_F(TaskschedulerTest, registerUser_UserAlreadyExists) {
	const char* pathFileUsers = "test_users.bin";
	User usersToWrite[2] = {
		{1, "TestName1", "TestSurname1", "test1@example.com", "password1"},
		{2, "TestName2", "TestSurname2", "test2@example.com", "password2"}
	};

	std::ofstream file(pathFileUsers, std::ios::binary);
	int userCount = 2;
	file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
	file.write(reinterpret_cast<const char*>(usersToWrite), sizeof(usersToWrite));
	file.close();

	simulateUserInput("\n\n");

	User newUser = { 0, "TestName2", "TestSurname2", "test2@example.com", "password2" };
	EXPECT_EQ(::registerUser(newUser, pathFileUsers, in, out), 0);

	remove(pathFileUsers);
	}

TEST_F(TaskschedulerTest, registerUserMenu_Success) {
	const char* pathFileUsers = "test_users.bin";
	std::ofstream file(pathFileUsers, std::ios::binary);
	int userCount = 0;
	file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
	file.close();

	simulateUserInput("NewName\nNewSurname\nnew@example.com\nnewpassword\n\n\n");

	EXPECT_EQ(::registerUserMenu(pathFileUsers, in, out), 1);

	remove(pathFileUsers);
}

TEST_F(TaskschedulerTest, registerUserMenu_UserAlreadyExists) {
	const char* pathFileUsers = "test_users.bin";
	User usersToWrite[2] = {
		{1, "TestName1", "TestSurname1", "test1@example.com", "password1"},
		{2, "TestName2", "TestSurname2", "test2@example.com", "password2"}
	};

	std::ofstream file(pathFileUsers, std::ios::binary);
	int userCount = 2;
	file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
	file.write(reinterpret_cast<const char*>(usersToWrite), sizeof(usersToWrite));
	file.close();

	simulateUserInput("TestName2\nTestSurname2\ntest2@example.com\npassword2\n\n\n");

	EXPECT_EQ(::registerUserMenu(pathFileUsers, in, out), 1);

	remove(pathFileUsers);
}

TEST_F(TaskschedulerTest, UserMenu1) {
	simulateUserInput("asd\n\n1\n8\n6\n4\n");
	EXPECT_EQ(userOperations(in, out), 0);
}

TEST_F(TaskschedulerTest, UserMenu2) {
	simulateUserInput("2\n3\n6\n4\n");
	EXPECT_EQ(userOperations(in, out), 0);
}


TEST_F(TaskschedulerTest, UserMenu3) {
	simulateUserInput("3\n3\n6\n4\n");
	EXPECT_EQ(userOperations(in, out), 0);
}

TEST_F(TaskschedulerTest, UserMenu4) {
	simulateUserInput("4\n5\n6\n4\n");
	EXPECT_EQ(userOperations(in, out), 0);
}

TEST_F(TaskschedulerTest, LoginUser_NoUsersRegistered) {
	const char* pathFileUsers = "test_users.bin";

	std::ofstream file(pathFileUsers, std::ios::binary);
	int userCount = 0;
	file.write(reinterpret_cast<const char*>(&userCount), sizeof(int));
	file.close();

	User loginUser = { 0, "TestName", "TestSurname", "test@example.com", "password" };

	simulateUserInput("\n");

	EXPECT_EQ(::loginUser(loginUser, pathFileUsers, in, out), 0);

	remove(pathFileUsers);
}

int main(int argc, char** argv) {
#ifdef ENABLE_TASKSCHEDULER_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}