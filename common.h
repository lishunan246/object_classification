#pragma once

#include <iostream>
#include "opencv/cv.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "ml.h"
#include <QDirIterator>
#include <QDebug>
#include <vector>
#include <map>
#include <fstream>

void catalog_init();
void get_vocabulary();
void get_training_data();
void trainSVM();
void evaluate();
QString predic(const cv::Mat& img);