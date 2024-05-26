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



int main(int argc, char** argv) {
#ifdef ENABLE_TASKSCHEDULER_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}