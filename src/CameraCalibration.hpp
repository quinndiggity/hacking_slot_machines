
#include <opencv2/opencv.hpp>

class CameraCalibration { // stores camera calibration info
		cv::Matx33f     m_intrinsic;
		cv::Mat_<float> m_distortion;

	public:
		CameraCalibration();
		CameraCalibration(float fx, float fy, float cx, float cy);
		CameraCalibration(float fx, float fy, float cx, float cy, float distortionCoeff[5]);

		void getMatrix34(float cparam[3][4]) const;

		const cv::Matx33f& getIntrinsic() const;
		const cv::Mat_<float>&  getDistortion() const;

		float& fx();
		float& fy();

		float& cx();
		float& cy();

		float fx() const;
		float fy() const;

		float cx() const;
		float cy() const;
};
