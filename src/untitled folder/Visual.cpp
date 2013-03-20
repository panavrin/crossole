#include "Visual.h"

bool SnapRotateRight = false;
bool SnapRotateLeft = false;
bool SnapRHForward = false;
bool LHSnapUp = false;
bool LHSnapDown = false;
bool LHSnapRight = false;
bool LHSnapLeft = false;
bool RHSnapDown = false;
bool RHSnapUp = false;
bool RHPlayNote = false;
bool LHPlayNote = false;
bool controlKinect = true; // 1 if it is kinect, 0 if it is mouse

int prevKey = 0;
int prevKey2 = 0;

//--------------------------------------------------------------
void Visual::setup(){
	
	kinectListener.setup();
	ofAppBaseWindow * test = new ofAppBaseWindow();
	
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
}


//--------------------------------------------------------------
void Visual::update(){
	
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
			
			//			ofSetColor(block->m_colorRed,block->m_colorGreen,block->m_colorBlue,block->m_colorAlpha);
			ofSetColor(255,255,255,255);
			
			ofNoFill();
			
			//		ofRect(blockX - blockSize/2, blockY - blockSize/2, adjustBlockSize,adjustBlockSize);
			if ( block == m_inst->currentPlayingBlock()){
				ofSetColor(100 + 155 * (cos(m_inc) + 1.0 ) /2.0,
						   block->m_colorGreen + (255 - block->m_colorGreen) * (cos(m_inc * 1.2) + 1.0 ) /2.0,
						   block->m_colorBlue + (255 - block->m_colorBlue) * (cos(m_inc * 0.8) + 1.0 ) /2.0
						   ,block->m_colorAlpha);
				/*		ofSetColor(block->m_colorRed ,
				 block->m_colorGreen,
				 block->m_colorBlue
				 ,block->m_colorAlpha *  (cos(m_inc) + 1 ) /2);
				 */
				float shiver = cos(m_inc)  * 6;
				ofFill();
				ofRect(blockX - blockSize/2  + shiver/2, blockY - blockSize/2  + shiver/2, adjustBlockSize - shiver,adjustBlockSize- shiver);
			}
			else
			{
				//	float shiver = cos(m_inc)  * 4;
				ofSetColor(block->m_colorRed,block->m_colorGreen,block->m_colorBlue,block->m_colorAlpha);//*blockAlpha);
				ofFill();
				ofRect(blockX - blockSize/2, blockY - blockSize/2 , adjustBlockSize,adjustBlockSize);
				//	ofRect(blockX - blockSize/2  + shiver/2, blockY - blockSize/2  + shiver/2, adjustBlockSize - shiver,adjustBlockSize- shiver);
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
	
	glutWireCube(blockSize+2);
	
	char buffer [3];
	sprintf(buffer, "%d", m_pressedKey);
	glLineWidth(2.0);
	
	ofSetColor(255,0,100,255);	
	//	if (DEBUG)drawString(0,0,50,buffer,1);
	
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
		//		m_font.loadFont("verdana.ttf",  blockSize/3);
		// loading m_font makes visualization really slow
		//		m_font.drawString(ChordBook::baseNote[k] , blockX + pos -blockSize/2 + 5 ,blockY + offsetY+ blockSize/2);
		m_smallFont.drawString(*it , blockX  -blockSize/2 + 30 , 10 + pos + blockY + offsetY+ blockSize/2);
		//		m_font.drawString(ofToString((double)k, 0) , blockX + pos -blockSize/2 + 5 , 20 + blockY + offsetY+ blockSize/2);
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
//	cout<<"selected : "<<selectedBaseNote<<","<<selectedChordType<<"\n";
	ofPopMatrix();
}

void Visual::drawMenu(){
	ofSetColor(255,255,255);
	// chord type : random / sequence 
	/*	ofRect(m_centerX + 50, DEFAULT_CREATED_BLOCKY, 30, 30);
	 // tempo 
	 ofCircle(m_centerX + 100, DEFAULT_CREATED_BLOCKY + 15, 30); 
	 // volume
	 ofRect(m_centerX - 50, DEFAULT_CREATED_BLOCKY, 30, 30);
	 
	 // panning 
	 ofCircle(m_centerX - 100, DEFAULT_CREATED_BLOCKY + 15, 30); 
	 */	
	// channel	
	ofPushMatrix();
	glTranslatef(m_centerX + 100 , m_screenH -100, 0);
	for (int i = 0; i < 5 ; i++)
	{
		
		glTranslatef(60,0,0);
		ofSetColor(150,200,255,150);		
		if (i == 2){
			ofPushMatrix();
			ofSetColor(255,100,150,150);
			ofRotateZ(m_inc * 8.0);
		}
		
		glutSolidCube(30);
		//		ofSetColor(150,200,255,255);
		glutWireCube(30);
		if (i == 2)
			ofPopMatrix();
		
	}
	ofPopMatrix();
	
	ofPushMatrix();
	glTranslatef(m_centerX - 130 , m_screenH -100, 0);
	for (int i = 0; i < 5 ; i++)
	{
		
		glTranslatef(-60,0,0);
		ofSetColor(150,200,255,150);		
		if (i == 2){
			ofPushMatrix();
			ofSetColor(255,100,150,150);
			ofRotateZ(m_inc * 8.0);
		}
		
		glutSolidCube(30);
		//		ofSetColor(150,200,255,255);
		glutWireCube(30);
		if (i == 2)
			ofPopMatrix();
		
	}
	ofPopMatrix();
	
	ofCircle(m_screenW-100,100, 50);
	// automode on / off 
}

//--------------------------------------------------------------
void Visual::draw(){
	
	ofEnableSmoothing();
	
	//	setupPerspective();
	//	ofSetColor(255,100,255,255);
	//	m_font.drawString("crossole : crosspuzzle of sound", 10,30);
	
	ofSetColor(0,0,0,255);
	
	GLfloat ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	drawInstrument();
	if ( m_blockCreated )
	{
		// If we are in the placement state, set mouse coords
		// to the RH coords;
	/*	if( controlKinect && m_grabbedCreatedBlock)
		{
			if (!SnapRHForward)
			{
				m_mouseX = kinectListener.GetCoords(R_HAND, X_dim)*SCREEN_WIDTH+SCREEN_WIDTH/2;
				m_mouseY = kinectListener.GetCoords(R_HAND, Y_dim)*SCREEN_HEIGHT+SCREEN_HEIGHT/2;
			}
		}
	*/	
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
	
	drawMenu();
	if ( DEBUG)kinectListener.draw();
	
}

//--------------------------------------------------------------
//  Basic Controls demo
void Visual::GestureDetect()
{
	if (kinectListener.MoveInto(Z_dim, 1.5))
	{
		// 2. Rotate block using R-H position
		// 2.a. Rotate Right
		if (kinectListener.DetectSnap(R_HAND, X_dim, 0.4, &SnapRotateRight)) 
		{
			if ((m_blockCreated) && (!m_grabbedCreatedBlock) && (m_CPressed))
			{
				m_tCounterY =(m_tCounterY + 90);
				m_counter ++;
				if ( m_tCounterY <360 )
				{	
					m_tCounterY += 360;
					m_counterY += 360;
				}
				m_createdBlock->setRandomColor();
			}
		}
		
		// 2.b. Rotate Left
		if (kinectListener.DetectSnap(R_HAND, X_dim, -0.2, &SnapRotateLeft)) 
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
				m_createdBlock->setRandomColor();
			}
		}
		
		// Rotate down
		if (kinectListener.DetectSnap(R_HAND, Y_dim, 0.2, &RHSnapDown)) 
		{
			if ((m_blockCreated) && (!m_grabbedCreatedBlock) && (m_CPressed))
			{
				m_tCounterX =(m_tCounterX + 90);
				m_counter2++;
				m_createdBlock->setRandomColor();
			}
		}
		
		// Rotate up
		if (kinectListener.DetectSnap(R_HAND, Y_dim, -0.4, &RHSnapUp)) 
		{
			if ((m_blockCreated) && (!m_grabbedCreatedBlock) && (m_CPressed))
			{
				m_tCounterX =(m_tCounterX - 90);
				m_counter2--;
				m_createdBlock->setRandomColor();
			}
			
		}
		
		
		
		if (kinectListener.DetectStretch(R_HAND, Z_dim, -0.3)) 
		{
			if (!SnapRHForward)
			{
				mousePressed(kinectListener.GetCoords(R_HAND, X_dim)*SCREEN_WIDTH+SCREEN_WIDTH/2, kinectListener.GetCoords(R_HAND, Y_dim)*SCREEN_HEIGHT+SCREEN_HEIGHT/2, 0);
				SnapRHForward = true;
			}
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
		if (m_inst->getNumBlock() >0){
			
			if (kinectListener.DetectStretch(L_HAND, Y_dim, -0.4))
			{
				if (!LHSnapUp)
					m_inst->slideCursor(UP);
				LHSnapUp = true;
			}
			else {
				if (LHSnapUp)
					LHSnapUp = false;
			}

			
			if (kinectListener.DetectStretch(L_HAND, X_dim, -0.5))
			{
				if(!LHSnapLeft)
					m_inst->slideCursor(LEFT);
				LHSnapLeft = true;
			}
			else {
				if(LHSnapLeft)
					LHSnapLeft = false;
			}
			
			if (kinectListener.DetectStretch(L_HAND, Y_dim, 0.2))
			{
				if (!LHSnapDown)
					m_inst->slideCursor(DOWN);
				LHSnapDown = true;
			}
			else {
				if (LHSnapDown)
					LHSnapDown = false;
			}

			if (kinectListener.DetectStretch(L_HAND, X_dim, 0.01))
			{
				if(!LHSnapRight)
					m_inst->slideCursor(RIGHT);
				LHSnapRight = true;
			}
			else {
				if(LHSnapRight)
					LHSnapRight = false;
			}
		}
		
		m_inst->SendControls((kinectListener.GetCoords(R_HAND, X_dim)*SCREEN_WIDTH+SCREEN_WIDTH/2)*127/SCREEN_HEIGHT, 127+(kinectListener.GetCoords(R_HAND, Y_dim)*SCREEN_HEIGHT+SCREEN_HEIGHT/2)*-127/SCREEN_HEIGHT, -kinectListener.GetCoords(R_HAND, Z_dim)*127);
	}
	else { // If you are standing up close
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
	}
}

//--------------------------------------------------------------
void Visual::keyPressed(int key){
	m_pressedKey = key;
	if ( key == 358) 
	{
		m_tCounterY =(m_tCounterY + 90);
		m_counter ++;
		m_createdBlock->setRandomColor();
		
	}
	else if ( key  == 356 ) 
	{
		m_tCounterY =(m_tCounterY - 90);
		m_counter --;
		m_createdBlock->setRandomColor();
	}
	else if ( key == 357 )
	{
		m_tCounterX =(m_tCounterX + 90);
		m_counter2--;
		m_createdBlock->setRandomColor();
	}
	else if (key == 359) 
	{
		m_tCounterX =(m_tCounterX - 90);
		m_counter2++;
		m_createdBlock->setRandomColor();
	}
	else if (key == (int)'c')
	{
		if ( !m_blockCreated)
		{
			m_blockCreated = true;
			//	m_createdBlock = new Block();
			//	m_createdBlock->setRandomColor();
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
		m_inst->slideCursor(UP);
	else if (key == (int)'g')
		m_inst->slideCursor(LEFT);
	else if (key == (int)'h')
		m_inst->slideCursor(DOWN);
	else if (key == (int)'j')
		m_inst->slideCursor(RIGHT);
	else if (key == (int)'q')
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
}

//--------------------------------------------------------------
void Visual::keyReleased(int key){
}

//--------------------------------------------------------------
void Visual::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void Visual::mouseDragged(int x, int y, int button){
	m_mouseX = x;
	m_mouseY = y;
}

//--------------------------------------------------------------
void Visual::mousePressed(int x, int y, int button){
	m_mousePressed_= true;
	
	// grabbing the block (new block at the bottom or the block after selecting chord)
	if ( distance( m_createdBlock->getPointX(), m_createdBlock->getPointY(), x, y) < m_createdBlock->getSize() ) 
	{
		m_mouseX = x;
		m_mouseY = y;
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
			m_mouseX = x;
			m_mouseY = y;
			if ( m_CPressed ) 
				m_CPressed = false;
			m_blockCreated = true;
			delete m_createdBlock;
			m_createdBlock = new Block(block);
			m_createdBlock->setRandomColor();
			m_counter2 = m_createdBlock->m_counter2;
			m_counter = m_createdBlock->m_counter1;
		}
	}
	//ofCircle(m_screenW-100,100, 50);
	if ( distance(x,y,m_screenW-100,100) < 100)
	{
		m_inst->SendBassNote();
		cout<<"Succeded Bass Note !!!!!\n";
	}
	
}

//--------------------------------------------------------------
float Visual:: distance(float x1, float y1, float x2, float y2)
{
	return (float)sqrt(pow((x1-x2),2) + pow(y1-y2,2));
}

//--------------------------------------------------------------
void Visual::mouseReleased(int x, int y, int button){
	
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
		m_mouseX = m_screenW - 200;
		m_mouseY = 200;
		
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



Visual::~Visual()
{
	cout<<"visual destructor start\n";
	delete m_inst;
	delete m_createdBlock;
	cout<<"visual destructor end\n";
}
