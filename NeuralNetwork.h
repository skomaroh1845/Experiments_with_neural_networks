#pragma once

#include <vector>
#include "Neuron.h"

class NeuralNetwork
{

	std::vector<std::vector<Neuron*>> config;

	std::vector<std::vector<float>> sample;

public:

	~NeuralNetwork();
	
	void init(int nLayers, int* nNeurons);  // nLayers - a number of neurons layers, nNeurons - numbers of neurons in each layer in order from 0 to nLayers-1

	void loadSample(const char* path);

	void learn(float k, float err);

	void save(const char* path);

	void loadFromFile(const char* path);

	std::vector<float> getAnswer(int n);

private:

	void clear(); 
};

