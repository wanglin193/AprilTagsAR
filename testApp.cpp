#include "testApp.h"  
#include<glut.h>  

float tagsize = 162.5f;//mm

GLuint texid;
void initGLTexture()
{ 
	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);  
	glGenTextures (1, &texid);   
	glEnable(GL_DEPTH_TEST);
}
//--------------------------------------------------------------
void testApp::setup()
{   
	if( webCam.init(0)  )
	{
		cout<<"Init Web camera OK."<<endl; 
	}
	else
	{
		printf("Init Web camera error.\n");
		::exit(0);
	}

	Atag.init(AprilTags::tagCodes36h11);
	myfont.loadFont("NewMedia Fett.ttf",12); 

    if( readMatrix("data\\Kweb_cam.txt",Kweb_cam)==false )
	{
		Kweb_cam = Mat3::Zero(3,3);
		Kweb_cam(2,2) = 1.0;
		Kweb_cam(0,0)=Kweb_cam(1,1)=815.0; 
		Kweb_cam(0,2)=320;//webCam.getWid()/2;
		Kweb_cam(1,2)=240;//webCam.getHei()/2;
	} 
	cout<<Kweb_cam<<endl;
	//init AR pose
	ARpose = Mat4x4::Identity();  
	ARpose(2,3) = -2000;   
	initGLTexture(); 
} 
//--------------------------------------------------------------
void testApp::update()
{ 
	if( webCam.capture())
	{    
		Mat4x4 RT_ = ARpose;
		if( Atag.detectFirst( webCam.frame, Kweb_cam, tagsize, RT_ ) )
		{
			ARpose = RT_; 
			ARpose.row(1) = -ARpose.row(1) ;
			ARpose.row(2) = -ARpose.row(2) ; 
		}  
	}  
}
//--------------------------------------------------------------
ofRectangle testApp::drawBackgroundAuto(  )
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
	drawBackground( winbg );
	return winbg;
}
void testApp::drawBackground( ofRectangle& view_port )
{ 
	int wid = webCam.getWid();
	int hei = webCam.getHei();
	int ofwid = view_port.width ;
	int ofhei = view_port.height ; 
			
	glBindTexture (GL_TEXTURE_2D, texid); 
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  

	if(webCam.getData() != NULL) 
	{	 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		{
			//gluOrtho2D(0.0, ofwid , ofhei,0);
			glMatrixMode(GL_MODELVIEW); 
			glPushMatrix(); 
			glEnable(GL_TEXTURE_2D); 
			glTexImage2D(GL_TEXTURE_2D, 0, 3, wid, hei, 0, GL_BGR, GL_UNSIGNED_BYTE,webCam.getData());
			{ 
				glBegin(GL_QUADS); 
				{
					glTexCoord2i(0, 0); glVertex2i(view_port.x,view_port.y);
					glTexCoord2i(1, 0); glVertex2i(view_port.x+view_port.width,view_port.y);
					glTexCoord2i(1, 1); glVertex2i(view_port.x+view_port.width,view_port.y+view_port.height);
					glTexCoord2i(0, 1); glVertex2i(view_port.x,view_port.y+view_port.height);
				}
				glEnd();
			}
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
		}
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	} 
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
	//draw Canvas
	ofRectangle view_port = ofGetCurrentViewport();	 
	view_port = drawBackgroundAuto( );  

	char fpsStr[255]; 
	sprintf(fpsStr, "fps: %4.1f", ofGetFrameRate());
	myfont.setLineHeight(6);
	myfont.drawString(fpsStr, ofGetViewportWidth()-100,  ofGetViewportHeight()-10); 

	ofDisableLighting();   
	mycam.begin( view_port );  
	{ 	
		setFrustum( Kweb_cam, webCam.getWid(), webCam.getHei(), 50, 9999 );

		glLoadMatrixf( (float*)(ARpose.data()) );//raw opengl
		ofDrawGrid( tagsize/2.0, 4, false, false, false, true );
		ofDrawAxis( tagsize );

		light.enable();
		light.setPosition(1000,1000,3000); 		

		ofBox(0,0,tagsize/2,tagsize);
		//ofCone(0,0,0,tagsize/2,tagsize);
		//ofSphere(0,0,tagsize/2,tagsize/2);

		light.disable();
	} 
	mycam.begin();
}   
void testApp::keyPressed( int key )
{
	switch( key )
	{ 
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
