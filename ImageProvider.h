#pragma once

#include <QQuickImageProvider>
#include <QMap>
class ImageProvider: public QQuickImageProvider
{
public:
	ImageProvider():QQuickImageProvider(QQuickImageProvider::Image) {};
	QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
	void insertImage(const QString &id, const QImage& image);
private:
	QMap<QString, QImage> image_map;
};