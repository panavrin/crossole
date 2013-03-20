/*
 *  oscKinect.cpp
 *  crossole
 *
 *  Created by Akito on 2/7/11.
 *  Copyright 2011 GTCMT. All rights reserved.
 *
 */

#include "oscKinect.h"

oscKinect::oscKinect()
{
	
}

oscKinect::~oscKinect()
{
};

//--------------------------------------------------------------
void oscKinect::setup(){
	
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
	current_msg_string = 0;
	
	initialize();
}

//--------------------------------------------------------------
void oscKinect::initialize(){
	
	// Initialize the strings for each of the OSC dividers
	ArmLength = 0;
	joint = "/joint";
	userID = 0;
	
	// head
	head = "head";
	
	// neck
	neck = "neck";

	// torso
	 torso = "torso";
	
	// r_shoulder
	 r_shoulder = "r_shoulder";
	
	// r_elbow
	r_elbow = "r_elbow";
	
	// r_hand
	r_hand = "r_hand";

	// l_shoulder
	 l_shoulder = "l_shoulder";

	// l_elbow
	 l_elbow = "l_elbow";

	// l_hand
	 l_hand = "l_hand";

	// r_knee
	 r_knee = "r_knee";

	// r_ankle
	 r_ankle = "r_ankle";
	
	// r_foot
	 r_foot = "r_foot";
	
	// l_knee
	 l_knee = "l_knee";

	// l_ankle
	 l_ankle = "l_ankle";

	// l_foot
	 l_foot = "l_foot";
	
	// r_hip
	 r_hip = "r_hip";

	// l_hip
	 l_hip = "l_hip";
	
	
	// Set the scaling for the axes
	x_scaling = 2.0;
	y_scaling = 5.0;
	z_scaling = 1;
	
	
	
	// initialize coords
	for (int i = 0; i < NUM_JOINTS; i++) {
		Coords[i][0]=0;
		Coords[i][1]=0;
		Coords[i][2]=0;
		
		CoordsRel[i][0]=0;
		CoordsRel[i][1]=0;
		CoordsRel[i][2]=0;		
	}
}

// Function to receive and screen the co-ords of the joints
void oscKinect::parseJoints(){
	float X = 0;
	float Y = 0;
	float Z = 0;
	int Joint = -1;
	
	while ( receiver.hasWaitingMessages() ) {
		
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		// look for the /joint message
		if (m.getAddress() == "/joint")
		{
			// arguments are s i f f f
			// s = string JointName
			// i = int userID
			// f = float X-coord
			// f = float Y-coord
			// f = float Z-coord
			
			// get the name of the joint
			joint = m.getArgAsString( 0 );
			userID = m.getArgAsInt32( 1 );
			
			X = m.getArgAsFloat(2);
			Y = m.getArgAsFloat(3);
			Z = m.getArgAsFloat(4);
			
			Joint = mapJointToInt(joint);
			
			if (Joint >= 0)
			{
				
			// Get Absolute measurements
			GetAbsolute(Joint, X, Y, Z);
			
			// Get Relative measurements
			GetRelative(Joint);
			
			// Get Velocity measurements
			// GetVelocity(Joint);
			
			// Get Accn
			// GetAcceleration(Joint);
				
			if ((Joint == R_HAND))
			{
				ArmLength = 1.0;
				// ArmLength = (pow(pow( (double)Coords[R_SHOULDER][X_dim]-(double)Coords[R_ELBOW][X_dim], (double)2 ) + pow((double)Coords[R_SHOULDER][Y_dim]-(double)Coords[R_ELBOW][Y_dim], (double)2) + pow((double)Coords[R_SHOULDER][Z_dim]-(double)Coords[R_ELBOW][Z_dim], (double)2), (double)0.5)) + (pow(pow((double)Coords[R_HAND][X_dim]-(double)Coords[R_ELBOW][X_dim], (double)2) + pow((double)Coords[R_HAND][Y_dim]-(double)Coords[R_ELBOW][Y_dim], (double)2) + pow((double)Coords[R_HAND][Z_dim] - (double)Coords[R_ELBOW][Z_dim], (double)2), (double)0.5)) + (pow(pow((double)Coords[R_SHOULDER][X_dim]-(double)Coords[TORSO][X_dim], (double)2) + pow((double)Coords[R_SHOULDER][Y_dim]-(double)Coords[TORSO][Y_dim], (double)2) + pow((double)Coords[R_SHOULDER][Z_dim] - (double)Coords[TORSO][Z_dim], (double)2), (double)0.5));
			}
				
			}
		}
		else {
			// unrecognized message: display on the bottom of the screen
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for ( int i=0; i<m.getNumArgs(); i++ )
			{
				// get the argument type
				msg_string += m.getArgTypeName( i );
				msg_string += ":";
				// display the argument - make sure we get the right type
				if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
					msg_string += ofToString( m.getArgAsInt32( i ) );
				else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
					msg_string += ofToString( m.getArgAsFloat( i ) );
				else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
					msg_string += m.getArgAsString( i );
				else
					msg_string += "unknown";
			}
			// add to the list of strings to display
			msg_strings[current_msg_string] = msg_string;
			timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
			current_msg_string = ( current_msg_string + 1 ) % NUM_MSG_STRINGS;
			// clear the next line
			msg_strings[current_msg_string] = "";
		}

	}
}

