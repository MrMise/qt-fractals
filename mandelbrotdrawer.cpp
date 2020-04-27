#include "mandelbrotdrawer.h"

MandelbrotDrawer::MandelbrotDrawer(QObject *t_parent) : QThread(t_parent) {
	restart = false;
	stop = false;
	for (int i = 0; i < ncolors; ++i) {
		long double off = 0.267;
		long double sr = 1, sg = 1, sb = 1;
		for (int a = 0; a < 314; a++) {
			long double ra = a; ra /= 100.0;
			sr += sin(sr *powl(sin(ra), 1.61803398));
			sg += sin(sg * powl(sin(ra), 2.61803396));
			sb += sin(sb * powl(sin(ra), 4.23606790));
		}
		long double r = sin(0.1 * i + sr * off) * 127 + 128;
		long double g = sin(0.1 * i + sg * off) * 127 + 128;
		long double b = sin(0.1 * i + sb * off) * 127 + 128;
		colormap[i] = qRgb(int(r)%256, int(g)%256, int(b)%256);
	}
}

MandelbrotDrawer::~MandelbrotDrawer() {
	mutex.lock();
	stop = true;
	condition.wakeOne();
	mutex.unlock();
	wait();
}

void MandelbrotDrawer::draw(Pair<long double, long double> t_center, long double t_scale, QSize t_size) {
	QMutexLocker locker(&mutex);
	center = t_center;
	scale = t_scale;
	size = t_size;
	if (!isRunning()) {
		start(LowPriority);
	} else {
		restart = true;
		condition.wakeOne();
	}
}

void MandelbrotDrawer::run() {
	for(;;) {
		mutex.lock();
		mutex.unlock();
		QImage image(size, QImage::Format_RGB30);
		int pass = 0;
		long double origimag, origreal;
		while (pass < 6) {
			int limiter = (1 << (pass + 2))+(1 << (pass + 1));
			bool draw = false;
			origimag = center.second - (size.height() / 2 + 1) * scale;
			for (int i = 0; i < size.height(); i++) {
				origimag += scale;
				origreal = center.first - (size.width() / 2 + 1) * scale;
				for (int j = 0; j < size.width(); j++) {
					if (restart)
						break;
					if (stop)
						return;
					origreal += scale;
					Complex<long double> c(0.285, 0.01);
					Complex<long double> z(origreal, origimag);
//					Complex<long double> c(origreal, origimag);
//					Complex<long double> z(0, 0);
					int it = 0;
					while (z.magnitudeSquared() <= static_cast<long double>(4) && it < limiter) {
						z = z.square() + c;
						it++;
					}
					if (it < limiter) {
						image.setPixel(j, i, colormap[it % ncolors]);
						draw = true;
					} else
						image.setPixel(j, i, qRgb(0, 0, 0));
				}
			}
			if (draw && !restart)
					emit done(image, scale);
			++pass;
		}
		mutex.lock();
		if (!restart)
			condition.wait(&mutex);
		restart = false;
		mutex.unlock();
	}
}
