#pragma once

#include "ofMain.h"

class waterObj {
	public:
		double* x; // ��� x ��ǥ �迭
		double* y; // ��� y ��ǥ �迭
		int len; // ��� ����
		waterObj(int size); // ������
		~waterObj(); // �Ҹ���
		void get_path(int *dot, int **lines, int num_of_line, int height); // ��� ã��
};
