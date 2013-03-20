/*
 *  Route.cpp
 *  crossole
 *
 *  Created by Sertan Senturk on 3/30/11.
 *  Copyright 2011 Georgia Tech. All rights reserved.
 *
 */

#include "Route.h"

Route::Route() {
	m_startingBlock = NULL;
	m_numBlocksInRoute = 0;
	m_isFirstBlock  = true;
	m_isRecordingLoop = false;
}

void Route::startLoopRecording() {
	m_currrentIndex = -1;
	m_directionVector.clear();
	m_durationVector.clear();
	m_isRecordingLoop = true;
	
	cout << "Loop Started!" << endl;
}


bool Route::storeLoopRecording(Direction d, Block* currBlock, long currentClock) {
	currCTick = currentClock;
	
	if (m_isFirstBlock) { // first slide
		m_numBlocksInRoute++;
		
		m_startingBlock = currBlock;
		m_isFirstBlock = false;
		prevCTick = currCTick;
		return false;
		
	} else if (m_startingBlock == currBlock) { // last slide connecting to the first block
		m_isRecordingLoop = false;
		cout << "Loop Formed! duration:" <<(currCTick-prevCTick)<< endl;
		m_durationVector.push_back(currCTick-prevCTick);
		m_directionVector.push_back(d);
		return true;
		
	} else { // intermediate steps
		m_numBlocksInRoute++;
		cout << "Loop Formed! duration:" <<(currCTick-prevCTick)<< endl;		
		m_durationVector.push_back(currCTick-prevCTick);
		m_directionVector.push_back(d);
		prevCTick = currCTick;
		return false;
		
	}
}

void Route::nextAction(Direction * d, long * nextT, long currentT)
{
	m_currrentIndex++;
	m_currrentIndex = m_currrentIndex % m_numBlocksInRoute;
	
	*d = m_directionVector[m_currrentIndex];
	*nextT = m_durationVector[m_currrentIndex] + currentT;
}