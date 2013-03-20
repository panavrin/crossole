/*
 *  Route.h
 *  crossole
 *
 *  Created by Sertan Senturk on 3/30/11.
 *  Copyright 2011 Georgia Tech. All rights reserved.
 *
 */

#ifndef ROUTE_H
#define ROUTE_H

#include "Block.h"
#include "PlayCursor.h"

#include <time.h>

class Route {
private:
	int m_currrentIndex;
	int m_numBlocksInRoute;
	Block* m_startingBlock;
	vector <Direction> m_directionVector;
	vector <long> m_durationVector;
	
	long prevCTick, currCTick;
	
	bool m_isFirstBlock;
	
public:
	bool m_isRecordingLoop;
	
	Route();
	//	~Route();
	void startLoopRecording();
	bool storeLoopRecording(Direction d, Block* currentBlock, long currentTime);
	void nextAction(Direction * d, long * nextT, long currentTime);
	
};
#endif