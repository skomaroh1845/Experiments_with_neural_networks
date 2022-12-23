#include "NeuralNetwork.h"

#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>


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

		int nSamples;
		f >> nSamples;
		int sizeX, sizeY;
		f >> sizeX;
		f >> sizeY;
		f >> numAnswers;

		for (int i = 0; i < nSamples; ++i) {
			sample.push_back(vector<float>());
			sample[i].resize(sizeX * sizeY + numAnswers);
		}

		for (int i = 0; i < nSamples; ++i) {
			if (f.eof()) break;

			for (int j = 0; j < sizeX * sizeY + numAnswers; ++j)
			{
				f >> sample[i][j];
			}
		}
		f.close();
		cout << "Data set loaded successfully. It has " << nSamples << " samples." << endl;
		cout << "Recomended min error value for learning: " << nSamples * 0.1 * numAnswers << endl;
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
	
	if (config[0].size() != sample[0].size() - numAnswers) {
		cout << "Wrong data set! Amount of inputs neurons != sample size." << endl;
		exit(-1);
	}

	while (error > err)
	{
		error = 0;
		++iter;
		if (GetAsyncKeyState(' ')) {
			cout << "Learning was paused. Complite or continue? (1)/(0)." << endl;
			bool exit_flag;
			cin >> exit_flag;
			if (exit_flag)
			{
				cout << "Learning complited." << endl;
				return;
			}
		}

		for (int i = 0; i < sample.size(); ++i)
		{
			// load sample to input neurons
			
			for (int j = 0; j < config[0].size(); ++j) {
				config[0][j]->setValue(sample[i][j]);
			}

			// forward signal
			for (vector<Neuron*> L : config) {
				for (Neuron* neur : L)
					neur->computeValue();
			}

			// error processing
			
			//--- only for this data set ----
			if (sample[i][sample[i].size() - 1] == 1) {
				config[config.size() - 1][0]->setError(0);  // skip errors if last answer = 1
				config[config.size() - 1][1]->setError(0);
				config[config.size() - 1][2]->setError(0);
				float err = sample[i][sample[i].size() - 1] - config[config.size() - 1][3]->getValue();
				error += abs(err);
				config[config.size() - 1][3]->setError(err);
			}
			//-------------------------------
			else {
				for (int j = 0; j < numAnswers; ++j) {
					float err = sample[i][sample[i].size() - numAnswers + j] - config[config.size() - 1][j]->getValue();
					error += abs(err);
					config[config.size() - 1][j]->setError(err);
				}
			}

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

	// load sample to input neurons
	for (int j = 0; j < config[0].size(); ++j) {
		config[0][j]->setValue(sample[n][j]);
	}

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

std::vector<float> NeuralNetwork::getAnswer(std::vector<float> sample)
{
	using namespace std;

	// load sample to input neurons
	for (int j = 0; j < config[0].size(); ++j) {
		config[0][j]->setValue(sample[j]);
	}

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

void NeuralNetwork::setSampleSet(const std::vector<std::vector<float>>& sample)
{
	using namespace std;

	this->sample.clear();
	
	for (int i = 0; i < sample.size(); ++i) {
		this->sample.push_back(vector<float>());
		for (int j = 0; j < sample[i].size(); ++j) {
			this->sample[i].push_back(sample[i][j]);
		}
	}
}

int NeuralNetwork::getInConfig() const
{
	return config[0].size();
}

int NeuralNetwork::getOutConfig() const
{
	return numAnswers;
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
