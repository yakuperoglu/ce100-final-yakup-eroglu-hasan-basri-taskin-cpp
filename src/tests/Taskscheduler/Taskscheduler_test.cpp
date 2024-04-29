#define ENABLE_TASKSCHEDULER_TEST 
#include "gtest/gtest.h"
#include "../../taskscheduler/header/taskscheduler.h"  


class TaskschedulerTest : public ::testing::Test {
protected:
	void SetUp() override {
		// Setup test data
	}

	void TearDown() override {
		// Clean up test data
	}
};

int main(int argc, char** argv) {
#ifdef ENABLE_TASKSCHEDULER_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}