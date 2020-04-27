#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include <QImage>
#include <cmath>
#include "utility.h"
#include "complex.h"

class MandelbrotDrawer : public QThread
{
	Q_OBJECT
	QMutex mutex;
	QWaitCondition condition;
	static const int ncolors = 256;
	uint colormap[ncolors];
	long double scale;
	Pair<long double, long double> center;
	QSize size;
	bool restart, stop;
public:
	MandelbrotDrawer(QObject *t_parent = nullptr);
	~MandelbrotDrawer();
	void draw(Pair<long double, long double> t_center, long double t_scale, QSize t_size);

signals:
	void done(const QImage &t_image, double t_scale);

protected:
	void run() override;
};

#endif // RENDERTHREAD_H
