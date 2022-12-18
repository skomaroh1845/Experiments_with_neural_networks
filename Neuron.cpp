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
		for (Neuron* el : inputs) {
			this->inputs.insert({ el, (rand() % 100 - 50) / 1000.0 });
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

void Neuron::learn(float k)
{
	if (first_line) return;
	for (auto& el : this->inputs) {
		el.second += k * this->error * el.first->getValue() * this->value * (1 - this->value);
				// learning koef * error * input value * diff 
	}
	this->error = 0;
}

