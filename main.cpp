#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "NeuralNetwork.h"


/*
void save_config(const char* path, vector<Neuron*> L) {

	ofstream f(path);
	if (f.is_open()) {
		int n2 = W.L01[0].size() - 1;
		int n1 = W.L01.size();
		for (int i = 0; i < n1; ++i)
		{
			for (int j = 0; j < n2; ++j)
				f << W.L01[i][j] << " ";
			f << "\n";
		}
		n2 = W.L12[0].size() - 1;
		n1 = W.L12.size();
		for (int i = 0; i < n1; ++i)
		{
			for (int j = 0; j < n2; ++j)
				f << W.L12[i][j] << " ";
			f << "\n";
		}
		n2 = W.L23[0].size() - 1;
		n1 = W.L23.size();
		for (int i = 0; i < n1; ++i)
		{
			for (int j = 0; j < n2; ++j)
				f << W.L23[i][j] << " ";
			f << "\n";
		}

		f.close();
	}
	else {
		cout << "Save error. File could not be opened." << endl;
		exit(-1);
	}
}  */


int main() {

	using namespace std;

	// INIT //
	NeuralNetwork nn;
	int layers[4] = {10, 5, 4, 2};
	nn.init(4, layers);

	// learning samples  
	nn.loadSample("sample.txt");

	// learning
	nn.learn(0.1, 0.5);
		
	// testing
	while (true)
	{
		cout << "Enter number of sample for test learing quality: ";
		int n;
		cin >> n;
		if (n < 0) break;
		
		// answer
		vector<float> ans(nn.getAnswer(n));
		
		// interpritation
		if (ans[0] > 0.6 && ans[1] < 0.4) cout << "horizontal line" << endl << endl;
		if (ans[0] < 0.4 && ans[1] > 0.6) cout << "vertical line" << endl << endl;
		if (ans[0] > 0.6 && ans[1] > 0.6) cout << "both lines" << endl << endl;
		if (ans[0] < 0.4 && ans[1] < 0.4) cout << "no lines" << endl << endl;
	}
	
	return 0;
}

