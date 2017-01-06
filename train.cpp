#include <iostream>
#include "opencv/cv.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "ml.h"
#include <QDirIterator>
#include <QDebug>
#include <vector>
#include <map>
#include <fstream>
//QString dir = "2";
QString dir = "101_ObjectCategories";
std::string dictionary_filename = "dictionary.xml";
std::string training_data_filename = "training_data.xml";
std::string svm_filename = "svm.xml";
double Hessian = 800.0;

struct SurfDescriptorExtractor:public cv::FeatureDetector
{
	cv::Ptr<cv::Feature2D> surf;

	SurfDescriptorExtractor(double hessian = Hessian)
	{
		surf = cv::xfeatures2d::SURF::create(hessian);
	}

	void detectAndCompute(cv::InputArray in, cv::InputArray mask, std::vector<cv::KeyPoint>& pts, cv::OutputArray descriptors, bool useProvided = false)
	{
		surf->detectAndCompute(in, mask, pts, descriptors, useProvided);
	}

	void detect(cv::InputArray image, std::vector<cv::KeyPoint>& keypoints)
	{
		surf->detect(image, keypoints);
	}
};

struct SURFDetector
{
	cv::Ptr<cv::Feature2D> surf;

	SURFDetector(double hessian = Hessian)
	{
		surf = cv::xfeatures2d::SURF::create(hessian);
	}

	template <class T>
	void operator()(const T& in, const T& mask, std::vector<cv::KeyPoint>& pts, T& descriptors, bool useProvided = false)
	{
		surf->detectAndCompute(in, mask, pts, descriptors, useProvided);
	}

	void detect(cv::InputArray image, std::vector<cv::KeyPoint>& keypoints)
	{
		surf->detect(image, keypoints);
	}
};

auto matcher = cv::DescriptorMatcher::create("FlannBased");
SURFDetector surf;

int dictionarySize = 1500;
cv::TermCriteria tc(CV_TERMCRIT_ITER, 10, 0.001);
int retries = 1;
int flags = cv::KMEANS_PP_CENTERS;
auto bow_trainer = cv::BOWKMeansTrainer(dictionarySize, tc, retries, flags);
cv::Ptr<cv::DescriptorExtractor> extractor = new SurfDescriptorExtractor();
cv::BOWImgDescriptorExtractor bow_img_descriptor_extractor(extractor, matcher);

std::map<std::string, int> catalog_map;
std::vector<std::string> catalog_list;

void get_vocabulary()
{
	QDirIterator it(dir, QDirIterator::Subdirectories);
	int catalog_cnt = 0;
	int file_cnt = 0;
	while (it.hasNext())
	{
		auto filename = it.next();

		qDebug() << filename;
		if (filename.endsWith(".jpg"))
		{
			file_cnt++;
			auto t = filename.split("/");
			auto catalog = t[1].toStdString();
			if (catalog_map.find(catalog) == catalog_map.end())
			{
				catalog_map.insert({catalog, catalog_cnt});
				catalog_list.push_back(catalog);
				catalog_cnt++;
			}

			auto img = cv::imread(filename.toStdString());
			std::vector<cv::KeyPoint> key_points;
			cv::Mat features;
			surf(img, cv::Mat(), key_points, features);
			if (!features.empty())
				bow_trainer.add(features);
		}
	}
	qDebug() << "Pictures found: " << file_cnt;
	auto descripters = bow_trainer.getDescriptors();
	int count = 0;
	for (auto iter = descripters.begin(); iter != descripters.end(); ++iter)
	{
		count += iter->rows;
	}

	std::cout << "Clustering " << count << " features" << std::endl;

	auto dictionary = bow_trainer.cluster();
	

	cv::FileStorage dictionaryFS;
	dictionaryFS.open(dictionary_filename, cv::FileStorage::WRITE);
	dictionaryFS << "dictionary" << dictionary;
}

void get_training_data()
{
	auto labels = cv::Mat(0, 1, CV_32FC1);
	auto trainingData = cv::Mat(0, dictionarySize, CV_32FC1);
	cv::FileStorage dictionaryFS;
	dictionaryFS.open(dictionary_filename, cv::FileStorage::READ);
	cv::Mat dictionary;
	dictionaryFS["dictionary"]>>dictionary;

	bow_img_descriptor_extractor.setVocabulary(dictionary);
	QDirIterator it(dir, QDirIterator::Subdirectories);
	int catalog_cnt = 0;
	while (it.hasNext())
	{
		auto filename = it.next();
		if (filename.endsWith(".jpg"))
		{
			auto t = filename.split("/");
			auto catalog = t[1].toStdString();
			if (catalog_map.find(catalog) == catalog_map.end())
			{
				catalog_map.insert({catalog, catalog_cnt});
				catalog_list.push_back(catalog);
				catalog_cnt++;
			}
			auto img = cv::imread(filename.toStdString());
			std::vector<cv::KeyPoint> keypoints;
			surf.detect(img, keypoints);
			cv::Mat image_descriptor;
			bow_img_descriptor_extractor.compute(img, keypoints, image_descriptor);
			if(image_descriptor.dims==0)
			{
				qDebug() << filename << " ignored";
				continue;
			}
			trainingData.push_back(image_descriptor);

			labels.push_back(catalog_map[catalog]);
		}
	}

	cv::FileStorage trainingdataFS;
	trainingdataFS.open(training_data_filename, cv::FileStorage::WRITE);
	trainingdataFS << "trainingData" << trainingData;
	trainingdataFS << "label" << labels;

	qDebug() << "saving trainingData row: " << trainingData.rows << " cols: " << trainingData.cols;
	qDebug() << "saving label row: " << labels.rows << " cols: " << labels.cols;
}

void trainSVM()
{
	cv::Mat trainingData;
	cv::FileStorage trainingdataFS;
	trainingdataFS.open(training_data_filename, cv::FileStorage::READ);
	trainingdataFS["trainingData"] >> trainingData;
	cv::Mat labels;
	trainingdataFS["label"] >> labels;


	qDebug() << "loading trainingData row: " << trainingData.rows << " cols: " << trainingData.cols;
	qDebug() << "loading label row: " << labels.rows << " cols: " << labels.cols;

	auto svm = cv::ml::SVM::create();
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setKernel(cv::ml::SVM::RBF);
	//svm->setGamma(0.50625000000000009);
	svm->setTermCriteria(cvTermCriteria(CV_TERMCRIT_ITER, 100, 0.000001));
	//svm->setC(312.50000000000000);
	auto train_data = cv::ml::TrainData::create(trainingData, cv::ml::ROW_SAMPLE, labels);

	svm->trainAuto(train_data);
	auto trained=svm->isTrained();
	qDebug() << "svm trained: " << trained;
	
	svm->save(svm_filename);
}

int main()
{
	//get_vocabulary();
	get_training_data();
	trainSVM();
	system("pause");
	return 0;
}
