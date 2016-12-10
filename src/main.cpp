

#include "ARPipeline.hpp"
#include "OCR.hpp"
#include "QuestionLookup.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

int main(int argc, const char * argv[])
{
	// define objects and variables
	OCR ocr; // tesseract OCR object
	cv::VideoCapture capture;
	cv::Mat testImage;
	cv::Mat m_warpedImg;
	cv::Mat Q, A1, A2, A3, A4;
	std::string Q_text, A1_text, A2_text, A3_text, A4_text;

	// define prototype parameters - box heights and widths
	int qwidth = 455;
	int qheight = 80;
	int awidth = 203;
	int aheight = 46;

	// define prototype parameters - (x,y) of top-left corner
	int qx = 198;
	int qy = 193;
	int a1x = 193;
	int a1y = 301;
	int a2x = 451;
	int a2y = 301;
	int a3x = 193;
	int a3y = 379;
	int a4x = 451;
	int a4y = 379;

	// get corresponding prototype parameter segments
	cv::Rect qsegment  = cvRect( qx, qy,qwidth,qheight);
	cv::Rect a1segment = cvRect(a1x,a1y,awidth,aheight);
	cv::Rect a2segment = cvRect(a2x,a2y,awidth,aheight);
	cv::Rect a3segment = cvRect(a3x,a3y,awidth,aheight);
	cv::Rect a4segment = cvRect(a4x,a4y,awidth,aheight);

    // camera calibration object - needs to be redone if new camera
    CameraCalibration calibration(526.58037684199849f, 524.65577209994706f, 
								  318.41744018680112f, 202.96659047014398f);

	// load the prototype/pattern image
	cv::Mat patternImage = cv::imread("/slotbot/images/prototype1.jpg");
	if (patternImage.empty()) {
		std::cout << "Unable to load pattern image!" << std::endl;
		return(1);
	}

	testImage = cv::imread("/slotbot/images/pi_frame.jpg", CV_LOAD_IMAGE_COLOR);
	if (!testImage.empty()) {
	    ARPipeline pipeline(patternImage, calibration);
		bool goodmatch = pipeline.processFrame(testImage, m_warpedImg);
		if (goodmatch) {
			// crop the warped image to the questions and answers
			m_warpedImg( qsegment).copyTo(Q);
			m_warpedImg(a1segment).copyTo(A1);
			m_warpedImg(a2segment).copyTo(A2);
			m_warpedImg(a3segment).copyTo(A3);
			m_warpedImg(a4segment).copyTo(A4);

			//cv::imwrite("q_before.jpg", Q);

			Q_text = ocr.get_text(Q);
			A1_text = ocr.get_text(A1);
			A2_text = ocr.get_text(A2);
			A3_text = ocr.get_text(A3);
			A4_text = ocr.get_text(A4);
					
			QuestionLookup *qlookup = new QuestionLookup();
			std::string answertext = qlookup->getAnswer(Q_text, A1_text, A2_text, A3_text, A4_text);
			std::cout << answertext;
			delete qlookup;
			qlookup = NULL;
		}

		else {
			std::cout << "Dunno mate";
		}

	}
    return 0;
}
