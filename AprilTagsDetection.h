#pragma once
#include "common.h"

#include "AprilTags/TagDetector.h"
#include "AprilTags/Tag36h11.h" 

#ifdef _DEBUG
#pragma comment(lib, "AprilTagd.lib") 
#else
#pragma comment(lib, "AprilTag.lib") 
#endif

//tagID in a image <32
#define MAXID 31

class ArtagDetect
{	  
	cv::Mat image_gray;	 

public:
	 
	AprilTags::TagDetector* m_tagDetector;
	AprilTags::TagCodes m_tagCodes;
	std::vector<AprilTags::TagDetection> detections; 
	unsigned int flag_detect;//each bit hold a detect flag

	ArtagDetect():m_tagCodes(AprilTags::tagCodes36h11)
	{
		m_tagDetector = NULL;
		flag_detect = 0;
	}
	void init( AprilTags::TagCodes m_tagCodes_ )
	{		 		
		m_tagCodes = m_tagCodes_;
		m_tagDetector = new AprilTags::TagDetector(m_tagCodes);
		flag_detect = 0; 
	} 
 
	bool detectFirst( cv::Mat& image,Mat3& K, float m_tagSize, Mat4x4& RT )
	{  
		cv::cvtColor(image, image_gray, CV_BGR2GRAY);     
		detections  = m_tagDetector->extractTags(image_gray);  	
		 
    	if( detections.size()>0 )
		{ 
			Eigen::Matrix4d rtxd = detections[0].getRelativeTransform( m_tagSize,K(0,0),K(1,1),K(0,2),K(1,2) );
			RT = rtxd.cast<datatype>();
			return true;
		}
		else 
			return false;
	} 
	 
};
 
	
	