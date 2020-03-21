#include <qimage.h>
#include <math.h>
#include "Filters.h"
#include <iostream>
using namespace std;

#define PI 3.14159265
#define MH 3
#define MW 3

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
			int p_x = clamp<int>(x + i, photo.width() - 1, 0);
			int p_y = clamp<int>(y + j, photo.height() - 1, 0);
			QColor color = photo.pixelColor(p_x, p_y);//выяснили цвет пискселя в данной матрице на картинке
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
//-------------------------------------------------1ое задание(2 точеченых и 2 матричных)--------------------------------------------------------------------
QImage Transfer::calculateNewImagePixMap(const  QImage &photo, int radius) {//перенос/поворот
	QImage result_image(photo); //конструктор QImage
	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			if (x + 50 < photo.width()) {
				QColor photo_color = photo.pixelColor(x + 50, y);
				result_image.setPixelColor(x, y, photo_color);
			}
			else {
				QRgb value_color = qRgb(0, 0, 0); // черный цвет
				result_image.setPixelColor(x, y, value_color);
			}

		}
	}
	return result_image;
}
QImage Waves::calculateNewImagePixMap(const  QImage &photo, int radius) {//волны
	QImage result_image(photo); //конструктор QImage
	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			if (x + 20 * sin((2 * PI * x) / 30) < photo.width()) {
				QColor photo_color = photo.pixelColor(x + 20 * sin((2 * PI * x) / 30), y);
				result_image.setPixelColor(x, y, photo_color);
			}
			else {
				QColor photo_color = photo.pixelColor(x, y);
				result_image.setPixelColor(x, y, photo_color);
			}
		}
	}
	return result_image;
}
//----------------------------------------Median--------------------------------
QImage Median :: calculateNewImagePixMap(const QImage &photo, int radius) {
	QImage result_image(photo);
	int size = 2 * radius + 1;//диаметр
	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			int returnR = 0;
			int returnG = 0;
			int returnB = 0;
			int current_count = 0;
			int* Intencity_list;
			int* coordinates_list_x;
			int* coordinates_list_y;
			Intencity_list = new int[size * size];
			coordinates_list_x = new int[size * size];
			coordinates_list_y = new int[size * size];
			for (int i = -radius; i <= radius; i++) {
				for (int j = -radius; j <= radius; j++) {
					int p_x = clamp<int>(x + i, photo.width() - 1, 0);
					int p_y = clamp<int>(y + j, photo.height() - 1, 0);
					QColor color = photo.pixelColor(p_x, p_y);
					int Intencity = 0.36 * color.red() + 0.53 * color.green() + 0.11 * color.blue();
					Intencity_list[current_count] = Intencity;
					coordinates_list_x[current_count] = p_x;
					coordinates_list_y[current_count] = p_y;
					current_count++;
				}
			}
			float tmp;
			int tmpp;
			for (int i = 0; i < size * size; i++) { //сортировка пузырьком
				for (int j = 1; j < size * size; j++) {
					if (Intencity_list[j - 1] > Intencity_list[j]) {
						tmp = Intencity_list[j - 1];
						Intencity_list[j - 1] = Intencity_list[j];
						Intencity_list[j] = tmp;
						tmpp = coordinates_list_x[j - 1];
						coordinates_list_x[j - 1] = coordinates_list_x[j];
						coordinates_list_x[j] = tmpp;
						tmpp = coordinates_list_y[j - 1];
						coordinates_list_y[j - 1] = coordinates_list_y[j];
						coordinates_list_y[j] = tmpp;
					}
				}
			}
			int median_index = size * size / 2;
			int coord_x = coordinates_list_x[median_index];
			int coord_y = coordinates_list_y[median_index];
			QColor color = photo.pixelColor(coord_x, coord_y);
			result_image.setPixelColor(x, y, color);
		}
	}

	return result_image;
}

//----------------------------------------Серый мир--------------------------------
QImage Gray_world::calculateNewImagePixMap(const  QImage &photo, int radius) {//перенос/поворот
	QImage result_image(photo); //конструктор QImage
	int R = 0;
	int G = 0;
	int B = 0;
	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			QColor color = photo.pixelColor(x, y);
			R += color.red();
			G += color.green();
			B += color.blue();
		}
	}
	int kol = photo.width() * photo.height();
	int Rav = R / kol;
	int Gav = G / kol;
	int Bav = B / kol;
	int average = (Rav + Gav + Bav) / 3;

	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			QColor color = photo.pixelColor(x, y);
			color.setRgb(clamp(color.red() * average / Rav, 255, 0), clamp(color.green() * average / Gav, 255, 0), clamp(color.blue() * average / Bav, 255, 0));
			result_image.setPixelColor(x, y, color);
		}
	}
	return result_image;
}

