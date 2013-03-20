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
		m_chordBookPtr->arpeggioSequence();

	}
	
	return m_chordBookPtr;
}

// read chord formulas from text file
void ChordBook::readChordFormula(string chordFormulaFile){	
	string line, chordName, tempStr, noteStr;
	int note, octave;
	bool isBaseNotesRead = false;
	bool isArpeggioStructureRead = false;
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
					cout << "Base notes line:" << line << endl;
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
				} else if (!isArpeggioStructureRead) {
					cout << "Arpeggio notes line:" << line << endl;
					cout << "Note: ";
					// read arpeggio notes to use	
					stringstream os(line); 
					while (os >> tempStr){
						stringstream stream(tempStr);
						
						getline(stream, noteStr, '_');
						for (int i = 0; i < noteStr.length(); i++) {
							if ( !isdigit( noteStr[i] ) ) {
								cout << endl << "Base Note " << noteStr << " is not a digit!" << endl << endl;
								exit(1);
							}
						}
						stringstream(noteStr) >> note;
						
						getline(stream, noteStr, '_');
						for (int i = 0; i < noteStr.length(); i++) {
							if ( !isdigit( noteStr[i] ) ) {
								cout << endl << "Base Note " << noteStr << " is not a digit!" << endl << endl;
								exit(1);
							}
						}
						stringstream(noteStr) >> octave;
						
						arpeggioStructure.push_back(note);
						arpeggioOctaves.push_back(octave);						
					}
					cout << endl;
					isArpeggioStructureRead = true;					
				} else{
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
		vector<int>::iterator vectorIt;
		map <string, vector<int> >::iterator mapIt;
		cout << endl;
		cout << "Chords" << endl;
		for (mapIt=chordEntry.begin(); mapIt !=chordEntry.end(); mapIt++) {
			cout << (*mapIt).first << ",";
			for (vectorIt=(*mapIt).second.begin(); vectorIt != (*mapIt).second.end(); vectorIt++) {
				cout << " " << *vectorIt;		
				if (*vectorIt < 0 || *vectorIt > 24) {
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
		vector<int>::iterator vecIt;
		list<int>::iterator listIt;
		map <string, vector<int> >::iterator mapIt2;
		cout << "Notes of the scale associated with the chords" << endl;
		for (mapIt2=chordScale.begin(); mapIt2 !=chordScale.end(); mapIt2++) {
			cout << (*mapIt2).first << ",";
			for (vecIt=(*mapIt2).second.begin(); vecIt != (*mapIt2).second.end(); vecIt++) {
				cout << " " << *vecIt;		
				if (*vecIt < 0 || *vecIt > 24) {
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
		
		// arpeggio structure
		cout << endl << "Arpeggio Structure for this session: ";
		for (vectorIt = arpeggioStructure.begin(); vectorIt != arpeggioStructure.end(); vectorIt++) {
			cout << " " << *vectorIt;	
			if (*vectorIt < 0 || *vectorIt > 24) {
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

void ChordBook::arpeggioSequence()
{
	for (int i=0; i<arpeggioStructure.size(); i++)
	{
		int indexVisual = arpeggioStructure[i] + (arpeggioOctaves[i] - ARPEGGIO_MIN_OCTAVE)*ARPEGGIO_NUM_NOTE_IN_OCTAVE;
		cout<<"arpeggio index in visualization:"<<indexVisual<<"("<<arpeggioStructure[i] <<","<<arpeggioOctaves[i]<<")"<<endl;
		arpeggioVisual.push_back(indexVisual);
	}
}

// convert chord to notes in a given midi interval
string ChordBook::chord2notes(int note, string chordType, int octave) {
	if (note < 0)
		cerr << "Incorrect note number, it should more than 0" << endl;
	
	if (octave < 0)
		cerr << "Octave cannot be smaller than 0!" <<endl;
	
	stringstream noteListStr;
	noteListStr << "notes";
	int tempNote;
	
	// get the note structure for the given chordType
	map <string, vector<int> >::iterator chordIt;
	chordIt = chordEntry.find(chordType);
	
	for (int i = 0; i < arpeggioStructure.size(); i++) {
		if ( arpeggioOctaves[i] < 0)
			noteListStr << " -1";
		else	{
			tempNote = ((*chordIt).second)[arpeggioStructure[i]%((int)(*chordIt).second.size())] + (arpeggioOctaves[i]+arpeggioStructure[i]/((int)(*chordIt).second.size()))*12 + note; 
			noteListStr << " " << tempNote;
		}
	}
	
	cout << "arpeggio " << noteListStr.str() << endl;		

	//list<int>::iterator noteIt;
	/*
	 note += 60;
	 for (noteIt = (*chordIt).second.begin(); noteIt != (*chordIt).second.end(); noteIt++) {
	 tempNote = *noteIt + note;			
	 noteListStr << " " << tempNote;
	 
	 }
	 */
	//	for (noteIt = (*chordIt).second.begin(); noteIt != (*chordIt).second.end(); noteIt++) {
	//		tempNote = *noteIt + note;			
	//		for (int i = (minMidiNum - tempNote) / 12 + 1 ; i <= (maxMidiNum - tempNote) / 12; i++) {
	//			noteListStr << " " << (tempNote + i * 12);
	//		}		
	//	}
	
	//	noteListStr << " " <<chordIt.at(0); << " " 
	return noteListStr.str();
}
