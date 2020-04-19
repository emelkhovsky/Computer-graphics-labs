#include <QtOpenGL/QGLWidget>
#include "View.h"


View::View(){
}


View::~View(){
}

template <class T>//чтобы не выйти за границы 1
T clamp(T v, int max, int min) {
	if (v > max) {
		return max;
	}
	if (v < min) {
		return min;
	}
	return v;
}

float TransferFunction(short value) {//переводит значени€ плотности томограммы в черно-белый цвет
	int min = 0;
	int max = 2000;
	int newVal = clamp<int>((value - min) * 255 / (max - min), 0, 255);
	return clamp<float>(newVal / 200.f, 1, 0);//вычисл€ю в диапазоне от 0 до 1, потому что ???????????
}

void View::initializeGL() {//инициализаци€
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//очистка буфера изображени€
}

void View::resizeGL(int width, int height) {//окно вывода
	glShadeModel(GL_SMOOTH);//с градиентом фигура или нет
	glMatrixMode(GL_PROJECTION);//режим матрицы проекций
	glLoadIdentity();//инициализировали матрицу и сбросили до состо€ни€ по умолчанию(единичной???)
	glOrtho(0, test.X, 0, test.Y, -1, 1);//установка двумерной ортографической системы координат(тупо проекци€ картинки)
	glViewport(0, 0, width, height);//задали пр€моугольник, через который мы видим эту проекцию
}

void View::DrawQuads(int layerNumber) {//отрисовка четырехугольника
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//очищает буфер цвета и буфер глубины
	glBegin(GL_QUADS);//указываем, что будем рисовать пр€моугольник
	for (int x_coord = 0; x_coord < test.X - 1; x_coord++) {
		for (int y_coord = 0; y_coord < test.Y - 1; y_coord++) {
			short value;
			float transfer;
			//1 вершина
			value = test.array[x_coord + y_coord * test.X + layerNumber * test.X * test.Y];//находим значение
			transfer = TransferFunction(value);
			glColor3f(transfer, transfer, transfer);//задаем цвет
			glVertex2i(x_coord, y_coord);//указываем вершину
			//2 вершина
			value = test.array[x_coord + (y_coord + 1) * test.X + layerNumber * test.X * test.Y];
			transfer = TransferFunction(value);
			glColor3f(transfer, transfer, transfer);
			glVertex2i(x_coord, (y_coord + 1));
			//3 вершина
			value = test.array[(x_coord + 1) + (y_coord + 1) * test.X + layerNumber * test.X * test.Y];
			transfer = TransferFunction(value);
			glColor3f(transfer, transfer, transfer);
			glVertex2i((x_coord + 1), (y_coord + 1));
			//4 вершина
			value = test.array[(x_coord + 1) + y_coord * test.X + layerNumber * test.X * test.Y];
			transfer = TransferFunction(value);
			glColor3f(transfer, transfer, transfer);
			glVertex2i((x_coord + 1), y_coord);
		}
	}
	glEnd();//закончили рисовать
}

void View::paintGL() {//отрисовка четырехугольника
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//очищает буфер цвета и буфер глубины
	DrawQuads(layerNumber);
	//swapBuffers();
}

