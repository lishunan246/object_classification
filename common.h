#pragma once

#include "opencv/cv.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "ml.h"
#include <QDirIterator>
#include <QDebug>
#include <vector>
#include <map>
#include <fstream>

const QString train_dir = "train";
const QString validate_dir = "validate";

void catalog_init();
void get_vocabulary();
void get_training_data();
void trainSVM();
void evaluate(const QString dir);
QString predic(const cv::Mat& img);