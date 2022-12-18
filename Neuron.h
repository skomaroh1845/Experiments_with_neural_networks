#pragma once


#include <vector>
#include <map>

class Neuron
{
	std::map<Neuron*, float> inputs;  // neurons and weights of their connections

	float value;
	float error;

	bool first_line;

public:

	Neuron();
	
	Neuron(const std::vector<Neuron*>& inputs, bool first_line = false);

	float getValue() const;

	void setValue(float value);

	void computeValue();  

	void computeError();	// calculate part of error for input neurons

	void setError(float err);
	
	float getError() const;
	
	void learn(float k);  // k - learning coefficient 

};

