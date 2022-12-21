
#include <glut.h>
#include <iostream>
#include <Windows.h>
#include "SampleCreator.h"


SampleCreator sc;
float cx, cy;  // current mouse pos
float lx, ly;

void display() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 100, 0, 100, -1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	sc.printPart();
	
	if (lx > 0 && ly > 0) {  // draw line for eye monitoring
		glLineWidth(3);
		glColor3f(1, 0, 0);
		glBegin(GL_LINES);

		glVertex2f(lx, ly);
		glVertex2f(cx, cy);

		glEnd();
	}

	glutSwapBuffers();
}

void timer(int) { 
	glutPostRedisplay();
	glutTimerFunc(20, timer, 0);
}


void mouseClick(int button, int state, int x, int y) {
	
	x = x / 504.0 * 100;
	y = y / 504.0 * 100;
	if (lx < 0 && ly < 0)
	{
		lx = x;
		ly = 100 - y;
		cx = lx;
		cy = ly;
	}
	else {
		float _x = x - lx;
		float _y = (100 - y) - ly;
		sc.createDataSetSample(_x, _y, true);
		lx = -1;
		ly = -1;
	}
}

void mouseMove(int _x, int _y) {
	cx = _x / 504.0 * 100;
	cy = 100 - _y / 504.0 * 100;
}

void keyClick(unsigned char key, int x, int y) {
	
	using namespace std;
	
	if (key == '\x1b') {
		cout << "Save this data set? (1)/(0)" << endl;
		bool flag;
		cin >> flag;
		if (flag) {
			cout << "Shuffle data set? (1)/(0)";
			cin >> flag;
			if (flag) sc.shuffleDataSet();

			cout << "Saving. \nInput file name: ";
			string path;
			cin >> path;
			sc.save(path.c_str());
		}
		cout << "Exit? (1)/(0)" << endl;
		cin >> flag;
		if (flag) exit(0);
	}

	if (key == ' ') {
		sc.createDataSetSample(0, 0, false);
		lx = -1;
		ly = -1;
	}
	if (key == 'w') {
		sc.getRandomPart();
		lx = -1;
		ly = -1;
	}
}


int main(int argc, char** argv) {

	using namespace std;
	srand(time(0));

	cout << "Input source image file name: ";
	std::string path;
	cin >> path;
	sc.loadImage(path.c_str());

	cout << "Input settings ((int) sizeX, sizeY, num of Answers): ";
	int x, y, n;
	cin >> x >> y >> n;
	sc.setSettings(x, y, n);
	sc.getRandomPart();

	lx = -1; ly = -1;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(504, 504);
	glutInitWindowPosition(960 - 504 / 2, 540 - 504 / 2);
	glutCreateWindow("SampleScreator");
	glClearColor(0, 0.5, 0.5, 1.0);

	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyClick);
	glutDisplayFunc(display);
	timer(0);
	glutMainLoop();
}