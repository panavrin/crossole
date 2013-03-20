/*
 *  oscKinectGestures.h
 *  crossole
 *
 *  Created by Akito on 2/9/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "oscKinect.h"
#include "Constants.h"

#define NUM_GESTURES	 20
#define INERTIA			1

class oscKinectGestures {
	
public:
	oscKinectGestures();
	~oscKinectGestures();
	
	// initializations that were not done in constructor
	void setup();
	
	// Calls oscKinectListener.update() and does smoothing
	void update();
	
	// Draws all debug information
	void draw();
	
	// Draw cursors
	void DrawCursors();
	
	// Draw guidelines
	void DrawGuidelines();
	
	// Draw keyboard
	void DrawKeyboard();
	
	//  To detect when I move beyond a threshold
	bool MoveInto(unsigned int dimension, float relativeThreshold);
	
	//  DetectSnap()
	//  A snap is when I stretch out my hand and pull it back quickly.
	//  The snap is complete only when the limb is pulled back
	bool DetectSnap(unsigned int bodyPart, unsigned int dimension, float relativeThreshold, bool* GlobalGestureBool);
	
	//  DetectStretch()
	//  A stretch is when I move a limb into a certain region
	//  say I stretch my hand out to the right, or left and keep it there.
	bool DetectStretch(unsigned int bodyPart, unsigned int dimension, float relativeThreshold);
	
	//  DetectSwipe()
	//  A swipe is like a snap, only based on velocity - so speed is very important to trigger it
	bool DetectSwipe(unsigned int bodyPart, unsigned int dimension, float velocityThreshold);
	
	// GetCoords()
	double GetCoords(int bodyPart, int dimension);
	
	double GetAbsCoords(int bodyPart, int dimension);
	
	// Gesture detection
	bool SnapStart[NUM_JOINTS][NUM_DIMENSIONS];
	bool SwipeStart[NUM_JOINTS][NUM_DIMENSIONS];
	bool Stretch[NUM_JOINTS][NUM_DIMENSIONS];
	
private:
	
	bool block_create;
	
	// Normalizations
	double LegLength;
	
	// oscKinect Object
	oscKinect oscKinectListener;
};