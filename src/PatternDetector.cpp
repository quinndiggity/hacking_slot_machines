

#include "PatternDetector.hpp"
#include <cmath>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <cassert>

PatternDetector::PatternDetector(cv::Ptr<cv::FeatureDetector> detector, 
	cv::Ptr<cv::DescriptorExtractor> extractor, 
	cv::Ptr<cv::DescriptorMatcher> matcher, 
    bool ratioTest)
    : m_detector(detector)
    , m_extractor(extractor)
    , m_matcher(matcher)
    , enableRatioTest(ratioTest)
    , homographyReprojectionThreshold(3) {
}


void PatternDetector::train(const Pattern& pattern) {
    m_pattern = pattern; // store the pattern object

    m_matcher->clear(); // clear old data

    // add vector of descriptors (each descriptors matrix describes one image)
    std::vector<cv::Mat> descriptors(1);
    descriptors[0] = pattern.descriptors.clone(); 
    m_matcher->add(descriptors);

    m_matcher->train(); // after adding train data perform actual train
}

void PatternDetector::buildPatternFromImage(const cv::Mat& image, Pattern& pattern) const {
    int numImages = 4;
    float step = sqrtf(2.0f);

    // store original image in pattern structure
    pattern.size = cv::Size(image.cols, image.rows);
    pattern.frame = image.clone();
    getGray(image, pattern.grayImg);
    
    // build 2d and 3d contours (3d contours lie in XY plane since it's planar)
    pattern.points2d.resize(4);
    pattern.points3d.resize(4);

    // image dimensions
    const float w = image.cols;
    const float h = image.rows;

    // normalized dimensions:
    const float maxSize = std::max(w,h);
    const float unitW = w / maxSize;
    const float unitH = h / maxSize;

    pattern.points2d[0] = cv::Point2f(0,0);
    pattern.points2d[1] = cv::Point2f(w,0);
    pattern.points2d[2] = cv::Point2f(w,h);
    pattern.points2d[3] = cv::Point2f(0,h);

    pattern.points3d[0] = cv::Point3f(-unitW, -unitH, 0);
    pattern.points3d[1] = cv::Point3f( unitW, -unitH, 0);
    pattern.points3d[2] = cv::Point3f( unitW,  unitH, 0);
    pattern.points3d[3] = cv::Point3f(-unitW,  unitH, 0);

    extractFeatures(pattern.grayImg, pattern.keypoints, pattern.descriptors);
}



bool PatternDetector::findPattern(const cv::Mat& image, cv::Mat& m_warpedImg, PatternTrackingInfo& info) {
    getGray(image, m_grayImg); // convert input image to gray
    
    // extract feature points from input gray image
    extractFeatures(m_grayImg, m_queryKeypoints, m_queryDescriptors);
    
    // get matches with current pattern
    getMatches(m_queryDescriptors, m_matches);

    // find homography transformation and detect good matches
    bool homographyFound = refineMatchesWithHomography(
        m_queryKeypoints, 
        m_pattern.keypoints, 
        homographyReprojectionThreshold, 
        m_matches, 
        m_roughHomography);

    if (homographyFound) {
        // warp image using found homography
        cv::warpPerspective(m_grayImg, m_warpedImg, m_roughHomography, m_pattern.size, cv::WARP_INVERSE_MAP | cv::INTER_CUBIC);

        // get refined matches:
        std::vector<cv::KeyPoint> warpedKeypoints;
        std::vector<cv::DMatch> refinedMatches;

		cv::imwrite("warped.jpg", m_warpedImg);

        // detect features on warped image
        extractFeatures(m_warpedImg, warpedKeypoints, m_queryDescriptors);

        // match with pattern
        getMatches(m_queryDescriptors, refinedMatches);

        // estimate new refinement homography
        homographyFound = refineMatchesWithHomography(
            warpedKeypoints, 
            m_pattern.keypoints, 
            homographyReprojectionThreshold, 
            refinedMatches, 
            m_refinedHomography);

        // get a result homography as result of matrix product of refined and rough homographies:
        info.homography = m_roughHomography * m_refinedHomography;

        // transform contour with precise homography
        cv::perspectiveTransform(m_pattern.points2d, info.points2d, info.homography);
    }
    return homographyFound;
}

void PatternDetector::getGray(const cv::Mat& image, cv::Mat& gray) {
    if (image.channels()  == 3)
        cv::cvtColor(image, gray, CV_BGR2GRAY);
    else if (image.channels() == 4)
        cv::cvtColor(image, gray, CV_BGRA2GRAY);
    else if (image.channels() == 1)
        gray = image;
}

bool PatternDetector::extractFeatures(const cv::Mat& image, std::vector<cv::KeyPoint>& keypoints, cv::Mat& descriptors) const {
    assert(!image.empty());
    assert(image.channels() == 1);

    m_detector->detect(image, keypoints);
    if (keypoints.empty())
        return false;

    m_extractor->compute(image, keypoints, descriptors);
    if (keypoints.empty())
        return false;

    return true;
}

void PatternDetector::getMatches(const cv::Mat& queryDescriptors, std::vector<cv::DMatch>& matches) {
    matches.clear();

    if (enableRatioTest) {
        // to avoid NaNs when best match has zero distance we will use inverted ratio
        const float minRatio = 1.f / 1.5f;
        
        // KNN match will return 2 nearest matches for each query descriptor
        m_matcher->knnMatch(queryDescriptors, m_knnMatches, 2);

		#pragma omp parallel for
        for (size_t i=0; i<m_knnMatches.size(); i++) {
            const cv::DMatch& bestMatch   = m_knnMatches[i][0];
            const cv::DMatch& betterMatch = m_knnMatches[i][1];

            float distanceRatio = bestMatch.distance / betterMatch.distance;
            
            // Pass only matches where distance ratio between 
            // nearest matches is greater than 1.5 (distinct criteria)
            if (distanceRatio < minRatio) {
                matches.push_back(bestMatch);
            }
        }
    }
    else {
        // Perform regular match
        m_matcher->match(queryDescriptors, matches);
    }
}

bool PatternDetector::refineMatchesWithHomography
    (
    const std::vector<cv::KeyPoint>& queryKeypoints,
    const std::vector<cv::KeyPoint>& trainKeypoints, 
    float reprojectionThreshold,
    std::vector<cv::DMatch>& matches,
    cv::Mat& homography
    ) {
    const int minNumberMatchesAllowed = 40;

    if (matches.size() < minNumberMatchesAllowed)
        return false;

    // prepare data for cv::findHomography
    std::vector<cv::Point2f> srcPoints(matches.size());
    std::vector<cv::Point2f> dstPoints(matches.size());

	#pragma omp parallel for
    for (size_t i = 0; i < matches.size(); i++) {
        srcPoints[i] = trainKeypoints[matches[i].trainIdx].pt;
        dstPoints[i] = queryKeypoints[matches[i].queryIdx].pt;
    }

    // Find homography matrix and get inliers mask
    std::vector<unsigned char> inliersMask(srcPoints.size());
    homography = cv::findHomography(srcPoints, 
                                    dstPoints, 
                                    CV_FM_RANSAC, 
                                    reprojectionThreshold, 
                                    inliersMask);

    std::vector<cv::DMatch> inliers;

	#pragma omp parallel for
    for (size_t i=0; i<inliersMask.size(); i++) {
        if (inliersMask[i])
            inliers.push_back(matches[i]);
    }

    matches.swap(inliers);
    return matches.size() > minNumberMatchesAllowed;
}
