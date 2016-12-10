

#include "OCR.hpp"

OCR::OCR() {	// initialize tesseract
    const char* configs = "/slotbot/tessconfig";
	tess.SetVariable("tessedit_char_whitelist", "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890?'!\"£%()'.,=+-/:;\\#-<>");

    if (tess.Init(NULL, "eng", tesseract::OEM_DEFAULT)) {
      std::cout << "Could not initialize tesseract" << std::endl;
    }
    tess.ReadConfigFile(configs);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
};

std::string OCR::get_text(cv::Mat& im)
{
	// do some preprocessing
	cv::Mat g1, g2;
	cv::GaussianBlur(im, g1, cv::Size(7,7), 0);
	cv::GaussianBlur(im, g2, cv::Size(1,1), 0);
	im = g1 - g2;
	bitwise_not(im,im);
	
	const int EDGES_THRESHOLD = 247;
	cv::threshold(im, im, EDGES_THRESHOLD, 255, cv::THRESH_BINARY);

    // pass it to Tesseract API
    tess.SetImage((uchar*)im.data, im.cols, im.rows, 1, im.cols);
	char* answer = tess.GetUTF8Text();

	// get rid of the 2 question marks at end
	//answer[strlen(answer)-1]=0; 
	//answer[strlen(answer)-1]=0;

  	std::string answer_string = answer;

	// get rid of newline characters
	std::string newl = "\n";
	std::string::size_type i = answer_string.find(newl);

	if (i != std::string::npos)
	   answer_string.replace(i, newl.length(), " ");

	if (answer_string == "" or  answer_string == " " or answer_string == "  ") {
		answer_string = 'X';
	}

    return(answer_string);
}
