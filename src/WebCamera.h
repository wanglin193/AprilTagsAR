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
	bool init(int id_=0,int w=640,int h=480)
	{ 	 
		id = id_;
		cap.open(id);

		if( !cap.isOpened() )
		{
			std::cout << "Could not open WebCamera ...\n";
			return false;
		}
		    
		cap.set(CV_CAP_PROP_FRAME_WIDTH, w);
		cap.set(CV_CAP_PROP_FRAME_HEIGHT,h); 

		wid = w;
		hei = h;

		//printf("web cam size = %d x %d.\n",wid,hei);
		return true;
	}
	int getWid() { return wid; }
	int getHei() { return hei; }

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