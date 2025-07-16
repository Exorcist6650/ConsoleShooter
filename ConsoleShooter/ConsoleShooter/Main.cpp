#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <conio.h>
#include <Windows.h>

const int HEIGHT = 15, WIDTH = 15; // ������ �����
int gameScore = 0; // ������� ����
int gameCounter = 10000; // ������� ������� ()
int scoreTime = gameCounter;
const char mapSymbol = '.';

// ��������� �������
struct Position {
	int x;
	int y;
	char symbol;
};

Position player{ WIDTH / 2, HEIGHT / 2, 'A' }; // ��������� ��������� ������
Position enemy{ -1, -1, '@' }; // C��������� ��������� ����������

enum playerDirection { UP, DOWN, LEFT, RIGHT }; // ����������� ������

char map[HEIGHT][WIDTH]; // ������� �������� ����

// C������ �������
void hideConsoleCursor() {
	// �������� �� ����������� ������� �������
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	CONSOLE_CURSOR_INFO cursorInfo;
	// ������� ��������� �������
	if (GetConsoleCursorInfo(hStdOut, &cursorInfo)) {
		cursorInfo.bVisible = FALSE;  // ������� �������
		SetConsoleCursorInfo(hStdOut, &cursorInfo);
	}
}

// �������� ������� �� �����
void mapCreate(const int x, const int y, const char symbol) {
	map[y][x] = symbol;
}

// �������� ������� ������ �� ����� (�����)
void playerCreate() {
	mapCreate(player.x, player.y, player.symbol);
}

// ������������� ����
void gameInitializate() {
	for (int i = 0; i < HEIGHT; i++) { // ������
		for (int j = 0; j < WIDTH; j++) { // ������
			map[i][j] = mapSymbol;
		}
	}
	playerCreate(); // ������������� ������ �� ��������� �����������
}

