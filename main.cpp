#include <QApplication>
#include "mandelbrotset.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	MandelbrotSet widget;
	widget.show();
	return app.exec();
}
