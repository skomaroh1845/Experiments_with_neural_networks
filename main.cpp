#include <vector>
#include <iostream>

#include "NeuralNetwork.h"

int main() {

	using namespace std;

	NeuralNetwork nn;
	
	cout << "Select Neural Network mod: creation (1) / loading (0)" << endl;
	bool flag;
	cin >> flag;

	// learning samples  
	nn.loadSample("sample.txt");

	if (flag) {	// creation
		// init
		int layers[4] = { 10, 5, 4, 2 };
		nn.init(4, layers);

		// learning
		nn.learn(0.1, 0.5);
	}
	else {	
		// loading
		cout << "Loading. Input file name: ";
		string path;
		cin >> path;
		nn.loadFromFile(path.c_str());
	}

	// testing
	while (true)
	{
		cout << "Enter number of sample for test learing quality (0 to 15; -1 for exit): ";
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

	cout << "Do you want to save this network? ( 1/0 )" << endl;;
	cin >> flag;
	if (flag)
	{
		// saving
		cout << "Saving. Input file name: ";
		string path;
		cin >> path;
		nn.save(path.c_str());
		if (!nn.is_save())
		{
			cout << "Input another file name: ";
			cin >> path;
			nn.save(path.c_str());
		}
	}

	return 0;
}

