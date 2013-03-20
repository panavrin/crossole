/*
 *  Grid.h
 *  crossole
 *
 *  Created by Sertan Senturk on 2/10/11.
 *  Copyright 2011 Georgia Tech. All rights reserved.
 *
 */

#ifndef _GRID_H
#define _GRID_H
#include <vector>

using namespace std;

class Grid {
	public : 
		Grid();
		~Grid();
		bool addBlock(int x, int y, Block * t);
		int getNumCol();
		int getNumRow();
		Block * get(int x, int y);
		int getMinRow();
		int getMinCol();
		bool hasBlockNear(int row, int col);
	private : 
		vector < vector<Block *> > grid;
//		int m_maxLengthX;
//		int m_maxLengthY;
		bool m_firstTime;
		int m_originRow;
		int m_originCol;
		int m_numCol;
};
#endif
