#include "SuffixTree.h"

SuffixTree::SuffixTree(char* inputFile, char* alphabetFile)
{
	inputFileName = inputFile;
	ifstream inFile(inputFile);
	if (inFile.is_open()) {
		cout << "Input file read successfully!" << inputFileName <<endl;
		string line;
		getline(inFile, line);
		int pos = line.find(' ', 0);
		stringName = line.substr(1, pos);

		while (getline(inFile, line)) {
			String.append(line);
		}
		String.push_back('$');
	}
	else {
		cout << "Error! Couldn't read input file." << endl;
	}
	inFile.close();

	stringLength = String.length();
	cout << "String length: " << stringLength << endl;

	SetAlphabet(alphabetFile);

	numInternalNodes = 1;
	numLeaves = 1;
	fileName = stringName;

	char badChars[] = "|:";
	for (unsigned int i = 0; i < strlen(badChars); ++i)
	{
		fileName.erase(std::remove(fileName.begin(), fileName.end(), badChars[i]), fileName.end());
	}
}

SuffixTree::~SuffixTree() {
	delete ChildBranch;
	delete newLeafNode;
	delete prev;
	delete extendNode;
	cout << "\nSpace has been freed!" << endl;
}

void SuffixTree::SetAlphabet(char* alphabetFile)
{
	ifstream inFile(alphabetFile);
	if (inFile.is_open()) {
		string line = "";
		if (getline(inFile, line)) {
			char c;
			std::istringstream stream(line);
			while (stream >> c) {
				alphabet.push_back(c);
			}
		}
	}
	else {
		cout << "Error! Couldn't read alphabet file." << endl;
	}
}

void SuffixTree::BuildTree()
{
	suffixNode = new node{0, {0,0}, 0, nullptr, nullptr };
	for (int i = 0; i < stringLength; i++) {
		FindPath(suffixNode, i);
	}
}

bool SuffixTree::FindPath(node* N, int index)
{
	ChildBranch = N->left;
	while (ChildBranch != nullptr) {
		if (String[ChildBranch->parent_edge_label[0]] == String[index]) {
			break;
		}
		ChildBranch = ChildBranch->right;
	}
	if (ChildBranch == nullptr) {
		newLeafNode = new node{
			numLeaves++, {index, stringLength}, stringLength - index, NULL, NULL
		};
		if (N->left == nullptr) {
			N->left = newLeafNode;
		}
		else {
			ChildBranch = N->left;
			if (String[newLeafNode->parent_edge_label[0]] < String[ChildBranch->parent_edge_label[0]]) {
				newLeafNode->right = ChildBranch;
				N->left = newLeafNode;
			}
			else {
				prev = ChildBranch;
				while (ChildBranch != nullptr && String[newLeafNode->parent_edge_label[0]] > String[ChildBranch->parent_edge_label[0]]) {
					prev = ChildBranch;
					ChildBranch = ChildBranch->right;
				}
				if (ChildBranch != nullptr) {
					prev->right = newLeafNode;
					newLeafNode->right = ChildBranch;
				}
				else {
					prev->right = newLeafNode;
				}
			}
		}
		return false;
	}

	string alpha = String.substr(ChildBranch->parent_edge_label[0], (ChildBranch->parent_edge_label[1] - ChildBranch->parent_edge_label[0] + 1));
	int j = 0, i = index;
	while (i <= stringLength && alpha[j] != NULL) {
		if (String[i] == alpha[j]) {
			i++;
			j++;
		}
		else {
			newLeafNode = new node{numLeaves++, {index + j, stringLength}, stringLength - index, nullptr, nullptr };
			extendNode = new node{ChildBranch->id, {ChildBranch->parent_edge_label[0] + j, ChildBranch->parent_edge_label[1]},ChildBranch->string_depth, ChildBranch->left, nullptr };
			ChildBranch->id = stringLength + numInternalNodes++;
			ChildBranch->parent_edge_label[1] = ChildBranch->parent_edge_label[0] + j - 1;
			ChildBranch->string_depth = j;
			ChildBranch->left = nullptr;

			if (String[extendNode->parent_edge_label[0]] < String[newLeafNode->parent_edge_label[0]]) {
				ChildBranch->left = extendNode;
				extendNode->right = newLeafNode;
			}
			else {
				ChildBranch->left = newLeafNode;
				newLeafNode->right = extendNode;
			}
			return false;
		}
	}
	return FindPath(ChildBranch, i);
}

