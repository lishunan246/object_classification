#pragma once
#include <QImage>
#include <opencv2/opencv.hpp>
class ImageConverter
{
public:
	static QImage Mat2QImage(const cv::Mat& image);
	static cv::Mat QImage2Mat(const QImage& image);
};


