

#include <tesseract/baseapi.h>
#include <opencv2/opencv.hpp>

class OCR { // class to handle a tesseract API object
	tesseract::TessBaseAPI tess;

	public:
	  OCR();
	  std::string get_text(cv::Mat& im);
};
