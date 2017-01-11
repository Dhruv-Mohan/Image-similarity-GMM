#include "gmm_object.h"



gmm_object::gmm_object()
{
	ps.covs = NULL;
	ps.means = NULL;
	ps.weights = NULL;
	ps.probs = NULL;
	ps.nclusters = 4;
	ps.cov_mat_type = CvEM::COV_MAT_DIAGONAL;
	ps.start_step = CvEM::START_AUTO_STEP;
	ps.term_crit.max_iter = 10000;
	ps.term_crit.epsilon = 0.0001;
	ps.term_crit.type = CV_TERMCRIT_EPS;
}


gmm_object::~gmm_object()
{
}


void gmm_object::set_image(string path) {
	image = imread(path);

}

void gmm_object::process_image() {


	float scaling = 1.0;
	if (image.cols>400) {
		float cols = image.cols;
		scaling = 400 / cols;


	}
	std::cout << "[SCALING]\t" << scaling << "\n";
	resize(image, processed_image, Size(), scaling, scaling, INTER_LANCZOS4);
	imshow("Current image", processed_image);
	waitKey(2);
	cvtColor(processed_image, processed_image, CV_BGR2Lab);
}


void gmm_object::train_gmm() {
	std::cout << "[TRAINING GMM]\n";
	Mat samples;
	Mat final_samples(processed_image.rows*processed_image.cols, 5, CV_32FC1);

	processed_image.reshape(1, processed_image.rows*processed_image.cols).convertTo(samples, CV_32FC1);
	int totalcount = 0;
	for (int y = 0; y<processed_image.rows; y++) {
		for (int x = 0; x<processed_image.cols; x++) {

			final_samples.at<float>(totalcount, 0) = samples.at<float>(totalcount, 0);
			final_samples.at<float>(totalcount, 1) = samples.at<float>(totalcount, 1);
			final_samples.at<float>(totalcount, 2) = samples.at<float>(totalcount, 2);
			final_samples.at<float>(totalcount, 3) = x;
			final_samples.at<float>(totalcount, 4) = y;

			totalcount++;



		}
	}
	model.train(final_samples, Mat(), ps, &lables);
	std::cout << "[GMM TRAINED]\n";
	//std::cout<<"number of clusters"<<model.get_nclusters()<<"\n";
	weights = model.get_weights();
	//std::cout<<"weight of clusters"<<weights<<"\n";
	means = model.get_means();
	//std::cout<<"means of clusters="<<means<<"\n";
	lables = lables.reshape(1, processed_image.rows);
	model.getCovs(covs);
	//std::cout<<"covs vec size="<<covs.size()<<"\n";
	//  std::cout<<lables.size()<<"\n";
	/*cv::normalize(lables, lables, 0, 255, NORM_MINMAX, CV_8UC1);
	Mat dispimage;
	cvtColor(processed_image, dispimage, CV_Lab2BGR); */

}


void gmm_object::vectorise_model() {
	double temp_weight;
	Mat temp_mean(1, 5, CV_64FC1);
	for (int i = 0; i<4; i++) {

		temp_weight = weights.at<double>(0, i);
		weight_vec.push_back(temp_weight);
		temp_mean.at<double>(0, 0) = means.at<double>(i, 0);
		temp_mean.at<double>(0, 1) = means.at<double>(i, 1);
		temp_mean.at<double>(0, 2) = means.at<double>(i, 2);
		temp_mean.at<double>(0, 3) = means.at<double>(i, 3);
		temp_mean.at<double>(0, 4) = means.at<double>(i, 4);
		temp_mean.convertTo(temp_mean, 6);
		means_vec.push_back(temp_mean.clone());
	}



}

void gmm_object::write_image(string path)
{

	imwrite(path, image);
}
