#include <vector>
#include <iostream>
#include <string>
#include <set>

using namespace std;

enum class Direction {
	DOWN,
	RIGHT,
	DIAGONAL
};

struct coord {
	unsigned int x;
	unsigned int y;
	bool operator <(const coord& rhs) const {
		if (x != rhs.x) return x < rhs.x;
		return y < rhs.y;
	}
	bool operator ==(const coord& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
};

struct winning_play {
	vector<coord> coords;
	Direction dir;
};

unsigned int n, m;
int boardSum;
int emptySquares = 0;
vector<vector<int>> board;
vector<winning_play> winning_plays;

void print_values() {
    cout << "Board Sum: " << boardSum << endl;
}

void read_board() {
	cin >> n >> m;
	//Resize board
	board.resize(n);
	for (unsigned int i = 0; i < n; i++) {
		board[i].resize(n);
	}
	
	string buf;
	getline(cin, buf);
	for(unsigned i = 0; i < n; i++) {
		getline(cin, buf);
		for(unsigned int j = 0; j < buf.size(); j++) {
			switch(buf[j]) {
			case '.':
				board[i][j] = 0;
				emptySquares++;
				break;
			case 'X':
				board[i][j] = 1;
				boardSum += 1;
				break;
			case 'O':
				board[i][j] = -1;
				boardSum += -1;
				break;
			default:
				break;
			}
		}
	}
}

/*
 * (X) for case handled
 *
 * Error Cases:
 *	(X) Too many x or o -> Board Sum
 *	Won in more then one spot at a time.
 *	(X) Both won at the same time
 *	(X) More then m*2-1 in a row as a winning move
 *	
 */

bool board_in_error_state() {
	// Too many x or o
	if (boardSum > 1 || boardSum < 0) {
		return true;
	}
	
	// More then m*2-1 in a row as a winning move
	for (const auto& v : winning_plays) {
		if (v.coords.size() > m * 2 - 1) return true;
	}


	
	if(!winning_plays.empty()) {
		// Both won at the same time
		const coord winnerCoord = winning_plays[0].coords[0];
		const int winner = board[winnerCoord.x][winnerCoord.y];
		for (const auto& v : winning_plays) {
			const coord c = v.coords[0];
			if (board[c.x][c.y] != winner) return true;
		}

		// X won, but O has as many placed tiles.
		if(winner == 1 && boardSum == 0) {
			return true;
		}

		// O won, but X has placed more.
		if (winner == -1 && boardSum == 1) {
			return true;
		}
	}


	//Check if all wins at the same location overlaps
	set<coord> hitSpots;
	bool spotFound = false;
	coord spot;
	for (const auto& v : winning_plays) {
		bool overlapFound = false;
		for(const auto& c: v.coords) {
			if (hitSpots.find(c) == hitSpots.end()) {
				hitSpots.insert(c);
			} else {
				overlapFound = true;
				if (spotFound) {
					if (c.x != spot.x || c.y != spot.y) return true;
				} else {
					spot = c;
					spotFound = true;
				}
			}
		}
		if (winning_plays.size() > 1 && (!overlapFound && !(winning_plays[0].coords == v.coords))) {
			return true;
		}
	}

	
	
	return false;
}

void inline find_horizontal_wins() {
	vector<coord> current_row;
	int current_place = 0;
	int tmp = 0;
	for(unsigned int i = 0; i < n; i++) {
		for (unsigned int j = 0; j < n; j++) {
			tmp = board[i][j];
			if (tmp != current_place) {
				if (current_row.size() >= m) {
					winning_plays.push_back({ current_row, Direction::RIGHT });
				}
				current_place = tmp;
				current_row.clear();
				if (current_place != 0) {
					current_row.push_back({ i, j });
				}
			} else {
				if (current_place != 0) {
					current_row.push_back({ i, j });
				}
			}
		}
		if (current_row.size() >= m) {
			winning_plays.push_back({ current_row, Direction::RIGHT });
		}
		current_place = 0;
		current_row.clear();
	}
}

void inline find_vertical_wins() {
	vector<coord> current_column;
	int current_place = 0;
	int tmp = 0;
	for (unsigned int j = 0; j < n; j++) {
		for (unsigned int i = 0; i < n; i++) {
			tmp = board[i][j];
			if (tmp != current_place) {
				if (current_column.size() >= m) {
					winning_plays.push_back({ current_column, Direction::DOWN });
				}
				current_place = tmp;
				current_column.clear();
				if (current_place != 0) {
					current_column.push_back({ i, j });
				}
			}
			else {
				if (current_place != 0) {
					current_column.push_back({ i, j });
				}
			}
		}
		if (current_column.size() >= m) {
			winning_plays.push_back({ current_column, Direction::DOWN });
		}
		current_place = 0;
		current_column.clear();
	}
}

void inline doTheDo(int& tmp, unsigned int i, unsigned int j, int& current_place, std::vector<coord>& current_diagonal)
{
	tmp = board[i][j];
	if (tmp != current_place) {
		if (current_diagonal.size() >= m) {
			winning_plays.push_back({ current_diagonal, Direction::DIAGONAL });
		}
		current_place = tmp;
		current_diagonal.clear();
		if (current_place != 0) {
			current_diagonal.push_back({ i, j });
		}
	}
	else {
		if (current_place != 0) {
			current_diagonal.push_back({ i, j });
		}
	}
}

void inline find_diagonal_wins() {
	vector<coord> current_diagonal;
	int current_place = 0;
	int tmp = 0;
	for (unsigned int y = 0, x = 0; y < n; y++) {
		for (unsigned int i = x, j = y; i < n && j < n; j++, i++) {
			doTheDo(tmp, i, j, current_place, current_diagonal);
		}
		if (current_diagonal.size() >= m) {
			winning_plays.push_back({ current_diagonal, Direction::DIAGONAL });
		}
		current_place = 0;
		current_diagonal.clear();
	}
	for (unsigned int y = 0, x = 1; x < n; x++) {
		for (unsigned int i = x, j = y; i < n && j < n; j++, i++) {
			doTheDo(tmp, i, j, current_place, current_diagonal);
		}
		if (current_diagonal.size() >= m) {
			winning_plays.push_back({ current_diagonal, Direction::DIAGONAL });
		}
		current_place = 0;
		current_diagonal.clear();
	}
	for (unsigned int y = 0, x = n-1; y < n; y++) {
		for (unsigned int i = x, j = y; i < n && j < n; j++, i--) {
			doTheDo(tmp, i, j, current_place, current_diagonal);
		}
		if (current_diagonal.size() >= m) {
			winning_plays.push_back({ current_diagonal, Direction::DIAGONAL });
		}
		current_place = 0;
		current_diagonal.clear();
	}
	for (unsigned int y = 0, x = n-2; x < n; x--) {
		for (unsigned int i = x, j = y; i < n && j < n; j++, i--) {
			doTheDo(tmp, i, j, current_place, current_diagonal);
		}
		if (current_diagonal.size() >= m) {
			winning_plays.push_back({ current_diagonal, Direction::DIAGONAL });
		}
		current_place = 0;
		current_diagonal.clear();
	}
}

void find_winning_plays() {
	find_horizontal_wins();
	find_vertical_wins();
	find_diagonal_wins();
}

bool board_has_one_winner() {
	return !winning_plays.empty();
}

bool board_in_draw_state() {
	return emptySquares == 0;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    read_board();
	find_winning_plays();
	if (board_in_error_state()) {
		cout << "ERROR" << endl;
		return 0;
	}
	if (board_has_one_winner()) {
		const coord loc = winning_plays[0].coords[0];
		const char winner = board[loc.x][loc.y] == 1 ? 'X' : 'O';
		cout << winner << " WINS" << endl;
		return 0;
	}
	if(board_in_draw_state()) {
		cout << "DRAW" << endl;
		return 0;
	}
	
	cout << "IN PROGRESS" << endl;

	
    //print_values();
    return 0;
}
