/*
 *  Instrument.h
 *  crossole
 *
 *  Created by Sang Won Lee on 2/6/11.
 *  Copyright 2011 GTCMT. All rights reserved.
 *
 */
#include <vector>
#include <iostream>

#ifndef _INSTRUMENT_H
#define _INSTRUMENT_H
#include "Constants.h"
#include "Block.h"
#include "PlayCursor.h"
#include "ChordBook.h"
#include "Route.h"
#include <string>

#define FIRST_BLOCK_X 0
#define FIRST_BLOCK_Y 0
#define INITIAL_CURSOR_X FIRST_BLOCK_X
#define INITIAL_CURSOR_Y FIRST_BLOCK_Y - 1

//using namespace std;

class Instrument {
private:
	vector < vector<Block *> > grid;
	vector< GridLocation * > m_blockLocations;

	int m_numCol;
	int m_originRow;
	int m_originCol;
	int m_id;
	int m_numBlock;

	Block * m_firstBlock;
	Block * m_lastBlock;
	PlayCursor * m_cursor;
	Route * route;
	
	float m_originPosX;
	float m_originPosY;
	float m_originPosZ;
	float m_sizeOfBlock;
	
	string m_playMethod;
	bool m_isLooping; 
	bool m_isRecording;
	
	ChordBook * chordBook;
	
public:
	Instrument();
	~Instrument();
	
	int getNumRow();
	int getNumCol();
	int getMinRow();
	int getMinCol();
	int getNumBlock();
	Block * getBlock(int x, int y);
	Block * getBlock(int index);
	int getBlockGridX(int index);
	int getBlockGridY(int index);
	void startRecordingRoute();
	void stopRecordingRoute();
	
	GridLocation getBlockGridLocation(int i);
	Block * getLastBlock();
	
	bool isLooping();
	bool isCursorOnBlock();
	Block * currentPlayingBlock();
	void sendQuitMessage();
	void sendNewContour();
	
	string getPlayMethod();
	void setPlayMethod(string playMethod);
	
	// METHOD
	bool addBlock(int x, int y, Block * t);
	bool addFirstBlock(Block * t);
	bool hasBlockNear(int row, int col);
	bool hasBlockAt(int row, int col);
	float distance(float x1, float y1, float x2, float y2);
	
	void nextAction(Direction * d, long *t, long currentTime);
	void findNearestEmptySpot(float* x, float* y, float* z, int *row, int *col);
	point3f convertGridPosToVisualPos(int row, int col);
	void extendGridIfNeeded(int row, int col);
	void slideCursor(Direction direction, long currentTime);
	point3f getCursorLocation();
	void SendNote(int MIDINote, int velocity);
	void SendBassNote();
	void SendControls(int x, int y, int z);
	void SendControls(int x1, int y1, int z1, int x2, int y2, int z2);
};
#endif