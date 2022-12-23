#pragma once

#include <vector>


struct Pixel {
		float x, y;
		float grey;
	};

class SampleCreator
{	
	int img_w;
	int img_h;

	std::vector<std::vector<float>> dataSet;

	std::vector<Pixel> image;

	int sizeX, sizeY;
	int nAnsw;

	int partStartPoint;

public:
	
	void setSettings(int sizeX, int sizeY, int nAnsw);	// sizeX * sizeY = amount of input neurons, nAnsw - amount of output neurons

	void createDataSetSample(float x, float y, bool found);  

	std::vector<std::vector<float>> getDataSet() const;

	std::vector<Pixel> getImage() const;

	std::vector<Pixel> getRandImagePart();

	std::vector<Pixel> getImagePart(int n);

	std::pair<int, int> getImgSize() const;

	void save(const char* path);

	void loadImage(const char* path);

	void printPart() const;

	void getRandomPart();

	void shuffleDataSet();

private:

	void dataAugmenter(const std::vector<float>& sample);  // makes extra samples from existing one

	void addSample(const std::vector<float>& sample);

	std::vector<float> rotateSampleM(const std::vector<float>& sample);  // rotate samples 'matrix' on 90 degrees; !only for square samples (sizeX = sizeY)! 

	std::vector<float> inverseSampleM(const std::vector<float>& sample);  // inverse samples grey channel 

	std::vector<float> reflectSampleM(const std::vector<float>& sample);  // reflect samples 'matrix'

};

