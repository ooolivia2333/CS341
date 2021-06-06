#include <iostream>
#include <list>
#include <queue>
#include <climits>
#include <cmath>
using namespace std;

struct Position {
	pair<int, int> A;
	pair<int, int> B;

	Position(int ax, int ay, int bx, int by) {
		A.first = ax;
		A.second = ay;
		B.first = bx;
		B.second = by;
	}
};

Position getPos(int pos, int n) {
	int ax = pos / (int)(pow(n, 3));
	pos -= ax * (int)pow(n, 3);
	int ay = pos / (int)(pow(n, 2));
	pos -= ay * (int)pow(n, 2);
	int bx = pos / n;
	pos -= bx * n;
	return Position(ax, ay, bx, pos);
}

void initializeAdjacency(char** matrix, int** adjancency, int possiblePos, int n) {
	for (int i = 0; i < possiblePos * possiblePos; i++) {
		adjancency[i] = new int[8];
		for (int j = 0; j < 8; j++) {
			adjancency[i][j] = INT_MAX;
		}
	}

	for (int a = 0; a < possiblePos; a++) {
		for (int b = 0; b < possiblePos; b++) {
			if (a == b or matrix[a / n][a % n] == '#' or matrix[b / n][b % n] == '#'
				or matrix[a / n][a % n] == 'T' or matrix[b / n][b % n] == 'T') {
				continue;
			}
			Position pos = Position(a / n, a % n, b / n, b % n);
			int curr = pos.A.first * (int)pow(n, 3) + pos.A.second * (int)pow(n, 2) + pos.B.first * n + pos.B.second;

			// a right
			int y = pos.A.second;
			int x = pos.A.first;
			for (; y < n; y++) {
				if (x * n + y == b or matrix[x][y] == '#') {
					y--;
					break;
				}
			}
			if (y == n) y = n - 1;
			if (y != pos.A.second) {
				adjancency[curr][0] = pos.A.first * (int)pow(n, 3) + y * (int)pow(n, 2) + pos.B.first * n + pos.B.second;
			}

			// a left
			y = pos.A.second;
			for (; y >= 0; y--) {
				if (x * n + y == b or matrix[x][y] == '#') {
					y++;
					break;
				}
			}
			if (y == -1) y = 0;
			if (y != pos.A.second) {
				adjancency[curr][1] = pos.A.first * (int)pow(n, 3) + y * (int)pow(n, 2) + pos.B.first * n + pos.B.second;
			}

			// a up
			x = pos.A.first;
			y = pos.A.second;
			for (; x >= 0; x--) {
				if (x * n + y == b or matrix[x][y] == '#') {
					x++;
					break;
				}
			}
			if (x == -1) x = 0;
			if (x != pos.A.first) {
				adjancency[curr][2] = x * (int)pow(n, 3) + pos.A.second * (int)pow(n, 2) + pos.B.first * n + pos.B.second;
			}

			// a down
			x = pos.A.first;
			for (; x < n; x++) {
				if (x * n + y == b or matrix[x][y] == '#') {
					x--;
					break;
				}
			}
			if (x == n) x--;
			if (x != pos.A.first) {
				adjancency[curr][3] = x * (int)pow(n, 3) + pos.A.second * (int)pow(n, 2) + pos.B.first * n + pos.B.second;
			}

			// b right
			x = pos.B.first;
			y = pos.B.second;
			for (; y < n; y++) {
				if (x * n + y == a or matrix[x][y] == '#') {
					y--;
					break;
				}
			}
			if (y == n) y--;
			if (y != pos.B.second) {
				adjancency[curr][4] = pos.A.first * (int)pow(n, 3) + pos.A.second * (int)pow(n, 2) + pos.B.first * n + y;
			}

			// b left
			y = pos.B.second;
			for (; y >= 0; y--) {
				if (x * n + y == a or matrix[x][y] == '#') {
					y++;
					break;
				}
			}
			if (y == -1) y++;
			if (y != pos.B.second) {
				adjancency[curr][5] = pos.A.first * (int)pow(n, 3) + pos.A.second * (int)pow(n, 2) + pos.B.first * n + y;
			}

			// b up
			x = pos.B.first;
			y = pos.B.second;
			for (; x >= 0; x--) {
				if (x * n + y == a or matrix[x][y] == '#') {
					x++;
					break;
				}
			}
			if (x == -1) x++;
			if (x != pos.B.first) {
				adjancency[curr][6] = pos.A.first * (int)pow(n, 3) + pos.A.second * (int)pow(n, 2) + x * n + pos.B.second;
			}

			// b down
			x = pos.B.first;
			for (; x < n; x++) {
				if (x * n + y == a or matrix[x][y] == '#') {
					x--;
					break;
				}
			}
			if (x == n) x--;
			if (x != pos.B.first) {
				adjancency[curr][7] = pos.A.first * (int)pow(n, 3) + pos.A.second * (int)pow(n, 2) + x * n + pos.B.second;
			}

			//cout << "a: " << a / n << " " << a % n << " b: " << b / n << " " << b % n << ": " << endl;
			//for (int i = 0; i < 8; i++) {
			//	if (adjancency[curr][i] != INT_MAX) {
			//		int pos = adjancency[curr][i];
			//		int ax = pos / (int)(pow(n, 3));
			//		pos -= ax * pow(n, 3);
			//		int ay = pos / (int)(pow(n, 2));
			//		pos -= ay * pow(n, 2);
			//		int bx = pos / n;
			//		pos -= bx * n;
			//		cout << ax << " " << ay << " " << bx << " " << pos % n << endl;
			//	}
			//}
			//cout << endl;
		}
	}
}

