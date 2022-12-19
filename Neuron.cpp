#include "Neuron.h"

#include <algorithm>
#include <iostream>

Neuron::Neuron() : Neuron( std::vector<Neuron*>(1, nullptr), true )
{
}

Neuron::Neuron(const std::vector<Neuron*>& inputs, bool first_line) : value(0), error(0), first_line(first_line)
{
	using namespace std;

	srand(time(0));
	if (!first_line)
		for (int i = 0; i < inputs.size(); ++i) {
			this->inputs.push_back({ inputs[i], (rand() % 100 - 50) / 1000.0});
		}
}

float Neuron::getValue() const
{
	return value;
}

void Neuron::setValue(float value)
{
	this->value = value;
}

void Neuron::computeValue()
{
	if (first_line) return;
	float sum = 0;
	for (const auto& el : this->inputs) {
		sum += el.second * el.first->getValue();
	}
	this->value = 1 / ( 1 + exp( -1 * sum ) );
}

void Neuron::computeError()
{
	if (first_line) return;
	for (auto& el : this->inputs) {
		el.first->setError( el.first->getError() + this->error * el.second );
	}
}

void Neuron::setError(float err)
{	
	this->error = err;
}

float Neuron::getError() const
{
	return this->error;
}

bool Neuron::is_first_line() const
{
	return first_line;
}

std::vector<float> Neuron::getWeights() const
{
	using namespace std;
	
	vector<float> weights;

	if (this->first_line) return weights;
	
	for (int i = 0; i < inputs.size(); ++i) {
		weights.push_back(inputs[i].second);
	}
		
	return weights;
}

void Neuron::setWeights(const std::vector<float>& weights)
{
	if (this->first_line) return;
	
	for (int i = 0; i < inputs.size(); ++i) {
		inputs[i].second = weights[i];
	}
}

void Neuron::learn(float k)
{
	if (first_line) return;
	for (auto& el : this->inputs) {
		el.second += k * this->error * el.first->getValue() * this->value * (1 - this->value);
				// learning koef * error * input value * diff 
	}
	this->error = 0;
}

