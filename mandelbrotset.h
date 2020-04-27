#ifndef MANDELBROTSET_H
#define MANDELBROTSET_H

#include <QPixmap>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <math.h>
#include "mandelbrotdrawer.h"
#include "utility.h"

class MandelbrotSet : public QWidget
{
	Q_OBJECT
	void dragimg(int t_x, int t_y);
	MandelbrotDrawer drawer;
	QPixmap img;
	QPoint imgoff, dragloc;
	Pair<long double, long double> center;
	long double truscale, scale;

public:
	MandelbrotSet(QWidget *t_parent = nullptr);

protected:
	void paintEvent(QPaintEvent *t_event) override;
	void resizeEvent(QResizeEvent *t_event) override;
	void wheelEvent(QWheelEvent *t_event) override;
	void mousePressEvent(QMouseEvent *t_event) override;
	void mouseMoveEvent(QMouseEvent *t_event) override;
	void mouseReleaseEvent(QMouseEvent *t_event) override;

private slots:
	void updateimg(const QImage &t_image, long double t_factor);
	void zoom(long double t_factor);
};

#endif // MANDELBROTSET_H
