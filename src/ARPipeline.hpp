
#include "PatternDetector.hpp"
#include "GeometryTypes.hpp"

class ARPipeline {
		CameraCalibration   m_calibration;
		Pattern             m_pattern;
		PatternTrackingInfo m_patternInfo;

	public:
		ARPipeline(const cv::Mat& patternImage, const CameraCalibration& calibration);
		bool processFrame(const cv::Mat& inputFrame, cv::Mat& m_warpedImg);
		const Transformation& getPatternLocation() const;
		PatternDetector     m_patternDetector;
};
