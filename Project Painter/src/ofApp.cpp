#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	// setup oF
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetBackgroundColor(0);
	ofSetFrameRate(60);
	ofHideCursor();
	
	// setup for fluid
	ofEnableAlphaBlending();
	ofSetCircleResolution(100);
	
	fluid.allocate(ofGetWidth(), ofGetHeight());
	fluid.dissipation = 0.99;
	fluid.velocityDissipation = 0.99;
	fluid.setGravity(ofVec2f(0.0, 0.0));
	
	// setup for particles
#ifdef ADD_PARTICLES
	for (int i = 0; i < MAX_NUM_HANDS; i++)
	{
		handEmitter[i].velSpread = ofVec3f(25.0, 25.0);
		handEmitter[i].life = 10.0;
		handEmitter[i].lifeSpread = 5.0;
		handEmitter[i].numPars = 10;
		handEmitter[i].color = ofColor(200, 200, 255);
		handEmitter[i].colorSpread = ofColor(20, 20, 0);
		handEmitter[i].size = 32;
		oldM[i] = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
		oldD[i] = ofPoint(ofGetWidth()/2, ofGetHeight()/2);
	}
#endif
	
	// setup Kinect
	kinect.setup();
	
	kinect.addDepthGenerator();
	kinect.addHandsGenerator();
	kinect.addAllHandFocusGestures();
	
	kinect.setMaxNumHands(MAX_NUM_HANDS);
	kinect.setMirror(true);
	
	kinect.start();
}

//--------------------------------------------------------------
void ofApp::update()
{
	fluid.update();
	kinect.update();
	
#ifdef ADD_PARTICLES
	particleSystem.update(min(ofGetLastFrameTime(), 0.1), 0.5);
#endif
}

//--------------------------------------------------------------
void ofApp::draw()
{
	for (int i = 0; i < kinect.getNumTrackedHands(); i++)
	{
		ofPoint handPosition = kinect.getTrackedHand(i).getPosition();
		
		float handPositionX = ofMap(handPosition.x, 0, kinect.getWidth (), 0, ofGetWidth ());
		float handPositionY = ofMap(handPosition.y, 0, kinect.getHeight(), 0, ofGetHeight());
		
		ofPoint m = ofPoint(handPositionX, handPositionY);
		ofPoint d = (m - oldM[i]) * 10.0;
		
		float r = 0.5 + sin(ofGetElapsedTimef()+ FIRST_PI) / 2;
		float g = 0.5 + sin(ofGetElapsedTimef()+SECOND_PI) / 2;
		float b = 0.5 + sin(ofGetElapsedTimef()+ THIRD_PI) / 2;
		
		fluid.addTemporalForce(m, d, ofFloatColor(r, g, b, 0.8f), 7.0f);
		
#ifdef ADD_PARTICLES
		handEmitter[i].setPosition(oldM[i], m);
		handEmitter[i].posSpread = ofVec3f(10.0, 10.0, 0.0);
		handEmitter[i].setVelocity(oldD[i], d);
		particleSystem.addParticles(handEmitter[i]);
		oldD[i] = d;
#endif
		
		oldM[i] = m;
	}
	
	fluid.draw();
	
#ifdef ADD_PARTICLES
	particleSystem.draw();
#endif
}

//--------------------------------------------------------------
void ofApp::exit()
{
	kinect.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key ==  27) exit(); // Press [esc] to exit.
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}