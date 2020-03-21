#pragma once
#include <qimage.h>
#include <iostream>
#include <string>
#include "Filters.h"
using namespace std;

class Filters{
public:
	Filters();
	~Filters();

	virtual QImage calculateNewImagePixMap(const QImage &photo, int radius) = 0;//дл€ работы с пиксел€ми дл€ всех фильтров, считаем новую картинку и возвращаем ее
};

//----------------------------------invert--------------------------------------
class Invert_filter: public Filters {
public:
	Invert_filter() {};
	~Invert_filter() {};

	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};
//----------------------------------matrix--------------------------------------
class Matrix_filter: public Filters {
public:
	float* vector;
	int mRadius;
	Matrix_filter(int radius = 1) : mRadius(radius) {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);//новый цвет дл€ отдельного пиксел€
};
//----------------------------------------Blur--------------------------------
class Blur_filter : public Matrix_filter {
public:
	Blur_filter() {
		int size = 2 * mRadius + 1;
		vector = new float[size * size];
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				vector[i * size + j] = 1.0f / (size * size);
			}
		}
	}
};
//----------------------------------------Gaussian--------------------------------
class Gaussian_blur_filter : public Matrix_filter {
public:
	Gaussian_blur_filter() {
		createGaussianVector(3, 2);
	}
	~Gaussian_blur_filter() {};
	void createGaussianVector(int radius, int sigma);
};

//-------------------------------------------------«адачи дл€ самосто€тельного выполнени€-------------------------------------------------------------------

//----------------------------------------GrayScaleFilter--------------------------------
class GrayScaleFilter : public Filters {
public:
	GrayScaleFilter() {};
	~GrayScaleFilter() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};
//----------------------------------------Seppia--------------------------------
class Seppia : public Filters {
public:
	Seppia() {};
	~Seppia() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};
//----------------------------------------Brightness--------------------------------
class Brightness: public Filters {
public:
	Brightness() {};
	~Brightness() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};
//----------------------------------------Sobel--------------------------------
class Sobelx : public Matrix_filter {
public:
	Sobelx() {
		int size = 3;
		vector = new float[size * size];
		vector[0] = -1; 
		vector[1] = 0; 
		vector[2] = 1; 
		vector[3] = -2; 
		vector[4] = 0;
		vector[5] = 2; 
		vector[6] = -1; 
		vector[7] = 0; 
		vector[8] = 1;
	};
	~Sobelx() {};
};

class Sobely : public Matrix_filter {
public:
	Sobely() {
		int size = 2 * mRadius + 1;
		vector = new float[size * size];
		vector[0] = -1; vector[1] = -2; vector[2] = -1; vector[3] = 0; vector[4] = 0;
		vector[5] = 0; vector[6] = 1; vector[7] = 2; vector[8] = 1;
	};
	~Sobely() {};
};
//----------------------------------------Sharpness(резкость)--------------------------------
class Sharpness : public Matrix_filter {
public:
	Sharpness() {
		int size = 3;
		vector = new float[size * size];
		vector[0] = 0; vector[1] = -1; vector[2] = 0; vector[3] = -1; vector[4] = 5;
		vector[5] = -1; vector[6] = 0; vector[7] = -1; vector[8] = 0;
	};
	~Sharpness() {};
};

//-------------------------------------------------«адачи дл€ сдачи лабораторной работы-------------------------------------------------------------------
//-------------------------------------------------1ое задание(2 точеченых и 2 матричных)--------------------------------------------------------------------
class Transfer : public Filters {//перенос/поворот
public:
	Transfer() {};
	~Transfer() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};
class Waves : public Filters {//волны
public:
	Waves() {};
	~Waves() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};
class SharpnessLab : public Matrix_filter {//матричный резкость
public:
	SharpnessLab() {
		int size = 3;
		vector = new float[size * size];
		vector[0] = -1; vector[1] = -1; vector[2] = -1; vector[3] = -1; vector[4] = 9;
		vector[5] = -1; vector[6] = -1; vector[7] = -1; vector[8] = -1;
	};
	~SharpnessLab() {};
};
class Motion_blur : public Matrix_filter{//матричный motion blur
public:
	Motion_blur() : Matrix_filter(5) {
		int size = 2 * mRadius + 1;
		vector = new float[size * size];
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++){
				if (i == j) {
					vector[i * size + j] = 1.0f / size;
				}
				else {
					vector[i * size + j] = 0;
				} 
			}
		}

	}
};

//----------------------------------------ћедианный фильтр--------------------------------
class Median : public Filters {
public:
	Median() {};
	~Median() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

//----------------------------------------—ерый мир--------------------------------
class Gray_world : public Filters {
public:
	Gray_world() {};
	~Gray_world() {};
	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

//----------------------------------линейное раст€жение--------------------------------------
class Linear_tension : public Filters {
public:
	Linear_tension() {};
	~Linear_tension() {};

	QImage calculateNewImagePixMap(const QImage &photo, int radius);
};

//---------------------------------морфологические операции---------------------------------
class Morfo{//перенос/поворот
public:
	Morfo() {};
	~Morfo() {};
	QImage Dilation(const QImage &photo, bool *mask);
	QImage Erosion(const QImage &photo, bool *mask);
	QImage Opening(const QImage &photo, bool *mask);
	QImage Closing(const QImage &photo, bool *mask);
	QImage Grad(const QImage &photo, bool *mask);
};