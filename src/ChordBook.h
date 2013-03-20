/*
 *  ChordBook.h
 *  oscSenderExample
 *
 *  Created by Sertan Senturk on 3/2/11.
 *  Copyright 2011 Georgia Tech. All rights reserved.
 *
 */

#ifndef _CHORD_BOOK_H
#define _CHORD_BOOK_H

#include "ofUtils.h"

#include "Constants.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <map>
#include <list>

enum baseNoteEnum {
	C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B
}; 

class ChordBook {
private:
	ChordBook() {};
	ChordBook(ChordBook const&) {}; 
	ChordBook& operator= (ChordBook const&) {}; 
	
	void readBaseNotesInSession(string line, int note, bool isBaseNotesRead);
	void readChordsInSession();
	
protected:
	
public: 
	static ChordBook* m_chordBookPtr;
	static const std::string baseNote[12];

	std::list< int > baseNoteInd;
	std::vector< int > arpeggioStructure;
	std::vector< int > arpeggioOctaves;
	std::vector<int> arpeggioVisual;
	std::list< std::string > chordType;

	std::map <std::string, std::vector<int> > chordEntry;
	std::map <std::string, std::vector<int> > chordScale;

	// methods 
	static ChordBook* getChordBook();
	void readChordFormula(std::string chordFormulaFile);
	void arpeggioSequence();

	std::string chord2notes(int note, std::string chordType, int octave);
};

#endif