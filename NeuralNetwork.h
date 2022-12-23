#pragma once

#include <vector>
#include "Neuron.h"

class NeuralNetwork
{

	std::vector<std::vector<Neuron*>> config;

	std::vector<std::vector<float>> sample;

	bool saved;

	int numAnswers;

public:

	~NeuralNetwork();
	
	void init(int nLayers, int* nNeurons);  // nLayers - a number of neurons layers, nNeurons - numbers of neurons in each layer in order from 0 to nLayers-1

	void loadSample(const char* path);

	void learn(float k, float err);

	void save(const char* path);

	bool is_save() const;

	void loadFromFile(const char* path);

	std::vector<float> getAnswer(int n);

	std::vector<float> getAnswer(std::vector<float> sample);

	void setSampleSet(const std::vector<std::vector<float>>& sample);

	int getInConfig() const;

	int getOutConfig() const;

private:

	void clear(); 
};

