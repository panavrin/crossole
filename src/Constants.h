/*
 *  Constants.h
 *  oscSenderExample
 *
 *  Created by Sertan Senturk on 3/2/11.
 *  Copyright 2011 Georgia Tech. All rights reserved.
 *
 */

#ifndef _CONSTANTS_H
#define _CONSTANTS_H

//Screen
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 800

// ChordBook constants
#define CHORD_FORMULA_FILE_PATH "ChordFormulas.txt"
#define CHORD_FORMULA_FILE_COMMENT_CHAR '%'
#define NUM_BASE_NOTE 12
#define ARPEGGIO_MIN_OCTAVE 4
#define ARPEGGIO_NUM_NOTE_IN_OCTAVE 4

// Block constants
#define DEFAULT_BLOCK_SIZE 100 
#define DEFAULT_BLOCK_ADJUSTEMENT 5
#define DEFAULT_CURSOR_SIZE 99 //(DEFAULT_BLOCK_SIZE - DEFAULT_BLOCK_ADJUSTEMENT + 4)

#define DEFAULT_CREATED_BLOCKX (SCREEN_WIDTH / 2)
#define DEFAULT_CREATED_BLOCKY 200

#define DEFAULT_X 0
#define DEFAULT_Y 0
#define DEFAULT_Z 0

//Cursor
#define ADJUST_RH_CURSOR -200
#define ADJUST_LH_CURSOR 200

// Keyboard
#define NUM_COLUMN_KEYBOARD 8.0
#define NUM_ROW_KEYBOARD 8.0
#define MARGIN_KEYBOARD 120

// OSC
//#define NUM_MSG_STRINGS 20
//#define HOST "192.168.1.8"
#define HOST "127.0.0.1"
#define SENDING_PORT 7400
#define RECEIVING_PORT 7401



#endif