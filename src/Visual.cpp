#include "Visual.h"

bool SnapRotateRight = false;
bool SnapRotateLeft = false;
bool SnapRHForward = false;
bool LHSnapUp = false;
bool LHSnapDown = false; scree
bool LHSnapRight = false;
bool LHSnapLeft = false;
bool RHSnapDown = false;
bool RHSnapUp = false;
bool controlKinect = true; // 1 if it is kinect, 0 if it is mouse
bool RHPlayNote = false;
bool LHPlayNote = false;

int prevKey = 0;
int prevKey2 = 0;

//--------------------------------------------------------------
void Visual::setup(){
	
	kinectListener.setup();
	ofAppBaseWindow * test = new ofAppBaseWindow();
	
	cout << "listening for osc messages on port " << PORT << "\n";
	m_listener.setup( RECEIVING_PORT );

	
	m_screenW = SCREEN_WIDTH;
	m_screenH = SCREEN_HEIGHT;
	int l;
	m_counter = 100;
	m_counter2 = 100;
	m_tCounterX = 	m_tCounterY = 	m_tCounterZ = m_counterX = m_counterY = m_counterZ  = 0;
	m_blockCreated = false;
	m_mousePressed_ = false;
	m_mouseX = m_centerX = SCREEN_WIDTH / 2;
	m_mouseY = m_centerY = SCREEN_HEIGHT / 2;
	ofSetWindowPosition(m_screenW/2-(int)m_centerX, m_screenH/2 - (int)m_centerY);
	ofSetFrameRate(60);
	ofBackground(0,0,0);
	//	ofBackground(255,255,255);
	
	m_CPressed = false;
	m_switchAxis = false;
	m_createdBlock = NULL;
	m_grabbedCreatedBlock = false;
	blockAlpha = 0.5;
	m_inst = new Instrument();
	//fglPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	m_inc = 0;
	
	DEBUG = true;
	
	m_chordBook = ChordBook::getChordBook();
	m_font.loadFont("verdana.ttf",  30);
	m_smallFont.loadFont("verdana.ttf", 20);
	
	m_createdBlock = new Block();
	m_createdBlock->setPoint(DEFAULT_CREATED_BLOCKX, DEFAULT_CREATED_BLOCKY, HIGHLEVEL_Z); // if mouse is dragged
	m_createdBlock->setRandomColor();
	routeMode = false;
	m_firstMoveInRoute = false;
	m_keyboard = false;
	m_previousRow = -1;
	m_previousCol = -1;
	m_KeyboardCursorAlpha = -1;
	m_receivedNum = 0;
	m_highlightKey = false;
	m_chordChanged = false;
	ofToggleFullscreen();
	m_rhX= m_rhY= m_rhZ= m_lhX= m_lhY= m_lhZ = 0;
}
void Visual::checkOSCMsg(){
	// check for waiting messages
	while( m_listener.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		m_listener.getNextMessage( &m );
		m_receivedNum = m.getArgAsInt32(0);
		m_highlightKey = 150;
		cout<<"Received Message:"<< m_receivedNum <<endl;
	}	
	
}

//--------------------------------------------------------------
void Visual::update(){
	
	checkOSCMsg();
	if (routeMode)
		proceedRoute();
	if ( controlKinect )kinectListener.update();
	if ( controlKinect )GestureDetect();
	m_inc +=0.1;
}

