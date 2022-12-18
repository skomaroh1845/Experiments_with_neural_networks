#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Neuron.h"

using namespace std;




struct cluster {
	int px[9];
	float res[2];
};

void load_sample(const char* path, cluster* sample) {
	ifstream f(path);
	if (f.is_open()) {

		int k = 0;
		while (true) {
			if (f.eof()) break;
			
			for (int i = 0; i < 9; ++i)
					f >> sample[k].px[i];
			
			string a;
			f >> a;
			sample[k].res[0] = stof(a);
			a.clear();
			f >> a;
			sample[k].res[1] = stof(a);

			++k;
			if (k > 15) break;
		}
		f.close();
	}
	else {
		cout << "File open error" << endl;
		exit(-1);
	}
}



/*
void save_coef(const char* path, weight& W) {

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
} 
*/



int main() {
	// INIT //
	// first 
	vector<Neuron*> L0;
	for (int i = 0; i < 9; ++i) {
		Neuron* neur = new Neuron();
		L0.push_back(neur);
	}
	Neuron* neur = new Neuron();	// shift neuron
	L0.push_back(neur);
	L0[9]->setValue(1);  

	// second 
	vector<Neuron*> L1;
	for (int i = 0; i < 4; ++i) {
		Neuron* neur = new Neuron(L0);
		L1.push_back(neur);
	}
	neur = new Neuron();	// shift neuron
	L1.push_back(neur);
	L1[4]->setValue(1);

	// third 
	vector<Neuron*> L2;
	for (int i = 0; i < 3; ++i) {
		Neuron* neur = new Neuron(L1);
		L2.push_back(neur);
	}
	neur = new Neuron();	// shift neuron
	L2.push_back(neur);
	L2[3]->setValue(1);

	// last
	vector<Neuron*> L3;
	for (int i = 0; i < 2; ++i) {
		Neuron* neur = new Neuron(L2);
		L3.push_back(neur);
	}


	// learning massive  
	cluster sample[16];
	load_sample("sample.txt", sample);

	// learning
	float error = 10;
	int iter = 0;
	float k = 0.1;

	while (error > 1)
	{
		error = 0;
		++iter;
		
		for (int i = 0; i < 16; ++i)
		{
			// load sample to input neurons
			for (int j = 0; j < 9; ++j) {
				L0[j]->setValue( sample[i].px[j] );
			}
			
			// forward signal
			for (Neuron* el : L1) {
				el->computeValue();
			}
			for (Neuron* el : L2) {
				el->computeValue();
			}
			for (Neuron* el : L3) {
				el->computeValue();
			}
			
			// error processing
			float err1 = sample[i].res[0] - L3[0]->getValue();
			float err2 = sample[i].res[1] - L3[1]->getValue();
			L3[0]->setError(err1);
			L3[1]->setError(err2);

			error += abs(err1) + abs(err2);

			for (Neuron* el : L3) {
				el->computeError();
			}
			for (Neuron* el : L2) {
				el->computeError();
			}
			for (Neuron* el : L1) {
				el->computeError();
			}

			// learning
			for (Neuron* el : L3) {
				el->learn(k);
			}
			for (Neuron* el : L2) {
				el->learn(k);
			}
			for (Neuron* el : L1) {
				el->learn(k);
			}
		}

		std::cout << "abs error: " << error << "; iteration " << iter << endl;
	}

	// testing
	cout << "Learning complited." << endl << endl;
	
	while (true)
	{
		cout << "Enter number of sample for test learing quality: ";
		int n;
		cin >> n;
		if (n < 0) break;

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				cout << sample[n].px[j + i * 3] << " ";
			}
			cout << endl;
		}

		// load sample to input neurons
		for (int j = 0; j < 9; ++j) {
			L0[j]->setValue(sample[n].px[j]);
		}

		// forward signal
		for (Neuron* el : L1) {
			el->computeValue();
		}
		for (Neuron* el : L2) {
			el->computeValue();
		}
		for (Neuron* el : L3) {
			el->computeValue();
		}

		// answer
		if (L3[0]->getValue() > 0.6 && L3[1]->getValue() < 0.4) cout << "horizontal line" << endl << endl;
		if (L3[0]->getValue() < 0.4 && L3[1]->getValue() > 0.6) cout << "vertical line" << endl << endl;
		if (L3[0]->getValue() > 0.6 && L3[1]->getValue() > 0.6) cout << "both lines" << endl << endl;
		if (L3[0]->getValue() < 0.4 && L3[1]->getValue() < 0.4) cout << "no lines" << endl << endl;
	}


	// memory cleaning
	for (Neuron* el : L0) {
		delete el;
	}
	for (Neuron* el : L1) {
		delete el;
	}
	for (Neuron* el : L2) {
		delete el;
	}
	for (Neuron* el : L3) {
		delete el;
	}
	return 0;
}

