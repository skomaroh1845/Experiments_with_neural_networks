#pragma once

#include <vector>


class SampleCreator
{
	struct Pixel {
		float x, y;
		float grey;
	};
	
	int img_w;
	int img_h;

	std::vector<std::vector<float>> dataSet;

	std::vector<Pixel> image;

	bool created;

	int sizeX, sizeY;
	int nAnsw;

	int partStartPoint;

public:
	
	void setSettings(int sizeX, int sizeY, int nAnsw);	// sizeX * sizeY = amount of input neurons, nAnsw - amount of output neurons

	void createDataSetSample(float x, float y, bool found);  

	std::vector<std::vector<float>>& getDataSet();

	void save(const char* path);

	void loadImage(const char* path);

	std::pair<int, int> getImgSize() const;

	bool is_created() const;

	void printPart() const;

	void getRandomPart();

};

