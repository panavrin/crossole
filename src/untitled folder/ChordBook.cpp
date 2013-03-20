/*
 *  ChordBook.cpp
 *  oscSenderExample
 *
 *  Created by Sertan Senturk on 3/2/11.
 *  Copyright 2011 Georgia Tech. All rights reserved.
 *
 */

#include "ChordBook.h"
#include "ofMain.h"
using namespace std;

const std::string ChordBook:: baseNote[] = {"C", "Db", "D", "Eb","E", "F", "Gb", "G", "Ab", "A", "Bb", "B"};

// Global static pointer used to ensure a single instance of the class.
ChordBook* ChordBook::m_chordBookPtr = NULL;  

/** This function is called to create an instance of the class. 
 Calling the constructor publicly is not allowed. The constructor 
 is private and is only called by this Instance function.
 */
ChordBook* ChordBook::getChordBook()
{
	if (!m_chordBookPtr) {  // Only allow one instance of class to be generated.
		m_chordBookPtr = new ChordBook();
		cout << "ChordBook instantiated! " << endl;
		
		m_chordBookPtr->readChordFormula(CHORD_FORMULA_FILE_PATH);
	}
	return m_chordBookPtr;
}

// read chord formulas from text file
void ChordBook::readChordFormula(string chordFormulaFile){	
	string line, chordName, tempStr;
	int note;
	bool isBaseNotesRead = false;
	bool isFirstParameter = true;
	bool isChordNotesRead = false;

	chordFormulaFile = ofToDataPath(chordFormulaFile, 0);
	
	cout << endl;
	cout << "Chord formulas are being read..." << endl;
	
	ifstream chordData(chordFormulaFile.c_str());
	if (chordData.is_open())
	{
		while ( chordData.good() )
		{
			getline (chordData, line, '\n');
			// comment lines are skipped
			if ((line[0] == CHORD_FORMULA_FILE_COMMENT_CHAR) || (line == "") || (line == " ")) {
			} else {
				if ( !isBaseNotesRead ) {
					// read base notes to use	
					stringstream os(line); 
					while (os >> tempStr){
						stringstream(tempStr) >> note;
						for (int i = 0; i < tempStr.length(); i++) {
							if ( !isdigit( tempStr[i] ) ) {
								cout << endl << "Base Note " << tempStr << " is not a digit!" << endl << endl;
								exit(1);
							}
						}
						baseNoteInd.push_back(note);						
					}
					isBaseNotesRead = true;
				} else {
					// read chords
					stringstream os(line); 
					isFirstParameter = true;
					isChordNotesRead = false;
					while (os >> tempStr){
						if (isFirstParameter) {
							chordName = tempStr;
							chordEntry[chordName];
							chordScale[chordName];
							chordType.push_back(chordName);
							isFirstParameter = false;
						} else if (!isChordNotesRead) {
							if (tempStr == "/") {
								isChordNotesRead = true;
							}
							else {
								for (int i = 0; i < tempStr.length(); i++) {
									if ( !isdigit( tempStr[i] ) ) {
										cout << endl << "Chord Note " << tempStr << " is not a digit!" << endl << endl;
										exit(1);
									}
								}
								stringstream(tempStr) >> note;
								chordEntry[chordName].push_back(note);
							}
						} else {
							for (int i = 0; i < tempStr.length(); i++) {
								if ( !isdigit( tempStr[i] ) ) {
									cout << endl << "Chord Note " << tempStr << " is not a digit!" << endl << endl;
									exit(1);
								}
							}
							stringstream(tempStr) >> note;
							chordScale[chordName].push_back(note);
						}
					}
				}
			}
		}
		
		// print chords & error checking
		list<int>::iterator listIt;
		map <string, list<int> >::iterator mapIt;
		cout << endl;
		cout << "Chords" << endl;
		for (mapIt=chordEntry.begin(); mapIt !=chordEntry.end(); mapIt++) {
			cout << (*mapIt).first << ",";
			for (listIt=(*mapIt).second.begin(); listIt != (*mapIt).second.end(); listIt++) {
				cout << " " << *listIt;		
				if (*listIt < 0 || *listIt > 24) {
					cout << endl;
					cout << "Invalid chord note! It has to be between 0 (C) and 11 (B)!" << endl;
					cout << endl;
					exit(1);
				}
			}
			cout << endl;
		}		

		// print associated scales & error checking
		cout << endl;
		cout << "Notes of the scale associated with the chords" << endl;
		for (mapIt=chordScale.begin(); mapIt !=chordScale.end(); mapIt++) {
			cout << (*mapIt).first << ",";
			for (listIt=(*mapIt).second.begin(); listIt != (*mapIt).second.end(); listIt++) {
				cout << " " << *listIt;		
				if (*listIt < 0 || *listIt > 24) {
					cout << endl;
					cout << "Invalid scale note! It has to be between 0 (C) and 11 (B)!" << endl;
					cout << endl;
					exit(1);
				}
			}
			cout << endl;
		}		
		
		// print base notes & error checking
		cout << endl << "Base Notes for this session: ";
		for (listIt = baseNoteInd.begin(); listIt != baseNoteInd.end(); listIt++) {
			cout << " " << *listIt;	
			if (*listIt < 0 || *listIt > 24) {
				cout << "Invalid base note! It has to be between 0 (C) and 11 (B)!" << endl;
				cout << endl;
				exit(1);
			}
		}
		cout << endl;
		
		cout << endl << "Chord formulas are read successfully!" << endl;
		chordData.close();
	} else {
		cout << endl << "Chord Book File not Found!" << endl;
		cout << "File location: " << chordFormulaFile << endl;
//		cout << "Current directry: " << getcwd(NULL, 0) << endl 
		cout << endl;
		exit(1);
	}
}


// convert chord to notes in a given midi interval
string ChordBook::chord2notes(int note, string chordType, int minMidiNum, int maxMidiNum) {
	if (note < 0 && note > 11)
		cerr << "Incorrect note number, it should be between 0 (C) and 11 (B)" << endl;
	
	if (minMidiNum > maxMidiNum)
		cerr << "Invalid interval, minMidiNum cannot be bigger than maxMidiNum!" <<endl;
	
	stringstream noteListStr;
	noteListStr << "notes";
	int tempNote;
	note += 60;
	// get the note structure for the given chordType
	map <string, list<int> >::iterator chordIt;
	chordIt = chordEntry.find(chordType);
	
	list<int>::iterator noteIt;
	for (noteIt = (*chordIt).second.begin(); noteIt != (*chordIt).second.end(); noteIt++) {
		tempNote = *noteIt + note;			
		noteListStr << " " << tempNote;
		
	}
	
//	noteListStr << " " <<chordIt.at(0); << " " 
	return noteListStr.str();
}
