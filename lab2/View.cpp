#include <QtOpenGL/QGLWidget>
#include "View.h"

View::View() {
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

void View::input_layer(int value) {//�������, ������ ��� ����� ����� � �������
	if (value >= 2) {
		layerNumber = value;
	}
	else {
		layerNumber = 1;
	}

	updateGL();//� ����� �-��, ��� ���-�� ����������������, ������� ��� �-��
	if (mode == 2) {
		glEnable(GL_TEXTURE_2D);//�������� �����-�� ����������� gl
		genTextureImage(layerNumber);
		Load2dTexture();
		updateGL();
	}
	else {
		glDisable(GL_TEXTURE_2D);//��������� �����-�� ����������� gl
	}
}

void View::setmode(int value) {
	mode = value;
	if (mode == 2) {
		glEnable(GL_TEXTURE_2D);//�������� �����-�� ����������� gl
		genTextureImage(layerNumber);
		Load2dTexture();
		updateGL();
	}
	else {
		glDisable(GL_TEXTURE_2D);//��������� �����-�� ����������� gl
	}
	paintGL();
}

int TransferFunction(short value, int imin, int imax) {//��������� �������� ��������� ���������� � �����-����� ����
	int min = imin;
	int max = imax;
	int newVal = (value - min) * 255 / (max - min);
	return newVal;
}

void View::initializeGL() {//�������������
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//������� ������ �����������
	glGenTextures(1, &VBOtexture);//���������� ���������� ����� ��������, ������� ������������, ��� ����� ��� ������, ��������� � ���������
}

void View::resizeGL(int width, int height) {//���� ������
	glShadeModel(GL_SMOOTH);//� ���������� ������ ��� ���
	glMatrixMode(GL_PROJECTION);//����� ������� ��������
	glLoadIdentity();//���������������� ������� � �������� �� ��������� �� ���������(���������???)
	if (slaider_axis == "Z") {
		first = test.X;
		second = test.Y;
	}
	else if (slaider_axis == "X") {
		first = test.Y;
		second = test.Z;
	}
	else {
		first = test.X;
		second = test.Z;
	}
	cout << first << " " << second << endl;
	glOrtho(0, first, 0, second, -1, 1);//��������� ��������� ��������������� ������� ���������(���� �������� ��������)
	glViewport(0, 0, width, height);//������ �������������, ����� ������� �� ����� ��� ��������
}

void View::paintGL() {//��������� ����������������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������� ����� ����� � ����� �������
	switch (mode) {
	case 0://��������� ������������������
		DrawQuads(layerNumber);
		break;
	case 1://����� 4 ����� ��������������, � ����� ��������� �� 2
		DrawQuadStrip();
		break;
	case 2:
		VizualizationTexture();//� ����������
		break;
	}
}

void View::DrawQuads(int layerNumber) {//��������� ����������������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//������� ����� ����� � ����� �������
	glBegin(GL_QUADS);//���������, ��� ����� �������� �������������
	for (int x_coord = 0; x_coord < first - 1; x_coord++) {
		for (int y_coord = 0; y_coord < second - 1; y_coord++) {
			short value;
			int transfer;
			//1 �������
			value = test.array[x_coord + y_coord * first + layerNumber * first * second];//������� ��������
			transfer = TransferFunction(value, min, max);
			glColor3f(transfer, transfer, transfer);//������ ����
			glVertex2i(x_coord, y_coord);//��������� �������
			//2 �������
			value = test.array[x_coord + (y_coord + 1) * first + layerNumber * first * second];
			transfer = TransferFunction(value, min, max);
			glColor3f(transfer, transfer, transfer);
			glVertex2i(x_coord, (y_coord + 1));
			//3 �������
			value = test.array[(x_coord + 1) + (y_coord + 1) * first + layerNumber * first * second];
			transfer = TransferFunction(value, min, max);
			glColor3f(transfer, transfer, transfer);
			glVertex2i((x_coord + 1), (y_coord + 1));
			//4 �������
			value = test.array[(x_coord + 1) + y_coord * first + layerNumber * first * second];
			transfer = TransferFunction(value, min, max);
			glColor3f(transfer, transfer, transfer);
			glVertex2i((x_coord + 1), y_coord);
		}
	}
	glEnd();//��������� ��������
}



void View::Load2dTexture() {//�������� �������� � ������ ����������
	glBindTexture(GL_TEXTURE_2D, VBOtexture);//����������� �������� � ������������� ����������� ����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage.width(), textureImage.height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, textureImage.bits());//��������� �������� � ������ ����������(����� ���������, ������� ������� �����������?, ������(��������) ��������, ������, ������, �������, ������(��������) ��������, ��� ��������, ������)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//������������� ��������� ��� ������� ��������, ����������� � ����������� �����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//����� ��������, ��������������� ��� ��������� ������� ����(�����), �������� �����������(��� ������ �� ��������)
}

void View::genTextureImage(int layerNumber) {//��������� ����������������� ��������������
	int w = first;
	int h = second;
	textureImage = QImage(w, h, QImage::Format_RGB32);
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int intensity = TransferFunction(test.array[layerNumber * w * h + w * y + x], min, max);
			QColor c = QColor::fromRgbF(intensity, intensity, intensity);
			textureImage.setPixelColor(x, y, c);
		}
	}
}

void View::VizualizationTexture() {//������ ���� ������������� � ���������� ���������
	glBegin(GL_QUADS);
	qglColor(QColor(255, 255, 255));

	glTexCoord2f(0, 0);
	glVertex2i(0, 0);

	glTexCoord2f(0, 1);
	glVertex2i(0, second);

	glTexCoord2f(1, 1);
	glVertex2i(first, second);

	glTexCoord2f(1, 0);
	glVertex2i(first, 0);

	glEnd();
}


void View::DrawQuadStrip() {
	short value;
	int transfer;
	for (int y_coord = 0; y_coord < second - 1; y_coord++){
		for (int x_coord = 0; x_coord < first; x_coord++){

				glBegin(GL_QUAD_STRIP);
				
				value = test.array[x_coord + y_coord * first + layerNumber * first * second];//������� ��������
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);//������ ����
				glVertex2i(x_coord, y_coord);//��������� �������
				//2 �������
				value = test.array[x_coord + (y_coord + 1) * first + layerNumber * first * second];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i(x_coord, (y_coord + 1));
				//3 �������
				value = test.array[(x_coord + 1) + (y_coord + 1) * first + layerNumber * first * second];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((x_coord + 1), (y_coord + 1));
				//4 �������
				value = test.array[(x_coord + 1) + y_coord * first + layerNumber * first * second];
				transfer = TransferFunction(value, min, max);
				glColor3f(transfer, transfer, transfer);
				glVertex2i((x_coord + 1), y_coord);

				glEnd();
		}
	}
}

