#include "console.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

#define BOARD_SIZE 20

#define MOVE_DELAY 10

#define LOSE_TEXT "YOU LOSE"
#define WIN_TEXT "YOU WIN"
#define TRY_TEXT "Try again? (Enter)"

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
bool board[BOARD_SIZE-2][BOARD_SIZE-2]; // 뱀의 위치를 나타내는 2차원 배열
int direction; //뱀의 이동 방향 (0: 오른쪽, 1: 위쪽, 2: 왼쪽, 3: 아래쪽)

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
	score = 0;
	nTail = 0;
	direction = 0;
	for(int i=0;i<BOARD_SIZE;i++) 
	{
        for(int j=0;j<BOARD_SIZE;j++) 
				{
            board[i][j] = false; // 보드 초기화
        }
  }
	board[y][x] = true;
	    // 사과 배치
  while (true) 
	{
    int appleX = rand() % (width - 2) + 1; // 사과의 x 좌표
    int appleY = rand() % (height - 2) + 1; // 사과의 y 좌표
    if (!board[appleY][appleX]) 
		{ // 뱀이 없는 위치에 사과를 배치
        fruitX = appleX;
        fruitY = appleY;
        break;
    }
  }	
}

void Draw() {

	for (int i = 1; i < height-1; i++) 
	{
		for (int j = 1; j < width-1; j++) 
		{
			if (i == y && j == x)
			{
				console::draw(j,i,SNAKE_STRING); // 뱀의 머리
				board[j][i] = true;
			}
			else if (i == fruitY && j == fruitX)
				console::draw(j,i,APPLE_STRING); // 사과
			else 
			{
				for (int k = 0; k < nTail; k++) 
				{
					if (tailX[k] == j && tailY[k] == i)
					{
						console::draw(j,i,SNAKE_BODY_STRING); // 뱀의 몸통
						board[j][i] = true;
					}
				}
			}
		}
	}

	std::string score_text = "Score : " + std::to_string(score);
	console::draw((BOARD_SIZE - score_text.length()) / 2, BOARD_SIZE, score_text);
	
}



void handleInput() {
	if(console::key(console::K_LEFT) && nTail==0) {
		direction = 2;
	} 
	if(console::key(console::K_RIGHT) && nTail==0) {
		direction = 0;
	} 
	if(console::key(console::K_DOWN) && nTail==0) {
		direction = 3;
	} 
	if(console::key(console::K_UP) && nTail==0) {
		direction = 1;
	} 
  if (console::key(console::K_LEFT) && direction!=2 && direction!=0) {
		direction = 2;
  }
  if (console::key(console::K_RIGHT) && direction!=0 && direction!=2) {
		direction = 0;
  }
  if (console::key(console::K_UP) && direction!=1 && direction!=3) {
    direction = 1;
  }
  if (console::key(console::K_DOWN) && direction!=3 && direction!=1) {
    direction = 3;
  }
  if (console::key(console::K_ESC)) {
    exit(0);
  }    
}




void Logic() {
	// 승리했을 때 아래 작동
	int trueCount = 0;
	for(int i=0;i<BOARD_SIZE-2;i++) {
		for(int j=0;j<BOARD_SIZE-2;j++) {
			if(board[j][i]==true) trueCount++;
		}
	}
	if(trueCount == (BOARD_SIZE-2)*(BOARD_SIZE-2)) {
		std::string WIN_str = WIN_TEXT;
		console::draw((BOARD_SIZE - WIN_str.length()) / 2, BOARD_SIZE / 2, WIN_str);
		console::wait();
	}


  //과일을 먹으면 점수를 올리고, 꼬리가 길어지며, 과일을 다시 배치한다.
	if (x == fruitX && y == fruitY) {
    srand(time(nullptr));
		score += 10;
		while (true) 
		{
			int appleX = rand() % (width - 2) + 1; // 사과의 x 좌표
			int appleY = rand() % (height - 2) + 1; // 사과의 y 좌표
			if (!board[appleY][appleX]) 
			{ // 뱀이 없는 위치에 사과를 배치
					fruitX = appleX;
					fruitY = appleY;
					break;
			}
		}
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

	// 사용자로부터 받은 입력을 실제로 적용
	if(direction==0)
	{
		x++;
	}
	else if(direction==1)
	{
		y--;
	}
	else if(direction==2)
	{
		x--;
	}
	else if(direction==3)
	{
		y++;
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
	Setup();
	int frame = -1;
  console::init();

  while(true) {
		frame++;
		handleInput();

		if(gameOver) {
			std::string lose_str = LOSE_TEXT;
			std::string try_str = TRY_TEXT;
			console::draw((BOARD_SIZE - lose_str.length()) / 2, BOARD_SIZE / 2, LOSE_TEXT);		
			console::draw((BOARD_SIZE - try_str.length()) / 2, (BOARD_SIZE / 2)+1, TRY_TEXT);
			console::wait();	

			if (console::key(console::K_ENTER)) {
				gameOver = false;
				Setup();
 			 } 
			continue;			 
			
		}	

		if (0 < frame && frame < MOVE_DELAY) {
      console::wait();
      continue;
    }

    console::clear();
    drawMap();
    Draw();
    Logic();
    console::wait();
		frame = 0;
  }

}

int main() {
  game();
  return 0;
}
