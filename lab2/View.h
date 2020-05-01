#pragma once
#include "Bin.h"

class View : public QGLWidget {
public:
	Bin test;
	int layerNumber;
	GLuint VBOtexture;//номер текстуры в памяти видеокарты
	QImage textureImage;
	int mode;
	int min = 0;
	int max = 2000;
	View();
	View(string path, QWidget* parent): QGLWidget(parent){
		test.readBIN(path);
		layerNumber = 1;
	}
	~View();
	void DrawQuads(int);
	virtual void initializeGL();
	virtual void resizeGL(int, int);
	virtual void paintGL();
	void input_layer(int);
	void Load2dTexture();
	void genTextureImage(int);
	void VizualizationTexture();
	void setmode(int);
	void DrawQuadStrip();
;};
	