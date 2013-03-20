/*
 *  Instrument.cpp
 *  crossole
 *
 *  Created by Sang Won Lee on 2/6/11.
 *  Copyright 2011 GTCMT. All rights reserved.
 *
 */

#include "Instrument.h"


Block * m_firstBlock;
Block * m_lastBlock;

/********************************
 CONSTRUCTORS
 *******************************/
Instrument:: Instrument()
{
	m_numBlock = 0;
	m_originRow = m_originCol = 0;
	m_numCol = 5;
	
	vector<Block *> firstrow(m_numCol,(Block *) NULL);
	grid.push_back(firstrow);
	
	route = new Route();
	m_firstBlock = NULL;
	m_lastBlock = m_firstBlock;
	
	m_cursor = new PlayCursor(new GridLocation(INITIAL_CURSOR_X,INITIAL_CURSOR_Y));
	
	m_playMethod = "sequence";
	m_isLooping = false;
	m_isRecording = false;
}

/********************************
 DESTRUCTORS
 *******************************/
Instrument:: ~Instrument()
{
	// TODO delete or release all block. 
	cout<<"instrument destructor\n";
	GridLocation * gridLoc;
	
/*	for ( int i=0; i< m_blockLocations.size(); i++)
	{
		gridLoc = m_blockLocations.at(i);
		Block * block = getBlock(gridLoc->getX(), gridLoc->getY());
		delete block;
	}
*/	
	
}

/********************************
 GETTERS
 *******************************/
/*
 Get number of rows
 */
int Instrument:: getNumRow() { return grid.size(); }

/*
 Get number of columns
 */
int Instrument:: getNumCol() { return m_numCol; }

/*
 Get origin of the row
 */
int Instrument::getMinRow() { return m_originRow; }

/*
 Get origin of the column
 */
int Instrument::getMinCol() { return m_originCol; }

/*
 Get the block from index
 */
Block * Instrument::getBlock(int row, int col)
{
	return grid[row - m_originRow][col - m_originCol];
}

Block * Instrument::getBlock(int index)
{
	
	return grid[getBlockGridX(index) - m_originRow][getBlockGridY(index) - m_originCol];
}

int Instrument::getBlockGridX(int ind)
{
	return m_blockLocations[ind]->getX();
}

int Instrument::getBlockGridY(int ind)
{
	return m_blockLocations[ind]->getY();
}

GridLocation Instrument::getBlockGridLocation(int i)
{
	return *m_blockLocations[i];
}

Block * Instrument:: getLastBlock()
{ 
	return m_lastBlock; 
}

string Instrument::getPlayMethod()
{
	return m_playMethod;
}

void Instrument::setPlayMethod(string playMethod)
{
	m_playMethod = playMethod;
}

bool Instrument:: addFirstBlock( Block *block)
{
	m_firstBlock = block;
	m_lastBlock = block;

	m_originPosX = block->getPointX();
	m_originPosY = block->getPointY();
	m_originPosZ = block->getPointZ();
	m_sizeOfBlock = block->getSize();
	
//	extendGridIfNeeded(INITIAL_CURSOR_X,INITIAL_CURSOR_Y);
	
	return addBlock(FIRST_BLOCK_X,FIRST_BLOCK_X,block);
	
}

float Instrument:: distance(float x1, float y1, float x2, float y2)
{
	return (float)sqrt(pow((x1-x2),2) + pow(y1-y2,2));
}

void Instrument::findNearestEmptySpot(float * x, float * y, float * z, int * row, int * col)
{
	if ( m_numBlock == 0)
		return;
	*col = round((*x - m_originPosX) / m_sizeOfBlock);
	*row = round((*y - m_originPosY) / m_sizeOfBlock);
	point3f p = convertGridPosToVisualPos(*row, *col);
	*x = p.x; *y = p.y; *z = p.z;
	return;
}


point3f Instrument::convertGridPosToVisualPos(int row, int col)
{
	point3f p;
	p.x = m_originPosX + m_sizeOfBlock * col;
	p.y = m_originPosY + m_sizeOfBlock * row; 
	p.z = m_originPosZ;
	return p;
}

