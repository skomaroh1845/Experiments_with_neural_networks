(ATTENTION! It is not a fully finished project, so there are some crutches and hardcoded constructions used only for the example at the end!)

Here is my self-made neural network library. I've done it just for fun because I enjoy coding something interesting things.
It is my first such experience so this library does not pretend on full correctness or fast working.

Here are the features I put into NeuralNetwork.h and SampleCreator.h:
* create networks with any amount of layers and any number of neurons in them (but 2 is a minimun amount in each internal layer, 
because my library at the current moment converts last neuron in layer into a shift neuron, so if you set only one neuron, 
there will just shift neuron in the layer and such network won't work)
* learn and relearn networks created by my library
* save created or relearned networks in a .txt file
* load done networks from files created by my library
* create simple data sets


#### Below is a diary of my experience
#### Okay, now let's talk about hard reality) 
After testing the pilot code I decided to create a neural network that redraws images via lines (I saw it on youtube). So construction is the following: 49 input neurons, 30 internal, and 4 output. The input neurons get a 7x7 piece of a picture and the 4 output neurons must indicate the line direction of this image element. But there are some difficulties with output neurons because the first 3 are used to calculate the angle and the last one is for criteria of existing the direction. After all, if these 49 pixels have the same color or don't have distinguished lines there is no direction (such a scheme with 4 output neurons was taken from youtube, but unfortunately I didn't achieve special success with teaching this "artificially intelligent" (look below)). 

I started by creating SampleCreator.h to be able to make big (several thousand samples) data set. I annotated data by my hand, but only around 500 samples, other samples were generated automatically (I coded a primitive function that augments data by making extra 15 samples from one via turning, reversing pixels, and reflecting by vertical axis). 
Than the data set was created I learn this network, but not almost successfully - overall absolute error reduce only twice after 50 thousand learning loops. Because my roughly estimated of overall error was not much lower I decided to finish learning.
Next, I did a mainPA - program interpreter for this neural network. It was made in haste so it has some crutches and I decided to not bring it to a happy end because optimization and correction of its working require refactoring the architecture of all this "library" and it is irrational by time (I just continue my experiments in python there are a lot of specialized packages for ML). It was my first neural network experience and even though this program doesn't work as well as wanted I'm glad that it worked at least somehow.

I think the main reasons for my failure are the following: a not quality learning data set, unoptimal network architecture, and not suitable activation function into neurons.

There are some examples of work of my masterpiece (the result of the program work on the left and the original image on the right): 

![image_2022-12-24_00-18-18](https://user-images.githubusercontent.com/61201241/209396852-40543f63-89bc-4718-a370-3322bc6a850c.png)


![image_2022-12-24_00-19-22](https://user-images.githubusercontent.com/61201241/209396861-b07de1b6-b9eb-48e5-94f7-531e82b3762c.png)


![image_2022-12-24_00-20-28](https://user-images.githubusercontent.com/61201241/209396878-06726b41-0453-4613-88e6-736efee454bc.png)


![image_2022-12-24_00-21-32](https://user-images.githubusercontent.com/61201241/209396870-74d33837-329c-42a0-bfe2-4e33aa873cb0.png)