void SuffixTree::DFSDetailed(node* N)
{
	if (N) {
		cout << "\n{\tID: " << N->id << "\tlabel: " << String.substr(N->parent_edge_label[0], (N->parent_edge_label[1] - N->parent_edge_label[0] + 1)) <<
			"\n\tstringDepth:" << N->string_depth <<
			"\n\tparent_edges: {" << N->parent_edge_label[0] << ", " << N->parent_edge_label[1] << "}" <<
			"\n\tleftChildID: " << (N->left != nullptr ? N->left->id : NULL) <<
			"\n\trightSiblingID: " << (N->right != nullptr ? N->right->id : NULL) <<
			"\n} ";
		DFSDetailed(N->left);
		DFSDetailed(N->right);
	}
}

void SuffixTree::DFS(node* N,int& counter) {//same with pre-order in tree
	if (N) {
		cout << N->string_depth << " ";
		counter++;
		if (counter % 10 == 0)cout << endl;
		DFS(N->left, counter);
		DFS(N->right, counter);
	}
}

void SuffixTree::PostOrder(node* N, int& counter) {
	if (N) {	

		DFS(N->left, counter);
		DFS(N->right, counter);
		cout << N->string_depth << " ";
		counter++;	
		if (counter % 10 == 0)cout << endl;			
	}
}

void SuffixTree::BWTIndex(node* N)
{
	if (N->left == nullptr) {
		if (N->id == 1)
			cout << String[stringLength - 1] << endl;
		else
			cout << String[N->id - 2] << endl;
	}
	node* ptr = N->left;
	while (ptr != nullptr) {
		BWTIndex(ptr);
		ptr = ptr->right;
	}
}

void SuffixTree::BWTIndexTest(node* N)
{
	if (N->left == nullptr) {
		cout << N->id << " - 1" << endl;
	}

	node* ptr = N->left;
	while (ptr != nullptr) {
		BWTIndexTest(ptr);
		ptr = ptr->right;
	}
}

void SuffixTree::BWTIndexToFile()
{
	cout << "\nWritting BWT index to file..." << endl;
	string* bwt = new string();
	BWTIndexToFileHelper(suffixNode, bwt);

	ofstream outfile("BWTResult_"+ inputFileName + ".txt");

	outfile << *bwt;
	outfile.close();
	delete bwt;
}

void SuffixTree::BWTIndexToFileHelper(node* N, string* output)
{
	if (N->left == nullptr) {
		if (N->id == 1)
			(*output).push_back(String[stringLength - 1]);
		else
			(*output).push_back(String[N->id - 2]);
		(*output).push_back('\n');
	}

	node* ptr = N->left;
	while (ptr != nullptr) {
		BWTIndexToFileHelper(ptr, output);
		ptr = ptr->right;
	}
}

void SuffixTree::Statistics()
{
	int* depthSum = new int(0), * deepest = new int(0);
	node* deepestInternal = new node();
	StatisticsHelper(suffixNode, depthSum, deepest);
	averageDepth = double(*depthSum) / double(numInternalNodes - 1);
	DeepestInternalNode = *deepest;
	numduplicate = 0;
	LongestCoordinates(deepestInternalNode, numduplicate);
	delete depthSum;
	delete deepest;
	delete deepestInternal;
}

void SuffixTree::StatisticsHelper(node* N, int* depthSum, int* deepest)
{
	if (N->left != nullptr) {
		*depthSum += N->string_depth;
		if (*deepest < N->string_depth) {
			*deepest = N->string_depth;
			deepestInternalNode = N;
		}
	}
	node* ptr = N->left;
	while (ptr != nullptr) {
		StatisticsHelper(ptr, depthSum, deepest);
		ptr = ptr->right;
	}
}

