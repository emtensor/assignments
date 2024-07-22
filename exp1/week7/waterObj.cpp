#include "waterObj.h"

waterObj::waterObj(int size) {
	// x, y ���� �Ҵ�
	x = new double[size];
	y = new double[size];
	len = 0; // ��� ���� 0���� �ʱ�ȭ
}

waterObj::~waterObj() {
	// x, y ���� �Ҵ� ����
	delete[] x;
	delete[] y;
}

void waterObj::get_path(int* dot, int** lines, int num_of_line, int height) {
	int i = 0; // x, y �ε���

	// ������ ���� ��ǥ ����
	x[0] = dot[0];
	y[0] = dot[1]; 

	while (y[i] < height) {
		int j = 0; // ���� ������ �Ÿ��� �ּ��� �ε���
		int min = height; // ���� ������ �ּ� �Ÿ�
		double min_intersection = height; // ���� ������ �Ÿ��� �ּ��� ��, ���� x ��ǥ�� ���п� �����Ѵ�.
		double intersection = height; // ���� x ��ǥ�� ���п� �����Ѵ�.

		for (int k = 0; k < num_of_line; k++) {
			// ������ ����
			double slope = double((lines[k][3] - lines[k][1])) / double((lines[k][2] - lines[k][0]));
			// ���� x ��ǥ�� ������ x ��ǥ ������ �ְ�, ���� ������ ���� ������ �� TRUE
			if ((x[i] > lines[k][0]) && (x[i] < lines[k][2]) &&
				(y[i] < slope * (x[i] - lines[k][0]) + lines[k][1])) {
				intersection = slope * (x[i] - lines[k][0]) + lines[k][1];
				// ����� intersection - y[i]�� min���� ������ TRUE
				if (intersection - y[i] < min) {
					// ���� ������ �ּ� �Ÿ��� ���� ���� ����
					min = intersection - y[i];
					min_intersection = intersection;
					j = k;
				}
			}
		}

		// for �ݺ����� if ���ǹ��� ��ġ�� ������ TRUE
		if (min_intersection >= height) {
			// �������� �ϴ��� ������ ��η� �߰�
			x[i + 1] = x[i];
			y[i + 1] = height - 50.0;
			i++;
			break;
		}

		// ������ ���� ��η� �߰�
		x[i + 1] = x[i];
		y[i + 1] = min_intersection;
		i++;

		// ������ ���� ���� ��η� �߰�
		// �������� y ��ǥ�� Ŭ���� �Ʒ��� ��������.
		if (lines[j][1] > lines[j][3]) {
			x[i + 1] = lines[j][0];
			y[i + 1] = lines[j][1];
		}
		else {
			x[i + 1] = lines[j][2];
			y[i + 1] = lines[j][3];
		}
		i++;
	}

	len = i; // ��� ���� ����
}