bool  Instrument::hasBlockAt(int row, int col)
{
	if ( row >= getMinRow() && row < getMinRow() + getNumRow() && col >= getMinCol() && col < getMinCol() + getNumCol() ) 
		if ( getBlock(row, col) != NULL )
			return true;
	return false;
}
void Instrument::sendQuitMessage()
{
	m_cursor->sendQuitMessage();
}
bool Instrument:: isCursorOnBlock()
{
	return m_cursor->isOnBlock();
}
Block * Instrument::currentPlayingBlock(){	return m_cursor->currentBlock();}

void Instrument::slideCursor(Direction direction, long currentTime) {
	int row = m_cursor->getCurrentLocation()->getX();
	int col = m_cursor->getCurrentLocation()->getY();
	
	switch (direction) {
		case LEFT:
			if (convertGridPosToVisualPos(row, col).x < 0 )
				return;
			col--;
			break;
		case RIGHT:
			if (convertGridPosToVisualPos(row, col).x >SCREEN_WIDTH )
				return;
			col++;
			break;
		case UP:
			if (convertGridPosToVisualPos(row, col).y < 0 )
				return;
			row--;
			break;
		case DOWN:
			if (convertGridPosToVisualPos(row, col).y >SCREEN_HEIGHT )
				return;
			row++;
			break;
		default:
			break;
	}
	Block * block = NULL;
	if ( hasBlockAt(row, col) ) 
	{
		block = getBlock(row,col);
		m_cursor->sendOSCMessage(block, m_playMethod, true, false);
	}
	else 
	{
		m_cursor->sendQuitMessage();
	}

	m_cursor->setCurrentBlock(new GridLocation(row, col), block);

	if ( route->m_isRecordingLoop){
		m_isLooping = route->storeLoopRecording(direction, block, currentTime);
	}
}

bool Instrument::isLooping(){return m_isLooping;}

void Instrument::startRecordingRoute(){
	route->startLoopRecording();
}
void Instrument::stopRecordingRoute(){
	delete route;
	route = new Route();
	m_isLooping = false;
	m_isRecording = false;
	route->m_isRecordingLoop = false;
	if ( currentPlayingBlock() !=NULL)
		m_cursor->sendOSCMessage(currentPlayingBlock(), m_playMethod, false, true);
}

void Instrument::nextAction(Direction * d, long *t, long currentTime){
	route->nextAction(d, t, currentTime);
}

void Instrument::SendControls(int x, int y, int z)
{
	// TODO get all the infromation from block and send proper osc message;
	printf("sending osc msg: %d, %d, %d\n",x, y, z);
	
	string sentMsg;
	sentMsg = ofToString(x) + " " + ofToString(y) + " " + ofToString(z);
	
	
	cout << sentMsg << endl;
	m_cursor->m_msg.setAddress(sentMsg);
	m_cursor->m_sender.sendMessage( m_cursor->m_msg);
}

void Instrument::SendControls(int x1, int y1, int z1, int x2, int y2, int z2)
{
	// TODO get all the infromation from block and send proper osc message;
	printf("sending osc msg: %d, %d, %d %d %d %d\n",x1, y1, z1, x2, y2, z2);
	
	string sentMsg;
	sentMsg = ofToString(x1) + " " + ofToString(y1) + " " + ofToString(z1) + " " + ofToString(x2) + " " + ofToString(y2) + " " + ofToString(z2);
	
	
	cout << sentMsg << endl;
	m_cursor->m_msg.setAddress(sentMsg);
	m_cursor->m_sender.sendMessage( m_cursor->m_msg);
}

void Instrument::sendNewContour(){
	Block * currentBlock = m_cursor->currentBlock();
	if ( currentBlock != NULL ) 
	{
		m_cursor->sendOSCMessage(currentBlock, m_playMethod, false, false);
	}
}


void Instrument::SendNote(int MIDINote, int velocity)
{
	if ( m_cursor->currentBlock() != NULL){
		int octave = MIDINote / 7;
		
		MIDINote = MIDINote % 7;
		if (MIDINote < 0)
			MIDINote +=7;
		int bassnote = m_cursor->currentBlock()->getBaseNote();
		int note = 	ChordBook::getChordBook()->chordScale[m_cursor->currentBlock()->getChordType()].at(MIDINote) + octave * 12 +  48 +  	bassnote;
		m_cursor->sendOSCMessage(note, velocity);
	}
}

void Instrument::SendBassNote()
{
	m_cursor->sendOSCMessagebassNote();
}
	
