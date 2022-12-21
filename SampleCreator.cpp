#include "SampleCreator.h"

#include <iostream>
#include <fstream>
#include <glut.h>



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
	

	// get next random part;
	getRandomPart();

	// write answer
	x = x / sqrt(x * x + y * y);
	float alpha = acos(x);
	alpha = alpha * 180 / 3.141;
	if (y < 0) alpha = 180 - alpha;

	if (!found) {
		dataSet[id].push_back(0);	// neuron 1

		dataSet[id].push_back(0);	// neuron 2

		dataSet[id].push_back(0);	// neuron 3

		dataSet[id].push_back(1);   // neuron 4	

		cout << "Sample " << id << " was created. Angle not found." << endl;
		return;
	}
	
	if (alpha <= 60) {

		dataSet[id].push_back(1 - alpha / 60);	// neuron 1
		
		dataSet[id].push_back(1 - (60-alpha) / 60);	// neuron 2
		
		dataSet[id].push_back(0);	// neuron 3

		dataSet[id].push_back(0);   // neuron 4	

		cout << "Sample " << id << " was created. Angle: " << alpha << "; Neurons: " << 1 - alpha / 60 << " " 
			 << 1 - (60 - alpha) / 60 << " 0" << endl;
		return;
	}
	if (alpha > 60 && alpha <= 120) {
		
		dataSet[id].push_back(0);	// neuron 1

		dataSet[id].push_back(1 - (alpha - 60) / 60);	// neuron 2

		dataSet[id].push_back(1 - (120 - alpha) / 60);	// neuron 3

		dataSet[id].push_back(0);   // neuron 4	

		cout << "Sample " << id << " was created. Angle: " << alpha << "; Neurons: " << 0 << " "
			<< 1 - (alpha - 60) / 60 << " " << 1 - (120 - alpha) / 60 << endl;
		return;
	}
	if (alpha > 120) {
		
		dataSet[id].push_back(1 - (180 - alpha) / 60);	// neuron 1

		dataSet[id].push_back(0);	// neuron 2

		dataSet[id].push_back(1 - (alpha - 120) / 60);	// neuron 3

		dataSet[id].push_back(0);   // neuron 4	

		cout << "Sample " << id << " was created. Angle: " << alpha << "; Neurons: " << 1 - (180 - alpha) / 60 << " "
			<< 0 << " " << 1 - (alpha - 120) / 60 << endl;
		return;
	}

		
}

std::vector<std::vector<float>>& SampleCreator::getDataSet()
{
	return this->dataSet;
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

std::pair<int, int> SampleCreator::getImgSize() const
{
	return std::pair<int, int>(img_w, img_h);
}

bool SampleCreator::is_created() const
{
	return created;
}

void SampleCreator::printPart() const
{
	float x = 50 - 5 * (sizeX / 2.0);
	float y = 50 + 5 * (sizeY / 2.0);
	for (int i = 0; i < sizeY; ++i) {
		for (int j = 0; j < sizeX; ++j)
		{
			float grey = image[partStartPoint + j + i * img_w].grey;
			glColor3f(grey, grey, grey);
			glRectf(x + j * 5, y - i * 5, x + j * 5 + 5, y - i * 5 - 5);
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
