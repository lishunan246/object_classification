#include "ImageProvider.h"

QImage ImageProvider::requestImage(const QString& id, class QSize* size, const class QSize& requestedSize)
{
	return image_map[id];
}

void ImageProvider::insertImage(const QString& id, const QImage& image)
{
	image_map.insert(id, image);
}