void SuffixTree::LongestCoordinates(node* N, int& numduplicate)
{
	if (N->left == nullptr) {
		longestStart.push_back(N->parent_edge_label[1] - N->string_depth);
		numduplicate++;
	}

	node* ptr = N->left;
	while (ptr != nullptr) {
		LongestCoordinates(ptr,numduplicate);
		ptr = ptr->right;
	}
}

void SuffixTree::DFSDisplayToString(node* N, int counter, string* output)
{
	(*output).append("\n{\tID: " + to_string(N->id) + "\tlabel: " +
		String.substr(N->parent_edge_label[0], (N->parent_edge_label[1] - N->parent_edge_label[0] + 1)) +
		"\n\tstringDepth: " + to_string(N->string_depth) +
		"\n\tparent_edges: {" + to_string(N->parent_edge_label[0]) + ", " +
		to_string(N->parent_edge_label[1]) + "}" +
		"\n\tleftChildID: " + to_string(N->left != nullptr ? N->left->id : NULL) +
		"\n\trightSiblingID: " + to_string(N->right != nullptr ? N->right->id : NULL) +
		"\n} ");
	counter++;

	node* ptr = N->left;
	while (ptr != nullptr) {
		DFSDisplayToString(ptr, counter, output);
		ptr = ptr->right;
	}
}

void SuffixTree::OutputToTerminal() {
	cout << "-> Internal nodes: " << numInternalNodes << endl <<
		"-> Leaves: " << numLeaves - 1 << endl <<
		"-> Nodes total: " << numInternalNodes + numLeaves - 1 << endl;
	//if (stringLength < 500) {
	//	int dfsCounter = 0;
	//	cout << "\n\nDFS Depth:\n " << endl;
	//	DFS(suffixNode, dfsCounter);
	//	cout << "\n\nPost-Order Depth:\n " << endl;
	//	int postOrderCounter = 0;
	//	PostOrder(suffixNode, postOrderCounter);
	//	//cout << "\nDFS in Detailed:\n";
	//	//DFSDetailed(suffixNode);
	//	//cout << "\n\nBWTIndexTest:\n";
	//	//BWTIndexTest(suffixNode);
	//	cout << "\n\nBWTIndex:\n";
	//	BWTIndex(suffixNode);
	//}
}

void SuffixTree::WriteFile(int time)
{
	cout << "Writting Statistics to file..." << endl;
	Statistics();
	ofstream outfile1("Statistics_" + inputFileName + ".txt");

	outfile1 << "-> Running time of ST construction (in microseconds): " << time << endl <<
		"-> Internal nodes: " << numInternalNodes << endl <<
		"-> Leaves: " << numLeaves - 1 << endl <<
		"-> Nodes total: " << numInternalNodes + numLeaves - 1 << endl <<
		"-> Size of tree (in Bytes): " << (numInternalNodes + numLeaves - 1) * sizeof(node) << endl <<
		"-> Average string-depth of internal nodes: " << averageDepth << endl <<
		"-> String-depth of deepest internal node: " << DeepestInternalNode << endl <<
		"-> Length of longest exact matching repeat (deepest internal node): " << deepestInternalNode->string_depth << endl <<
		"-> Longest exact matching repeated times: " << numduplicate << endl <<
		"-> Start coordinates of each occurrence of longest exact match (indexing from 0): {";
	for (int i : longestStart) {
		outfile1 << " " << i;
	}
	outfile1 << " }" << endl;
	outfile1.close();

	ofstream outfile2("DFS_" + inputFileName + ".txt");
	try {
		struct node* test = suffixNode;
		string* dfs = new string("");
		DFSDisplayToString(test, 1, dfs);
		outfile2 << *dfs;
		delete dfs;
	}
	catch (exception) {
		cout << "\nFailed to randomly select node for DFS printout." << endl;
	}
	outfile2.close();
}