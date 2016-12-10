add_executable(slotbot 
OCR.cpp
OCR.hpp
CameraCalibration.cpp
CameraCalibration.hpp
GeometryTypes.cpp
GeometryTypes.hpp
main.cpp
ARPipeline.hpp
ARPipeline.cpp
Pattern.cpp
Pattern.hpp
PatternDetector.cpp
PatternDetector.hpp
QuestionLookup.cpp
)
target_link_libraries( slotbot ${OpenCV_LIBRARIES} )
target_link_libraries ( slotbot lept tesseract)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OPENMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPENMP_CXX_FLAGS}")
install (TARGETS slotbot DESTINATION bin)
