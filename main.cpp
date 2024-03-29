#include "console.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define BOARD_SIZE 20
#define MOVE_DELAY 60
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

using namespace std;
bool gameOver;
const int width = BOARD_SIZE;
const int height = BOARD_SIZE;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
void drawMap();
void Setup();
void Draw();
void handleInput();
void Logic();
void game();


void drawMap() {

  console::draw(0,0,WALL_LEFT_TOP_STRING);
  console::draw(0,19,WALL_LEFT_BOTTOM_STRING);
  console::draw(19,0,WALL_RIGHT_TOP_STRING);
  console::draw(19,19,WALL_RIGHT_BOTTOM_STRING);

  for (int i = 1; i < width-1; i++)
    console::draw(i,0,WALL_HORIZONTAL_STRING);
  for (int i = 1; i < width-1; i++)
    console::draw(i,19,WALL_HORIZONTAL_STRING);
  for (int i = 1; i < width-1; i++)
    console::draw(0,i,WALL_VERTICAL_STRING);
  for (int i = 1; i < width-1; i++)
    console::draw(19,i,WALL_VERTICAL_STRING);

}

void Setup() {
  srand(time(nullptr));
	gameOver = false;;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width +1;
	fruitY = rand() % height;
	score = 0;
}

void Draw() {

	for (int i = 1; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (i == y && j == x)
				console::draw(j,i,SNAKE_STRING); // 뱀의 머리
			else if (i == fruitY && j == fruitX)
				console::draw(j,i,APPLE_STRING); // 사과
			else {
				bool print = false;
				for (int k = 0; k < nTail; k++) {
					if (tailX[k] == j && tailY[k] == i) {
						console::draw(j,i,SNAKE_BODY_STRING); // 뱀의 몸통
						print = true;
					}
				}
				if (!print)
					cout << " ";
			}
		}
	}

	cout << "\n";
	cout << "\n";
	cout << "Score : " << score;
}



void handleInput() {
  if (console::key(console::K_LEFT)) {
    x--;
  }
  if (console::key(console::K_RIGHT)) {
    x++;
  }
  if (console::key(console::K_UP)) {
    y--;
  }
  if (console::key(console::K_DOWN)) {
    y++;
  }
  if (console::key(console::K_ESC)) {
    gameOver = true;
  }  
  if (console::key(console::K_ENTER)) {
    game();
  }   
}




void Logic() {
	//꼬리
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++) {
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

  //벽을 만나면 게임 종료
	if (x > width || x < 0 || y > height || y < 0)
		gameOver = true;
  //뱀의 꼬리에 머리가 닿으면 게임 종료
	for (int i = 0; i < nTail; i++) {
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;
  }
    //과일을 먹으면 점수를 올리고, 꼬리가 길어지며, 과일을 다시 배치한다.
	if (x == fruitX && y == fruitY) {
    srand(time(nullptr));
		score += 10;
		fruitX = rand() % width + 1;
		fruitY = rand() % height;
		nTail++;
	}
  
}




void game() {

  // 콘솔 라이브러리를 초기화한다.
  console::init();
  Setup();

  while (!gameOver) {
    console::clear();
    drawMap();
    Draw();
    handleInput();
    Logic();
    console::wait();
  }
}



int main() {
  game();

  return 0;
}