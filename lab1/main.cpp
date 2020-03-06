#include <QtCore/QCoreApplication>
#include <QImage>
#include <iostream>
#include <string>
#include "Filters.h"
using namespace std;

int main(int argc, char *argv[]){
	QImage photo;
	string s;
	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "-p") && (i + 1 < argc)) { //0, ���� ����� ������ ��� � ���� �����, ��� ������ ���-�� ����
			s = argv[i + 1];
		}
	}
	photo.load(QString(s.c_str()));//�������� ������, ������������ � ��������� �� ������ �����, � ������ ��������� � ��� QString(��� load �����)
	photo.save("Starting.jpg");

	Invert_filter* invert = new Invert_filter();
	QImage invertImage = invert->calculateNewImagePixMap(photo, 0);
	invertImage.save("Invert.jpg");

	Blur_filter* blur = new Blur_filter();
	QImage blur_image = blur->calculateNewImagePixMap(photo, 1);
	blur_image.save("Blur.jpg");

	Gaussian_blur_filter* gaus = new Gaussian_blur_filter();
	QImage gaus_image = gaus->calculateNewImagePixMap(photo, 3);
	gaus_image.save("Gaus_blur.jpg");

	GrayScaleFilter* gray = new GrayScaleFilter();
	QImage gray_image = gray->calculateNewImagePixMap(photo, 0);
	gray_image.save("GrayScaleFilter.jpg");

	Seppia* sep = new Seppia();
	QImage sep_image = sep->calculateNewImagePixMap(photo, 0);
	sep_image.save("Seppia.jpg");

	Brightness* bright = new Brightness();
	QImage bright_image = bright->calculateNewImagePixMap(photo, 0);
	bright_image.save("Brightness.jpg");

	Sobelx* sobelx = new Sobelx();
	QImage sobelx_image = sobelx->calculateNewImagePixMap(photo, 1);
	sobelx_image.save("Sobelx.jpg");

	Sobely* sobely = new Sobely();
	QImage sobely_image = sobely->calculateNewImagePixMap(photo, 1);
	sobely_image.save("Sobely.jpg");

	Sharpness* sharp = new Sharpness();
	QImage sharp_image = sharp->calculateNewImagePixMap(photo, 1);
	sharp_image.save("Sharpness.jpg");

	Median* med = new Median();
	QImage med_image = med->calculateNewImagePixMap(photo, 1);
	med_image.save("Median.jpg");
}