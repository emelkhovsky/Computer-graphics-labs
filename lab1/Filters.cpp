#include <qimage.h>
#include <math.h>
#include "Filters.h"
#include <iostream>
using namespace std;

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

template< class T >
void insertSort(T* a, int kol){
	T tmp;
	for (int i = 1, j; i < kol; ++i){ // цикл проходов, i - номер прохода
		tmp = a[i];
		for (j = i - 1; j >= 0 && a[j] > tmp; --j) // поиск места элемента в готовой последовательности 
			a[j + 1] = a[j];    // сдвигаем элемент направо, пока не дошли
		a[j + 1] = tmp; // место найдено, вставить элемент    
	}
}

Filters::Filters(){
}

Filters::~Filters(){
}

//----------------------------------invert--------------------------------------
 //создали переменную типа QImage 
QImage Invert_filter::calculateNewImagePixMap(const  QImage &photo, int radius) {
	QImage result_image(photo); //конструктор QImage
	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			QColor photo_color = photo.pixelColor(x, y);
			photo_color.setRgb(255 - photo_color.red(), 255 - photo_color.green(), 255 - photo_color.blue());
			result_image.setPixelColor(x, y, photo_color);
		}
	}
	return result_image;
}

//----------------------------------matrix--------------------------------------
QColor Matrix_filter::calculateNewPixelColor(QImage photo, int x, int y, int radius) {
	int returnR = 0;
	int returnG = 0;
	int returnB = 0;
	int size = 2 * radius + 1;//диаметр
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			int idx = (i + radius) * size + j + radius;
			QColor color = photo.pixelColor(clamp<int>(x + i, photo.width() - 1, 0), clamp<int>(y + j, photo.height() - 1, 0));//выяснили цвет пискселя в данной матрице на картинке
			returnR += color.red() * vector[idx];
			returnG += color.green() * vector[idx];
			returnB += color.blue() * vector[idx];
		}
	}
	return QColor(clamp(returnR, 255, 0), clamp(returnG, 255, 0), clamp(returnB, 255, 0));//чтобы не выйти за границы 255
}

QImage Matrix_filter::calculateNewImagePixMap(const QImage &photo, int radius) {
	QImage result_photo(photo);
	for(int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			QColor photo_color = calculateNewPixelColor(photo, x, y, radius);
			result_photo.setPixelColor(x, y, photo_color);
		}
	}
	return result_photo;
}

//----------------------------------------Gaussian--------------------------------
void Gaussian_blur_filter:: createGaussianVector(int radius, int sigma) {
	const unsigned int size = 2 * radius + 1;
	float norm = 0;//коэффициент нормировки ядра
	vector = new float[size * size];
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			int idx = (i + radius) * size + (j + radius);
			vector[idx] = exp(-(i * i + j * j) / (sigma * sigma));
			norm += vector[idx];
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			vector[i * size + j] /= norm;
		}
	}
}
//----------------------------------------GrayScaleFilter--------------------------------
QImage GrayScaleFilter::calculateNewImagePixMap(const  QImage &photo, int radius) {
	QImage result_photo(photo);
	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			QColor photo_color = photo.pixelColor(x, y);
			int Intencity = 0.36 * photo_color.red() + 0.53 * photo_color.green() + 0.11 * photo_color.blue();
			photo_color.setRgb(Intencity, Intencity, Intencity);
			result_photo.setPixelColor(x, y, photo_color);
		}
	}
	return result_photo;
}
//----------------------------------------Seppia--------------------------------
QImage Seppia::calculateNewImagePixMap(const  QImage &photo, int radius) {
	QImage result_photo(photo);
	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			QColor photo_color = photo.pixelColor(x, y);
			int Intencity = 0.36 * photo_color.red() + 0.53 * photo_color.green() + 0.11 * photo_color.blue();
			int k = 4;
			int R = clamp(Intencity + 2 * k, 255, 0);
			int G = clamp(Intencity + 0.5 * k, 255, 0);
			int B = clamp(Intencity - 1 * k, 255, 0);
			photo_color.setRgb(R, G, B);
			result_photo.setPixelColor(x, y, photo_color);
		}
	}
	return result_photo;
}
//----------------------------------------Brightness--------------------------------
QImage Brightness::calculateNewImagePixMap(const  QImage &photo, int radius) {
	QImage result_photo(photo);
	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			QColor photo_color = photo.pixelColor(x, y);
			int c = 50;
			photo_color.setRgb(clamp(photo_color.red() + c, 255, 0), clamp(photo_color.green() + c, 255, 0), clamp(photo_color.blue() + c, 255, 0));
			result_photo.setPixelColor(x, y, photo_color);
		}
	}
	return result_photo;
}

//-------------------------------------------------Задачи для сдачи лабораторной работы-------------------------------------------------------------------

//----------------------------------------Median--------------------------------
QColor Median::calculateNewPixelColor(QImage photo, int x, int y, int radius) {
	int size = 2 * radius + 1;//диаметр
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			int idx = (i + radius) * size + j + radius;
			QColor color = photo.pixelColor(clamp<int>(x + i, photo.width() - 1, 0), clamp<int>(y + j, photo.height() - 1, 0));//выяснили цвет пискселя в данной матрице на картинке
			int Intencity = 0.36 * color.red() + 0.53 * color.green() + 0.11 * color.blue();
			vector[idx] = Intencity;

		}
	}
	int kol = size * size;
	insertSort(vector, kol);
	int median_index = kol / 2;
	QColor color = photo.vector[median_index];

	return color;
}