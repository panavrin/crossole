/*
 *  PlayCursor.cpp
 *  crossole
 *
 *  Created by Sang Won Lee on 3/6/11.
 *  Copyright 2011 Stanford. All rights reserved.
 *
 */

#include "PlayCursor.h"

#define HOST "192.168.1.3"
#define PORT 7400

PlayCursor::PlayCursor(GridLocation * gridLocation)
{
	m_currentBlock = NULL;
	m_currentLocation = gridLocation;
	m_size = DEFAULT_CURSOR_SIZE;
	m_chordBook = ChordBook::getChordBook();
	m_sender.setup( HOST, PORT );		
}

PlayCursor::~PlayCursor()
{
	delete m_currentLocation;
}

GridLocation * PlayCursor::getCurrentLocation()
{
	return m_currentLocation;
}
bool PlayCursor::isOnBlock(){ return !(m_currentBlock == NULL); }
Block * PlayCursor::currentBlock(){ return m_currentBlock; }

void PlayCursor:: sendOSCMessage(Block * block)
{
	// TODO get all the infromation from block and send proper osc message;
	cout<<"sending osc msg\n";
	if ( block->getChordType() == "")
		cout<<"something wrong here\n";
	cout<<"Selected Block Chord: "<<ChordBook::baseNote[block->getBaseNote()]<< " selected ChordType:" << block->getChordType()<<"\n";
	
//	int baseNote = ChordBook::baseNote[block->getBaseNote()];
	/*
	 M 0 4 7

	 */
	string sentMsg;
	sentMsg = "id 2, source midi, " + m_chordBook->chord2notes(block->getBaseNote(), block->getChordType(), 48, 71) + ", parameters sequence 2 140";
		
	
	cout << sentMsg << endl;
	m_msg.setAddress(sentMsg);
	m_sender.sendMessage( m_msg);
}

void PlayCursor::sendOSCMessage(int MIDINote, int velocity)
{
	// TODO get all the infromation from block and send proper osc message;
	cout<<"sending note through osc\n";
	cout<<"Selected Note: "<<MIDINote<<"\n";
	
	string sentMsg;
	sentMsg = "id 2, source midi, " "notes "+ ofToString(MIDINote, 0) + ", parameters notekey 2 " + ofToString(velocity, 0);
	
	
	cout << sentMsg << endl;
	m_msg.setAddress(sentMsg);
	m_sender.sendMessage( m_msg);
	
}

void PlayCursor::sendOSCMessagebassNote()
{
	if (m_currentBlock == NULL)
		return;
	string sentMsg;
	int bassnote = atoi(ChordBook::baseNote[m_currentBlock->getBaseNote()].c_str());
	bassnote +=48;
	sentMsg = "id 2, source midi, " "notes "+ ofToString(bassnote)+ ", parameters notekey 4 100";
		
	cout << sentMsg << endl;
	m_msg.setAddress(sentMsg);
	m_sender.sendMessage( m_msg);
	
}
void PlayCursor:: sendQuitMessage()
{
	string sentMsg;
	sentMsg = "q";
	cout << "Write a message: ";
	cout << sentMsg << endl;
	m_msg.setAddress(sentMsg);
	m_sender.sendMessage( m_msg);
	
}

void PlayCursor::setCurrentBlock(GridLocation * location, Block * block)
{
	delete m_currentLocation;
	m_currentLocation = location;
	m_currentBlock = block;
}

int PlayCursor::getSize() {return m_size;}
