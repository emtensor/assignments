/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.

	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

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

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;

void ofApp::setup() {

	ofSetWindowTitle("Maze Example");
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);

	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();

	/*
	isDFS = false;
	isBFS = false;
	isLHOW = false;
	isRHOW = false;
	*/

	isOpen = 0;

	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	myFont.loadFont("verdana.ttf", 12, true, true);

	hWnd = WindowFromDC(wglGetCurrentDC());

	ofSetEscapeQuitsApp(false);

	menu = new ofxWinMenu(this, hWnd);

	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	HMENU hMenu = menu->CreateWindowMenu();

	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	menu->AddPopupItem(hPopup, "Open", false, false);

	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true; 
	menu->AddPopupItem(hPopup, "Show DFS", false, false); // DFS ��ư
	bTopmost = false;
	menu->AddPopupItem(hPopup, "Show BFS", false, false); // BFS ��ư
	menu->AddPopupItem(hPopup, "Show Left Hand on Wall", false, false); // �¼��� ��ư
	menu->AddPopupItem(hPopup, "Show Right Hand on Wall", false, false); // �켱�� ��ư
	bFullscreen = false;
	menu->AddPopupItem(hPopup, "Full screen", false, false);

	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false);

	menu->SetWindowMenu();

}

void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if (title == "Open") {
		readFile();
	}
	if (title == "Exit") {
		ofExit(); // Quit the application
	}

	// Show DFS: DFS �˰��� ����
	if (title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			// isDFS�� 1�� ����
			isDFS = true;
			isBFS = false;
			isLHOW = false;
			isRHOW = false;
			DFS(); // DFS �˰��� ����
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	// Show BFS: BFS �˰��� ����
	if (title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly

		if (isOpen)
		{
			// isBFS�� 1�� ����
			isDFS = false;
			isBFS = true;
			isLHOW = false;
			isRHOW = false;
			BFS(); // BFS �˰��� ����
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	// Show Left Hand on Wall: �¼��� �˰��� ����
	if (title == "Show Left Hand on Wall") {
		doTopmost(bChecked); // Use the checked value directly

		if (isOpen)
		{
			// isLHOW�� 1�� ����
			isDFS = false;
			isBFS = false;
			isLHOW = true;
			isRHOW = false;
			LHOW(); // �¼��� �˰��� ����
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	// Show Right Hand on Wall: �켱�� �˰��� ����
	if (title == "Show Right Hand on Wall") {
		doTopmost(bChecked); // Use the checked value directly

		if (isOpen)
		{
			// isRHOW�� 1�� ����
			isDFS = false;
			isBFS = false;
			isLHOW = false;
			isRHOW = true;
			RHOW(); // �켱�� �˰��� ����
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

	if (title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if (title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;

	// TO DO : DRAW MAZE; 
	// ����� �ڷᱸ���� �̿��� �̷θ� �׸���.
	// add code here

	// �̷� �׸���: ' ', '+' ����, if ���ǹ����� (i - 1) �Ǵ� (j - 1)�� -1�� �Ǵ� ��� ����
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (input[i][j] == '-')
				ofDrawLine((j - 1) * LENGTH, i * LENGTH, (j + 1) * LENGTH, i * LENGTH);
			else if (input[i][j] == '|')
				ofDrawLine(j * LENGTH, (i - 1) * LENGTH, j * LENGTH, (i + 1) * LENGTH);
		}
	}
	// �Ա�, �ⱸ �׸���, �Ա�: �����, �ⱸ: û�ϻ�
	if (isOpen) {
		ofSetColor(255, 255, 0);
		ofDrawRectangle((start.col - 1) * LENGTH, (start.row - 1) * LENGTH, LENGTH * 2, LENGTH * 2);
		ofSetColor(0, 255, 255);
		ofDrawRectangle((end.col - 1) * LENGTH, (end.row - 1) * LENGTH, LENGTH * 2, LENGTH * 2);
	}
	// ���ӿ��� ���� ��ġ �׸���, ��ȫ��
	if (gameFlag) {
		ofSetColor(255, 0, 255);
		ofDrawRectangle((game.col - 1) * LENGTH, (game.row - 1) * LENGTH, LENGTH * 2, LENGTH * 2);
	}
	// DFS ��� �׸���
	if (isDFS) {
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	// BFS ��� �׸���
	if (isBFS) {
		ofSetLineWidth(5);
		if (isOpen)
			bfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	// �¼��� ��� �׸���
	if (isLHOW) {
		ofSetLineWidth(5);
		if (isOpen)
			drawLHOW();
		else
			cout << "You must open file first" << endl;
	}
	// �켱�� ��� �׸���
	if (isRHOW) {
		ofSetLineWidth(5);
		if (isOpen)
			drawRHOW();
		else
			cout << "You must open file first" << endl;
	}
	if (bShowInfo) {
		// Show keyboard duplicates of menu functions
		ofSetColor(200);
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight() - 20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if (bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else {
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if (bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Escape key exit has been disabled but it can be checked here
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if (key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

	// iŰ �Է�: �Ա�, �ⱸ ����
	if (key == 'i') {
		int r, c;

		printf("Entrance and Exit Settings\n");
		printf("The left top corner entry : 1 1\n");

		// ��ȿ�� ���� �Է��� ������ �ݺ�
		while (1) {
			printf("Input an entry of an entrance: ");
			scanf("%d %d", &r, &c);
			if (r <= 0 || c <= 0 || r * 2 - 1 > HEIGHT - 2 || c * 2 - 1 > WIDTH - 2)
				printf("Invalid input\n");
			else {
				start.row = r * 2 - 1;
				start.col = c * 2 - 1;
				break;
			}
		}

		// ��ȿ�� ���� �Է��� ������ �ݺ�
		while (1) {
			printf("Input an entry of an exit: ");
			scanf("%d %d", &r, &c);
			if (r <= 0 || c <= 0 || r * 2 - 1 > HEIGHT - 2 || c * 2 - 1 > WIDTH - 2)
				printf("Invalid input\n");
			else {
				end.row = r * 2 - 1;
				end.col = c * 2 - 1;
				break;
			}
		}

		// �˰��� �ٽ� ����
		if (isDFS) DFS(); 
		if (isBFS) BFS();
		if (isLHOW) LHOW();
		if (isRHOW) RHOW();
	}

	// qŰ �Է�: ���� ��� on/off
	if (key == 'q') {
		gameFlag = 1 - gameFlag;
		if (gameFlag) {
			// ���� ��ġ�� �Ա��� ����
			game.row = start.row;
			game.col = start.col;
			printf("GAME START\n");
		}
	}
} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	// ���� ���, ����Ű �Է����� ���� ��ġ �̵�, ���̸� �̵� �Ұ�
	if (gameFlag) {
		if (key == OF_KEY_UP && !graph[game.row - 1][game.col])
			game.row -= 2;
		else if (key == OF_KEY_DOWN && !graph[game.row + 1][game.col])
			game.row += 2;
		else if (key == OF_KEY_LEFT && !graph[game.row][game.col - 1])
			game.col -= 2;
		else if (key == OF_KEY_RIGHT && !graph[game.row][game.col + 1])
			game.col += 2;

		// �ⱸ�� ã���� ���� ����
		if (game.row == end.row && game.col == end.col) {
			gameFlag = 0;
			printf("CLEAR\n");
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		printf("file name is %s\n", fileName);
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0;

			// TO DO
			// .maz ������ input���� �޾Ƽ� ������ �ڷᱸ���� �ִ´�

			ofBuffer::Line it = buffer.getLines().begin();
			string line = *it;
			WIDTH = line.size(); // �̷��� �ʺ� ����
			HEIGHT = 1; // �̷��� ���� ����
			it++;

			// input ���� �Ҵ�
			input = (char**)malloc(sizeof(char*) * MAX_HEIGHT);
			for (int i = 0; i < MAX_HEIGHT; i++)
				input[i] = (char*)malloc(sizeof(char) * WIDTH);

			strcpy(input[idx++], line.c_str()); // �Է� �� �� ����
			for (ofBuffer::Line end = buffer.getLines().end(); it != end; it++, HEIGHT++) {
				string line = *it;
				strcpy(input[idx++], line.c_str()); // �Է� �� �� ���� ��
			}

			// �ʿ� ���� ���� �Ҵ� ����
			for (int i = idx; i < MAX_HEIGHT; i++)
				free(input[i]);

			// graph, visited ���� �Ҵ�
			// pathDFS, pathBFS ���� �Ҵ�
			graph = (int**)malloc(sizeof(int*) * HEIGHT);
			visited = (int**)malloc(sizeof(int*) * HEIGHT);
			pathDFS = (room**)malloc(sizeof(room*) * HEIGHT);
			pathBFS = (room**)malloc(sizeof(room*) * HEIGHT);
			pathLHOW = (room**)malloc(sizeof(room*) * HEIGHT);
			pathRHOW = (room**)malloc(sizeof(room*) * HEIGHT);
			for (int i = 0; i < HEIGHT; i++) {
				graph[i] = (int*)malloc(sizeof(int) * WIDTH);
				visited[i] = (int*)malloc(sizeof(int) * WIDTH);
				pathDFS[i] = (room*)malloc(sizeof(room) * WIDTH);
				pathBFS[i] = (room*)malloc(sizeof(room) * WIDTH);
				pathLHOW[i] = (room*)malloc(sizeof(room) * WIDTH);
				pathRHOW[i] = (room*)malloc(sizeof(room) * WIDTH);
			}
			
			// graph[i][j] = 1: ��, = 0: ��, visited �ʱ�ȭ
			// pathDFS, pathBFS �ʱ�ȭ
			// pathLHOW, pathRHOW �ʱ�ȭ
			for (int i = 0; i < HEIGHT; i++) {
				for (int j = 0; j < WIDTH; j++) {
					if(input[i][j] == '+' || input[i][j] == '-' || input[i][j] == '|')
						graph[i][j] = 1;
					else
						graph[i][j] = 0;
					visited[i][j] = 0;
					pathDFS[i][j].row = -1;
					pathDFS[i][j].col = -1;
					pathBFS[i][j].row = -1;
					pathBFS[i][j].col = -1;
					pathLHOW[i][j].row = -1;
					pathLHOW[i][j].col = -1;
					pathRHOW[i][j].row = -1;
					pathRHOW[i][j].col = -1;
				}
			}

			// �⺻ �Ա�, �ⱸ
			start.row = 1;
			start.col = 1;
			end.row = HEIGHT - 2;
			end.col = WIDTH - 2;

			// �˰��� �÷��� �ʱ�ȭ
			isDFS = false;
			isBFS = false;
			isLHOW = false;
			isRHOW = false;

			return true;
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}

void ofApp::freeMemory() {
	nodePointer temp;

	// input, graph, visited ���� �Ҵ� ����
	for (int i = 0; i < HEIGHT; i++) {
		free(input[i]);
		free(graph[i]);
		free(visited[i]);
		free(pathDFS[i]);
		free(pathBFS[i]);
	}
	free(input);
	free(graph);
	free(visited);
	free(pathDFS);
	free(pathBFS);

	// DFS ���� ���� �Ҵ� ����
	while (top) {
		temp = top;
		top = top->link;
		free(temp);
	}

	// DFS ��� ���� �Ҵ� ����
	while (topPathDFS) {
		temp = topPathDFS;
		topPathDFS = topPathDFS->link;
		free(temp);
	}

	// BFS ť ���� �Ҵ� ����
	while (front) {
		temp = front;
		front = front->link;
		free(temp);
	}

	// BFS ��� ���� �Ҵ� ����
	while (topPathBFS) {
		temp = topPathBFS;
		topPathBFS = topPathBFS->link;
		free(temp);
	}

	// �¼��� ��� ���� �Ҵ� ����
	while (topPathLHOW) {
		temp = topPathLHOW;
		topPathLHOW = topPathLHOW->link;
		free(temp);
	}

	// �켱�� ��� ���� �Ҵ� ����
	while (topPathRHOW) {
		temp = topPathRHOW;
		topPathRHOW = topPathRHOW->link;
		free(temp);
	}
}

bool ofApp::DFS() {
	room curr, next; // curr: ���� ��, next: ���� ��

	// visited, pathDFS �ʱ�ȭ
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			visited[i][j] = 0;
			pathDFS[i][j].row = -1;
			pathDFS[i][j].col = -1;
		}
	}
	
	freePath();

	top = NULL; // ���� �ʱ�ȭ
	topPathDFS = NULL; // ��� ���� �ʱ�ȭ
	adds(&top, start.row, start.col); // �Ա� push
	countDFS = 0; // �湮 Ƚ�� �ʱ�ȭ

	while (top) {
		curr = deletes(&top); // pop

		if (visited[curr.row][curr.col]) continue; // ���� ���� �湮������ true
		visited[curr.row][curr.col] = 1; // ���� �� �湮
		adds(&topPathDFS, curr.row, curr.col); // ��� push
		countDFS++; // ��� ���� ����

		if (curr.row == end.row && curr.col == end.col) break; // ���� �� == �ⱸ

		for (int i = 0; i < 4; i++) {
			// ���� ��
			next.row = curr.row + dir[i][0];
			next.col = curr.col + dir[i][1];

			// ���� ���� �̷ο� �ְ�, �湮���� �ʰ�, ���� �ƴϸ� true
			if (0 < next.row && next.row < HEIGHT - 1 &&
				0 < next.col && next.col < WIDTH - 1 &&
				!visited[next.row][next.col] &&
				!graph[next.row][next.col]) {
				adds(&top, next.row, next.col); // ���� �� push
				// ���� ���� ���� �� = ���� ��
				pathDFS[next.row][next.col].row = curr.row; 
				pathDFS[next.row][next.col].col = curr.col;
			}
		}
	}

	printf("# of visited rooms in DFS: %d\n", countDFS);

	return 0;
}

void ofApp::dfsdraw() {
	// ��� ���� top���� �׸���
	nodePointer currNode = topPathDFS;
	room curr, prev = pathDFS[currNode->roomPos.row][currNode->roomPos.col];

	// ����� ��� ���� �� ����
	ofSetColor(0, 0, 255);
	while (currNode && prev.row != -1 && prev.col != -1) {
		ofDrawLine(currNode->roomPos.col * LENGTH, currNode->roomPos.row * LENGTH, prev.col * LENGTH, prev.row * LENGTH);
		currNode = currNode->link;
		prev = pathDFS[currNode->roomPos.row][currNode->roomPos.col];
	}

	// �ⱸ���� �׸���
	curr.row = end.row;
	curr.col = end.col;
	prev = pathDFS[curr.row][curr.col];

	// ���� ��� ���� �� ����
	ofSetColor(255, 0, 0);
	while (prev.row != -1 && prev.col != -1) {
		ofDrawLine(prev.col * LENGTH, prev.row * LENGTH, curr.col * LENGTH, curr.row * LENGTH);
		curr = prev;
		prev = pathDFS[prev.row][prev.col];
	}
}

bool ofApp::BFS() {
	room curr, next; // curr: ���� ��, next: ���� ��

	// visited, pathBFS �ʱ�ȭ
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			visited[i][j] = 0;
			pathBFS[i][j].row = -1;
			pathBFS[i][j].col = -1;
		}
	}

	freePath();

	// ť �ʱ�ȭ
	front = NULL;
	rear = NULL;
	topPathBFS = NULL; // ��� ���� �ʱ�ȭ
	countBFS = 0; // �湮 Ƚ�� �ʱ�ȭ

	addq(&front, &rear, start.row, start.col); // �Ա� enqueue

	while (front) {
		curr = deleteq(&front); // dequeue

		if (visited[curr.row][curr.col]) continue; // ���� ���� �湮������ true
		visited[curr.row][curr.col] = 1; // ���� �� �湮
		adds(&topPathBFS, curr.row, curr.col); // ��� push
		countBFS++; // ��� ���� ����

		if (curr.row == end.row && curr.col == end.col) break; // ���� �� == �ⱸ

		for (int i = 0; i < 4; i++) {
			// ���� ��
			next.row = curr.row + dir[i][0];
			next.col = curr.col + dir[i][1];

			// ���� ���� �̷ο� �ְ�, �湮���� �ʰ�, ���� �ƴϸ� true
			if (0 < next.row && next.row < HEIGHT - 1 &&
				0 < next.col && next.col < WIDTH - 1 &&
				!visited[next.row][next.col] &&
				!graph[next.row][next.col]) {
				addq(&front, &rear, next.row, next.col); // ���� �� enqueue
				// ���� ���� ���� �� = ���� ��
				pathBFS[next.row][next.col].row = curr.row;
				pathBFS[next.row][next.col].col = curr.col;
			}
		}
	}

	printf("# of visited rooms in BFS: %d\n", countBFS);

	return 0;
}

void ofApp::bfsdraw() {
	// ��� ���� top���� �׸���
	nodePointer currNode = topPathBFS;
	room curr, prev = pathBFS[currNode->roomPos.row][currNode->roomPos.col];

	// ����� ��� ���� �� ����
	ofSetColor(0, 0, 255);
	while (currNode && prev.row != -1 && prev.col != -1) {
		ofDrawLine(currNode->roomPos.col * LENGTH, currNode->roomPos.row * LENGTH, prev.col * LENGTH, prev.row * LENGTH);
		currNode = currNode->link;
		prev = pathBFS[currNode->roomPos.row][currNode->roomPos.col];
	}

	// �ⱸ���� �׸���
	curr.row = end.row;
	curr.col = end.col;
	prev = pathBFS[curr.row][curr.col];

	// ���� ��� ���� �� ����
	ofSetColor(255, 0, 0);
	while (prev.row != -1 && prev.col != -1) {
		ofDrawLine(prev.col * LENGTH, prev.row * LENGTH, curr.col * LENGTH, curr.row * LENGTH);
		curr = prev;
		prev = pathBFS[prev.row][prev.col];
	}
}

bool ofApp::LHOW() {
	pRoom curr, next1, next2; // curr: ���� ��, next: ���� ��

	// pathLHOW �ʱ�ȭ
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			pathLHOW[i][j].row = -1;
			pathLHOW[i][j].col = -1;
		}
	}

	freePath(); // ����, ť ���� �Ҵ� ����

	topPathLHOW = NULL; // ��� ���� �ʱ�ȭ

	// ���� ��ġ�� �Ա��� ����
	curr.dir = 0;
	curr.row = start.row;
	curr.col = start.col;

	while (1) {
		adds(&topPathLHOW, curr.row, curr.col); // ��� push

		if (curr.row == end.row && curr.col == end.col) break; // ���� �� == �ⱸ

		// �������� ȸ��
		next1.dir = (curr.dir + 3) % 4;
		next1.row = curr.row + pDir[next1.dir][0];
		next1.col = curr.col + pDir[next1.dir][1];

		// ����
		next2.dir = curr.dir;
		next2.row = curr.row + pDir[next2.dir][0];
		next2.col = curr.col + pDir[next2.dir][1];

		// �������� �̵��� �� ������ �������� ����
		if (0 < next1.row && next1.row < HEIGHT - 1 &&
			0 < next1.col && next1.col < WIDTH - 1 &&
			!graph[next1.row][next1.col])
			curr.dir = next1.dir;
		// �������� �̵��� �� ����, ������ �� ������ ���������� ����
		else if (0 >= next2.row || next2.row >= HEIGHT - 1 ||
			0 >= next2.col || next2.col >= WIDTH - 1 ||
			graph[next2.row][next2.col])
			curr.dir = (curr.dir + 1) % 4;

		// ���� ��ġ�� ��ȿ�ϸ� pathLHOW�� ���� ���� �����ϰ�, ���� ��ġ�� ���� ��ġ�� ����
		if (0 < curr.row + pDir[curr.dir][0] && curr.row + pDir[curr.dir][0] < HEIGHT - 1 &&
			0 < curr.col + pDir[curr.dir][1] && curr.col + pDir[curr.dir][1] < WIDTH - 1 &&
			!graph[curr.row + pDir[curr.dir][0]][curr.col + pDir[curr.dir][1]]) {
			pathLHOW[curr.row + pDir[curr.dir][0]][curr.col + pDir[curr.dir][1]].row = curr.row;
			pathLHOW[curr.row + pDir[curr.dir][0]][curr.col + pDir[curr.dir][1]].col = curr.col;

			curr.row += pDir[curr.dir][0];
			curr.col += pDir[curr.dir][1];
		}
	}

	return 0;
}

void ofApp::drawLHOW() {
	// ��� ���� top���� �׸���
	nodePointer currNode = topPathLHOW;
	room curr, prev = pathLHOW[currNode->roomPos.row][currNode->roomPos.col];

	// ����� ��� ���� �� ����
	ofSetColor(0, 0, 255);
	while (currNode && prev.row != -1 && prev.col != -1) {
		ofDrawLine(currNode->roomPos.col * LENGTH, currNode->roomPos.row * LENGTH, prev.col * LENGTH, prev.row * LENGTH);
		currNode = currNode->link;
		prev = pathLHOW[currNode->roomPos.row][currNode->roomPos.col];
		if (prev.row == start.row && prev.col == start.col) break;
	}
}

bool ofApp::RHOW() {
	pRoom curr, next1, next2; // curr: ���� ��, next: ���� ��

	// pathRHOW �ʱ�ȭ
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			pathRHOW[i][j].row = -1;
			pathRHOW[i][j].col = -1;
		}
	}

	freePath(); // ����, ť ���� �Ҵ� ����

	topPathRHOW = NULL; // ��� ���� �ʱ�ȭ

	// ���� ��ġ�� �Ա��� ����
	curr.dir = 0;
	curr.row = start.row;
	curr.col = start.col;

	while (1) {
		adds(&topPathRHOW, curr.row, curr.col); // ��� push

		if (curr.row == end.row && curr.col == end.col) break; // ���� �� == �ⱸ

		// �������� ȸ��
		next1.dir = (curr.dir + 1) % 4;
		next1.row = curr.row + pDir[next1.dir][0];
		next1.col = curr.col + pDir[next1.dir][1];

		// ����
		next2.dir = curr.dir;
		next2.row = curr.row + pDir[next2.dir][0];
		next2.col = curr.col + pDir[next2.dir][1];

		// ���������� �̵��� �� ������ ���������� ����
		if (0 < next1.row && next1.row < HEIGHT - 1 &&
			0 < next1.col && next1.col < WIDTH - 1 &&
			!graph[next1.row][next1.col])
			curr.dir = next1.dir;
		// ���������� �̵��� �� ����, ������ �� ������ �������� ����
		else if (0 >= next2.row || next2.row >= HEIGHT - 1 ||
			0 >= next2.col || next2.col >= WIDTH - 1 ||
			graph[next2.row][next2.col])
			curr.dir = (curr.dir + 3) % 4;

		// ���� ��ġ�� ��ȿ�ϸ� pathRHOW�� ���� ���� �����ϰ�, ���� ��ġ�� ���� ��ġ�� ����
		if (0 < curr.row + pDir[curr.dir][0] && curr.row + pDir[curr.dir][0] < HEIGHT - 1 &&
			0 < curr.col + pDir[curr.dir][1] && curr.col + pDir[curr.dir][1] < WIDTH - 1 &&
			!graph[curr.row + pDir[curr.dir][0]][curr.col + pDir[curr.dir][1]]) {
			pathRHOW[curr.row + pDir[curr.dir][0]][curr.col + pDir[curr.dir][1]].row = curr.row;
			pathRHOW[curr.row + pDir[curr.dir][0]][curr.col + pDir[curr.dir][1]].col = curr.col;

			curr.row += pDir[curr.dir][0];
			curr.col += pDir[curr.dir][1];
		}
	}

	return 0;
}

void ofApp::drawRHOW() {
	// ��� ���� top���� �׸���
	nodePointer currNode = topPathRHOW;
	room curr, prev = pathRHOW[currNode->roomPos.row][currNode->roomPos.col];

	// ����� ��� ���� �� ����
	ofSetColor(0, 0, 255);
	while (currNode && prev.row != -1 && prev.col != -1) {
		ofDrawLine(currNode->roomPos.col * LENGTH, currNode->roomPos.row * LENGTH, prev.col * LENGTH, prev.row * LENGTH);
		currNode = currNode->link;
		prev = pathRHOW[currNode->roomPos.row][currNode->roomPos.col];
		if (prev.row == start.row && prev.col == start.col) break;
	}
}

void ofApp::freePath() {
	nodePointer temp;

	// DFS ���� ���� �Ҵ� ����
	while (top) {
		temp = top;
		top = top->link;
		free(temp);
	}

	// DFS ��� ���� �Ҵ� ����
	while (topPathDFS) {
		temp = topPathDFS;
		topPathDFS = topPathDFS->link;
		free(temp);
	}

	// BFS ť ���� �Ҵ� ����
	while (front) {
		temp = front;
		front = front->link;
		free(temp);
	}

	// BFS ��� ���� �Ҵ� ����
	while (topPathBFS) {
		temp = topPathBFS;
		topPathBFS = topPathBFS->link;
		free(temp);
	}

	// �¼��� ��� ���� �Ҵ� ����
	while (topPathLHOW) {
		temp = topPathLHOW;
		topPathLHOW = topPathLHOW->link;
		free(temp);
	}

	// �켱�� ��� ���� �Ҵ� ����
	while (topPathRHOW) {
		temp = topPathRHOW;
		topPathRHOW = topPathRHOW->link;
		free(temp);
	}

	// ����, ť NULL �ʱ�ȭ
	top = NULL;
	front = NULL;
	rear = NULL;
	topPathDFS = NULL;
	topPathBFS = NULL;
	topPathLHOW = NULL;
	topPathRHOW = NULL;
}