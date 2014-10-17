#pragma once

// Definitions
#define ADD_PARTICLES // Comment this line out if you don't want to add particle-effects.
#define MAX_NUM_HANDS	4
#define FIRST_PI		0.0
#define SECOND_PI		TWO_PI/3
#define THIRD_PI		2*TWO_PI/3

// Addons
#include "ofMain.h"
#include "ofxFluid.h"
#include "ofxOpenNI.h"
#ifdef ADD_PARTICLES
	#include "ofxParticles.h"
#endif

// ofApp class
class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	// ofxOpenNI
	ofxOpenNI kinect;

	// ofxFluid
	ofxFluid fluid;
	ofPoint oldM[MAX_NUM_HANDS];

	// ofxParticles
#ifdef ADD_PARTICLES
	ofxParticleSystem particleSystem;
	ofxParticleEmitter handEmitter[MAX_NUM_HANDS];
	ofPoint oldD[MAX_NUM_HANDS];
#endif
};
