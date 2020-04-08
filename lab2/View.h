#include "Bin.h"

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
	int max = 2000;
	int newVal = clamp<int>((value - min) * 255 / (max - min), 0, 255);
	GLint *v = new GLint[3];
	v[0] = newVal;
	v[1] = newVal;
	v[2] = newVal;
	return v;
}


class View : public QGLWidget {
public:
	View();
	~View();

	void SetupView(int width, int height) {//���� ������
		glShadeModel(GL_SMOOTH);//� ���������� ������ ��� ���
		glMatrixMode(GL_PROJECTION);//����� ������� ��������
		glLoadIdentity();//���������������� ������� � �������� �� ��������� �� ���������(���������???)
		glOrtho(0, Bin.X, 0, Bin.Y, -1, 1);//��������� ��������� ��������������� ������� ���������(���� �������� ��������)
		glViewport(0, 0, width, height);//������ �������������, ����� ������� �� ����� ��� ��������
	}

	void DrawQuads(int layerNumber) {//��������� ����������������
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������� ����� ����� � ����� �������
		glBegin(GL_QUADS);//���������, ��� ����� �������� �������������
		for (int x_coord = 0; x_coord < Bin.X - 1; x_coord++) {
			for (int y_coord = 0; y_coord < Bin.Y - 1; y_coord++) {
				short value;
				//1 �������
				value = Bin.array[x_coord + y_coord * Bin.X + layerNumber * Bin.X * Bin.Y];//������� ��������
				glColor3i(TransferFunction(value)[0], TransferFunction(value)[1], TransferFunction(value)[2]);//������ ����
				glVertex2i(x_coord, y_coord);//��������� �������
				//2 �������
				value = Bin.array[x_coord + (y_coord + 1) * Bin.X + layerNumber * Bin.X * Bin.Y];
				glColor3i(TransferFunction(value)[0], TransferFunction(value)[1], TransferFunction(value)[2]);
				glVertex2i(x_coord, (y_coord + 1));
				//3 �������
				value = Bin.array[(x_coord + 1) + (y_coord + 1) * Bin.X + layerNumber * Bin.X * Bin.Y];
				glColor3i(TransferFunction(value)[0], TransferFunction(value)[1], TransferFunction(value)[2]);
				glVertex2i((x_coord + 1), (y_coord + 1));
				//4 �������
				value = Bin.array[(x_coord + 1) + y_coord * Bin.X + layerNumber * Bin.X * Bin.Y];
				glColor3i(TransferFunction(value)[0], TransferFunction(value)[1], TransferFunction(value)[2]);
				glVertex2i((x_coord + 1), y_coord);
			}
		}
		glEnd();//��������� ��������
	}
};