//  Function to smooth the coords of the two hands
void oscKinect::SmoothHands()
{
	double AvgRH[3] = {0};
	
	// Right hand
	for (int i = 1; i < SMOOTHING_LIMIT; i++) {
		RHSmooth[i][X_dim] = RHSmooth[i-1][X_dim];
		RHSmooth[i][Y_dim] = RHSmooth[i-1][Y_dim];
		RHSmooth[i][Z_dim] = RHSmooth[i-1][Z_dim];
	}
	
	RHSmooth[0][X_dim] = Coords[R_HAND][X_dim];
	RHSmooth[0][Y_dim] = Coords[R_HAND][Y_dim];
	RHSmooth[0][Z_dim] = Coords[R_HAND][Z_dim];
	
	for (int i = 0; i < SMOOTHING_LIMIT; i++) {
		AvgRH[X_dim] += RHSmooth[i][X_dim]/SMOOTHING_LIMIT;
		AvgRH[Y_dim] += RHSmooth[i][Y_dim]/SMOOTHING_LIMIT;
		AvgRH[Z_dim] += RHSmooth[i][Z_dim]/SMOOTHING_LIMIT;
	}
	
	CoordsRel[R_HAND][X_dim] = AvgRH[X_dim] - Coords[TORSO][X_dim] - 0.25;
	CoordsRel[R_HAND][Y_dim] = AvgRH[Y_dim] - Coords[TORSO][Y_dim] + 0.25;
	CoordsRel[R_HAND][Z_dim] = AvgRH[Z_dim] - Coords[TORSO][Z_dim];	
}

void oscKinect::GetAbsolute( int Joint, float X, float Y, float Z)
{
		Coords[Joint][0] = x_scaling*X;
		Coords[Joint][1] = y_scaling*Y;
		Coords[Joint][2] = z_scaling*Z;

}

void oscKinect::GetRelative(int Joint)
{
	// Measurements relative to torso
	// Subtract everything from torso
	if (Joint != R_HAND) {
		CoordsRel[Joint][0] = Coords[Joint][0] - Coords[TORSO][0];
		CoordsRel[Joint][1] = Coords[Joint][1] - Coords[TORSO][1];
		CoordsRel[Joint][2] = Coords[Joint][2] - Coords[TORSO][2];
	}
	else {
		SmoothHands();
	}

	
	
}

