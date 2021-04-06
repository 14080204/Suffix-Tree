#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <tuple>
#include <ctime>
#include <sstream>
#include <algorithm>

using namespace std;

struct node {//Each internal node should be a structure with at least the following information : 
	//{ node ID, suffix - link pointer, parent pointer, parent edge label(i.e., label of the incoming edge from the parent), children pointers, string - depth}.
	int id;
	int parent_edge_label[2];
	int string_depth;
	struct node* left;
	struct node* right;
};

class SuffixTree {
public:
	struct node* suffixNode;

	SuffixTree(char* inputFile, char* alphabetFile);
	~SuffixTree();
	void BuildTree();
	void WriteFile(int time);
	void BWTIndexToFile();
	void OutputToTerminal();

private:
	int stringLength;	
	int numInternalNodes;
	int numLeaves;	
	int DeepestInternalNode;
	int longestRepeat;
	int numduplicate;
	double averageDepth;

	vector<int> longestStart;

	node* deepestInternalNode;
	node* ChildBranch;
	node* newLeafNode;
	node* prev;
	node* extendNode;

	string inputFileName;
	string stringName;
	string String;//String is the string from original data
	string alphabet;
	string fileName;

	void BWTIndex(node* N);
	void BWTIndexTest(node* N);	
	void DFSDetailed(node* N);
	void DFS(node* N,int& counter);
	void PostOrder(node* N, int& counter);
	void DFSDisplayToString(node* N, int counter, string* output);
	void SetAlphabet(char* alphabetFile);
	void Statistics();
	bool FindPath(node* N, int index);
	void BWTIndexToFileHelper(node* N, string* output);
	void StatisticsHelper(node* N, int* depthSum, int* deepest);
	void LongestCoordinates(node* N,int& numduplicate);
};
