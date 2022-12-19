#include "NeuralNetwork.h"

#include <fstream>
#include <iostream>
#include <string>


NeuralNetwork::~NeuralNetwork()
{
	clear();
}

void NeuralNetwork::init(int nLayers, int* nNeurons)
{
	using namespace std;

	clear();
	saved = false;

	for (int i = 0; i < nLayers; ++i) {
		
		config.push_back(vector<Neuron*>());
		
		if (i == 0) {
			for (int j = 0; j < nNeurons[i] - 1; ++j) {
				config[i].push_back(new Neuron());
			}
		}
		else {
			for (int j = 0; j < nNeurons[i] - 1; ++j) {
				config[i].push_back(new Neuron( config[i-1] ));
			}
		}

		if (i < nLayers - 1)
		{
			config[i].push_back(new Neuron());	// shift neuron
			config[i][nNeurons[i]-1]->setValue(1);
		}
		else 
		{
			config[i].push_back(new Neuron(config[i - 1]));
		}
	}
}

void NeuralNetwork::loadSample(const char* path)
{
	using namespace std;

	sample.clear();

	
	ifstream f(path);
	if (f.is_open()) {

		int k = 0;
		
		while (true) {
			if (f.eof()) break;
			
			sample.push_back(vector<float>());

			for (int i = 0; i < 11; ++i)
			{
				float a;
				f >> a;
				sample[k].push_back(a);
			}
			
			++k;
		}

		f.close();
	}
	else {
		cout << "File open error" << endl;
		exit(-1);
	}
}

void NeuralNetwork::learn(float k, float err)
{
	using namespace std;

	float error = err + 1;
	int iter = 0;
	saved = false;

	while (error > err)
	{
		error = 0;
		++iter;

		for (int i = 0; i < sample.size(); ++i)
		{
			// load sample to input neurons
			for (int j = 0; j < 9; ++j) {
				config[0][j]->setValue(sample[i][j]);
			}

			// forward signal
			for (vector<Neuron*> L : config) {
				for (Neuron* neur : L)
					neur->computeValue();
			}

			// error processing
			float err1 = sample[i][sample[i].size() - 2] - config[config.size() - 1][0]->getValue();
			float err2 = sample[i][sample[i].size() - 1] - config[config.size() - 1][1]->getValue();
			error += abs(err1) + abs(err2);
			
			config[config.size() - 1][0]->setError(err1);
			config[config.size() - 1][1]->setError(err2);
			
			for (int i = config.size() - 1; i > 0; --i) {
				for (Neuron* neur : config[i])
					neur->computeError();
			}
			
			// learning
			for (int i = config.size() - 1; i > 0; --i) {
				for (Neuron* neur : config[i])
					neur->learn(k);
			}
		}

		std::cout << "iteration " << iter << "; abs error: " << error << endl;
	}
	
	cout << "Learning complited." << endl << endl;
}

void NeuralNetwork::save(const char* path)
{
	using namespace std;

	ofstream fout;
	fout.open(path);

	if (fout.is_open()) {

		fout << config.size() << "\n";  // save number of layers
		
		for (int i = 0; i < config.size(); ++i) {	// save number of neurons in each layer
			fout << config[i].size() << "\n";
		}

		for (int i = 1; i < config.size(); ++i) {	// save weights of connections
			for (int j = 0; j < config[i].size(); ++j) {
				vector<float> weights(config[i][j]->getWeights());
				if (weights.empty()) continue;
				for (float w : weights)
					fout << w << " ";
				fout << "\n";
			}
		}
		fout.close();
		cout << "Saving complited successfully.";
		saved = true;
	}
	else {
		cout << "Saving error. File could not be created/opened. " << endl;
		saved = false;
	}
}

bool NeuralNetwork::is_save() const
{
	return saved;
}

void NeuralNetwork::loadFromFile(const char* path)
{
	using namespace std;
	
	ifstream f;
	f.open(path);
	if (f.is_open()) {
		
		int nLayers;
		f >> nLayers;  // read a num of layers
		
		int* nNeurons = new int[nLayers];	// read a num of neurons in each layer
		for (int i = 0; i < nLayers; ++i)
			f >> nNeurons[i];

		init(nLayers, nNeurons);	// init neural network

		// set weights
		for (int i = 1; i < nLayers; ++i) {
			for (Neuron* neur : config[i]) {
				if (neur->is_first_line()) continue;	// shift neurons don't have input weights
				
				vector<float> weights;
				for (int j = 0; j < config[i - 1].size(); ++j) {	// read weights
					float w;
					f >> w;
					weights.push_back(w);
				}
				neur->setWeights(weights);	
			}
		}

		f.close();
		delete[] nNeurons;
		saved = true;
		cout << "Loading complited successfully." << endl;
	}
	else {
		cout << "File open error" << endl;
		exit(-1);
	}
}

std::vector<float> NeuralNetwork::getAnswer(int n)
{
	using namespace std;
	
	cout << endl;
	cout << "Sample " << n << endl;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			cout << sample[n][j + i * 3] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << "Gained answer: ";

	// load sample to input neurons
	for (int j = 0; j < 9; ++j) {
		config[0][j]->setValue(sample[n][j]);
	}

	// forward signal
	// forward signal
	for (vector<Neuron*> L : config) {
		for (Neuron* neur : L)
			neur->computeValue();
	}

	// answer
	vector<float> answ;
	for (Neuron* neur : config[config.size() - 1]) {
		answ.push_back(neur->getValue());
	}
	return answ;
}

void NeuralNetwork::clear()
{
	if (!config.empty()) {
		for (std::vector<Neuron*> el : config) {
			for (Neuron* neur : el) {
				delete neur;
			}
		}
		config.clear();
	}
}
