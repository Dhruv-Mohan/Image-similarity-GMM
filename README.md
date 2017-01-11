#Image-similarity-GMM #
The project checks the similarity of a query image against a set of images by calculating the KL divergence of the Gaussian mixture models of the images.
##Requirements##
       
1. Image Similarity
  1. Visual Studio 2013
  2. Opencv 2.4.10
2. Image Grabber
  1. json
  2. tweepy
  3. wget
  
The VS project looks for opencv at "C\opencv\build"

##Program Flow##

Upon initialization, the user is prompted to enter the path of the query image and the folder containing the test image set. The query image is scaled and it's Gaussian Mixture Model(GMM) created. 

The program then walks through the test set directory, reads an image and creates the image's GMM. The KL divergence of the query image and the test image is calculated and the test image is classified as either SIMILAR or NOTSIMILAR and written into the respective folder.

##Additional Notes##
For the collection of images I've written a small python script that collectes a set number of images off twitter with a specitic tag, the script can be found in the Twitter image grabber folder

The Gaussian mixture model presently is trained on 5 features

1. The three slices of an Lab colorspace image
2. The corresponding X and Y coordinates of a pixel

The idea behind that was that similar pixels should be closer to eachother, more features can be easily added by editing the train_gmm() function in gmm_object.cpp

To accomodate a new feature matrix the function will change from  
```c++
void gmm_object::train_gmm() {

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

```
to
```c++
void gmm_object::train_gmm() {
 Mat new_feature(processed_image.rows*processed_image.cols, 1, CV_32FC1); //Example new feature matrix
	Mat final_samples(processed_image.rows*processed_image.cols, 6, CV_32FC1); //Additional column added 
	processed_image.reshape(1, processed_image.rows*processed_image.cols).convertTo(samples, CV_32FC1);
	int totalcount = 0;
	for (int y = 0; y<processed_image.rows; y++) {
		for (int x = 0; x<processed_image.cols; x++) {

			final_samples.at<float>(totalcount, 0) = samples.at<float>(totalcount, 0);
			final_samples.at<float>(totalcount, 1) = samples.at<float>(totalcount, 1);
			final_samples.at<float>(totalcount, 2) = samples.at<float>(totalcount, 2);
			final_samples.at<float>(totalcount, 3) = x;
			final_samples.at<float>(totalcount, 4) = y;
      final_samples.at<float>(totalcount, 5) = new_feature.at<float>(totalcount,0); 
			totalcount++;
		}
	}

```

