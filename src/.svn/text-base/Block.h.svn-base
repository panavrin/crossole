/*
 *  Block.h
 *  crossole
 *
 *  Created by Sang Won Lee on 2/6/11.
 *  Copyright 2011 GTCMT. All rights reserved.
 *
 */

#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>

#include "Constants.h"
struct point3f{
	float x;
	float y;
	float z;
};


class Block {
public:
	Block();
	Block(Block * duplicateBlock);
	~Block();
	void setPoint(float x, float y, float z);
	void setPoint(point3f p);
	void setChord(int baseNote, std::string chordType, int counter2, int counter1);
	void setPlayMethod(std::string playMethod);
	float getPointX();
	float getPointY();
	float getPointZ();
	int getSize();
	int getBaseNote();
	std::string getChordType();
	std::string getPlayMethod();
	
	void setRandomColor();
	void setColorByChord();
	void setColor(int red, int green, int blue, int alpha);
	
	std::string checkMajorMinor();
	
	int m_colorRed;
	int m_colorBlue;
	int m_colorGreen;
	int m_colorAlpha;
	int m_counter2,	m_counter1; 
	
private:
	int m_baseNote;
	std::string m_chordType;
	std::string m_playMethod;
	
protected : 
	int	m_id;
	int m_size;
	bool m_attached;
	point3f	m_p;
	
};
#endif
