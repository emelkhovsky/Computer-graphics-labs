#include "Bin.h"

template <class T>//чтобы не выйти за границы 255
T clamp(T v, int max, int min) {
	if (v > max) {
		return max;
	}
	if (v < min) {
		return min;
	}
	return v;
}

GLint* TransferFunction(short value) {//переводит значения плотности томограммы в черно-белый цвет
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

	void SetupView(int width, int height) {//окно вывода
		glShadeModel(GL_SMOOTH);//с градиентом фигура или нет
		glMatrixMode(GL_PROJECTION);//режим матрицы проекций
		glLoadIdentity();//инициализировали матрицу и сбросили до состояния по умолчанию(единичной???)
		glOrtho(0, Bin.X, 0, Bin.Y, -1, 1);//установка двумерной ортографической системы координат(тупо проекция картинки)
		glViewport(0, 0, width, height);//задали прямоугольник, через который мы видим эту проекцию
	}

	void DrawQuads(int layerNumber) {//отрисовка четырехугольника
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//очищает буфер цвета и буфер глубины
		glBegin(GL_QUADS);//указываем, что будем рисовать прямоугольник
		for (int x_coord = 0; x_coord < Bin.X - 1; x_coord++) {
			for (int y_coord = 0; y_coord < Bin.Y - 1; y_coord++) {
				short value;
				//1 вершина
				value = Bin.array[x_coord + y_coord * Bin.X + layerNumber * Bin.X * Bin.Y];//находим значение
				glColor3i(TransferFunction(value)[0], TransferFunction(value)[1], TransferFunction(value)[2]);//задаем цвет
				glVertex2i(x_coord, y_coord);//указываем вершину
				//2 вершина
				value = Bin.array[x_coord + (y_coord + 1) * Bin.X + layerNumber * Bin.X * Bin.Y];
				glColor3i(TransferFunction(value)[0], TransferFunction(value)[1], TransferFunction(value)[2]);
				glVertex2i(x_coord, (y_coord + 1));
				//3 вершина
				value = Bin.array[(x_coord + 1) + (y_coord + 1) * Bin.X + layerNumber * Bin.X * Bin.Y];
				glColor3i(TransferFunction(value)[0], TransferFunction(value)[1], TransferFunction(value)[2]);
				glVertex2i((x_coord + 1), (y_coord + 1));
				//4 вершина
				value = Bin.array[(x_coord + 1) + y_coord * Bin.X + layerNumber * Bin.X * Bin.Y];
				glColor3i(TransferFunction(value)[0], TransferFunction(value)[1], TransferFunction(value)[2]);
				glVertex2i((x_coord + 1), y_coord);
			}
		}
		glEnd();//закончили рисовать
	}
};
