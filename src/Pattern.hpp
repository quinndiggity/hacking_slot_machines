

#include "GeometryTypes.hpp"
#include "CameraCalibration.hpp"

struct Pattern { // store the image data and computed descriptors of target pattern
	cv::Size                  size;
	cv::Mat                   frame;
	cv::Mat                   grayImg;

	std::vector<cv::KeyPoint> keypoints;
	cv::Mat                   descriptors;

	std::vector<cv::Point2f>  points2d;
	std::vector<cv::Point3f>  points3d;
};

struct PatternTrackingInfo { // intermediate pattern tracking info structure
	cv::Mat                   homography;
	std::vector<cv::Point2f>  points2d;
	Transformation            pose3d;

	// compute pattern pose using PnP algorithm
	void computePose(const Pattern& pattern, const CameraCalibration& calibration);
};
