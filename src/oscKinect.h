/*
 *  oscKinect.h
 *  crossole
 *
 *  Created by Avinash on 2/7/11.
 *  Copyright 2011 GTCMT. All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxOsc.h"

// listen on port 7110
#define PORT 7110
#define NUM_MSG_STRINGS 40
#define NUM_JOINTS 17
#define NUM_DIMENSIONS 3

// joint definitions
#define HEAD 0
#define NECK 1
#define TORSO 2
#define R_SHOULDER 3
#define R_ELBOW 4
#define R_HAND 5
#define L_SHOULDER 6
#define L_ELBOW 7
#define L_HAND 8
#define R_KNEE 9
#define R_ANKLE 10
#define R_FOOT 11
#define L_KNEE 12
#define L_ANKLE 13
#define L_FOOT 14
#define R_HIP 15
#define L_HIP 16

// Dimension Defs
#define X_dim	0
#define Y_dim	1
#define Z_dim	2

// Smoothing limit
#define SMOOTHING_LIMIT	10

//--------------------------------------------------------
class oscKinect {
	
public:

	oscKinect();
	~oscKinect();
	
	void setup();
	void update();
	void draw();
	
	// Keep these public so we can access them from outside
	
	double ArmLength;
	
	// Absolute measurements
	float Coords[NUM_JOINTS][NUM_DIMENSIONS];
	
	// Relative to Torso measurements
	float CoordsRel[NUM_JOINTS][NUM_DIMENSIONS];
	float CoordsPrev[NUM_JOINTS][NUM_DIMENSIONS];
	
	// Smoothing for the 2 hands
	float RHSmooth[SMOOTHING_LIMIT][NUM_DIMENSIONS];
	float LHSmooth[SMOOTHING_LIMIT][NUM_DIMENSIONS];
	
	void SmoothHands();
	
	// Velocity
	float CoordsVel[NUM_JOINTS][NUM_DIMENSIONS];
	float CoordsPrevVel[NUM_JOINTS][NUM_DIMENSIONS];
	
	// Acceleration
	float CoordsAccn[NUM_JOINTS][NUM_DIMENSIONS];
	
	ofTrueTypeFont		font;
	
private:
	
	void initialize();
	void parseJoints();
	int mapJointToInt(string Joint);
	
	void GetAbsolute( int Joint, float X, float Y, float Z);
	void GetRelative(int Joint);
	void GetVelocity(int Joint);
	void GetAcceleration(int Joint);
	
	ofxOscReceiver	receiver;
	
	int				current_msg_string;
	string		msg_strings[NUM_MSG_STRINGS];
	float			timers[NUM_MSG_STRINGS];
	
	// Message strings and ints
	string joint;
	int userID;
	
	// scaling
	float x_scaling;
	float y_scaling;
	float z_scaling;
	
	// head
	string head;

	// neck
	string neck;
	
	// torso
	string torso;

	// r_shoulder
	string r_shoulder;

	// r_elbow
	string r_elbow;

	// r_hand
	string r_hand;
	
	// l_shoulder
	string l_shoulder;
	
	// l_elbow
	string l_elbow;
	
	// l_hand
	string l_hand;
	
	// r_knee
	string r_knee;
	
	// r_ankle
	string r_ankle;

	
	// r_foot
	string r_foot;

	// l_knee
	string l_knee;

	// l_ankle
	string l_ankle;

	
	// l_foot
	string l_foot;

	// r_hip
	string r_hip;

	// l_hip
	string l_hip;

	};
