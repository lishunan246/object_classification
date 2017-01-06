#include "ImageLoader.h"



ImageLoader::ImageLoader(ImageProvider* image_provider)
{
	ImageLoader::image_provider = image_provider;
}


ImageLoader::~ImageLoader()
{
}

void ImageLoader::loadImage(QUrl url)
{
	auto file = url.toLocalFile();
	QImage image;
	if (!image.load(file))
		throw "load image failed";
	
	auto mat = ImageConverter::QImage2Mat(image);
	//cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
	auto img = ImageConverter::Mat2QImage(mat);
	image_provider->insertImage(url.fileName(), img);

}

QString ImageLoader::Hello()
{
	return "123";
}