list<int> BFS(int** adjancency, int n, int start, int destx, int desty) {
	int combinations = (int)pow(n, 4);
	bool* visited = new bool[combinations];
	int* parent = new int[combinations]();
	int* distance = new int[combinations];
	for (int i = 0; i < combinations; i++) {
		visited[i] = false;
		distance[i] = INT_MAX;
	}

	queue<int> q;
	while (!q.empty()) {
		q.pop();
	}
	q.push(start);
	visited[start] = true;
	distance[start] = 0;

	while (!q.empty()) {
		int curr = q.front();
		q.pop();

		for (int i = 0; i < 8; i++) {
			int child = adjancency[curr][i];
			if (child != INT_MAX and !visited[child]) {
				q.push(child);
				visited[child] = true;
				parent[child] = curr;
				distance[child] = distance[curr] + 1;
				/*Position cur = getPos(curr, n);
				cout << cur.A.first << " " << cur.A.second << " " << cur.B.first << " " << cur.B.second << ": " << distance[curr] << endl;
				Position c = getPos(child, n);
				cout << c.A.first << " " << c.A.second << " " << c.B.first << " " << c.B.second << ": " << distance[child] << endl;
				cout << endl;*/
			}
		}
	}

	list<int> path;
	int min = -1;
	int minDistance = INT_MAX;
	for (int i = 0; i < combinations; i++) {
		if (distance[i] != INT_MAX and distance[i] < minDistance) {
			Position end = getPos(i, n);
			if ((end.A.first == destx and end.A.second == desty) or (end.B.first == destx and end.B.second == desty)) {
				min = i;
				minDistance = distance[i];
			}
		}
	}

	Position end = getPos(min, n);/*
	cout << destx << " " << desty << endl;
	cout << end.A.first << " " << end.A.second << " " << end.B.first << " " << end.B.second << endl;
	cout << start << endl;*/
	if (min != -1) {
		int curr = min;
		while (curr != start) {
			/*cout << curr << " ";*/
			path.push_front(curr);
			curr = parent[curr];
		}
	}
	return path;
}

int main() {
	int n = 0;
	cin >> n;
	char** matrix = new char*[n];
	int A = 0;
	int B = 0;
	int tx = 0;
	int ty = 0;

	for (int i = 0; i < n; i++) {
		matrix[i] = new char[n];
		for (int j = 0; j < n; j++) {
			char temp;
			cin >> temp;
			matrix[i][j] = temp;
			if (temp == 'A') {
				A = i * n + j;
				matrix[i][j] = '.';
			}
			else if (temp == 'B') {
				B = i * n + j;
				matrix[i][j] = '.';
			}
			else if (temp == 'T') {
				tx = i;
				ty = j;
			}
		}
	}

	int possiblePos = n * n;

	int** adjacen = new int*[possiblePos * possiblePos];

	initializeAdjacency(matrix, adjacen, possiblePos, n);

	int start = A * n * n + B;

	list<int> path = BFS(adjacen, n, start, tx, ty);

	int step = path.size();
	if (step == 0) {
		cout << "-1" << endl;
	}
	else {
		cout << step << endl;
		int ax = A / n;
		int ay = A % n;
		int bx = B / n;
		int by = B % n;
		for (int i : path) {
			Position pos = getPos(i, n);
			if (pos.A.first != ax) {
				if (pos.A.first < ax) {
					cout << "A up" << endl;
				}
				else {
					cout << "A down" << endl;
				}
				ax = pos.A.first;
			}
			else if (pos.A.second != ay) {
				if (pos.A.second < ay) {
					cout << "A left" << endl;
				}
				else {
					cout << "A right" << endl;
				}
				ay = pos.A.second;
			}
			else if (pos.B.first != bx) {
				if (pos.B.first < bx) {
					cout << "B up" << endl;
				}
				else {
					cout << "B down" << endl;
				}
				bx = pos.B.first;
			}
			else if (pos.B.second != by) {
				if (pos.B.second < by) {
					cout << "B left" << endl;
				}
				else {
					cout << "B right" << endl;
				}
				by = pos.B.second;
			}
		}
	}

	for (int i = 0; i < possiblePos; i++) {
		delete[] adjacen[i];
	}
	delete[] adjacen;
	
}