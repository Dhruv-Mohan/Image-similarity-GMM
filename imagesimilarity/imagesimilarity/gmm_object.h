#pragma once
#include <iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\legacy\legacy.hpp>
#include<opencv2\ml\ml.hpp>


using namespace cv;
class gmm_object
{
public:
	gmm_object();
	~gmm_object();

	int gauss = 4;
	CvEM model;
	CvEMParams ps;
	Mat image;
	Mat processed_image;
	void set_image(string path);
	void train_gmm();
	void process_image();
	void vectorise_model();
	void write_image(string path);
	Mat lables;
	Mat weights;
	Mat means;
	vector<Mat> covs;
	vector<Mat> means_vec;
	vector<double> weight_vec;


};

