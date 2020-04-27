#include "mandelbrotset.h"
const long double eps = 1e-9;

MandelbrotSet::MandelbrotSet(QWidget *t_parent): QWidget(t_parent) {
	center = {-0.6, -0.07};
	truscale = 0.01;
	scale = 0.01;
	connect(&drawer, &MandelbrotDrawer::done, this, &MandelbrotSet::updateimg);
	resize(430, 370);
}

void MandelbrotSet::paintEvent(QPaintEvent *) {
	QPainter painter(this);
	painter.fillRect(rect(), Qt::black);
	if (img.isNull())
		return;
	if (abs(scale-truscale) < eps) {
		painter.drawPixmap(imgoff, img);
	} else {
		double scaleFactor = truscale / scale;
		Pair<long double, long double> newsize((int)(img.width() * scaleFactor), (int)(img.height() * scaleFactor));
		Pair<int, int> newloc(imgoff.x() + (img.width() - newsize.first) / 2, imgoff.y() + (img.height() - newsize.second) / 2);
		painter.save();
		painter.translate(newloc.first, newloc.second);
		painter.scale(scaleFactor, scaleFactor);
		QRectF savecur = painter.transform().inverted().mapRect(rect()).adjusted(-1, -1, 1, 1);
		painter.drawPixmap(savecur, img, savecur);
		painter.restore();
	}
}

void MandelbrotSet::resizeEvent(QResizeEvent *) {
	drawer.draw(center, scale, size());
}

void MandelbrotSet::wheelEvent(QWheelEvent *t_event) {
	zoom(powl(0.8, t_event->angleDelta().y() / 120.0f));
}

void MandelbrotSet::mousePressEvent(QMouseEvent *t_event) {
	if (t_event->button() == Qt::LeftButton)
		dragloc = t_event->pos();
}

void MandelbrotSet::mouseMoveEvent(QMouseEvent *t_event) {
	if (t_event->buttons() & Qt::LeftButton) {
		imgoff += t_event->pos() - dragloc;
		dragloc = t_event->pos();
		update();
	}
}

void MandelbrotSet::mouseReleaseEvent(QMouseEvent *t_event) {
	if (t_event->button() == Qt::LeftButton) {
		imgoff += t_event->pos() - dragloc;
		dragloc = QPoint();
		dragimg((width() - img.width()) / 2 - imgoff.x(), (height() - img.height()) / 2 - imgoff.y());
	}
}

void MandelbrotSet::updateimg(const QImage &t_img, long double t_factor) {
	if (!dragloc.isNull())
		return;
	img = QPixmap::fromImage(t_img);
	imgoff = QPoint();
	dragloc = QPoint();
	truscale = t_factor;
	update();
}

void MandelbrotSet::zoom(long double t_factor) {
	scale *= t_factor;
	update();
	drawer.draw(center, scale, size());
}

void MandelbrotSet::dragimg(int t_x, int t_y) {
	center.first += t_x * scale;
	center.second += t_y * scale;
	update();
	drawer.draw(center, scale, size());
}
