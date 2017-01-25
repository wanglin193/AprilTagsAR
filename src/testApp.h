#pragma once

#include "ofMain.h" 
#include "matrix_io.h"

#include "AprilTagsDetection.h" 
#include "WebCamera.h"
 
class testApp : public ofBaseApp
{
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		testApp( int argc, char **argv )  
		{ 			 
		} 
 
		ArtagDetect Atag;
		unsigned int AtagBitFlag;
		Mat4x4 ARpose;
		WebCamera webCam;
		Mat3 Kweb_cam;

		ofTrueTypeFont myfont;  
		ofCamera mycam;
		ofImage bgim;
		ofMesh bgmesh;//static background mesh
 
		ofLight light; 
		
		void setup();
		void update();
		void draw();
		void drawBackground(ofRectangle& view_port);
		ofRectangle getRectBG( );
 
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
};

