Here is my self-made neural network library. I've done it just for fun because I enjoy coding something interesting things.
It is my first such experience so this library does not pretend on full correctness or super fast working. But in fact, this heap of virtual neurons works!

Here are some things which you can do via NeuralNetwork.h:
* create networks with any amount of layers and any number of neurons in them (but 2 is a minimun amount in each internal layer, 
because my library at the current moment converts last neuron in layer into a shift neuron, so if you set only one neuron, 
there will just shift neuron in the layer and such network won't work)
* learn and relearn networks created by my library
* save created or relearned networks in a .txt file
* load done networks from files created by my library
