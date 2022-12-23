#include "SampleCreator.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <glut.h>

// help func
std::vector<float> neurWeights(float angle, bool found) {

	std::vector<float> weights;
	if (!found) {
		weights.push_back(0);	// neuron 1
		weights.push_back(0);	// neuron 2
		weights.push_back(0);	// neuron 3
		weights.push_back(1);   // neuron 4	
	}
	else if (angle <= 60) {
		weights.push_back(1 - angle / 60);	// neuron 1
		weights.push_back(1 - (60 - angle) / 60);	// neuron 2
		weights.push_back(0);	// neuron 3
		weights.push_back(0);   // neuron 4	
	}
	else if (angle > 60 && angle <= 120) {
		weights.push_back(0);	// neuron 1
		weights.push_back(1 - (angle - 60) / 60);	// neuron 2
		weights.push_back(1 - (120 - angle) / 60);	// neuron 3
		weights.push_back(0);   // neuron 4	
	}
	else if (angle > 120) {
		weights.push_back(1 - (180 - angle) / 60);	// neuron 1
		weights.push_back(0);	// neuron 2
		weights.push_back(1 - (angle - 120) / 60);	// neuron 3
		weights.push_back(0);   // neuron 4
	}
	return weights;
}


void SampleCreator::setSettings(int sizeX, int sizeY, int nAnsw)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->nAnsw = nAnsw;
}

void SampleCreator::createDataSetSample(float x, float y, bool found)
{
	using namespace std;

	int id = dataSet.size();
	dataSet.push_back(vector<float>());

	// write neccessary pixels
	for (int i = 0; i < sizeY; ++i) {
		for (int j = 0; j < sizeX; ++j)
		{
			dataSet[id].push_back(image[partStartPoint + j + i * img_w].grey);
		}
	}

	// write answer
	x = x / sqrt(x * x + y * y);
	float alpha = acos(x);
	alpha = alpha * 180 / 3.141;
	if (y < 0) alpha = 180 - alpha;

	vector<float> weights(neurWeights(alpha, found));
	if (weights.empty()) {
		cout << "Sample creation error: wrong weights. \nSample creation was aborted." << endl;
		dataSet.pop_back();
		return;
	}

	dataSet[id].push_back(weights[0]);	// neuron 1
	dataSet[id].push_back(weights[1]);	// neuron 2
	dataSet[id].push_back(weights[2]);	// neuron 3
	dataSet[id].push_back(weights[3]);   // neuron 4	

	cout << "Sample " << id << " was created. Angle";
	if (weights[3] == 1) {
		cout << " was not specified." << endl;
	}
	else {
		cout << ": " << alpha << "; Neurons: " << weights[0] << " " << weights[1] << " " << weights[2] << endl;
	}
	 
	if (sizeX == sizeY) {
		cout << "Data augmentaion started..." << endl;
		dataAugmenter(dataSet[id]);
		cout << "Data augmentaion complited." << endl;
	}

	// get next random part;
	getRandomPart();
}

std::vector<std::vector<float>> SampleCreator::getDataSet() const
{
	return this->dataSet;
}

std::vector<Pixel> SampleCreator::getImage() const
{
	return this->image;
}

std::vector<Pixel> SampleCreator::getRandImagePart()
{
	using namespace std;
	getRandomPart();

	vector<Pixel> imgPart;

	// write neccessary pixels
	for (int i = 0; i < sizeY; ++i) {
		for (int j = 0; j < sizeX; ++j)
		{
			imgPart.push_back(image[partStartPoint + j + i * img_w]);
		}
	}

	return imgPart;
}

std::vector<Pixel> SampleCreator::getImagePart(int n)
{
	using namespace std;

	vector<Pixel> imgPart;

	// write neccessary pixels
	for (int i = 0; i < sizeY; ++i) {
		for (int j = 0; j < sizeX; ++j)
		{
			imgPart.push_back(image[n + j + i * img_w]);
		}
	}

	return imgPart;
}

std::pair<int, int> SampleCreator::getImgSize() const
{
	return std::pair<int, int>(img_w, img_h);
}

