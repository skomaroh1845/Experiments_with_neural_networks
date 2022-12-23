#include <vector>
#include <iostream>

#include "NeuralNetwork.h"

int main(int argc, char** argv) {

	using namespace std;

	NeuralNetwork nn;
	
	cout << "Select Neural Network mod: creation (1) / loading (0)" << endl;
	bool flag;
	cin >> flag;

	if (flag) {	// creation
				
		// init
		cout << "Input neural network configuration." << endl;
		cout << "Number of layers: ";
		int n;
		cin >> n;
		int* layers = new int[n];
		for (int i = 0; i < n; ++i) {
			cout << "Amount of neurons in " << i << " layer: ";
			cin >> layers[i];
		}
		nn.init(n, layers);
		cout << "Network was successfully initialized." << endl;
		
		cout << "Input data set file name: ";
		string path;
		cin >> path;
		nn.loadSample(path.c_str());

		// learning
		cout << "Input learning settings: \nCoefficient: ";
		float k; cin >> k;
		cout << "Min error for learn ending: ";
		float err; cin >> err;
		nn.learn(k, err);
		delete[] layers;
	}
	else {	
		// loading
		cout << "Loading. Input file name: ";
		string path;
		cin >> path;
		nn.loadFromFile(path.c_str());
	}

	// testing
	/* cout << "Testing." << endl;
	cout << "Input testing data set file name: ";
	string path;
	cin >> path;
	nn.loadSample(path.c_str());

	while (true)
	{
		cout << "Enter number of sample for test learing quality (-1 for exit): ";
		int n;
		cin >> n;
		if (n < 0) break;

		// answer
		vector<float> ans(nn.getAnswer(n));

		// interpritation
	} */
	
	// saving
	cout << "Do you want to save this network? ( 1/0 )" << endl;;
	cin >> flag;
	if (flag)
	{
		
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

