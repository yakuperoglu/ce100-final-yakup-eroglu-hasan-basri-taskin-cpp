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



int main(int argc, char** argv) {
#ifdef ENABLE_TASKSCHEDULER_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}