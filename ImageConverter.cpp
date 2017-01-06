#include "ImageConverter.h"

QImage ImageConverter::Mat2QImage(const cv::Mat& image)
{
	if (image.type() == CV_8UC3)
		return QImage(static_cast<unsigned char*>(image.data), image.cols, image.rows, QImage::Format_RGB888).rgbSwapped().copy();
	if (image.type() == CV_8UC1)
		return QImage(static_cast<unsigned char*>(image.data), image.cols, image.rows, QImage::Format_Grayscale8).copy();
	throw "Unknown format";
}

cv::Mat ImageConverter::QImage2Mat(const QImage & image)
{
	if(image.format()== QImage::Format_RGB32||image.format()==QImage::Format_ARGB32)
	{
		auto cp = image.convertToFormat(QImage::Format_RGB888);
		return QImage2Mat(cp);
	}
	if(image.format()== QImage::Format_RGB888)
	{
		auto t = image.rgbSwapped();
		return cv::Mat(t.height(), t.width(), CV_8UC3,const_cast<uchar*>(t.bits()),t.bytesPerLine()).clone();
	}
		
	if(image.format()== QImage::Format_Grayscale8)
		return cv::Mat(image.height(), image.width(), CV_8UC1,const_cast<uchar*>(image.bits()),image.bytesPerLine()).clone();
	throw "Unknown format";
}
