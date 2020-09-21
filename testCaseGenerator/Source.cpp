#include <iostream>

using namespace std;

int counter = 0;

int square[9];
void place(int square_num) {
	if (square_num == 9) {
		for (int i = 0; i < 9; i++) {
			char ch = 0;
			switch (i & 3) {
			case 0:
				ch = '.';
				break;
			case 1:
				ch = 'X';
				break;
			case 2:
				ch = 'O';
				break;
			}
		}
	}

	for (int i = 0; i <= 2; i++) {
		square[square_num] = i;
		place(square_num + 1);
	}
}

int main() {
	place(0);
	return 0;
}
