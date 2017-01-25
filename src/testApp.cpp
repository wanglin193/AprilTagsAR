#include "testApp.h"  
#include<glut.h>  

float tagsize = 162.5f;//mm
float scale = 1.0f;
float angle = 0.0f; 

void draw_help( int x,int y )
{
	ofPushStyle();
	glDepthFunc(GL_ALWAYS); // draw on top of everything

	ofSetColor(255, 255, 255);
	 
	int hei=18;
	int py=y;
	
	ofDrawBitmapString("Press 'f' to toggle full screen", x, py);
	ofDrawBitmapString("Press keys UP/DOWN to change scale", x, py+=hei); 
	ofDrawBitmapString("Press keys LEFT/RIGHT to rotate", x, py+=hei); 

	glDepthFunc(GL_LESS);
	ofPopStyle();
}

//default K
void setK( Mat3& Kweb_cam,int imw,int imh )
{
	Kweb_cam = Mat3::Zero(3,3);
	Kweb_cam(2,2) = 1.0;
	Kweb_cam(0,0) = Kweb_cam(1,1) = 0.8*imw; 
	Kweb_cam(0,2) = imw/2;
	Kweb_cam(1,2) = imh/2;
}
//--------------------------------------------------------------
void testApp::setup()
{   
	if( webCam.init(0,640,480)  )
	{
		cout<<"Init Web camera OK."<<endl; 
	}
	else
	{
		cout<<"Init Web camera error."<<endl; 
		::exit(0);
	}

	if( readMatrix("data\\Kcam_640_480.txt",Kweb_cam) )
	{
		if(webCam.getWid()!=640)
		{
			cout<<"Scale K to camera frame size."<<endl; 
			Kweb_cam = Kweb_cam * ((float)webCam.getWid()/640.0);
			Kweb_cam(2,2) = 1.0;
		}
	}
	else
	{
		setK(Kweb_cam, webCam.getWid(),webCam.getHei());
	}
	cout<<"K:\n"<<Kweb_cam<<endl;
	
	bgim.allocate( webCam.getWid(),webCam.getHei(), OF_IMAGE_COLOR );

	Atag.init(AprilTags::tagCodes36h11); 
	//init AR pose
	ARpose = Mat4x4::Identity();  
	ARpose(2,3) = -2000;   
} 
//--------------------------------------------------------------
void testApp::update()
{ 
	if( webCam.capture())
	{    
		//set back ground 
		bgim.setFromPixels( webCam.getData(),webCam.getWid(),webCam.getHei(),OF_IMAGE_COLOR, false/*BGR*/ );

		//detect marker pose
		Mat4x4 RT_ = ARpose;
		if( Atag.detectFirst( webCam.frame, Kweb_cam, tagsize, RT_ ) )
		{
			ARpose = RT_; 
			//to GL space
			ARpose.row(1) = -ARpose.row(1) ;
			ARpose.row(2) = -ARpose.row(2) ; 
		}  
	}  
}
//--------------------------------------------------------------
ofRectangle testApp::getRectBG(  )
{ 
	int wid = webCam.getWid();
	int hei = webCam.getHei();
	int ofwid = ofGetViewportWidth();
	int ofhei = ofGetViewportHeight() ;

	ofRectangle winbg(0,0,ofwid,ofhei); 

	float sx = (float)ofwid/(float)wid;
	float sy = (float)ofhei/(float)hei;

	if(sx>sy)
	{
		winbg.width=(float)wid*sy; 
		winbg.x+=(ofwid - winbg.width)/2;
	}
	else
	{
		winbg.height=(float)hei*sx; 
		winbg.y+=(ofhei- winbg.height)/2;
	}  	
	return winbg;
}
void testApp::drawBackground( ofRectangle& view_port )
{ 	
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	ofDisableLighting();
	glDisable(GL_DEPTH_TEST);

	bgim.draw(view_port);
	
	glEnable(GL_DEPTH_TEST);
	ofEnableLighting(); 
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW); 
} 
void setFrustum( Mat3& K, float imWid, float imHei, float znear,float zfar )
{
	//Znear could be smaller than fol,so scale is needed
	float scalex = znear/K(0,0);//fol x
	float scaley = znear/K(1,1);//fol y

	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	glFrustum( -K(0,2)*scalex, (imWid-K(0,2))*scalex, -K(1,2)*scaley, (imHei-K(1,2))*scaley, znear, zfar );  
	glMatrixMode(GL_MODELVIEW); 
} 
void testApp::draw()
{  
	ofBackground(0,0,0);  

	ofRectangle view_port = ofGetCurrentViewport();	 
	
	view_port = getRectBG( );  
	drawBackground( view_port );
	
	ofDisableLighting();  
	mycam.begin( view_port );  
	{ 	
		setFrustum( Kweb_cam, webCam.getWid(), webCam.getHei(), 50, 9999 );
		ofLoadMatrix((float*)(ARpose.data()));//glLoadMatrixf( (float*)(ARpose.data()) );
		ofDrawGrid( tagsize/2.0, 4, false, false, false, true );
		ofDrawAxis( tagsize );

		light.enable();
		light.setPosition(1000,1000,3000); 	
		
		ofScale(scale,scale,scale);	//glScalef(downscale,downscale,downscale);
		ofRotateZ(angle);	

		ofBox(0,0,tagsize/2,tagsize);
		//ofCone(0,0,0,tagsize/2,tagsize);
		//ofSphere(0,0,tagsize/2,tagsize/2);

		light.disable();
	}
	mycam.end();
	ofEnableLighting();	
 
	ofDisableLighting();  
	draw_help( 10, ofGetViewportHeight()-60 ); 
	ofDrawBitmapString("fps: " + ofToString( ofGetFrameRate()), ofGetViewportWidth()-100,ofGetViewportHeight()-15);	
	ofEnableLighting(); 	
}   
void testApp::keyPressed( int key )
{
	switch( key )
	{ 
	case OF_KEY_UP: 
		scale*=1.02;
		break;

	case OF_KEY_DOWN: 
		scale/=1.02;
		break;

	case OF_KEY_LEFT: 
		angle+=5;
		break;

	case OF_KEY_RIGHT: 
		angle-=5;
		break;

	case 'f':
		ofToggleFullscreen();
		break;

	default:
		break;
	}
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){ }
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){}
//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){}
//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){}
