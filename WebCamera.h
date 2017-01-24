#ifndef __WEB_CAMERA_HPP__
#define __WEB_CAMERA_HPP__

using namespace cv;
struct WebCamera
{		
	cv::VideoCapture cap; 	
	cv::Mat frame;

	int id;
	int wid;
	int hei;
	WebCamera():wid(640),hei(480) {  }
	~WebCamera() 
	{ 
		cap.release();
	}
	bool init(int id_=0)
	{ 	 
		id = id_;
		cap.open(id);

		if( !cap.isOpened() )
		{
			std::cout << "Could not open WebCamera ...\n";
			return false;
		}
		/*
		//too dark can cause fail	 
		if(!cap.read(frame))
		{
		std::cout << "Could not read from WebCamera in cap.init().\n"; 
		return false;
		}

		hei = frame.rows;
		wid = frame.cols;
		*/
		//printf("web cam size = %d x %d.\n",wid,hei);
		return true;
	}	
	int getWid() { return frame.cols; }
	int getHei() { return frame.rows; }

	unsigned char * getData()
	{
		return (unsigned char *)(frame.data);
	}

	bool capture()
	{ 
		if(!cap.read(frame))//cap>>frame;
		{
			//	std::cout << "Could not read from WebCamera in cap.capture().\n";
			return false;
		}
		if( frame.empty() )
		{ 
			return false;
		}
		//frame.copyTo(image);
		//cvtColor(image, gray, COLOR_BGR2GRAY);
		return true;
	}

	void show()
	{		
		char name[20];
		sprintf(name,"Web camera %d",id);
		imshow( name, frame );
	}
};
#endif