#include<iostream>
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\legacy\legacy.hpp>
#include<opencv2\ml\ml.hpp>
#include "gmm_object.h"
#include"dirent.h"



using namespace cv;
using namespace std;
gmm_object obj1;
const int KLD_THRESHOLD = 20;
string similar_path, notsimilar_path;
int count = 0, error_count = 0;








double klc(double weight1, Mat mean1, Mat cov1, double weight2, Mat mean2, Mat cov2) {
	cv::Mat diff = mean1 - mean2;

	double a = log(cv::determinant(cov2) / cv::determinant(cov1));
	double b = cv::trace(cov2.inv() * cov1)[0] - (double)5;
	cv::Mat c = diff * cov2.inv() * diff.t();
	return (0.5 * (a + b + c.at<double>(0, 0)));
}



int klmatch(double weight1,
	Mat means1,Mat cov1, std::vector<double>& weight2, std::vector<Mat>& means2, std::vector<Mat> cov2) {

	vector<double> klcouts;
	double temp_out;
	for (int i = 0; i < weight2.size(); ++i) {


		temp_out = klc(weight1, means1, cov1, weight2.at(i), means2.at(i), cov2.at(i)) - log(weight2.at(i));
		klcouts.push_back(temp_out);
		//call klc for every weight populate double vector
	}
	//get index of minimum var in vector
	std::vector<double>::iterator result = std::min_element(std::begin(klcouts), std::end(klcouts));
	int distance= std::distance(std::begin(klcouts), result);
	return distance;
}




double match_kl(std::vector<double>& weight1,
	std::vector<Mat>& means1, std::vector<Mat> cov1, std::vector<double>& weight2, std::vector<Mat>& means2, std::vector<Mat> cov2) {
	double sum = 0.0;
	int match = 0;
	double result = 0;
	for (int i = 0; i < weight1.size(); ++i) {
		match = klmatch(weight1.at(i), means1.at(i), cov1.at(i), weight2, means2, cov2); 
		result =   weight1.at(i)*(klc(weight1.at(i), means1.at(i), cov1.at(i), weight2.at(match), means2.at(match), cov2.at(match)) + log(weight1.at(i) / weight2.at(match)));
		sum += result;
	}


	return sum;
}




void Similarity_check(string check_path,string filename) {

	count++;
	double kldistance = 0;
	char ans;
	int flag = 0;
	gmm_object obj2;
	obj2.set_image(check_path);
	obj2.process_image();
	/*
	std::cout << "[MANUAL ANNOTATION]\t are images similar y or n?\n";
	std::cin >> ans;
	if (ans == 'Y' || ans == 'y')
	{
		flag = 1;
	}
	*/
	obj2.train_gmm();
	obj2.vectorise_model();
	kldistance = match_kl(obj1.weight_vec, obj1.means_vec, obj1.covs, obj2.weight_vec, obj2.means_vec, obj2.covs);
	std::cout << "[KL Distance]\t" << kldistance << "\n";
	if (kldistance < KLD_THRESHOLD) {
		std::cout << "[IMAGE SIMILAR]\t [WRITING TO SIMILAR FOLDER]\n\n\n\n";
		if(flag==0)
		error_count++;
		obj2.write_image(similar_path + "/" + filename);
	}
	else {

		std::cout << "[IMAGES NOT SIMILAR]\t [WRITING TO NOT SIMILAR FOLDER]\n\n\n\n";
		obj2.write_image(notsimilar_path + "/" + filename);
		if (flag == 1)
			error_count++;
	}

}



bool isDir(string dir)//FUNCTION USED FOR DIR WALKING NOT NEEDEED FOR DLL
{
	struct stat fileInfo;
	stat(dir.c_str(), &fileInfo);
	if (S_ISDIR(fileInfo.st_mode)) {
		return true;
	}
	else {
		return false;
	}
}




void listFiles(string baseDir, bool recursive)// FUNCTION USED TO WALK DIR
{
	std::cout << "\n\n------------------------------------------------------------------------------------------------\n[STARTING SIMILARITY CHECK]\n";
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(baseDir.c_str())) == NULL) {
		std::cout << "[ERROR: " << " ] Couldn't open " << baseDir << "." << std::endl;
		std::cin;
		return;
	}
	else {
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_name != string(".") && dirp->d_name != string("..")) {
				if (isDir(baseDir + dirp->d_name) == true && recursive == true) {
					
					listFiles(baseDir + dirp->d_name + "/", true);
				}
				else {
					std::string fileName = baseDir + "\/" + std::string(dirp->d_name);
					//std::cout << "FIlename\t" << dirp->d_name << "\n";
					//LOAD GMM OBJECT
					Mat rawimage = imread(fileName.c_str());
					if (rawimage.data == NULL)
					{   
						
						continue;
					}
					std::cout << "[FILE]\t" << baseDir<<"\/" << dirp->d_name << std::endl;
					
					Similarity_check(fileName.c_str(),dirp->d_name);

				}

				}
			}
		}
		closedir(dp);
	}


void create_dir(string querydir) {


	similar_path = querydir + "/SIMILAR";
	notsimilar_path = querydir + "/NOTSIMLAR";

	std::wstring w_sp = std::wstring(similar_path.begin(), similar_path.end());
	const wchar_t* sp = w_sp.c_str();
	_wmkdir(sp);


	std::wstring w_nsp = std::wstring(notsimilar_path.begin(), notsimilar_path.end());
	const wchar_t* nsp = w_nsp.c_str();
	_wmkdir(nsp);
}



int main() {
	
	

	Mat inputimage, samples;
	Mat lables, cont;
	string queryimage,querydir;
	
	std::cout << "Enter full path of query image  \n[EXAMPLE]\t C:\\Users\\Dhruv\\Documents\\Test\\CgQnXMJWQAABJzs.jpg \n";
	
	getline(std::cin, queryimage);

	std::cout << "Enter path of directory containing images to be checked \n[EXAMPLE]\t C:\\Users\\Dhruv\\Documents\\Test\n";
	getline(std::cin,querydir);
	
   
	create_dir(querydir);


	std::cout << "Path entered=" << queryimage << "\n";
	obj1.set_image(queryimage);
	obj1.process_image();
	obj1.train_gmm();
	obj1.vectorise_model();


	listFiles(querydir,false);
	
	
	
	/*std::cout << "[ACCURACY]\t" << (((float)count-(float)error_count) / (float)count) * 100 << "\n";
	std::cout << "[ALL IMAGES PROCESSED]\n[PRESS ANY KEY TO EXIT]\n";
	*/
	waitKey(0);
	return 0;
}