void SampleCreator::save(const char* path)
{
	using namespace std;

	ofstream fout;
	fout.open(path);

	if (fout.is_open()) {

		fout << dataSet.size() << "\n";  // save number of samples

		fout << sizeX << "\n";  // save size of sample
		fout << sizeY << "\n";
		fout << nAnsw << "\n";

		for (int i = 1; i < dataSet.size(); ++i) {	// save weights of connections
			for (int j = 0; j < dataSet[i].size(); ++j) {
				fout << dataSet[i][j] << " ";
			}
			fout << "\n";
		}

		fout.close();
		cout << "Saving complited successfully." << endl;
	}
	else {
		cout << "Saving error. File could not be created/opened. " << endl;
	}
}

void SampleCreator::loadImage(const char* path)
{
	using namespace std;

	ifstream f;
	f.open(path, ios::in | ios::binary);

	if (!f.is_open()) {
		cout << "File could not be opened." << endl;
		return;
	}

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;

	unsigned char fileHeader[fileHeaderSize];
	f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

	if (fileHeader[0] != 'B' || fileHeader[1] != 'M')
	{
		cout << "The specified path is not a bitmap image." << endl;
		f.close();
		return;
	}

	unsigned char informationHeader[informationHeaderSize];
	f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	int fileSize = fileHeader[2] + (fileHeader[3] << 8) +
		(fileHeader[4] << 16) + (fileHeader[5] << 24);
	int m_width = informationHeader[4] + (informationHeader[5] << 8) +
		(informationHeader[6] << 16) + (informationHeader[7] << 24);
	int m_height = informationHeader[8] + (informationHeader[9] << 8) +
		(informationHeader[10] << 16) + (informationHeader[11] << 24);

	image.resize(m_width * m_height);

	const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);


	for (int y = 0; y < m_height; ++y)
	{
		for (int x = 0; x < m_width; ++x)
		{
			unsigned char colors[3];
			f.read(reinterpret_cast<char*>(colors), 3);
			image[x + y * m_width].grey = (static_cast<float>(colors[2]) * 0.299 +		// red
										    static_cast<float>(colors[1]) * 0.587 +		// green
										    static_cast<float>(colors[0]) * 0.114) /	// blue
											255.0f;  
			int new_x = x - 28;
			if (new_x < 0) new_x += m_width;

			image[x + y * m_width].x = new_x;
			image[x + y * m_width].y = y;
		}
		f.ignore(paddingAmount);
	}

	f.close();
	cout << "The file is successfully read." << endl;

	img_w = m_width;
	img_h = m_height;
}


void SampleCreator::printPart() const
{
	float partSize = 40 / std::max(sizeX, sizeY);
	
	float x = 50 - partSize * (sizeX / 2.0);
	float y = 50 + partSize * (sizeY / 2.0);
	for (int i = 0; i < sizeY; ++i) {
		for (int j = 0; j < sizeX; ++j)
		{
			float grey = image[partStartPoint + j + i * img_w].grey;
			glColor3f(grey, grey, grey);
			glRectf(x + j * partSize, y - i * partSize,
					x + j * partSize + partSize, y - i * partSize - partSize);
		}
	}
}

void SampleCreator::getRandomPart()
{
	int x = rand() % img_w;
	if (x >= img_w - sizeX) x -= sizeX;
	int y = rand() % img_h;
	if (y >= img_h - sizeY) y -= sizeY;
	
	partStartPoint = x + y * img_w;
}

void SampleCreator::shuffleDataSet()
{
	std::random_shuffle(dataSet.begin(), dataSet.end());
	std::cout << "Data set was successfully shuffled." << std::endl;
}

void SampleCreator::dataAugmenter(const std::vector<float>& sample)
{
	using namespace std;

	// inverse
	vector<float> inv1(inverseSampleM(sample));  // 1
	addSample(inv1);

	// rotation
	vector<float> rot1(rotateSampleM(inv1));  // 2
	addSample(rot1);

	// inverse
	vector<float> inv2(inverseSampleM(rot1));  // 3
	addSample(inv2);
	
	// rotation
	vector<float> rot2(rotateSampleM(inv2));  // 4
	addSample(rot2);

	// inverse
	vector<float> inv3(inverseSampleM(rot2));  // 5
	addSample(inv3);

	// rotation
	vector<float> rot3(rotateSampleM(inv3));  // 6
	addSample(rot3);

	// inverse
	vector<float> inv4(inverseSampleM(rot3));  // 7
	addSample(inv4);
	
	// reflection
	vector<float> ref1(reflectSampleM(inv4));  // 8
	addSample(ref1);

	// inverse
	vector<float> inv11(inverseSampleM(ref1));  // 9
	addSample(inv11);

	// rotation
	vector<float> rot11(rotateSampleM(ref1));  // 10
	addSample(rot11);

	// inverse
	vector<float> inv21(inverseSampleM(rot11));  // 11
	addSample(inv21);

	// rotation
	vector<float> rot21(rotateSampleM(inv21));  // 12
	addSample(rot21);

	// inverse
	vector<float> inv31(inverseSampleM(rot21));  // 13
	addSample(inv31);

	// rotation
	vector<float> rot31(rotateSampleM(inv31));  // 14
	addSample(rot31);

	// inverse
	vector<float> inv41(inverseSampleM(rot31));  // 15
	addSample(inv41);
}

