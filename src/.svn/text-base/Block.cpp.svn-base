/*
 *  Block.cpp
 *  crossole
 *
 *  Created by Sang Won Lee on 2/6/11.
 *  Copyright 2011 GTCMT. All rights reserved.
 *
 */
#include "Block.h"
using namespace std;


Block:: Block()
{
	m_attached = false;
	m_p.x = DEFAULT_X;
	m_p.y = DEFAULT_Y;
	m_p.z = DEFAULT_Z;
	
	m_size = DEFAULT_BLOCK_SIZE;
	
	m_colorRed = 255;
	m_colorGreen = 255;
	m_colorBlue = 255;
	m_colorAlpha = 255;
}


Block:: Block(Block * duplicateBlock)
{
	m_attached = false;
	m_p.x = DEFAULT_X;
	m_p.y = DEFAULT_Y;
	m_p.z = DEFAULT_Z;
	
	m_size = DEFAULT_BLOCK_SIZE;
	
	m_colorRed = 255;
	m_colorGreen = 255;
	m_colorBlue = 255;
	m_colorAlpha = 255;
	
	m_baseNote = duplicateBlock->getBaseNote();
	m_chordType = duplicateBlock->getChordType();
	m_counter2 = duplicateBlock->m_counter2;
	m_counter1 = duplicateBlock->m_counter1;

}

void Block:: setPoint(float x, float y, float z)
{
	m_p.x = x;
	m_p.y = y;
	m_p.z = z;
	return;
}

void Block:: setPoint(point3f p)
{
	m_p = p;
}

void Block::setChord(int baseNote, string chordType, int counter2, int counter1) {
	m_baseNote = baseNote;
	m_chordType = chordType;
	m_counter2 = counter2;
	m_counter1 = counter1;
}

/*
void Block::setPlayMethod(string playMethod) {
	m_playMethod = playMethod;
}
 */

void Block::setColor(int red, int green, int blue, int alpha){
	m_colorRed = red;
	m_colorGreen = green;
	m_colorBlue = blue;
	m_colorAlpha = alpha;
}

void Block::setColorByChord(){
	switch (m_baseNote) {
		case 0: // c
			m_colorRed = 103;
			m_colorGreen = 255;
			m_colorBlue = 50;		
			break;
		case 1: // c#
			m_colorRed = 255;
			m_colorGreen = 0;
			m_colorBlue = 0;
			break;
		case 2: // d
			m_colorRed = 255;
			m_colorGreen = 147;
			m_colorBlue = 80;
			break;
		case 3: // d#
			m_colorRed = 255;
			m_colorGreen = 255;
			m_colorBlue = 0;
			break;
		case 4: // e
			m_colorRed = 0;
			m_colorGreen = 255;
			m_colorBlue = 180;
			break;
		case 5: // f
			m_colorRed = 120;
			m_colorGreen = 255;
			m_colorBlue = 220;
			break;
		case 6: // f#
			m_colorRed = 0;
			m_colorGreen = 255;
			m_colorBlue = 127;
			break;
		case 7: // g
			m_colorRed = 240;
			m_colorGreen = 127;
			m_colorBlue = 200;
			break;
		case 8: // g#
			m_colorRed = 0;
			m_colorGreen = 255;
			m_colorBlue = 0;
			break;
		case 9: // a
			m_colorRed = 180;
			m_colorGreen = 80;
			m_colorBlue = 255;
			break;
		case 10: // a#
			m_colorRed = 63;
			m_colorGreen = 0;
			m_colorBlue = 255;
			break;
		case 11: // b
			m_colorRed = 127;
			m_colorGreen = 0;
			m_colorBlue = 255;
			break;
		default:
			break;
	}	
	
	string chordType = checkMajorMinor();
	if (chordType == "major") {
	//	m_colorAlpha = 1;
		;
		
	} else if (chordType == "minor") {
	//	m_colorAlpha = 0.5;
		int temp = m_colorRed;
		m_colorRed = m_colorBlue;
		m_colorBlue =m_colorGreen;
		m_colorGreen =temp;
	} else if (chordType == "other") {
	//	m_colorAlpha = 1;
		int temp = m_colorBlue;
		m_colorBlue =m_colorRed;
		m_colorRed = m_colorGreen;
		m_colorGreen =temp;
	} else {
	//	m_colorAlpha = 1;
		m_colorRed *=0;
		m_colorBlue *=0;
		m_colorGreen *=0;
	}
}


void Block::setRandomColor(){
		
	float redRandom, blueRandom, greenRandom ;
	/*
	 double r1 = double(rand()) / RAND_MAX;
	 double r2 = double(rand()) / RAND_MAX;
	 double r3 = double(rand()) / RAND_MAX;
	 
	 bool randomForPick1or2 = (r2>=0.5);
	 if ( randomForPick1or2 ) 
	 r1 = r1/5.0 + 0.8;
	 else 
	 r1 = r1/5.0;
	 
	 int pickRGB = floor(r3 * 3.0);
	 if (pickRGB == 0)
	 {
	 redRandom = r1;
	 blueRandom = (1-r1)/2;
	 greenRandom = (1-r1)/2;
	 }
	 else if ( pickRGB == 1)
	 {
	 redRandom = (1-r1)/2;
	 blueRandom = r1;
	 greenRandom = (1-r1)/2;
	 }
	 else if (pickRGB == 2)
	 {
	 redRandom = (1-r1)/2;
	 blueRandom = (1-r1)/2;
	 greenRandom = r1;
	 }
	 else {
	 cout<<"pickRGB Error/n";
	 }*/
	
	redRandom = Random();
	blueRandom = Random();
	greenRandom = Random();
	float sum = redRandom + blueRandom + greenRandom;
	redRandom /= sum;
	blueRandom /= sum;
	greenRandom /= sum;
	setColor((int)(55 + 200 * redRandom), (int)(55 + 200 * greenRandom), (int)(55 + 200 * blueRandom), 255);
}

string Block::checkMajorMinor(){
	string majorChords = " M maj 6 maj7 9 maj9 11 13 maj13 sus2(M) sus4(M) 6/9 ";
	string minorChords = " m m6 m7 m9 m11 m13 m(maj7) m(M7) sus2(m) sus4(m) ";
	string otherChords = " 5 M7 7 aug 9sus4 dim 7sus4 7b5 7b9 add9 aug9 ";

	int position = (majorChords.find(" "+m_chordType ));
	if ( position != (string::npos)) {
		return "major";
	} else if ((minorChords.find(" "+m_chordType)) != (string::npos)) {
		return "minor";
	} else if ((otherChords.find(" "+m_chordType )) != (string::npos)) {
		return "other";
	} else {
		return "wrongEntry";
	}
}


float Block:: getPointX(){return m_p.x;}
float Block:: getPointY(){return m_p.y;}
float Block:: getPointZ(){return m_p.z;}
int Block:: getSize(){return m_size;}
int Block::getBaseNote(){return m_baseNote;}
string Block::getChordType(){return m_chordType;}
//string Block::getPlayMethod(){return m_playMethod;};

Block:: ~Block()
{
}
/*
void Block::setColor(int red, int green, int blue, int alpha)
{
	;
}
*/