//----------------------------------------Линейное растяжение--------------------------------
QImage Linear_tension::calculateNewImagePixMap(const  QImage &photo, int radius) {//перенос/поворот
	QImage result_image(photo); //конструктор QImage
	int maxR = 0;
	int maxG = 0;
	int maxB = 0;
	int minR = 255;
	int minG = 255;
	int minB = 255;
	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			QColor color = photo.pixelColor(x, y);
			if (color.red() > maxR) {
				maxR = color.red();
			}
			if (color.red() < minR) {
				minR = color.red();
			}
			if (color.green() > maxG) {
				maxG = color.green();
			}
			if (color.green() < minG) {
				minG = color.green();
			}
			if (color.blue() > maxB) {
				maxB = color.blue();
			}
			if (color.blue() < minB) {
				minB = color.blue();
			}
		}
	}
	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			QColor color = photo.pixelColor(x, y);
			int R = (color.red() - minR) * 255 / (maxR - minR);
			int G = (color.green() - minG) * 255 / (maxG - minG);
			int B = (color.blue() - minB) * 255 / (maxB - minB);
			color.setRgb(R, G, B);
			result_image.setPixelColor(x, y, color);
		}
	}
	return result_image;
}


QImage Morfo ::Dilation(const  QImage &photo, bool *mask){
	QImage result_image(photo);
	// Width, Height – размеры исходного и результирующего изображений
	// MW, MH – размеры структурного множества
	int hieght = photo.height() - MH / 2;
	int width = photo.width() - MW / 2;
	for (int y = MH / 2; y < hieght; y++) {
		for (int x = MW / 2; x < width; x++) {
			int maxR = 0;
			int maxG = 0;
			int maxB = 0;
			for (int j = -MH / 2; j <= MH / 2; j++) {
				for (int i = -MW / 2; i <= MW / 2; i++) {
					int p_x = clamp<int>(x + i, photo.width() - 1, 0);
					int p_y = clamp<int>(y + j, photo.height() - 1, 0);
					QColor color = photo.pixelColor(p_x, p_y);
					if ((mask[i * MW + j]) && (color.red() > maxR)) {
						maxR = color.red();
					}
					if ((mask[i * MW + j]) && (color.green() > maxG)) {
						maxG = color.green();
					}
					if ((mask[i * MW + j]) && (color.blue() > maxB)) {
						maxB = color.blue();
					}
					color.setRgb(maxR, maxG, maxB);
					result_image.setPixelColor(x, y, color);
				}
			}
		}
	}
	return result_image;
}

QImage Morfo::Erosion(const  QImage &photo, bool *mask) {
	QImage result_image(photo);
	// Width, Height – размеры исходного и результирующего изображений
	// MW, MH – размеры структурного множества
	int hieght = photo.height() - MH / 2;
	int width = photo.width() - MW / 2;
	for (int y = MH / 2; y < hieght; y++) {
		for (int x = MW / 2; x < width; x++) {
			int minR = 255;
			int minG = 255;
			int minB = 255;
			for (int j = -MH / 2; j <= MH / 2; j++) {
				for (int i = -MW / 2; i <= MW / 2; i++) {
					int p_x = clamp<int>(x + i, photo.width() - 1, 0);
					int p_y = clamp<int>(y + j, photo.height() - 1, 0);
					QColor color = photo.pixelColor(p_x, p_y);
					if ((mask[i * MW + j]) && (color.red() < minR)) {
						minR = color.red();
					}
					if ((mask[i * MW + j]) && (color.green() < minG)) {
						minG = color.green();
					}
					if ((mask[i * MW + j]) && (color.blue() < minB)) {
						minB = color.blue();
					}
					color.setRgb(minR, minG, minB);
					result_image.setPixelColor(x, y, color);
				}
			}
		}
	}
	return result_image;
}

QImage Morfo::Opening(const  QImage &photo, bool *mask) {
	QImage result_image(photo);

	Morfo* eros = new Morfo();
	QImage erosImage = eros->Erosion(photo, mask);

	Morfo* dilat = new Morfo();
	QImage dilatImage = dilat->Dilation(erosImage, mask);

	return dilatImage;
}


QImage Morfo::Closing(const  QImage &photo, bool *mask) {
	QImage result_image(photo);

	Morfo* dilat = new Morfo();
	QImage dilatImage = dilat->Dilation(photo, mask);

	Morfo* eros = new Morfo();
	QImage erosImage = eros->Erosion(dilatImage, mask);

	return erosImage;
}

QImage Morfo::Grad(const  QImage &photo, bool *mask) {
	QImage result_image(photo);

	Morfo* dilat = new Morfo();
	QImage dilatImage = dilat->Dilation(photo, mask);

	Morfo* eros = new Morfo();
	QImage erosImage = eros->Erosion(photo, mask);

	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			QColor color = result_image.pixelColor(x, y);
			QColor color_dil = dilatImage.pixelColor(x, y);
			QColor color_er = erosImage.pixelColor(x, y);
			int red = color_dil.red() - color_er.red();
			int green = color_dil.green() - color_er.green();
			int blue = color_dil.blue() - color_er.blue();
			color.setRgb(red, green, blue);
			result_image.setPixelColor(x, y, color);
		}
	}
	return result_image;
}
