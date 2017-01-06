#pragma once
#include <QObject>
#include <QUrl>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include "ImageConverter.h"
#include "ImageProvider.h"

class ImageLoader :
	public QObject
{
	Q_OBJECT
public:
	ImageLoader(ImageProvider* image_provider);
	virtual ~ImageLoader();

	Q_INVOKABLE void loadImage(QUrl url);
	Q_INVOKABLE QString Hello();
private:
	ImageProvider* image_provider;
};

