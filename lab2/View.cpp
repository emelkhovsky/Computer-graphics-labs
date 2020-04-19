#include <QtOpenGL/QGLWidget>
#include "View.h"


View::View(){
}


View::~View(){
}

template <class T>//����� �� ����� �� ������� 1
T clamp(T v, int max, int min) {
	if (v > max) {
		return max;
	}
	if (v < min) {
		return min;
	}
	return v;
}

float TransferFunction(short value) {//��������� �������� ��������� ���������� � �����-����� ����
	int min = 0;
	int max = 2000;
	int newVal = clamp<int>((value - min) * 255 / (max - min), 0, 255);
	return clamp<float>(newVal / 200.f, 1, 0);//�������� � ��������� �� 0 �� 1, ������ ��� ???????????
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
			float transfer;
			//1 �������
			value = test.array[x_coord + y_coord * test.X + layerNumber * test.X * test.Y];//������� ��������
			transfer = TransferFunction(value);
			glColor3f(transfer, transfer, transfer);//������ ����
			glVertex2i(x_coord, y_coord);//��������� �������
			//2 �������
			value = test.array[x_coord + (y_coord + 1) * test.X + layerNumber * test.X * test.Y];
			transfer = TransferFunction(value);
			glColor3f(transfer, transfer, transfer);
			glVertex2i(x_coord, (y_coord + 1));
			//3 �������
			value = test.array[(x_coord + 1) + (y_coord + 1) * test.X + layerNumber * test.X * test.Y];
			transfer = TransferFunction(value);
			glColor3f(transfer, transfer, transfer);
			glVertex2i((x_coord + 1), (y_coord + 1));
			//4 �������
			value = test.array[(x_coord + 1) + y_coord * test.X + layerNumber * test.X * test.Y];
			transfer = TransferFunction(value);
			glColor3f(transfer, transfer, transfer);
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