void oscKinect::GetVelocity(int Joint) {
	// Velocity is just the difference between each update
	CoordsVel[Joint][0] = CoordsRel[Joint][0] - CoordsPrev[Joint][0];
	CoordsVel[Joint][1] = CoordsRel[Joint][1] - CoordsPrev[Joint][1];
	CoordsVel[Joint][2] = CoordsRel[Joint][2] - CoordsPrev[Joint][2];
	
	CoordsPrev[Joint][0] = CoordsRel[Joint][0];
	CoordsPrev[Joint][1] = CoordsRel[Joint][1];
	CoordsPrev[Joint][2] = CoordsRel[Joint][2];
}

void oscKinect::GetAcceleration(int Joint){
	// Acceleration is the difference in velocity
	CoordsAccn[Joint][0] = CoordsVel[Joint][0] - CoordsPrevVel[Joint][0];
	CoordsAccn[Joint][1] = CoordsVel[Joint][1] - CoordsPrevVel[Joint][1];
	CoordsAccn[Joint][2] = CoordsVel[Joint][2] - CoordsPrevVel[Joint][2];
	
	CoordsPrevVel[Joint][0] = CoordsVel[Joint][0];
	CoordsPrevVel[Joint][1] = CoordsVel[Joint][1];
	CoordsPrevVel[Joint][2] = CoordsVel[Joint][2];
}

int oscKinect::mapJointToInt(string Joint) {
	
	if (Joint == head) {
		return HEAD;
	}
	else if (Joint == neck) {
		return NECK;
	}
	else if (Joint == torso) {
		return TORSO;
	}
	else if (Joint == r_shoulder) {
		return R_SHOULDER;
	}
	else if (Joint == r_elbow) {
		return R_ELBOW;
	}
	else if (Joint == r_hand) {
		return R_HAND;
	}
	else if (Joint == l_shoulder) {
		return L_SHOULDER;
	}
	else if (Joint == l_elbow) {
		return L_ELBOW;
	}
	else if (Joint == l_hand) {
		return L_HAND;
	}
	else if (Joint == r_knee) {
		return R_KNEE;
	}
	else if (Joint == r_ankle) {
		return R_ANKLE;
	}
	else if (Joint == r_foot) {
		return R_FOOT;
	}
	else if (Joint == l_knee) {
		return L_KNEE;
	}
	else if (Joint == l_ankle) {
		return L_ANKLE;
	}
	else if (Joint == l_foot) {
		return L_FOOT;
	}
	else if (Joint == r_hip) {
		return R_HIP;
	}
	else if (Joint == l_hip) {
		return L_HIP;
	}
	else {
		return -1;
	}
}

//--------------------------------------------------------------
void oscKinect::update(){
	
	// hide old messages
	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
	{
		if ( timers[i] < ofGetElapsedTimef() )
			msg_strings[i] = "";
	}
	
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		parseJoints();
	}
}


//--------------------------------------------------------------
void oscKinect::draw(){
	
	string buf;
	buf = "listening for osc messages on port" + ofToString( PORT );
	ofDrawBitmapString( buf, 10, 20 );
	
	ofDrawBitmapString( "Joint: " + joint , 20, 460 );
	ofDrawBitmapString( "UserID: " + ofToString(userID, 1) , 20, 480 );
	
	for (int i = 0; i < NUM_JOINTS; i++)
	{
		buf = ofToString(i, 2) + " --> X:" + ofToString(CoordsRel[i][0], 2) + " Y:" + ofToString(CoordsRel[i][1], 2) + " Z:" + ofToString(CoordsRel[i][2], 2); 
		ofDrawBitmapString( buf, 20, 500 + 12*i );
		
		// Draw a nice stick figure thing
		ofSetColor(2*2550*CoordsVel[i][0] + 100, 2*2550*CoordsVel[i][1] + 100, 2*2550*CoordsVel[i][2] + 100);
		ofCircle(1024*CoordsRel[i][0] + 512, 768*CoordsRel[i][1]+384, (10 - 10*CoordsRel[i][2]));
	}
	
	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
	{
		//ofDrawBitmapString( msg_strings[i], 10, 40+15*i );
	}
}
