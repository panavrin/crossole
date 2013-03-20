/*
 *  Grid.cpp
 *  crossole
 *
 *  Created by Sertan Senturk on 2/10/11.
 *  Copyright 2011 Georgia Tech. All rights reserved.
 *
 */

#include "Grid.h"


Grid:: Grid()
{
	m_originRow = m_originCol = 0;
	m_numCol = 5;
	
	vector<Block *> firstrow(m_numCol);
	grid.push_back(firstrow);

}

Grid:: ~Grid()
{
}

bool Grid:: addBlock(int row, int col, Block * t)
{
	int numRow = grid.size();
	
	// you can't have block outside the origin. 
	if ( row < m_originRow - 1 || col < m_originCol -1 )
		return false;

	if ( row - m_originRow > numRow || col - m_originCol > m_numCol  )
		return false;
	
	// you need to have at least one block near you. 
	if ( true )
		; // TODO: not implemented
	
	// Let's check left and top part
	if ( row == m_originRow -1 ) // if we want to add a row at the top of matrix
	{
		vector <Block *> row (m_numCol) ;
		grid.insert(grid.begin(),row);
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
		vector <Block *> row (m_numCol) ;
		grid.push_back(row);
	}
	if (revisedCol >= m_numCol)
	{
		m_numCol = revisedCol + 1;
		for (int i = 0 ; i < grid.size(); i++)
			grid[i].resize(m_numCol,NULL);
	}
		
	grid.at(revisedRow)[revisedCol] = t;
	
	return true;
}

int Grid:: getNumRow()
{
	return grid.size();
}

int Grid:: getNumCol()
{
	return m_numCol;
}

Block * Grid::get(int row, int col)
{
	return grid[row - m_originRow][col - m_originCol];
	
}
int Grid::getMinRow() { return m_originRow;}
int Grid::getMinCol() { return m_originCol;}


bool Grid:: hasBlockNear(int row, int col){
	int revisedRow = row - m_originRow;
	int revisedCol = col - m_originCol;
	if (m_firstTime) {
		m_firstTime = false;
		return true;
	}
	
	// in case that xy coordinate might be outside the grid. 
	if ( revisedRow == -1 && revisedCol >= 0 && revisedCol <getNumCol()) { 
		if( get(row + 1,col) != NULL ) 
			return true;
	}
	else if ( revisedCol == -1 && revisedRow >= 0 && revisedRow <getNumRow()) {
		if( get(row ,col + 1) != NULL ) 
			return true;
	}
	else if ( revisedRow == getNumRow() && revisedCol >= 0 && revisedCol <getNumCol() ) {
		if( get(row -1,col ) != NULL ) 
			return true;
	}
	else if ( revisedCol == getNumCol()  && revisedRow >= 0 && revisedRow <getNumRow() ) {
		if ( get(row ,col -1 ) != NULL ) 
			return true;
	}
	else { // well it is inside the grid
		if (revisedRow > 0 && revisedRow < getNumRow() &&get(row - 1,col) != NULL)
			return true;
		if (revisedCol > 0  && revisedCol < getNumCol() && get(row ,col- 1) != NULL)
			return true;
		if (revisedRow >= 0 && revisedRow < getNumRow() - 1 &&get(row+1 ,col) != NULL)
			return true;
		if (revisedCol >= 0 && revisedCol < getNumCol() - 1 && get(row ,col+ 1) != NULL)
			return true;
	}
	
	return false;
	
}