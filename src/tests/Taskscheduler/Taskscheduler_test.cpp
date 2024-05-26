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

int main(int argc, char** argv) {
#ifdef ENABLE_TASKSCHEDULER_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}