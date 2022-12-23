#include <vector>
#include <iostream>
#include <glut.h>
#include <algorithm>
#include <numeric>

#include "NeuralNetwork.h"
#include "SampleCreator.h"


// global variables
NeuralNetwork nn;
SampleCreator sc;
int sizeImgPart;
int winW;
int winH;
int num;
struct line{
	float x1, y1;
	float x2, y2;
	float brightness;
};
std::vector<line> lines;



void display() {

	// image processing
	std::vector<Pixel> part(sc.getRandImagePart());
	std::vector<float> sample;
	sample.reserve(part.size());
	float mid_bright = 0;
	float mid_coord_x = 0;
	float mid_coord_y = 0;
	for (int i = 0; i < part.size(); ++i) {
		sample.push_back(part[i].grey);
		mid_bright += part[i].grey;
		mid_coord_x += part[i].x;
		mid_coord_y += part[i].y;
	}
	mid_bright = 1 - mid_bright / part.size();  // average bright of this part 
	mid_coord_x = mid_coord_x / part.size();
	mid_coord_y = mid_coord_y / part.size();
	std::vector<float> answ(nn.getAnswer(sample));
	
	// line direction
	float angle;
	if (answ[answ.size() - 1] > 0.7) {
		angle = rand() % 180;
	}
	else {
		answ.pop_back();
		int id = std::min_element(answ.begin(), answ.end()) - answ.begin();
		answ[id] = 0;
		float sum = std::accumulate(answ.begin(), answ.end(), 0.0);
		for (int i = 0; i < answ.size(); ++i) {
			if (i == id) continue;
			answ[i] = answ[i] / sum;
		}
		if (answ[0] == 0) {
			angle = 30 * (3 - answ[1] + answ[2]);  // from 60 to 120
		}
		else if (answ[1] == 0) {
			angle = 30 * (5 - answ[2] + answ[0]);  // from 120 to 180
		}
		else {
			angle = 30 * (1 - answ[0] + answ[1]);  // from 0 to 60
		}
	}
	angle = angle * 3.141 / 180;

	// line
	float line_length = mid_bright * sizeImgPart / 2;  // line length is proportional to brightness
	
	// check for cancel line
	bool flag = true;
	lines.push_back({ mid_coord_x - line_length * cos(angle),
						  mid_coord_y - line_length * sin(angle),
						  mid_coord_x + line_length * cos(angle),
						  mid_coord_y + line_length * sin(angle),
						  1 - mid_bright });

	// drawing
	if (lines.size() % 1000 == 0)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, winW, 0, winH, -1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(1);
		

		glBegin(GL_LINES);
		for (line& l : lines)
		{
			glColor3f(l.brightness, l.brightness, l.brightness);
			glVertex2f(l.x1, l.y1);
			glVertex2f(l.x2, l.y2);
		} 
		glEnd();

		glutSwapBuffers();
	}
}

void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1, timer, 0);
}

void keyClick(unsigned char key, int x, int y) {

	using namespace std;

	if (key == '\x1b') {
		cout << "Exit? (1)/(0)" << endl;
		bool flag;
		cin >> flag;
		if (flag) exit(0);
	}
}


int main(int argc, char** argv) {

	using namespace std;
	srand(time(0));

	// neural network init
	//cout << "Input neural network file name: ";
	string path;
	//cin >> path;
	nn.loadFromFile("TreeLogFuncFirstEd.txt");//path.c_str());

	// load image
	cout << "Input image file name: ";
	cin >> path;
	sc.loadImage(path.c_str());

	// set sc settings for correct work (settings are neccessary for getRandImgPart func)
	int inputNeurs = nn.getInConfig();
	int nAnsw = nn.getOutConfig();
	sizeImgPart = trunc(sqrt(inputNeurs));
	sc.setSettings(sizeImgPart, sizeImgPart, nAnsw);	
	
	winW = sc.getImgSize().first;
	winH = sc.getImgSize().second;
	lines.reserve(winW * winH);
	
	// glut init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(winW, winH);
	glutInitWindowPosition(960 - winW / 2, 540 - winH / 2);
	glutCreateWindow("SampleScreator");
	glClearColor(1, 1, 1, 1.0);

	glutKeyboardFunc(keyClick);
	glutDisplayFunc(display);
	timer(0);
	glutMainLoop();
}




