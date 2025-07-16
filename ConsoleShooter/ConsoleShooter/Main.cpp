#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <conio.h>
#include <Windows.h>

const int HEIGHT = 15, WIDTH = 15; // Размер карты
int gameScore = 0; // Игровой счет
int gameCounter = 10000; // Счетчик времени ()
int scoreTime = gameCounter;
const char mapSymbol = '.';

// Структура позиций
struct Position {
	int x;
	int y;
	char symbol;
};

Position player{ WIDTH / 2, HEIGHT / 2, 'A' }; // Структура координат игрока
Position enemy{ -1, -1, '@' }; // Cтрукитура координат противника

enum playerDirection { UP, DOWN, LEFT, RIGHT }; // Направления игрока

char map[HEIGHT][WIDTH]; // Матрица игрового поля

// Cкрытие курсора
void hideConsoleCursor() {
	// Проверка на неизвестную позицию курсора
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	CONSOLE_CURSOR_INFO cursorInfo;
	// Текущие параметры курсора
	if (GetConsoleCursorInfo(hStdOut, &cursorInfo)) {
		cursorInfo.bVisible = FALSE;  // Скрытие курсора
		SetConsoleCursorInfo(hStdOut, &cursorInfo);
	}
}

// Создание символа на карте
void mapCreate(const int x, const int y, const char symbol) {
	map[y][x] = symbol;
}

// Создание символа игрока на карте (сахар)
void playerCreate() {
	mapCreate(player.x, player.y, player.symbol);
}

// Инициализация игры
void gameInitializate() {
	for (int i = 0; i < HEIGHT; i++) { // Высота
		for (int j = 0; j < WIDTH; j++) { // Ширина
			map[i][j] = mapSymbol;
		}
	}
	playerCreate(); // Инициализация игрока на начальных координатах
}

