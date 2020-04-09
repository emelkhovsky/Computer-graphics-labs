#include <QtOpenGL/QGLWidget>
#include "View.h"


View::View(){
}


View::~View(){
}

template <class T>//����� �� ����� �� ������� 255
T clamp(T v, int max, int min) {
	if (v > max) {
		return max;
	}
	if (v < min) {
		return min;
	}
	return v;
}

GLint* TransferFunction(short value) {//��������� �������� ��������� ���������� � �����-����� ����
	int min = 0;
	int max = 200;
	int newVal = clamp<int>((value - min) * 255 / (max - min), 0, 255);
	GLint *v = new GLint[3];
	v[0] = newVal;
	v[1] = newVal;
	v[2] = newVal;
	return v;
}

void View::initializeGL() {//�������������
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//������� ������ �����������
}

void View::resizeGL(int width, int height) {//���� ������
	glShadeModel(GL_SMOOTH);//� ���������� ������ ��� ���
	glMatrixMode(GL_PROJECTION);//����� ������� ��������
	glLoadIdentity();//���������������� ������� � �������� �� ��������� �� ���������(���������???)
	glOrtho(0, test.X, 0, test.Y, -1, 1);//��������� ��������� ��������������� ������� ���������(���� �������� ��������)
	glViewport(0, 0, width, height);//������ �������������, ����� ������� �� ����� ��� ��������
}

void View::DrawQuads(int layerNumber) {//��������� ����������������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������� ����� ����� � ����� �������
	glBegin(GL_QUADS);//���������, ��� ����� �������� �������������
	for (int x_coord = 0; x_coord < test.X - 1; x_coord++) {
		for (int y_coord = 0; y_coord < test.Y - 1; y_coord++) {
			short value;
			//1 �������
			value = test.array[x_coord + y_coord * test.X + layerNumber * test.X * test.Y];//������� ��������
			glColor3i(TransferFunction(value)[0] / 255.0f, TransferFunction(value)[1] / 255.0f, TransferFunction(value)[2] / 255.0f);//������ ����
			glVertex2i(x_coord, y_coord);//��������� �������
			//2 �������
			value = test.array[x_coord + (y_coord + 1) * test.X + layerNumber * test.X * test.Y];
			glColor3i(TransferFunction(value)[0] / 255.0f, TransferFunction(value)[1] / 255.0f, TransferFunction(value)[2] / 255.0f);
			glVertex2i(x_coord, (y_coord + 1));
			//3 �������
			value = test.array[(x_coord + 1) + (y_coord + 1) * test.X + layerNumber * test.X * test.Y];
			glColor3i(TransferFunction(value)[0] / 255.0f, TransferFunction(value)[1] / 255.0f, TransferFunction(value)[2] / 255.0f);
			glVertex2i((x_coord + 1), (y_coord + 1));
			//4 �������
			value = test.array[(x_coord + 1) + y_coord * test.X + layerNumber * test.X * test.Y];
			glColor3i(TransferFunction(value)[0] / 255.0f, TransferFunction(value)[1] / 255.0f, TransferFunction(value)[2] / 255.0f);
			glVertex2i((x_coord + 1), y_coord);
		}
	}
	glEnd();//��������� ��������
}

void View::paintGL() {//��������� ����������������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������� ����� ����� � ����� �������
	DrawQuads(layerNumber);
	//swapBuffers();
}

