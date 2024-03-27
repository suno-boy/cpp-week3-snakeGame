#include "console.h"

#include <iostream>
#include <ctime>

#define BOARD_SIZE 12
#define MOVE_DELAY 15
#define WALL_VERTICAL_STRING "┃"
#define WALL_HORIZONTAL_STRING "━"
#define WALL_RIGHT_TOP_STRING "┓"
#define WALL_LEFT_TOP_STRING "┏"
#define WALL_RIGHT_BOTTOM_STRING "┛"
#define WALL_LEFT_BOTTOM_STRING "┗"
#define SNAKE_STRING "■"
#define SNAKE_BODY_STRING "■"
#define APPLE_STRING "●"

enum Direction { UP, DOWN, LEFT, RIGHT };

int x = (BOARD_SIZE - 2) / 2;
int y = (BOARD_SIZE - 2) / 2;
int score = 0;
bool gameOver = false;
Direction direction = RIGHT;

char MapIndex[BOARD_SIZE - 2][BOARD_SIZE - 2];

void initMap() {
    for (int i = 0; i < BOARD_SIZE - 2; ++i) {
        for (int j = 0; j < BOARD_SIZE - 2; ++j) {
            MapIndex[i][j] = ' ';
        }
    }
}

void drawMap() {
    console::draw(0, 0, WALL_LEFT_TOP_STRING);
    console::draw(BOARD_SIZE - 1, 0, WALL_RIGHT_TOP_STRING);
    console::draw(0, BOARD_SIZE - 1, WALL_LEFT_BOTTOM_STRING);
    console::draw(BOARD_SIZE - 1, BOARD_SIZE - 1, WALL_RIGHT_BOTTOM_STRING);
    for (int i = 1; i < BOARD_SIZE - 1; i++) {
        console::draw(i, 0, WALL_HORIZONTAL_STRING);
        console::draw(i, BOARD_SIZE - 1, WALL_HORIZONTAL_STRING);
        console::draw(0, i, WALL_VERTICAL_STRING);
        console::draw(BOARD_SIZE - 1, i, WALL_VERTICAL_STRING);
    }
}

void generateApple() {
    int appleX, appleY;
    do {
        appleX = rand() % (BOARD_SIZE - 2) + 1;
        appleY = rand() % (BOARD_SIZE - 2) + 1;
    } while (MapIndex[appleX - 1][appleY - 1] != ' ');

    MapIndex[appleX - 1][appleY - 1] = APPLE_STRING[0];
}

void drawScore() {
    std::string scoreString = "Score: " + std::to_string(score);
    int scoreX = (BOARD_SIZE - scoreString.length()) / 2;
    console::draw(scoreX, BOARD_SIZE, scoreString);
}

void drawSnake() {
    console::draw(x, y, SNAKE_STRING);
}

void update() {
    if (!gameOver) {
       
        MapIndex[x][y] = ' ';

        
        switch (direction) {
            case UP:
                y--;
                break;
            case DOWN:
                y++;
                break;
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
        }

        
        if (x <= 0 || x >= BOARD_SIZE - 1 || y <= 0 || y >= BOARD_SIZE - 1) {
            gameOver = true;
            return;
        }
        
        
        if (MapIndex[x][y] == SNAKE_STRING[0]) {
            gameOver = true;
            return;
        }

       
        if (MapIndex[x][y] == APPLE_STRING[0]) {
            score += 10;
            generateApple();
        }

        
        MapIndex[x][y] = SNAKE_STRING[0];
    }
}

void game() {
    console::init();
    initMap();
    drawMap();
    generateApple();

    while (!gameOver) {
        update();
        console::clear();
        drawMap();
        drawSnake();
        drawScore();
        console::wait();
    }

    
    std::string gameOverMessage = "Game Over! Score: " + std::to_string(score);
    int messageX = (BOARD_SIZE - gameOverMessage.length()) / 2;
    int messageY = BOARD_SIZE / 2;
    console::draw(messageX, messageY, gameOverMessage);

    
    while (console::key(console::K_ENTER) == false) {
        console::wait();
    }

   
    x = (BOARD_SIZE - 2) / 2;
    y = (BOARD_SIZE - 2) / 2;
    score = 0;
    gameOver = false;
}

int main() {
    srand(time(NULL));
    game();
    return 0;
}
