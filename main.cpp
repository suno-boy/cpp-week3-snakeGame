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
int start;
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
	gameOver = false;
	x = width / 2; // 뱀의 시작 위치
	y = height / 2;
	fruitX = (rand()%(width-2))+1;
	fruitY = (rand()%(height-2))+1;
	score = 0;
	nTail = 0;
	for(int i=0;i<100;i++)
	{
		tailX[i] = -1;
		tailY[i] = -1;
	}
}

void Draw() {

	for (int i = 1; i < height-1; i++) 
	{
		for (int j = 1; j < width-1; j++) 
		{
			if (i == y && j == x)
				console::draw(j,i,SNAKE_STRING); // 뱀의 머리
			else if (i == fruitY && j == fruitX)
				console::draw(j,i,APPLE_STRING); // 사과
			else 
			{
				for (int k = 0; k < nTail; k++) 
				{
					if (tailX[k] == j && tailY[k] == i)
					{//tailX[k], tailY[k]의 인덱스에는 뱀의 몸통 좌표가 차례대로 대입되어 있어야함!
						console::draw(j,i,SNAKE_BODY_STRING); // 뱀의 몸통
					}
				}
			}
		}
	}

	console::draw(5,20,"Score : ");
	std::string score_str = std::to_string(score);
	console::draw(13,20,score_str);
	
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
    exit(0);
  }  
  if (console::key(console::K_ENTER)) {
    game();
  }   
}




void Logic() {
  //과일을 먹으면 점수를 올리고, 꼬리가 길어지며, 과일을 다시 배치한다.
	if (x == fruitX && y == fruitY) {
    srand(time(nullptr));
		score += 10;
		fruitX = (rand()%(width-2))+1;
		fruitY = (rand()%(height-2))+1;
		nTail++;
	}
	//머리자리에 몸통 넣어주는 로직
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
	if (x >= width-1 || x <= 0 || y >= height-1 || y <= 0)
		gameOver = true;
  //뱀의 꼬리에 머리가 닿으면 게임 종료
	for (int i = 0; i < nTail; i++) 
	{
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;
  }
  
}




void game() {

  console::init();// 콘솔 라이브러리를 초기화한다.
  
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