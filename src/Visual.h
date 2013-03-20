#ifndef _VISUAL_H
#define _VISUAL_H

#include "ofMain.h"
#include <sys/time.h>

#include <stdlib.h>
#include "oscKinectGestures.h"
#include "Instrument.h"
#include "ofxOsc.h"

#ifdef __MACOSX_CORE__
// note: for mac only
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif
// crossole classes 

#define HIGHLEVEL_Z 50
#define LOWLEVEL_Z 0
#define ROTATE_UNIT 10

class Visual : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	~Visual();
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void drawString( GLfloat x, GLfloat y, GLfloat z, string str, GLfloat scale);
	void createBlock();
	void DrawCursors();
	
	
	private : 
	int m_screenW;
	int m_screenH;
	float m_centerX;
	float m_centerY;

	int m_receivedNum;
	
	
	int m_lastKeboardKey;
	// rotate view 
	float m_counterX ;
	float m_counterY ;
	float m_counterZ ;
	int m_counter;
	int m_counter2;
	// targetCounter
	float m_tCounterX ;
	float m_tCounterY ;
	float m_tCounterZ ;
	// mosue position when pressed
	bool m_mousePressed_;
	bool m_grabbedCreatedBlock;
	int m_mouseX;
	int m_mouseY;
	bool DEBUG;
	bool m_keyboard;
	int m_previousCol;
		int m_previousRow;
	bool m_switchAxis;
	bool m_blockCreated;
	bool m_CPressed;
	
	// timbre visualization
	int m_rhX, m_rhY, m_rhZ, m_lhX, m_lhY, m_lhZ; 
	
	bool m_fullscreen;
	int m_pressedKey;
	int m_KeyboardCursorAlpha;
	ofTrueTypeFont m_font;
	ofTrueTypeFont m_smallFont;

	Block * m_createdBlock;
	int m_foundRow, m_foundCol;
	
	ofxOscReceiver m_listener;
	
	double m_inc;
	
	//	Instrument m_instrument;	
	Instrument * m_inst;	
	oscKinectGestures kinectListener;
	
	ChordBook* m_chordBook;
	
	bool isHigherLevel;
	float blockAlpha;
	
	bool routeMode;
	bool m_firstMoveInRoute;
	int m_highlightKey;
	Direction nextDirectionInRoute;
	
	bool m_chordChanged;
	long nextClockInRoute;
	
	void initialize_graphics();
	float distance(float x1, float y1, float x1, float x2);
	void drawCreatedBlock(bool rotate);
	void drawChordBookNear();
	void drawInstrument();
	void drawKeyboard();
	void checkOSCMsg();
	
	void drawMenu();
	void setupPerspective();
	// Demo Function
	void GestureDetect();
	void proceedRoute();
	
	long getCurrentTime();
};
#endif