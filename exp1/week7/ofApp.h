#pragma once

#include "ofMain.h"
#include "waterObj.h"
#define MAX_PATH_SIZE 101

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    /* WaterFall-related member variables Regions */
    
    // flag variables
    int draw_flag;
    int load_flag;
    
    // Line segment and dot related variables
    int num_of_line, num_of_dot;
    float dot_diameter;
    
    /* WaterFall-related member functions */

	// �߰��� ����, lines: ���� �迭, dots: �� �迭, cur_dot: ������ ��
	// 2���� �߰��� ����, flow_flag: ���� �帣���� ����
	int** lines, ** dots, cur_dot = 0;
	int flow_flag;
    
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void initializeWaterLines(); // 2nd week portion.
		
};
