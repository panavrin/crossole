/*
 *  PlayCursor.h
 *  crossole
 *
 *  Created by Sang Won Lee on 3/6/11.
 *  Copyright 2011 Stanford. All rights reserved.
 *
 */
#include <iostream>

#ifndef PLAYCURSOR_H
#define PLAYCURSOR_H

#include "Block.h"
#include "Constants.h"
#include "ChordBook.h"
#include "ofxOsc.h"

using namespace std;

enum Direction {LEFT, RIGHT, UP, DOWN, INVALID};

class GridLocation{
private:
	int x;
	int y;
public:
	GridLocation(int x_, int y_){
		x = x_; y=y_;}
	int getX(){ return x;}
	int getY(){ return y;}
};


class PlayCursor {
public:
	PlayCursor(GridLocation * gridLocation);
	~PlayCursor();
	GridLocation * getCurrentLocation();
	bool isOnBlock();
	int getSize();
	void setCurrentBlock(GridLocation * location, Block * block);
	void sendOSCMessage(Block * block);
	void sendOSCMessage(int MIDINote, int velocity);
	void sendOSCMessagebassNote();
	void sendQuitMessage();
	Block * currentBlock();
private:
	Block * m_currentBlock;
	GridLocation * m_currentLocation;
	int m_size;
	ofxOscSender m_sender;
	ofxOscMessage m_msg;
	ChordBook* m_chordBook;
	
};
#endif