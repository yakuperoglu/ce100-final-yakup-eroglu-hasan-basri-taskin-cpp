#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

using namespace std;

/**
 * @brief Structure representing a user.
 *
 * This structure stores information about a user including ID, name, surname, email, and password.
 */
typedef struct {
    int id;                /**< User ID */
    char name[100];        /**< User name */
    char surname[100];     /**< User surname */
    char email[100];       /**< User email */
    char password[100];    /**< User password */
} User;

/**
 * @brief Structure representing a task.
 *
 * This structure stores information about a task including ID, importance ID, owner, name, description, deadline, category, and dependencies.
 */
typedef struct {
    int id;                   /**< Task ID */
    int impid;                /**< Importance ID */
    User owner;               /**< Owner of the task */
    char name[100];           /**< Task name */
    char description[300];    /**< Task description */
    char deadLine[100];       /**< Task deadline */
    char category[100];       /**< Task category */
    bool isCategorized;       /**< Flag indicating if the task is categorized */
    bool isDeadlined;         /**< Flag indicating if the task has a deadline */
    int dependencies[10];     /**< Array of dependencies (task IDs) */
    int numDependencies;      /**< Number of dependencies */
} Task;

/**
 * @brief Structure representing a node in a Huffman tree.
 *
 * This structure stores information about a Huffman tree node including the character, its frequency, and pointers to its left and right children.
 */
struct HuffmanNode {
    char data;              /**< Character data */
    unsigned freq;          /**< Frequency of the character */
    HuffmanNode* left;      /**< Pointer to the left child */
    HuffmanNode* right;     /**< Pointer to the right child */

    /**
     * @brief Constructor for HuffmanNode.
     *
     * @param data Character data.
     * @param freq Frequency of the character.
     */
    HuffmanNode(char data, unsigned freq) : data(data), freq(freq), left(nullptr), right(nullptr) {}
};

/**
 * @brief Comparison function object for Huffman nodes.
 *
 * This structure defines a comparison operator for Huffman nodes based on their frequency.
 */
struct compare {
    /**
     * @brief Comparison operator for Huffman nodes.
     *
     * @param l Pointer to the left Huffman node.
     * @param r Pointer to the right Huffman node.
     * @return bool Returns true if the frequency of the left node is greater than the frequency of the right node.
     */
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return (l->freq > r->freq);
    }
};

/**
 * @brief Structure representing an edge in a graph.
 *
 * This structure stores information about an edge including the source vertex, destination vertex, and weight.
 */
struct Edge {
    int src;     /**< Source vertex */
    int dest;    /**< Destination vertex */
    int weight;  /**< Weight of the edge */
};


//TOOLS

void clearScreen();

bool enterToContinue(istream& in, ostream& out);

bool handleInputError(istream& in, ostream& out);

int getInput(istream& in);

int getNewUserId(User users[], int userCount);

int getNewTaskId(const char* pathFileTasks);

//TOOLS

//PRINT MENUS

bool printGuestMenu(ostream& out);

bool printMainMenu(ostream& out);

bool printUserMenu(ostream& out);

bool printCreateTaskMenu(ostream& out);

bool printDeadlineSettingsMenu(ostream& out);

bool printReminderSystemMenu(ostream& out);

bool printTaskPrioritizationMenu(ostream& out);

bool printFlowAlgorithmsMenu(ostream& out);

//PRINT MENUS

//MENUS

int createTaskMenu(istream& in, ostream& out);

int deadlineSettingsMenu(istream& in, ostream& out);

int reminderSystemMenu(istream& in, ostream& out);

int taskPrioritizationMenu(istream& in, ostream& out);

int flowAlgorithmsMenu(istream& in, ostream& out);

//MENUS

bool viewTask(const char* pathFileTasks, istream& in, ostream& out);

bool viewTaskForFunc(const char* pathFileTasks, istream& in, ostream& out);

int loadTasks(const char* pathFileTasks, Task** tasks);

int loadOwnedTasks(const char* pathFileTasks, Task** tasks, int userId);

bool viewDeadlines(const char* pathFileTasks, istream& in, ostream& out);

int addTask(const Task* newTask, const char* pathFileTasks);

int addTaskMenu(const char* pathFileTasks, istream& in, ostream& out);

int categorizeTask(const char* pathFileTasks, istream& in, ostream& out);

int assignDeadline(const char* pathFileTasks, istream& in, ostream& out);

int setReminders(istream& in, ostream& out);

int notificationSettings(istream& in, ostream& out);

int markTaskImportance(const char* pathFileTasks, istream& in, ostream& out);

int reorderTask(const char* pathFileTasks, istream& in, ostream& out);

bool similarTasks(const char* pathFileTasks, istream& in, ostream& out);

bool allegiances(const char* pathFileTasks, istream& in, ostream& out);

bool calculateMST(istream& in, ostream& out);

bool shortestPath(istream& in, ostream& out);

//Algorithms

bool bfs(int** residualGraph, int source, int sink, int parent[], int V);

bool dfs(int** residualGraph, int source, int sink, int parent[], int V);

bool loadTasksAndDependencies(const char* pathFileTasks);

int BFS(int startVertex);

int DFSUtil(int vertex, bool visited[]);

int DFS(int startVertex);

void heapify(User users[], int n, int i);

void buildMaxHeap(User users[], int n);

int longestCommonSubsequence(const char* text1, const char* text2);

void buildHuffmanTree(const char* pathFileTasks, const char* data, int size);

void storeCodes(HuffmanNode* root, string str);

void printCodes(ostream& out);

string encode(const char* data, int size);

void huffmanEncodingTaskMenu(const char* pathFileTasks, istream& in, ostream& out);

bool analyzeSCC(const char* pathFileTasks, istream& in, ostream& out);

void addEdgeWithWeight(int u, int v, int weight);

void primMST(int startVertex, ostream& out);

bool compareEdges(const Edge& a, const Edge& b);

int find(int parent[], int i);

void unionSets(int parent[], int x, int y);

void kruskalMST(int V, ostream& out);

void calculateShortestPath(int startVertex, ostream& out);

void calculateBellmanFord(int startVertex, int V, ostream& out);

int fordFulkerson(int** graph, int source, int sink, int V);

int edmondsKarp(int** graph, int source, int sink, int V);

/*
void encryptData(uint8_t* data, size_t length);

void decryptData(uint8_t* data, size_t length);
*/

//Algorithms

//LOGIN REGISTER

int loginUser(User loginUser, const char* pathFileUsers, istream& in, ostream& out);

int loginUserMenu(const char* pathFileUsers, istream& in, ostream& out);

int registerUser(User user, const char* pathFileUser, istream& in, ostream& out);

int registerUserMenu(const char* pathFileUsers, istream& in, ostream& out);

//LOGIN REGISTER

int guestOperation(istream& in, ostream& out);

int userOperations(istream& in, ostream& out);

int mainMenu(istream& in, ostream& out);

#endif // TASKSCHEDULER_H
