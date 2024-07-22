/*

	ofxWinMenu basic example - ofApp.h

	Copyright (C) 2016-2017 Lynn Jarvis.

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================
*/

#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon
#include <string.h>

#define MAX_HEIGHT 100 // �̷��� �ִ� ����
#define LENGTH 10 // �� ����

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key); // Traps escape key if exit disabled
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	bool readFile();
	void freeMemory();
	bool DFS();
	void dfsdraw();
	int HEIGHT;//�̷��� ����
	int WIDTH;//�̷��� �ʺ�
	char** input;//�ؽ�Ʈ ������ ��� ������ ��� ������ �迭�̴�.
	int** visited;//�湮���θ� ������ ����
	int maze_col;//�̷�ĭ�� ���� �ε����� ����Ų��.
	int maze_row;//�̷�ĭ�� ���� �ε����� ����Ų��.
	int k;
	int isOpen; //������ ���ȴ����� �Ǵ��ϴ� ����. 0�̸� �ȿ��Ȱ� 1�̸� ���ȴ�.
	int isDFS = false;
	int isBFS = false;
	// Menu
	ofxWinMenu* menu; // Menu object
	void appMenuFunction(string title, bool bChecked); // Menu return function

	// Used by example app
	ofTrueTypeFont myFont;
	ofImage myImage;
	float windowWidth, windowHeight;
	HWND hWnd; // Application window
	HWND hWndForeground; // current foreground window

	// Example menu variables
	bool bShowInfo;
	bool bFullscreen;
	bool bTopmost;
	bool isdfs;
	// Example functions
	void doFullScreen(bool bFull);
	void doTopmost(bool bTop);

	// �߰��� ���� �� �Լ�
	int** graph; // �׷���
	int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}}; // ����

	// ��
	typedef struct room {
		int row;
		int col;
	};

	// ���� ���� �����ϱ� ���� 2���� �迭
	room** pathDFS;
	room** pathBFS;

	// BFS
	bool BFS();
	void bfsdraw();

	// ���
	struct node {
		room roomPos;
		struct node* link;
	};
	typedef struct node* nodePointer;

	// ����, ť �ʱ�ȭ
	nodePointer top = NULL;
	nodePointer front = NULL;
	nodePointer rear = NULL;
	nodePointer topPathDFS = NULL;
	nodePointer topPathBFS = NULL;
	
	// ���� push
	void adds(nodePointer* top, int row, int col) {
		nodePointer temp = (nodePointer)malloc(sizeof(struct node));
		if (temp == NULL) return;

		temp->roomPos.row = row;
		temp->roomPos.col = col;
		temp->link = *top;
		*top = temp;
	}

	// ���� pop
	room deletes(nodePointer* top) {
		nodePointer temp = *top;
		room item;

		if (*top == NULL) {
			item.row = -1;
			item.col = -1;
			return item;
		}

		item = temp->roomPos;
		*top = temp->link;
		free(temp);

		return item;
	}

	// ť enqueue
	void addq(nodePointer* front, nodePointer* rear, int row, int col) {
		nodePointer temp = (nodePointer)malloc(sizeof(struct node));
		if (temp == NULL) return;

		temp->roomPos.row = row;
		temp->roomPos.col = col;
		temp->link = NULL;
		if (*front) (*rear)->link = temp;
		else *front = temp;
		*rear = temp;
	}

	// ť dequeue
	room deleteq(nodePointer* front) {
		nodePointer temp = *front;
		room item;

		if (*front == NULL) {
			item.row = -1;
			item.col = -1;
			return item;
		}

		item = temp->roomPos;
		*front = temp->link;
		free(temp);

		return item;
	}

	// ������Ʈ
	// �¼���, �켱���� �����ϱ� ���� ����ü
	typedef struct pRoom {
		int dir; // ����
		int row; // ��
		int col; // ��
	};

	room start, end; // start: �Ա�, end: �ⱸ
	int countDFS; // DFS���� �湮 Ƚ��
	int countBFS; // BFS���� �湮 Ƚ��
	room game; // ���ӿ��� ���� ��ġ
	int gameFlag = 0; // ���� �÷���, 1�̸� ���� ����
	int pDir[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} }; // ����, ��������
	int isLHOW = false; // �¼��� �÷���, 1�̸� �¼��� ����
	int isRHOW = false; // �켱�� �÷���, 1�̸� �켱�� ����
	room** pathLHOW; // �¼������� ���� ���� �����ϱ� ���� 2���� �迭
	room** pathRHOW; // �켱������ ���� ���� �����ϱ� ���� 2���� �迭
	nodePointer topPathLHOW = NULL; // �¼������� �湮�� ���� �����ϱ� ���� ����
	nodePointer topPathRHOW = NULL; // �켱������ �湮�� ���� �����ϱ� ���� ����

	bool LHOW(); // �¼��� �˰���
	bool RHOW(); // �켱�� �˰���
	void drawLHOW(); // �¼������� ã�� ��θ� �׸��� �Լ�
	void drawRHOW(); // �켱������ ã�� ��θ� �׸��� �Լ�
	void freePath(); // ����, ť�� ���� �Ҵ� �����ϴ� �Լ�
};