void SampleCreator::addSample(const std::vector<float>& sample)
{
	using namespace std;

	int id = dataSet.size();
	dataSet.push_back(vector<float>());

	for (int i = 0; i < sizeX * sizeY + nAnsw; ++i) {
		{
			dataSet[id].push_back(sample[i]);
		}
	}
}

std::vector<float> SampleCreator::rotateSampleM(const std::vector<float>& sample) {

	using namespace std;

	vector<float> rotated;

	// rotate px matrix
	for (int i = sizeX - 1; i > -1; --i) {
		for (int j = 0; j < sizeX; ++j) {
			rotated.push_back(sample[i + j * sizeX]);
		}
	}
	
	// compute direction and rotate it
	float alpha = 0;
	
	if (sample[sizeX * sizeY + nAnsw - 1] != 1) {
		if (sample[sizeX * sizeY] == 0) {
			alpha = 30 * (3 - sample[sizeX * sizeY + 1] + sample[sizeX * sizeY + 2]); // old angle
		}
		else if (sample[sizeX * sizeY + 1] == 0) {
			alpha = 30 * (5 - sample[sizeX * sizeY + 2] + sample[sizeX * sizeY]); // old angle
		}
		else {
			alpha = 30 * (1 - sample[sizeX * sizeY] + sample[sizeX * sizeY + 1]);
		}
		alpha += 90; //rotation
		if (alpha > 180) alpha -= 180;  // keep angle in the ring at 180;
	}

	vector<float> weights(neurWeights(alpha, true));
	rotated.push_back(weights[0]);
	rotated.push_back(weights[1]);
	rotated.push_back(weights[2]);
	rotated.push_back(weights[3]);

	cout << "Rotated sample: new angle = " << alpha << endl;

	return rotated;
}

std::vector<float> SampleCreator::inverseSampleM(const std::vector<float>& sample)
{
	std::vector<float> inv;
	// inverse px
	for (int i = 0; i < sizeX * sizeY; ++i) {
		inv.push_back(1 - sample[i]);
	}
	// save answ
	for (int i = sizeX * sizeY; i < sizeX * sizeY + nAnsw; ++i) {
		inv.push_back(sample[i]);
	}
	std::cout << "Inversed sample : same angle." << std::endl;
	return inv;
}

std::vector<float> SampleCreator::reflectSampleM(const std::vector<float>& sample)
{
	using namespace std;

	vector<float> reflected;

	// reflect px matrix
	for (int i = 0; i < sizeY; ++i) {
		for (int j = sizeX - 1; j > -1; --j) {
			reflected.push_back(sample[j + i * sizeX]);
		}
	}

	// compute direction and reflect it
	float alpha = 0;

	if (sample[sizeX * sizeY + nAnsw - 1] != 1) {
		if (sample[sizeX * sizeY] == 0) {
			alpha = 30 * (3 - sample[sizeX * sizeY + 1] + sample[sizeX * sizeY + 2]); // old angle
		}
		else if (sample[sizeX * sizeY + 1] == 0) {
			alpha = 30 * (5 - sample[sizeX * sizeY + 2] + sample[sizeX * sizeY]); // old angle
		}
		else {
			alpha = 30 * (1 - sample[sizeX * sizeY] + sample[sizeX * sizeY + 1]);
		}
		alpha = 180 - alpha; //reflect
	}

	vector<float> weights(neurWeights(alpha, true));
	reflected.push_back(weights[0]);
	reflected.push_back(weights[1]);
	reflected.push_back(weights[2]);
	reflected.push_back(weights[3]);

	cout << "Reflected sample: new angle = " << alpha << endl;

	return reflected;
}
