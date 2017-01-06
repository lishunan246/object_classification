#include "ImageLoader.h"



ImageLoader::ImageLoader(ImageProvider* image_provider)
{
	ImageLoader::image_provider = image_provider;
}


ImageLoader::~ImageLoader()
{
}

QString ImageLoader::loadImage(QUrl url)
{
	auto file = url.toLocalFile();
	QImage image;
	if (!image.load(file))
		throw "load image failed";
	
	auto mat = ImageConverter::QImage2Mat(image);
	//cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
	catalog_init();
	
	auto img = ImageConverter::Mat2QImage(mat);
	image_provider->insertImage(url.fileName(), img);

	return predic(mat);;
}

QString ImageLoader::Hello()
{
	return "123";
}