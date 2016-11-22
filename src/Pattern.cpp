

#include "Pattern.hpp"

void PatternTrackingInfo::computePose(const Pattern& pattern, const CameraCalibration& calibration) {
  cv::Mat Rvec;
  cv::Mat_<float> Tvec;
  cv::Mat raux,taux;

  cv::solvePnP(pattern.points3d, points2d, calibration.getIntrinsic(), calibration.getDistortion(),raux,taux);
  raux.convertTo(Rvec,CV_32F);
  taux.convertTo(Tvec ,CV_32F);

  cv::Mat_<float> rotMat(3,3); 
  cv::Rodrigues(Rvec, rotMat);

  // copy to transformation matrix
  for (int col=0; col<3; col++) {
    for (int row=0; row<3; row++) {        
     pose3d.r().mat[row][col] = rotMat(row,col); // copy rotation component
    }
    pose3d.t().data[col] = Tvec(col); // copy translation component
  }
  // solvePnP finds camera location w.r.t. marker pose so we must invert this
  pose3d = pose3d.getInverted();
}
