#include <qimage.h>
#include <math.h>
#include "Filters.h"
#include <iostream>
using namespace std;

#define PI 3.14159265

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

template< class T >
void insertSort(T* a, int kol){
	T tmp;
	for (int i = 1, j; i < kol; ++i){ // ���� ��������, i - ����� �������
		tmp = a[i];
		for (j = i - 1; j >= 0 && a[j] > tmp; --j) // ����� ����� �������� � ������� ������������������ 
			a[j + 1] = a[j];    // �������� ������� �������, ���� �� �����
		a[j + 1] = tmp; // ����� �������, �������� �������    
	}
}

Filters::Filters(){
}

Filters::~Filters(){
}

//----------------------------------invert--------------------------------------
 //������� ���������� ���� QImage 
QImage Invert_filter::calculateNewImagePixMap(const  QImage &photo, int radius) {
	QImage result_image(photo); //����������� QImage
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
	int size = 2 * radius + 1;//�������
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			int idx = (i + radius) * size + j + radius;
			int p_x = clamp<int>(x + i, photo.width() - 1, 0);
			int p_y = clamp<int>(y + j, photo.height() - 1, 0);
			QColor color = photo.pixelColor(p_x, p_y);//�������� ���� �������� � ������ ������� �� ��������
			returnR += color.red() * vector[idx];
			returnG += color.green() * vector[idx];
			returnB += color.blue() * vector[idx];
		}
	}
	
	return QColor(clamp(returnR, 255, 0), clamp(returnG, 255, 0), clamp(returnB, 255, 0));//����� �� ����� �� ������� 255
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
	float norm = 0;//����������� ���������� ����
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

//-------------------------------------------------������ ��� ����� ������������ ������-------------------------------------------------------------------
//-------------------------------------------------1�� �������(2 ��������� � 2 ���������)--------------------------------------------------------------------
QImage Transfer::calculateNewImagePixMap(const  QImage &photo, int radius) {//�������/�������
	QImage result_image(photo); //����������� QImage
	for (int x = 0; x < photo.width(); x++) {
		for (int y = 0; y < photo.height(); y++) {
			if (x + 50 < photo.width()) {
				QColor photo_color = photo.pixelColor(x + 50, y);
				result_image.setPixelColor(x, y, photo_color);
			}
			else {
				QRgb value_color = qRgb(0, 0, 0); // ������ ����
				result_image.setPixelColor(x, y, value_color);
			}

		}
	}
	return result_image;
}
QImage Waves::calculateNewImagePixMap(const  QImage &photo, int radius) {//�����
	QImage result_image(photo); //����������� QImage
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
	int size = 2 * radius + 1;//�������
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
			cout << x << " " << y << endl;
			for (int i = -radius; i <= radius; i++) {
				for (int j = -radius; j <= radius; j++) {
					int p_x = clamp<int>(x + i, photo.width() - 1, 0);
					int p_y = clamp<int>(y + j, photo.height() - 1, 0);
					if (x == 206 && y==15) {
						cout << "hmmm " << p_x << " " << p_y << endl;
					}
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
			for (int i = 0; i < size * size; i++) { //���������� ���������
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

//----------------------------------------����� ���--------------------------------
QImage Gray_world::calculateNewImagePixMap(const  QImage &photo, int radius) {//�������/�������
	QImage result_image(photo); //����������� QImage
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