bool Instrument::addBlock(int row, int col, Block * t)
{
	// check if the location already has a Block
	
	if ( hasBlockAt(row, col))
	{
		cout<<"AddBlock Failed : Already has a block at this point("<<row<<","<<col<<")\n";
		return false;
	}
	
	// you need to have at least one block near you. 
	if ( !hasBlockNear(row, col) )
	{
		cout<<"AddBlock Failed : Has no block near("<<row<<","<<col<<")\n";
		return false; 
	}
	
	extendGridIfNeeded(row, col);
	
	int revisedRow = row - m_originRow;
	int revisedCol = col - m_originCol;

	// finally add the block and maintain the variables. 
	t->setPoint(convertGridPosToVisualPos(row, col));
	
	grid.at(revisedRow)[revisedCol] = t;
	m_blockLocations.push_back(new GridLocation(row,col));
	m_numBlock++;
	m_lastBlock = t;
	return true;
}

void Instrument::extendGridIfNeeded(int row, int col)
{
	// extend the grid if the Block is attached out of borders
	// Let's check left and top part
	if ( row == m_originRow -1 ) // if we want to add a row at the top of matrix
	{
		vector <Block *> newRow (m_numCol, (Block *)NULL) ;
		grid.insert(grid.begin(),newRow);
		m_originRow--;
	}
	else if ( col == m_originCol - 1)	// or if you want to add one more coloum at the most left
	{	// then we need to shift one slot for all vector
		
		m_numCol++;
		for (int i = 0 ; i < grid.size(); i++){
			grid[i].insert(grid[i].begin(),NULL);
			grid[i].resize(m_numCol,NULL);
		}
		m_originCol--;
	}
	
	int revisedRow = row - m_originRow;
	int revisedCol = col - m_originCol;
	
	// let's check right and bottom part
	if ( revisedRow >= grid.size())
	{
		vector <Block *> newRow (m_numCol, (Block *) NULL) ;
		grid.push_back(newRow);
	}
	if (revisedCol >= m_numCol)
	{
		m_numCol = revisedCol + 1;
		for (int i = 0 ; i < grid.size(); i++)
			grid[i].resize(m_numCol,NULL);
	}
}

bool Instrument:: hasBlockNear(int row, int col){

	if (m_numBlock == 0) 
		return true;
	
	int revisedRow = row - m_originRow;
	int revisedCol = col - m_originCol;
	
	if ( revisedRow < -1 || revisedCol < -1 || revisedRow > getNumRow() || revisedCol > getNumCol() )
		return false;
	
	// in case that xy coordinate might be outside the grid. 
	if ( revisedRow == -1 && revisedCol >= 0 && revisedCol <getNumCol()) { 
		if( getBlock(row + 1,col) != NULL ) 
			return true;
	}
	else if ( revisedCol == -1 && revisedRow >= 0 && revisedRow <getNumRow()) {
		if( getBlock(row ,col + 1) != NULL ) 
			return true;
	}
	else if ( revisedRow == getNumRow() && revisedCol >= 0 && revisedCol <getNumCol() ) {
		if( getBlock(row -1,col ) != NULL ) 
			return true;
	}
	else if ( revisedCol == getNumCol()  && revisedRow >= 0 && revisedRow <getNumRow() ) {
		if ( getBlock(row ,col -1 ) != NULL ) 
			return true;
	}
//	else if (!( row >= getMinRow() && row < getMinRow() + getNumRow() && col >= getMinCol() && col < getMinCol() + getNumCol() ) )
	else { // well it is inside the grid
		if (revisedRow > 0 && revisedRow < getNumRow() && getBlock(row - 1,col) != NULL)
			return true;
		if (revisedCol > 0  && revisedCol < getNumCol() && getBlock(row ,col- 1) != NULL)
			return true;
		if (revisedRow >= 0 && revisedRow < getNumRow() - 1 && getBlock(row+1 ,col) != NULL)
			return true;
		if (revisedCol >= 0 && revisedCol < getNumCol() - 1 && getBlock(row ,col+ 1) != NULL)
			return true;
	}
	return false;	
}

int Instrument::getNumBlock() {return m_numBlock;}

point3f Instrument::getCursorLocation()
{
	return convertGridPosToVisualPos(m_cursor->getCurrentLocation()->getX(), m_cursor->getCurrentLocation()->getY());
}
