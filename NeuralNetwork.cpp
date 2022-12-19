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

	for (int i = 0; i < nLayers; ++i) {
		
		config.push_back(vector<Neuron*>());
		
		if (i == 0) {
			for (int j = 0; j < nNeurons[i]; ++j) {
				config[i].push_back(new Neuron());
			}
		}
		else {
			for (int j = 0; j < nNeurons[i]; ++j) {
				config[i].push_back(new Neuron( config[i-1] ));
			}
		}

		if (i < nLayers - 1)
		{
			config[i].push_back(new Neuron());	// shift neuron
			config[i][nNeurons[i]]->setValue(1);
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

void NeuralNetwork::loadFromFile(const char* path)
{
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