// ����������� ����
void gameDisplay() {
	// ����� � ����
	std::cout << "Score: " << gameScore << std::endl;
	std::cout << "Time: " << gameCounter / 1000 << std::endl;

	// ����� �������� ����
	for (int i = 0; i < HEIGHT; i++) { // ������
		for (int j = 0; j < WIDTH; j++) { // ������
			std::cout << map[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

// ��������� �����������
void enemyGenerate() {

	// C���� ��������� �������� ����������
	enemy.x = -1;
	enemy.y = -1;

	// ���������� ������ �������
	static int count = 0; // ��� ���������� ������� ������ 3 ����
	count++;
	if (count == 2) {
		if (scoreTime >= 3000) scoreTime -= 1000;
		count = 0; // ����� ��������
	}

	gameCounter = scoreTime; // C���� �������� �������

	// ��������� ���������
	bool exist = true;
	while (exist) {
		enemy.x = rand() % WIDTH;
		enemy.y = rand() % HEIGHT;

		// �������� �� ������ � ����� ���������
		if (enemy.x == player.x && enemy.y == player.y) continue;

		exist = false; // ����� �� �����
	}
	// �������� ���������� �� �����
	mapCreate(enemy.x, enemy.y, enemy.symbol);
}

// ������������ ������
bool playerMove(playerDirection direction) {

	system("cls"); // ������� �������

	// �������� �����������
	switch (direction) {
	case UP:
		// ����� �� ������� ���� ��������
		if (player.y > 0) {

			// �������� �� ����������
			if (map[player.y - 1][player.x] == enemy.symbol) {
				return false; // ������
				break;
			}

			player.symbol = 'A';
			mapCreate(player.x, player.y, mapSymbol); // ������� ������� ������� ������
			player.y--;
			playerCreate(); // �������� ������ �� ����� �������
			break;
		}
		else break;
	case DOWN:
		// ����� �� ������� ���� ��������
		if (player.y < HEIGHT - 1) {

			// �������� �� ����������
			if (map[player.y + 1][player.x] == enemy.symbol) {
				return false; // ������
				break;
			}

			player.symbol = 'V';
			mapCreate(player.x, player.y, mapSymbol); // ������� ������� ������� ������
			player.y++;
			playerCreate(); // �������� ������ �� ����� �������
			break;
		}
		else break;
	case LEFT:
		// ����� �� ������� ���� ��������
		if (player.x > 0) {

			// �������� �� ����������
			if (map[player.y][player.x - 1] == enemy.symbol) {
				return false; // ������
				break;
			}

			player.symbol = '<';
			mapCreate(player.x, player.y, mapSymbol); // ������� ������� ������� ������
			player.x--;
			playerCreate(); // �������� ������ �� ����� �������
			break;
		}
		else break;
	case RIGHT:
		// ����� �� ������� ���� ��������
		if (player.x < WIDTH - 1) {

			// �������� �� ����������
			if (map[player.y][player.x + 1] == enemy.symbol) {
				return false; // ������
				break;
			}

			player.symbol = '>';
			mapCreate(player.x, player.y, mapSymbol); // ������� ������� ������� ������
			player.x++;
			playerCreate(); // �������� ������ �� ����� �������
			break;
		}
		else break;
	}

	gameDisplay(); // ���������� �������� ����
	return true;
}

// �������� ������
void playerShoot(playerDirection direction) {

	system("cls"); // ������� �������

	Position bullet; // �������� ������� ����

	// �������� �� �����������
	switch (direction) {
	case UP:
		// �������� �� ������� �����
		if (player.y > 0) {
			bullet.symbol = '|';
			// �������� ��������� ������� ����
			bullet.y = player.y - 1;
			bullet.x = player.x;

			// �������� ���� �� �����
			while (bullet.y >= 0) {
				mapCreate(bullet.x, bullet.y, bullet.symbol);
				gameDisplay();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				mapCreate(bullet.x, bullet.y, mapSymbol);

				// �������� �� �� ����������
				if (bullet.x == enemy.x && bullet.y == enemy.y) {

					gameScore++; // ���������� �������� �����
					system("cls");
					enemyGenerate(); // ��������� ������ ����������
					break;
				}

				bullet.y--; // C������� ����
				system("cls");
			}

			break;
		}
		else break;

	case DOWN:
		// �������� �� ������� �����
		if (player.y < HEIGHT) {
			bullet.symbol = '|';
			// �������� ��������� ������� ����
			bullet.y = player.y + 1;
			bullet.x = player.x;

			// �������� ���� �� �����
			while (bullet.y < HEIGHT) {
				mapCreate(bullet.x, bullet.y, bullet.symbol);
				gameDisplay();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				mapCreate(bullet.x, bullet.y, mapSymbol);

				// �������� �� �� ����������
				if (bullet.x == enemy.x && bullet.y == enemy.y) {

					gameScore++; // ���������� �������� �����
					system("cls");
					enemyGenerate(); // ��������� ������ ����������
					break;
				}

				bullet.y++; // C������� ����
				system("cls");
			}

			break;
		}
		else break;
	case LEFT:
		// �������� �� ������� �����
		if (player.x > 0) {
			bullet.symbol = '-';
			// �������� ��������� ������� ����
			bullet.y = player.y;
			bullet.x = player.x - 1;

			// �������� ���� �� �����
			while (bullet.x >= 0) {
				mapCreate(bullet.x, bullet.y, bullet.symbol);
				gameDisplay();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				mapCreate(bullet.x, bullet.y, mapSymbol);

				// �������� �� �� ����������
				if (bullet.x == enemy.x && bullet.y == enemy.y) {

					gameScore++; // ���������� �������� �����
					system("cls");
					enemyGenerate(); // ��������� ������ ����������
					break;
				}

				bullet.x--; // C������� ����
				system("cls");
			}

			break;
		}
		else break;
	case RIGHT:
		// �������� �� ������� �����
		if (player.x < WIDTH) {
			bullet.symbol = '-';
			// �������� ��������� ������� ����
			bullet.y = player.y;
			bullet.x = player.x + 1;

			// �������� ���� �� �����
			while (bullet.x < WIDTH) {
				mapCreate(bullet.x, bullet.y, bullet.symbol);
				gameDisplay();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				mapCreate(bullet.x, bullet.y, mapSymbol);

				// �������� �� �� ����������
				if (bullet.x == enemy.x && bullet.y == enemy.y) {

					gameScore++; // ���������� �������� �����
					system("cls");
					enemyGenerate(); // ��������� ������ ����������
					break;
				}

				bullet.x++; // �������� ����
				system("cls");
			}

			break;
		}
		else break;
	}
	gameDisplay(); // ����������� �������� ����
}

int main() {
	srand(time(NULL)); // ���������� �������� �� ������� ��� ��������� ��������� �����

	hideConsoleCursor();

	gameInitializate(); // ������������� ����

	playerDirection direction = UP; // ����������� ������
	int gameSpeed = 100; // C������� ���� � �������������
	bool playing = true;
	bool playerAlive = true;

	enemyGenerate(); // ��������� ������� ����������
	gameDisplay(); // ����������� �������� ����

	int iteration = 1; // �������� �������� ����� (��� �������� ���������� �������)

	while (playing && playerAlive) {

		// ���������� �������� ��� � �������
		if (iteration >= 10) {
			system("cls");
			gameDisplay();
		}

		// ���������� ������� ������
		if (GetAsyncKeyState('W') & 0x8000) {
			if (playerMove(UP)) direction = UP;
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				playerAlive = false;
			}
		}
		if (GetAsyncKeyState('S') & 0x8000) {
			if (playerMove(DOWN)) direction = DOWN;
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				playerAlive = false;
			}
		}
		if (GetAsyncKeyState('A') & 0x8000) {
			if (playerMove(LEFT)) direction = LEFT;
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				playerAlive = false;
			}
		}
		if (GetAsyncKeyState('D') & 0x8000) {
			if (playerMove(RIGHT)) direction = RIGHT;
			else {
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				playerAlive = false;
			}
		}
		if (GetAsyncKeyState('E') & 0x8000)
			playerShoot(direction);
		// ����� �� ����
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			playing = false;

		// ������ �� ��������� �������
		if (gameCounter <= 0) playerAlive = false;

		std::this_thread::sleep_for(std::chrono::milliseconds(gameSpeed));
		gameCounter -= gameSpeed;

		iteration++;
	}


	// �������� �����
	if (!playerAlive) {
		std::cout << "You are die" << std::endl;
		if (gameCounter <= 0) {
			std::cout << "The time is out" << std::endl;
		}
		std::cout << "Score: " << gameScore << std::endl;
	}
	else {
		std::cout << "Game is over\nScore: " << gameScore << std::endl;
	}

	system("pause");
	return 0;
}