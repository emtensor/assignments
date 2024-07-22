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
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isDFS = false;
	isBFS = false;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS", false, false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
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

	//
	// Window menu
	//
	if (title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			DFS();
			isDFS = true;
			isBFS = false;
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "Show BFS") {
		doTopmost(bChecked); // Use the checked value directly

		if (isOpen)
		{
			BFS();
			isDFS = false;
			isBFS = true;
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
	if (isDFS) {
		ofSetLineWidth(5);
		if (isOpen)
			dfsdraw();
		else
			cout << "You must open file first" << endl;
	}
	if (isBFS) {
		ofSetLineWidth(5);
		if (isOpen)
			bfsdraw();
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

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

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
			for (int i = 0; i < HEIGHT; i++) {
				graph[i] = (int*)malloc(sizeof(int) * WIDTH);
				visited[i] = (int*)malloc(sizeof(int) * WIDTH);
				pathDFS[i] = (room*)malloc(sizeof(room) * WIDTH);
				pathBFS[i] = (room*)malloc(sizeof(room) * WIDTH);
			}
			
			// graph[i][j] = 1: ��, = 0: ��, visited �ʱ�ȭ
			// pathDFS, pathBFS �ʱ�ȭ
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
				}
			}

			isDFS = false;
			isBFS = false;

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
	while (topPathBFS) {
		temp = topPathDFS;
		topPathDFS = top->link;
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
		topPathBFS = top->link;
		free(temp);
	}
}

bool ofApp::DFS() {
	room curr, next; // curr: ���� ��, next: ���� ��

	// visited �ʱ�ȭ
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			visited[i][j] = 0;
	
	top = NULL; // ���� �ʱ�ȭ
	topPathDFS = NULL; // ��� ���� �ʱ�ȭ
	adds(&top, 1, 1); // �Ա� push

	while (top) {
		curr = deletes(&top); // pop

		if (visited[curr.row][curr.col]) continue; // ���� ���� �湮������ true
		visited[curr.row][curr.col] = 1; // ���� �� �湮
		adds(&topPathDFS, curr.row, curr.col); // ��� push

		if (curr.row == HEIGHT - 2 && curr.col == WIDTH - 2) break; // ���� �� == �ⱸ

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
	curr.row = HEIGHT - 2;
	curr.col = WIDTH - 2;
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

	// visited �ʱ�ȭ
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			visited[i][j] = 0;

	// ť �ʱ�ȭ
	front = NULL;
	rear = NULL;
	topPathBFS = NULL; // ��� ���� �ʱ�ȭ

	addq(&front, &rear, 1, 1); // �Ա� enqueue

	while (front) {
		curr = deleteq(&front); // dequeue

		if (visited[curr.row][curr.col]) continue; // ���� ���� �湮������ true
		visited[curr.row][curr.col] = 1; // ���� �� �湮
		adds(&topPathBFS, curr.row, curr.col); // ��� push

		if (curr.row == HEIGHT - 2 && curr.col == WIDTH - 2) break; // ���� �� == �ⱸ

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
	curr.row = HEIGHT - 2;
	curr.col = WIDTH - 2;
	prev = pathBFS[curr.row][curr.col];

	// ���� ��� ���� �� ����
	ofSetColor(255, 0, 0);
	while (prev.row != -1 && prev.col != -1) {
		ofDrawLine(prev.col * LENGTH, prev.row * LENGTH, curr.col * LENGTH, curr.row * LENGTH);
		curr = prev;
		prev = pathBFS[prev.row][prev.col];
	}
}
