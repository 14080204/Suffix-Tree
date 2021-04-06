#include<iostream>
#include<chrono>
#include"SuffixTree.h"

using namespace std;

void SystemConfiguration() {
	cout << "System Configurations:" << endl;
	cout << "-> CPU:" << endl;
	cout << "AMD Ryzen 5 3600\n" << endl;
	cout << "-> Clock Rate:" << endl;
	cout << "3.6 GHz\n" << endl;
	cout << "-> RAM:" << endl;
	cout << "16Gb DDR4 3200MHz\n" << endl;
	cout << "-> Cache size:" << endl;
	cout << "L1:384Kb \t L2:3.0Mb \t L3:32.0Mb\n" << endl;
}

int main(int argc, char* argv[]) {
	char* input = nullptr;
	char* alphabet = nullptr;
	if (argc != 3) {
		cout << "ERROR! Expecting 3 parameters(program name;string s;alphabet)" << endl;
		return 0;
	}
	else {
		input = argv[1];
		alphabet = argv[2];
	}	
	SystemConfiguration();
	SuffixTree tree(input, alphabet);

	auto started = std::chrono::high_resolution_clock::now();
	tree.BuildTree();
	auto done = std::chrono::high_resolution_clock::now();
	long long time = std::chrono::duration_cast<std::chrono::microseconds>(done - started).count();
	cout << "\nTime to construct suffix tree: " << time << " microseconds" << endl;

	tree.OutputToTerminal();
	tree.BWTIndexToFile();
	tree.WriteFile(time);

	return 0;
}