//--------------------------------------------------------------
void Visual::setupPerspective(){
	
	glViewport( 0, 0, m_screenW, m_screenH );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity();
    // create the viewing frustum
	// viewing angle, aspect ratio, nearest plane, farest plane 
    gluPerspective( 45.0, (GLfloat) m_screenW / (GLfloat) m_screenH, 1.0, 300 );
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity();
    // position the view point
    gluLookAt(0.0f, 0.0f,11, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
}

//--------------------------------------------------------------
void Visual::drawInstrument()
{
	
	ofPushMatrix();
	float blockX;
	float blockY;
	float blockSize;
	Block * block;
	
	if (m_inst->getNumBlock()>0)
	{
		// draw blocks. 
		for (int i = 0; i< m_inst->getNumBlock(); i++)
		{
			block = m_inst->getBlock(i);
			blockX = block->getPointX();
			blockY = block->getPointY();
			blockSize = block->getSize();
			
			int adjustBlockSize = blockSize - DEFAULT_BLOCK_ADJUSTEMENT;
			
			ofSetColor(255,255,255,255);
			ofNoFill();
			
			if ( block == m_inst->currentPlayingBlock()){
				ofSetColor(100 + 155 * (cos(m_inc) + 1.0 ) /2.0,
						   block->m_colorGreen + (255 - block->m_colorGreen) * (cos(m_inc * 1.2) + 1.0 ) /2.0,
						   block->m_colorBlue + (255 - block->m_colorBlue) * (cos(m_inc * 0.8) + 1.0 ) /2.0
						   ,block->m_colorAlpha);
				float shiver = cos(m_inc)  * 10;
				ofFill();
				ofRect(blockX - blockSize/2  + shiver/2, blockY - blockSize/2  + shiver/2, adjustBlockSize - shiver,adjustBlockSize- shiver);
			}
			else
			{
				ofSetColor(block->m_colorRed,block->m_colorGreen,block->m_colorBlue,block->m_colorAlpha);//*blockAlpha);
				ofFill();
				ofRect(blockX - blockSize/2, blockY - blockSize/2 , adjustBlockSize,adjustBlockSize);

			}
			ofSetColor(255,255,255,200);
			if(DEBUG)m_font.drawString(ChordBook::baseNote[block->getBaseNote()] , blockX-35 , blockY );
			if(DEBUG)m_smallFont.drawString(block->getChordType(), blockX-10 , blockY + 20);
		} 
		
		// draw Cursor
		point3f cursorPoint = m_inst->getCursorLocation();
		float cursorSize = DEFAULT_CURSOR_SIZE;
		//		if ( m_inst->isCursorOnBlock() )
		//			ofSetColor(255,100+(int)(150.0 * cos(m_inc)),100+(int)(150.0 * cos(m_inc)),255);
		//		else 
		ofSetColor(255,255,255,255);
		ofNoFill();
		ofRect(cursorPoint.x - cursorSize/2-DEFAULT_BLOCK_ADJUSTEMENT/2 -0.5 , cursorPoint.y - cursorSize/2-DEFAULT_BLOCK_ADJUSTEMENT/2 - 0.5, cursorSize, cursorSize);
		ofFill();
		
	}
	
	if ( m_grabbedCreatedBlock)
	{
		float x = m_mouseX ;
		float y = m_mouseY ;
		float z = LOWLEVEL_Z;
		
		m_inst->findNearestEmptySpot(&x, &y, &z, &m_foundRow, &m_foundCol);
		ofSetColor(m_createdBlock->m_colorRed,m_createdBlock->m_colorGreen,m_createdBlock->m_colorBlue,m_createdBlock->m_colorAlpha);
		ofNoFill();
		ofRect(x - DEFAULT_BLOCK_SIZE/2,y - DEFAULT_BLOCK_SIZE/2, DEFAULT_BLOCK_SIZE,DEFAULT_BLOCK_SIZE);
		ofSetColor(m_createdBlock->m_colorRed,m_createdBlock->m_colorGreen,m_createdBlock->m_colorBlue,m_createdBlock->m_colorAlpha * blockAlpha);
		ofFill();
		ofRect(x - DEFAULT_BLOCK_SIZE/2,y - DEFAULT_BLOCK_SIZE/2,DEFAULT_BLOCK_SIZE,DEFAULT_BLOCK_SIZE);
		
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void Visual::drawCreatedBlock(bool rotate){
	
	ofPushMatrix();
	
	
	float blockX = m_createdBlock->getPointX();
	float blockY = m_createdBlock->getPointY();
	float blockZ = m_createdBlock->getPointZ();
	float blockSize = m_createdBlock->getSize();
	
	if (rotate) blockY += cos(m_inc* 0.5) * 5;

	glTranslatef(blockX, blockY, blockZ);
	ofSetColor(m_createdBlock->m_colorRed,m_createdBlock->m_colorGreen,m_createdBlock->m_colorBlue,m_createdBlock->m_colorAlpha * blockAlpha);
	
	if ( m_tCounterY > m_counterY ) 
		m_counterY += ROTATE_UNIT;
	else if ( m_tCounterY < m_counterY ) 
		m_counterY -= ROTATE_UNIT;
	else 
		m_counterY = m_tCounterY = 0;
	if ( m_tCounterX > m_counterX ) 
		m_counterX += ROTATE_UNIT;
	else if ( m_tCounterX < m_counterX) 
		m_counterX -= ROTATE_UNIT;
	else 
		m_counterX = m_tCounterX = 0;
	if ( m_tCounterZ > m_counterZ ) 
		m_counterZ += ROTATE_UNIT;
	else if ( m_tCounterZ < m_counterZ) 
		m_counterZ -= ROTATE_UNIT;
	else 
		m_counterZ = m_tCounterZ = 0;
	
	ofRotateX(m_counterX);
	ofRotateY(m_counterY);
	ofRotateZ(m_counterZ);
	
	if (rotate){
		ofRotateY(m_inc * 10.0);
		ofPushMatrix();
		ofRotateX(90);
		glTranslatef(0,0,-50);
		ofRect(0 - blockSize/2,0 - blockSize/2,blockSize, blockSize);
		ofPopMatrix();
	}
	
	glutSolidCube(blockSize);
	ofSetColor(m_createdBlock->m_colorRed,m_createdBlock->m_colorGreen,m_createdBlock->m_colorBlue,m_createdBlock->m_colorAlpha);
//	ofSetColor(255,255,255);
	glutWireCube(blockSize+2);
	
	char buffer [3];
	sprintf(buffer, "%d", m_pressedKey);
	glLineWidth(2.0);
	
	ofSetColor(255,0,100,255);	
		if (DEBUG)drawString(0,0,50,buffer,1);
	
	ofPopMatrix();
	
}
//--------------------------------------------------------------
void Visual::drawChordBookNear()
{
	
	ofPushMatrix();
	float blockX = m_createdBlock->getPointX();
	float blockY =  m_createdBlock->getPointY();
	float blockZ = m_createdBlock->getPointZ();
	float blockSize = m_createdBlock->getSize();
	float offsetY = -blockSize/2;
	ofTranslate(0, 0, HIGHLEVEL_Z);
	list<string>::iterator it;
	
	int i = 0;
	ofNoFill();
	int selectedBaseNote;
	string selectedChordType;
	for(it = m_chordBook->chordType.begin(); it != m_chordBook->chordType.end(); it++)
	{
		int k = (i + m_counter2 ) % m_chordBook->chordType.size(); // r abs((i + (m_counter%5))%5);
		i++;
		if ( k > 4)
			continue;
		float pos = -2* blockSize + k * blockSize ;
		ofSetColor(m_createdBlock->m_colorRed,m_createdBlock->m_colorGreen,m_createdBlock->m_colorBlue,120 + 120 * - 0.25 * ( abs(k - 2) ));		
		ofRect(blockX  - blockSize/2,blockY + pos + offsetY,blockSize, blockSize);
		ofSetColor(255,255,255,240 + 240 * - 0.25 * ( abs(k - 2) ));	
		
		m_smallFont.drawString(*it , blockX  -blockSize/2 + 30 , 10 + pos + blockY + offsetY+ blockSize/2);
	
		ofSetColor(0xff3399);
		if ( k == 2) 
			selectedChordType = *it;
	}
	if ( selectedChordType == ""){
		cout<<"Error : Selected Chord Type is null\n";
	}
	
	//	offsetY -= 50;
	
	list<int>::iterator it2;
	i=0;
	for(it2 = m_chordBook->baseNoteInd.begin(); it2 != m_chordBook->baseNoteInd.end(); it2++)
	{
		
		int k = (i + m_counter ) % m_chordBook->baseNoteInd.size(); // r abs((i + (m_counter%5))%5);
		int size = m_chordBook->baseNoteInd.size();
		i++;
		int index = m_chordBook->baseNoteInd.size();
		if ( k > 4)
			continue;
		float pos = -2* blockSize+ k * blockSize ;
		ofSetColor(m_createdBlock->m_colorRed,m_createdBlock->m_colorGreen,m_createdBlock->m_colorBlue,120 + 120 * - 0.25 * ( abs(k - 2) ));		
		ofRect(blockX + pos - blockSize/2,blockY+ offsetY,blockSize, blockSize);
		ofSetColor(255,255,255,240 + 240 * - 0.25 * ( abs(k - 2) ));	
		//		m_font.loadFont("verdana.ttf",  blockSize/3);
		// loading m_font makes visualization really slow
		//		m_font.drawString(ChordBook::baseNote[k] , blockX + pos -blockSize/2 + 5 ,blockY + offsetY+ blockSize/2);
		m_font.drawString(ChordBook::baseNote[*it2] , blockX + pos -blockSize/2 + 5 ,blockY + offsetY+ blockSize/2);
		//		m_font.drawString(ofToString((double)k, 0) , blockX + pos -blockSize/2 + 5 , 20 + blockY + offsetY+ blockSize/2);
		ofSetColor(0xff3399);
		if ( k ==2) 
			selectedBaseNote = *it2;
	}
	m_createdBlock->setChord(selectedBaseNote, selectedChordType, m_counter2, m_counter);
	if ( m_chordChanged ) 
	{

		m_createdBlock->setColorByChord();
		m_chordChanged = false;
	}
//	m_createdBlock->setColorByChord();
	
//	cout<<"selected : "<<selectedBaseNote<<","<<selectedChordType<<"\n";
	ofPopMatrix();
}

void Visual::drawMenu(){
	ofSetColor(255,255,255);
	
	ofPushMatrix();
	glTranslatef(m_centerX - 130 , m_screenH-100, 0);
	for (int i = 0; i < 5 ; i++)
	{
		
		glTranslatef((float)m_lhX*i/3-100,-(float)m_lhY*i/3,(float)m_lhZ*i/3);
		ofSetColor(150,200,255,150);		
	/*	if (i >= 1){

			ofSetColor(255,100,150,150);
			ofRotateZ(m_inc * 10.0);
			ofRotateX(m_inc * 11.0);
			ofRotateY(m_inc * 12.5);
		}
	*/	
		glutSolidCube(30);
		//		ofSetColor(150,200,255,255);
		glutWireCube(30);
//		if (i >= 2)
//			ofPopMatrix();
		
	}
	ofPopMatrix();
	
	ofPushMatrix();
	glTranslatef(m_centerX + 100 , m_screenH-100, 0);
	for (int i = 0; i < 5 ; i++)
	{
		if ( m_rhX == 0 && m_rhY == 0&& m_rhZ == 0)
			glTranslatef(100,0,0);
		else 
			glTranslatef((float)m_rhX*+i/3+40,-(float)m_rhY*i/3,(float)m_rhZ*i/3);
		ofSetColor(150,200,255,150);		
	/*	if (i >= 1){
			
			ofSetColor(255,100,150,150);
			ofRotateZ(m_inc * 9.0);
			ofRotateX(m_inc * 10.0);
			ofRotateY(m_inc * 11.5);
		}
	*/	
		glutSolidCube(30);
		//		ofSetColor(150,200,255,255);
		glutWireCube(30);
//		if (i == 2)
//			ofPopMatrix();
		
	}
	ofPopMatrix();
	
	ofSetColor(255,0,0);
	// Recording
	if ( routeMode)
	{	
		ofFill();
		if ( m_inst->isLooping())
		{
			ofSetColor(0,255,0);
			ofTriangle(m_screenW - 90, 30,m_screenW - 90, 90, m_screenW - 30, 60);
			ofSetColor(255,0,0);
		}
		else
			ofCircle(m_screenW-70,70, 40);
	}
	else {
		ofNoFill();
		ofSetLineWidth(5);
		ofCircle(m_screenW-70,70, 40);
		ofSetLineWidth(2);
	}

	// automode on / off 
	

}

void Visual::proceedRoute(){
	if(m_inst->isLooping()){
		long currentTime = getCurrentTime();
		if ( !m_firstMoveInRoute && nextClockInRoute < currentTime)
		{
			m_inst->slideCursor(nextDirectionInRoute, currentTime);
			m_inst->nextAction(&nextDirectionInRoute, &nextClockInRoute, currentTime);
		}
		else if ( m_firstMoveInRoute){
			m_firstMoveInRoute = false;
			m_inst->nextAction(&nextDirectionInRoute, &nextClockInRoute, currentTime);
		}
	}
}

void Visual::drawKeyboard()
{
	// arpeggioOctaves[i]
	// arpeggioStructure[i]
	// draw keyboardd
	if (m_keyboard){
		checkOSCMsg();
		float sizeX = (float)(m_screenW - MARGIN_KEYBOARD) / NUM_COLUMN_KEYBOARD;
		float sizeY = (float)(m_screenH - MARGIN_KEYBOARD) / NUM_ROW_KEYBOARD;
		Block * block = m_inst->currentPlayingBlock();
		int red = 200;
		int blue = 200; 
		int green = 200;
		if (block != NULL)
		{
			red = block->m_colorRed;
			green = block->m_colorGreen;
			blue = block->m_colorBlue;
		}

		for (int i=0 ; i < NUM_COLUMN_KEYBOARD ; i++){
			for (int j=0; j < NUM_ROW_KEYBOARD ; j++){
				ofNoFill();

				if ( (NUM_ROW_KEYBOARD-j-1) == ChordBook::getChordBook()->arpeggioVisual[i])
				{		
					ofFill();
					ofSetColor(red,green,blue,200);
				}		
				else
					ofSetColor(100,100,100,200);
				
				ofRect(MARGIN_KEYBOARD/2 + i * sizeX, MARGIN_KEYBOARD/ 2 + j * sizeY, sizeX, sizeY);
			}
			
			if (m_highlightKey>0 && i == m_receivedNum)
			{
				ofFill();
				ofSetColor(100,100,100,200);
				ofRect(MARGIN_KEYBOARD/2 + i * sizeX, MARGIN_KEYBOARD/ 2 , sizeX, m_screenH - MARGIN_KEYBOARD);
				ofNoFill();
				ofSetColor(red,green,blue,200);
				ofRect(MARGIN_KEYBOARD/2 + i * sizeX, MARGIN_KEYBOARD/ 2 , sizeX, m_screenH - MARGIN_KEYBOARD);
				m_highlightKey -= 5;
			}
		}
		
		if ( m_mousePressed_ ) {
			
			int newCol = (int)((m_mouseX - MARGIN_KEYBOARD/2) / sizeX);
			int newRow = (int)((m_mouseY - MARGIN_KEYBOARD/2) / sizeY);
			if ( m_mouseX < MARGIN_KEYBOARD / 2)
				newCol = -1;
			if( m_KeyboardCursorAlpha > 0){
				ofSetColor(red,green,blue,200);
				ofFill();
				
				m_KeyboardCursorAlpha-=5;
				int newKeyX  = MARGIN_KEYBOARD/2 + newCol*sizeX;
				int newKeyY  = MARGIN_KEYBOARD/2 + newRow*sizeY;
				ofRect(newKeyX,newKeyY,  sizeX, sizeY);
			}
			if (m_previousCol != newCol)
			{
				if (m_previousCol < newCol&& newCol >=0 && newCol <NUM_COLUMN_KEYBOARD&& newRow >=0 && newRow <NUM_ROW_KEYBOARD)	
				{	
					m_KeyboardCursorAlpha = 255;
//					ChordBook::getChordBook()->arpeggioVisual[newCol] = NUM_ROW_KEYBOARD - 1 - newRow;
					ChordBook::getChordBook()->arpeggioOctaves[newCol] = (int)((NUM_ROW_KEYBOARD - 1 - newRow) / ARPEGGIO_NUM_NOTE_IN_OCTAVE) + ARPEGGIO_MIN_OCTAVE;
					ChordBook::getChordBook()->arpeggioStructure[newCol] = (int)(NUM_ROW_KEYBOARD - 1 - newRow) % ARPEGGIO_NUM_NOTE_IN_OCTAVE;
					ChordBook::getChordBook()->arpeggioVisual.clear();
					ChordBook::getChordBook()->arpeggioSequence();
					m_inst->sendNewContour();
				}
				else if (ChordBook::getChordBook()->arpeggioVisual[newCol] == NUM_ROW_KEYBOARD - 1 - newRow && m_previousCol > newCol && newCol >=0 && newCol <NUM_COLUMN_KEYBOARD
						 && newRow >=0 && newRow <NUM_ROW_KEYBOARD )
				{
					ChordBook::getChordBook()->arpeggioOctaves[newCol] = -1;//(int)((NUM_ROW_KEYBOARD - 1 - newRow) / ARPEGGIO_NUM_NOTE_IN_OCTAVE) + ARPEGGIO_MIN_OCTAVE;
					ChordBook::getChordBook()->arpeggioStructure[newCol] = 0;(int)(NUM_ROW_KEYBOARD - 1 - newRow) % ARPEGGIO_NUM_NOTE_IN_OCTAVE;
					ChordBook::getChordBook()->arpeggioVisual.clear();
					ChordBook::getChordBook()->arpeggioSequence();
					m_inst->sendNewContour();
				}
				
				m_previousCol = newCol;
				m_previousRow = newRow;

			}
				
	
		}
	}
}	

//--------------------------------------------------------------
void Visual::draw(){
	if (routeMode)
		proceedRoute();

	ofEnableSmoothing();
	
	//	setupPerspective();
	if ( routeMode)
	{
		if (m_inst->isLooping())
		{
			ofSetColor(0,255,0,255);
			m_font.drawString("Playing", m_screenW-250,70);
		}else
		{
			ofSetColor(255,0,0,255);
			m_font.drawString("REC...", m_screenW-330,80);
		}
	}
	
	ofSetColor(0,0,0,255);
	
	GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	drawInstrument();

	if ( m_blockCreated )
	{
		m_createdBlock->setPoint(m_mouseX, m_mouseY, HIGHLEVEL_Z); // if mouse is dragged
		drawCreatedBlock(false);
		if ( m_CPressed)
			drawChordBookNear();
	}
	else{
		ofPushMatrix();
		drawCreatedBlock(true);
		ofPopMatrix();
	}
	if (( routeMode && m_inst->isLooping())|| m_keyboard)
	{
		ofSetColor(0,0,0,150);
		ofRect(0,0,m_screenW, m_screenH);
	}

	drawKeyboard();
/*	ofSetColor(0, 255, 0);
	m_font.drawString(ofToString(m_blockCreated), 100, 50);
	ofSetColor(255, 0, 0);
	m_font.drawString(ofToString(m_keyboard), 50, 50);
	ofSetColor(0, 0, 255);
	m_font.drawString(ofToString(routeMode), 150, 50);
	ofSetColor(255, 0, 0);
	m_font.drawString(ofToString(m_inst->isLooping()), 200, 50);
*/
	
	drawMenu();
	if ( DEBUG) kinectListener.draw();
	
}

//--------------------------------------------------------------
//  Basic Controls demo
void Visual::GestureDetect()
{
	if (kinectListener.MoveInto(Z_dim, 1.5))
	{
		// Disable keyboard and mouse 
		m_keyboard = false;
		m_mousePressed_ = false;
		
		// 2. Rotate block using R-H position
		// 2.a. Rotate Right
		if (kinectListener.DetectStretch(R_HAND, X_dim, 0.6)) 
		{
			if (!SnapRotateRight)
			{
				if ((m_blockCreated) && (!m_grabbedCreatedBlock) && (m_CPressed))
				{
					m_tCounterY = (m_tCounterY + 90);
					m_counter ++;
					if ( m_tCounterY <360 )
					{	
						m_tCounterY += 360;
						m_counterY += 360;
					}
					m_chordChanged = true;
//					m_createdBlock->setRandomColor();
				}
			}
			SnapRotateRight = true;
		}
		else {
			if (SnapRotateRight)
				SnapRotateRight = false;
		}

		
		// 2.b. Rotate Left
		if (kinectListener.DetectStretch(R_HAND, X_dim, -0.2)) 
		{
			if (!SnapRotateLeft)
			{
				if ((m_blockCreated) && (!m_grabbedCreatedBlock) && (m_CPressed))
				{
					m_tCounterY =(m_tCounterY - 90);
					if ( m_tCounterY >=360 )
					{	
						m_tCounterY -= 360;
						m_counterY -= 360;
					}
					m_counter --;
					m_chordChanged = true;
//					m_createdBlock->setRandomColor();
				}
				SnapRotateLeft = true;
			}
		}
		else {
			if (SnapRotateLeft)
				SnapRotateLeft = false;

		}

		
		// Rotate down
		if (kinectListener.DetectStretch(R_HAND, Y_dim, 0.2)) 
		{
			if (!RHSnapDown)
			{
				if ((m_blockCreated) && (!m_grabbedCreatedBlock) && (m_CPressed))
				{
					m_tCounterX =(m_tCounterX + 90);
					m_counter2++;
					m_chordChanged = true;
//					m_createdBlock->setRandomColor();
				}
				RHSnapDown = true;
			}
		}
		else {
			if (RHSnapDown)
				RHSnapDown = false;
		}
		
		// Rotate up
		if (kinectListener.DetectStretch(R_HAND, Y_dim, -0.4)) 
		{
			if (!RHSnapUp)
			{
				if ((m_blockCreated) && (!m_grabbedCreatedBlock) && (m_CPressed))
				{
					m_tCounterX =(m_tCounterX - 90);
					m_counter2--;
					m_chordChanged = true;
//					m_createdBlock->setRandomColor();
				}
				RHSnapUp = true;
			}
		}
		else {
			if (RHSnapUp)
				RHSnapUp = false;
		}
		
		if (kinectListener.DetectStretch(R_HAND, Z_dim, -0.3)) 
		{
			if (!SnapRHForward)
			{
				mousePressed(kinectListener.GetCoords(R_HAND, X_dim)*SCREEN_WIDTH+SCREEN_WIDTH/2, kinectListener.GetCoords(R_HAND, Y_dim)*SCREEN_HEIGHT+SCREEN_HEIGHT/2, -1);
				SnapRHForward = true;
			}
			if (SnapRHForward && kinectListener.DetectStretch(R_HAND, Z_dim, -0.4))
				mouseDragged(kinectListener.GetCoords(R_HAND, X_dim)*SCREEN_WIDTH+SCREEN_WIDTH/2, kinectListener.GetCoords(R_HAND, Y_dim)*SCREEN_HEIGHT+SCREEN_HEIGHT/2, 0);		
		}
		else
		{
			if (SnapRHForward)
			{
				mouseReleased(kinectListener.GetCoords(R_HAND, X_dim)*SCREEN_WIDTH+SCREEN_WIDTH/2, kinectListener.GetCoords(R_HAND, Y_dim)*SCREEN_HEIGHT+SCREEN_HEIGHT/2, 0);
				SnapRHForward = false;
			}
			
		}
		
		// 4. Left hand controls play cursor
		// Using basic snap gestures for now
		if (m_inst->getNumBlock() >0 && !m_inst->isLooping()){
			
			if (kinectListener.DetectStretch(L_HAND, Y_dim, -0.4))
			{
				if (!LHSnapUp)
					m_inst->slideCursor(UP, getCurrentTime());
				LHSnapUp = true;
			}
			else {
				if (LHSnapUp)
					LHSnapUp = false;
			}
			
			if (kinectListener.DetectStretch(L_HAND, X_dim, -0.5))
			{
				if(!LHSnapLeft)
					m_inst->slideCursor(LEFT, getCurrentTime());
				LHSnapLeft = true;
			}
			else {
				if(LHSnapLeft)
					LHSnapLeft = false;
			}
			
			if (kinectListener.DetectStretch(L_HAND, Y_dim, 0.2))
			{
				if (!LHSnapDown)
					m_inst->slideCursor(DOWN, getCurrentTime());
				LHSnapDown = true;
			}
			else {
				if (LHSnapDown)
					LHSnapDown = false;
			}

			if (kinectListener.DetectStretch(L_HAND, X_dim, 0.01))
			{
				if(!LHSnapRight)
					m_inst->slideCursor(RIGHT, getCurrentTime());
				LHSnapRight = true;
			}
			else {
				if(LHSnapRight)
					LHSnapRight = false;
			}
		}
		
		if ( routeMode ) 
			if (m_inst->isLooping())
			{
				m_rhX = (kinectListener.GetCoords(R_HAND, X_dim)*SCREEN_WIDTH+SCREEN_WIDTH/2)*127/SCREEN_HEIGHT;
				m_rhY = 127+(kinectListener.GetCoords(R_HAND, Y_dim)*SCREEN_HEIGHT+SCREEN_HEIGHT/2)*-127/SCREEN_HEIGHT;
				m_rhZ =-kinectListener.GetCoords(R_HAND, Z_dim)*127;
				m_lhX = (kinectListener.GetCoords(L_HAND, X_dim)*SCREEN_WIDTH+SCREEN_WIDTH/2)*127/SCREEN_HEIGHT;
				m_lhY = 127+(kinectListener.GetCoords(L_HAND, Y_dim)*SCREEN_HEIGHT+SCREEN_HEIGHT/2)*-127/SCREEN_HEIGHT;
				m_lhZ = -kinectListener.GetCoords(L_HAND, Z_dim)*127;
				m_inst->SendControls(m_rhX, m_rhY, m_rhZ, m_lhX, m_lhY, m_lhZ);
			}
	}
	else { 
		// Enable the keyboard
		m_keyboard = true;
		m_mousePressed_ = true;
		
		m_mouseX = (kinectListener.GetCoords(R_HAND, X_dim)*SCREEN_WIDTH+SCREEN_WIDTH/2);
		m_mouseY = (kinectListener.GetCoords(R_HAND, Y_dim)*SCREEN_HEIGHT+SCREEN_HEIGHT/2);
		
		/*
		// If you are standing up close
		// 1. Hands are now cursor-things
		if (kinectListener.DetectStretch(R_HAND, Z_dim, -0.3))
		{
			float key = (kinectListener.GetCoords(R_HAND, X_dim)*SCREEN_WIDTH+SCREEN_WIDTH/2)*12/SCREEN_WIDTH;
			
			if ((int)key != prevKey)
			{
				printf("Key = %d, prevKey = %d", (int)key, prevKey);
				prevKey = (int)key;
				m_inst->SendNote(60 + (int)key, 127+(kinectListener.GetCoords(R_HAND, Y_dim)*SCREEN_HEIGHT+SCREEN_HEIGHT/2)*-127/SCREEN_HEIGHT);
			}
		}
		else {
			RHPlayNote = false;
			prevKey = 0;
		}
		
		if (kinectListener.DetectStretch(L_HAND, Z_dim, -0.3))
		{
			float key2 = (kinectListener.GetCoords(L_HAND, X_dim)*SCREEN_WIDTH+SCREEN_WIDTH/2)*12/SCREEN_WIDTH;
			
			if ((int)key2 != prevKey2)
			{
				printf("Key = %d, prevKey = %d", (int)key2, prevKey2);
				prevKey2 = (int)key2;
				m_inst->SendNote(60 + (int)key2, 127+(kinectListener.GetCoords(L_HAND, Y_dim)*SCREEN_HEIGHT+SCREEN_HEIGHT/2)*-127/SCREEN_HEIGHT);
			}
		}
		else {
			LHPlayNote = false;
			prevKey2 = 0;
		}
		 */
	}
}

//--------------------------------------------------------------
void Visual::keyPressed(int key){
	m_pressedKey = key;
	if ( key == 358) 
	{
		m_tCounterY =(m_tCounterY + 90);
		m_counter ++;
		m_chordChanged = true;
//		m_createdBlock->setRandomColor();
		
	}
	else if ( key  == 356 ) 
	{
		m_tCounterY =(m_tCounterY - 90);
		m_counter --;
		m_chordChanged = true;
	//	m_createdBlock->setRandomColor();
	}
	else if ( key == 357 )
	{
		m_tCounterX =(m_tCounterX + 90);
		m_counter2--;
		m_chordChanged = true;
	//	m_createdBlock->setRandomColor();
	}
	else if (key == 359) 
	{
		m_tCounterX =(m_tCounterX - 90);
		m_counter2++;
		m_chordChanged = true;
	//	m_createdBlock->setRandomColor();
	}
	else if (key == (int)'c')
	{
		if ( !m_blockCreated)
		{
			m_blockCreated = true;

		}
	}
	else if (key == (int)' '){
		m_CPressed = !m_CPressed;
	}
	else if ( key == (int)'d')
		DEBUG = !DEBUG;
	else if (key == (int)'f')
		ofToggleFullscreen();	
	else if (key == (int)'k')
		controlKinect != controlKinect;
	else if (key == (int)'y')
		m_inst->slideCursor(UP, getCurrentTime());
	else if (key == (int)'g')
		m_inst->slideCursor(LEFT, getCurrentTime());
	else if (key == (int)'h')
		m_inst->slideCursor(DOWN, getCurrentTime());
	else if (key == (int)'j')
		m_inst->slideCursor(RIGHT, getCurrentTime());
	else if (key == (int)'a')
		m_keyboard = !m_keyboard;
/*	else if (key == (int)'q')
	{
		ofxOscSender m_sender;
		ofxOscMessage m_msg;
		
		cout<<"sending osc msg\n";
		string sentMsg;
		m_sender.setup( "192.168.1.3", 7400 );
		sentMsg = "q";
		cout << sentMsg << endl;
		m_msg.setAddress(sentMsg);
		m_sender.sendMessage( m_msg);
	}
*/	else if ( key == (int)'r')
	{
		if( routeMode )
		{
			routeMode = false;
			m_inst->stopRecordingRoute();
//			m_inst->SendControls(0,0,0,0,0,0);
			
		}
		else {
			routeMode = true;
			m_firstMoveInRoute = true;
			m_inst->startRecordingRoute();
		}

	}
}

//--------------------------------------------------------------
void Visual::keyReleased(int key){
}

//--------------------------------------------------------------
void Visual::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void Visual::mouseDragged(int x, int y, int button){
	bool grabbedTheBlock = distance( m_createdBlock->getPointX(), m_createdBlock->getPointY(), x, y) < m_createdBlock->getSize();
	if ( button != -1 ){
		m_mouseX = x;
		m_mouseY = y;
	}
	else if ( button == -1 && grabbedTheBlock)
	{
		m_mouseX = x;
		m_mouseY = y;
	}
	
}

//--------------------------------------------------------------
void Visual::mousePressed(int x, int y, int button){
	
	//ofCircle(m_screenW-100,100, 50);
/*	if ( distance(x,y,m_screenW-100,100) < 100)
	{
		if( routeMode )
		{
			routeMode = false;
			m_inst->stopRecordingRoute();
//			m_inst->SendControls(0,0,0,0,0,0);
		}
		else {
			routeMode = true;
			m_firstMoveInRoute = true;
			m_inst->startRecordingRoute();
		}
	}
*/	
	if ( m_inst->isLooping())
		return;
	
	m_mousePressed_= true;
/*	float sizeX = (float)(m_screenW - MARGIN_KEYBOARD) / NUM_COLUMN_KEYBOARD;
	float sizeY = (float)(m_screenH - MARGIN_KEYBOARD) / NUM_ROW_KEYBOARD;
	m_previousCol = (int)((m_mouseX - MARGIN_KEYBOARD/2) / sizeX);
	m_previousRow = (int)((m_mouseY - MARGIN_KEYBOARD/2) / sizeY);
*/
	//	m_previousCol = m_mouseX / (int)((m_screenW) / 4.0);
//	m_previousRow = m_mouseX / (int)((m_screenW) / 4.0);
	// grabbing the block (new block at the bottom or the block after selecting chord)
	bool grabbedTheBlock = distance( m_createdBlock->getPointX(), m_createdBlock->getPointY(), x, y) < m_createdBlock->getSize();
	
	if ( button != -1 ){
		m_mouseX = x;
		m_mouseY = y;
	}
	else if ( button == -1 && grabbedTheBlock)
	{
		m_mouseX = x;
		m_mouseY = y;
	}
			 
	if (grabbedTheBlock) 
	{

		if (m_blockCreated) m_grabbedCreatedBlock = true;
		if ( m_CPressed ) 
			m_CPressed = false;
		m_blockCreated = true;
	}
	else  
	{
		float xf = x;
		float yf = y;
		float z = LOWLEVEL_Z;
		int row, col;
		row  = col = -100;
		m_inst->findNearestEmptySpot(&xf, &yf, &z, &row, &col);
		
		
		// duplicating the block from the figure
		if ( m_inst->hasBlockAt(row, col))
		{
			Block * block = m_inst->getBlock(row, col);

			if ( m_CPressed ) 
				m_CPressed = false;
			m_blockCreated = true;
			delete m_createdBlock;
			m_createdBlock = new Block(block);
//			m_createdBlock->setRandomColor();
			m_chordChanged = true;
			m_createdBlock->setPoint(m_mouseX, m_mouseY, HIGHLEVEL_Z); // if mouse is dragged

			m_counter2 = m_createdBlock->m_counter2;
			m_counter = m_createdBlock->m_counter1;
		}
	}
	
	if ( distance(x,y,50,50) < 50 && !routeMode)
	{
		delete m_inst;
		m_inst = new Instrument();
		m_inst->sendQuitMessage();
	}
	
}

//--------------------------------------------------------------
float Visual:: distance(float x1, float y1, float x2, float y2)
{
	return (float)sqrt(pow((x1-x2),2) + pow(y1-y2,2));
}

//--------------------------------------------------------------
void Visual::mouseReleased(int x, int y, int button){
	m_mousePressed_= false;

	if ( m_blockCreated && m_grabbedCreatedBlock ) 
	{
		// TODO : add block to the Instrument
		if  (m_inst->getNumBlock() == 0){
			m_createdBlock->setPoint(m_mouseX, m_mouseY, LOWLEVEL_Z);
			if ( !m_inst->addFirstBlock(m_createdBlock))
				cout<< "The Block was not created succesfully\n";
			cout<<"Num Block:" << m_inst->getNumBlock()<<"\n";
		}
		else {
			m_inc++;
			if ( !m_inst->addBlock(m_foundRow,m_foundCol, m_createdBlock) ) 
				cout<< "The Block was not created succesfully\n";
			cout<<"Num Block:" << m_inst->getNumBlock()<<"\n";
		}
		m_blockCreated = false;
		m_grabbedCreatedBlock = false;
		m_centerX = m_screenW / 2;
		m_centerY = m_screenH / 2;
	//	m_mouseX = m_screenW - 200;
	//	m_mouseY = 200;
		
		m_createdBlock = new Block();
		m_createdBlock->setRandomColor();

		m_createdBlock->setPoint(DEFAULT_CREATED_BLOCKX, DEFAULT_CREATED_BLOCKY, HIGHLEVEL_Z); // if mouse is dragged
	}
	else if ( m_blockCreated && !m_grabbedCreatedBlock)
		// placing the block grabbed from the bottom or the duplicateed block
	{
		m_CPressed = true;
	}
	
}

//--------------------------------------------------------------
void Visual::windowResized(int w, int h){
	
}

void Visual::drawString( GLfloat x, GLfloat y, GLfloat z, string str, GLfloat scale = 1.0f )
{
    GLint len = str.length(), i;
	// line width    glScalef( .001f * scale, .001f * scale, .001f * scale );
  	ofDrawBitmapString(str, x,y);
}

void Visual::createBlock()
{
	
}


long Visual::getCurrentTime()
{
	struct timeval curTime;
    gettimeofday(&curTime, NULL);
	return (curTime.tv_sec * 1000 + curTime.tv_usec / 1000);
}


Visual::~Visual()
{
	cout<<"visual destructor start\n";
	delete m_inst;
	delete m_createdBlock;
	cout<<"visual destructor end\n";
}