// Отображение игры
void gameDisplay() {
	// Время и очки
	std::cout << "Score: " << gameScore << std::endl;
	std::cout << "Time: " << gameCounter / 1000 << std::endl;

	// Вывод игрового поля
	for (int i = 0; i < HEIGHT; i++) { // Высота
		for (int j = 0; j < WIDTH; j++) { // Ширина
			std::cout << map[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

// Генерация противников
void enemyGenerate() {

	// Cброс координат прошлого противника
	enemy.x = -1;
	enemy.y = -1;

	// Уменьшение общего таймера
	static int count = 0; // Для уменьшения времени каждые 3 хода
	count++;
	if (count == 2) {
		if (scoreTime >= 3000) scoreTime -= 1000;
		count = 0; // Сброс счетчика
	}

	gameCounter = scoreTime; // Cброс игрового таймера

	// Генерация координат
	bool exist = true;
	while (exist) {
		enemy.x = rand() % WIDTH;
		enemy.y = rand() % HEIGHT;

		// Проверка на игрока в точке генерации
		if (enemy.x == player.x && enemy.y == player.y) continue;

		exist = false; // Выход из цикла
	}
	// Создание противника на карте
	mapCreate(enemy.x, enemy.y, enemy.symbol);
}

// Передвижение игрока
bool playerMove(playerDirection direction) {

	system("cls"); // Очистка консоли

	// Проверка направления
	switch (direction) {
	case UP:
		// Выход за границы поля проверка
		if (player.y > 0) {

			// Проверка на противника
			if (map[player.y - 1][player.x] == enemy.symbol) {
				return false; // Смерть
				break;
			}

			player.symbol = 'A';
			mapCreate(player.x, player.y, mapSymbol); // Очистка текущей позиции игрока
			player.y--;
			playerCreate(); // Создание игрока на новой позиции
			break;
		}
		else break;
	case DOWN:
		// Выход за границы поля проверка
		if (player.y < HEIGHT - 1) {

			// Проверка на противника
			if (map[player.y + 1][player.x] == enemy.symbol) {
				return false; // Смерть
				break;
			}

			player.symbol = 'V';
			mapCreate(player.x, player.y, mapSymbol); // Очистка текущей позиции игрока
			player.y++;
			playerCreate(); // Создание игрока на новой позиции
			break;
		}
		else break;
	case LEFT:
		// Выход за границы поля проверка
		if (player.x > 0) {

			// Проверка на противника
			if (map[player.y][player.x - 1] == enemy.symbol) {
				return false; // Смерть
				break;
			}

			player.symbol = '<';
			mapCreate(player.x, player.y, mapSymbol); // Очистка текущей позиции игрока
			player.x--;
			playerCreate(); // Создание игрока на новой позиции
			break;
		}
		else break;
	case RIGHT:
		// Выход за границы поля проверка
		if (player.x < WIDTH - 1) {

			// Проверка на противника
			if (map[player.y][player.x + 1] == enemy.symbol) {
				return false; // Смерть
				break;
			}

			player.symbol = '>';
			mapCreate(player.x, player.y, mapSymbol); // Очистка текущей позиции игрока
			player.x++;
			playerCreate(); // Создание игрока на новой позиции
			break;
		}
		else break;
	}

	gameDisplay(); // Обновление игрового поля
	return true;
}

// Стрельба игрока
void playerShoot(playerDirection direction) {

	system("cls"); // Очистка консоли

	Position bullet; // Создание объекта пули

	// Проверка на направление
	switch (direction) {
	case UP:
		// Проверка на границу карты
		if (player.y > 0) {
			bullet.symbol = '|';
			// Создание начальной позиции пули
			bullet.y = player.y - 1;
			bullet.x = player.x;

			// Движение пули по карте
			while (bullet.y >= 0) {
				mapCreate(bullet.x, bullet.y, bullet.symbol);
				gameDisplay();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				mapCreate(bullet.x, bullet.y, mapSymbol);

				// Проверка на на противника
				if (bullet.x == enemy.x && bullet.y == enemy.y) {

					gameScore++; // Увеличение счетчика очков
					system("cls");
					enemyGenerate(); // Генерация нового противника
					break;
				}

				bullet.y--; // Cмещение пули
				system("cls");
			}

			break;
		}
		else break;

	case DOWN:
		// Проверка на границу карты
		if (player.y < HEIGHT) {
			bullet.symbol = '|';
			// Создание начальной позиции пули
			bullet.y = player.y + 1;
			bullet.x = player.x;

			// Движение пули по карте
			while (bullet.y < HEIGHT) {
				mapCreate(bullet.x, bullet.y, bullet.symbol);
				gameDisplay();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				mapCreate(bullet.x, bullet.y, mapSymbol);

				// Проверка на на противника
				if (bullet.x == enemy.x && bullet.y == enemy.y) {

					gameScore++; // Увеличение счетчика очков
					system("cls");
					enemyGenerate(); // Генерация нового противника
					break;
				}

				bullet.y++; // Cмещение пули
				system("cls");
			}

			break;
		}
		else break;
	case LEFT:
		// Проверка на границу карты
		if (player.x > 0) {
			bullet.symbol = '-';
			// Создание начальной позиции пули
			bullet.y = player.y;
			bullet.x = player.x - 1;

			// Движение пули по карте
			while (bullet.x >= 0) {
				mapCreate(bullet.x, bullet.y, bullet.symbol);
				gameDisplay();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				mapCreate(bullet.x, bullet.y, mapSymbol);

				// Проверка на на противника
				if (bullet.x == enemy.x && bullet.y == enemy.y) {

					gameScore++; // Увеличение счетчика очков
					system("cls");
					enemyGenerate(); // Генерация нового противника
					break;
				}

				bullet.x--; // Cмещение пули
				system("cls");
			}

			break;
		}
		else break;
	case RIGHT:
		// Проверка на границу карты
		if (player.x < WIDTH) {
			bullet.symbol = '-';
			// Создание начальной позиции пули
			bullet.y = player.y;
			bullet.x = player.x + 1;

			// Движение пули по карте
			while (bullet.x < WIDTH) {
				mapCreate(bullet.x, bullet.y, bullet.symbol);
				gameDisplay();
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				mapCreate(bullet.x, bullet.y, mapSymbol);

				// Проверка на на противника
				if (bullet.x == enemy.x && bullet.y == enemy.y) {

					gameScore++; // Увеличение счетчика очков
					system("cls");
					enemyGenerate(); // Генерация нового противника
					break;
				}

				bullet.x++; // Смещение пули
				system("cls");
			}

			break;
		}
		else break;
	}
	gameDisplay(); // Отображение игрового поля
}

int main() {
	srand(time(NULL)); // Отключение привязки ко времени при генерации рандомных чисел

	hideConsoleCursor();

	gameInitializate(); // Инициализация игры

	playerDirection direction = UP; // Направление игрока
	int gameSpeed = 100; // Cкорость игры в миллисекундах
	bool playing = true;
	bool playerAlive = true;

	enemyGenerate(); // Генерация первого противника
	gameDisplay(); // Отображение игрового поля

	int iteration = 1; // Итерации игрового цикла (для подсчета секундного времени)

	while (playing && playerAlive) {

		// Обновление счетчика раз в секунду
		if (iteration >= 10) {
			system("cls");
			gameDisplay();
		}

		// Считывание зажатие клавиш
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
		// Выход из игры
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			playing = false;

		// Смерть по истечению таймера
		if (gameCounter <= 0) playerAlive = false;

		std::this_thread::sleep_for(std::chrono::milliseconds(gameSpeed));
		gameCounter -= gameSpeed;

		iteration++;
	}


	// Конечный вывод
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