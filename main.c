/*
***C Terminal Snake version 1
@ Hannu Korvala 29.10.2021

***What?
This is a snake game made using C programming language and is run on your linux terminal. Should be fully functional.
Works with Ubuntu 21.04 (actually Zorian OS 16 Core). Other compatibility has not been checked.

***How to compile and use:
1. Open terminal in the folder that has snake.c and terminalstyle.h
2. type gcc main.c -o snake
3. type ./snake

That should work! If terminal says to install gcc, do as it says :)!


***terminalstyle.h
https://github.com/korvahannu/terminalstyle

***Big thanks to Alnitak for this code
https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

// Main components. Importing order is important!
#include "components/misc.h"					// Contains misc helper functions
#include "components/terminalstyle.h"	// Contains code for styling the terminal
#include "components/screen.h"				// Contains code for the typedef Screen
#include "components/snake.h"					// Contains most logical code for typedef snake
#include "components/apple.h"					// Contains most apple stuff, duh!
#include "components/drawobject.h"		// Contains the functions that are called when a thing is drawn
#include "components/alnitak.h"				// Script by Alnitak that makes this game possible
#include "components/menu.h"					// Contains menus, such as the welcome screen
// *********
enum Collider {
	appleCollider,
	snakeBodyCollider,
	wallCollider,
	nullCollider
};
// Prototypes
void initialize(void);
void update(void);
void handleInput(void);
void draw(void);
enum Collider getCollider(void);
// *********

Snake snake;
Screen screen;
Apple apple;

int updateSpeed = 55000;	// Game speed, too fast may result in graphical glitches. Lower is faster

// Movement
char keyUp = 'w';
char keyDown = 's';
char keyLeft = 'a';
char keyRight = 'd';

int score = 0;
bool dead = false;

int main(void) {
	
	initialize();
	
	do
	{
		if(!dead)
			update();
		
		draw();
		
		usleep(updateSpeed);
	}
	while(!dead);
	
	return 0;
}

// Function: Some things need to be initialized before we start up the game
// This is the first function to be called in the main -function
void initialize(void) {

	srand(time(NULL));
	
	welcomeScreen();

	screen.positionX = 1;
	screen.positionY = 1;
	screen.width = 40;
	screen.height = 10;
	apple.scoreFactor = 50;
	
	initializeSnake(&snake, &screen);
	
	randomizeApplePosition(&apple, &snake, &screen);
}

// Game update function. Checks collision, moves snake etc.
void update(void) {
	
	
	saveSnakePreviousLocation(&snake);
	
	handleInput();

	moveSnakeOnce(&snake);

	enum Collider currentCollider = getCollider();
	
	if(currentCollider == appleCollider)
	{
		restoreSnakePreviousLocation(&snake);
		
		// Make the apples position the new snakes head
		int head = getSnakeHead(snake);
		snake.location[head+1][0] = apple.positionX;
		snake.location[head+1][1] = apple.positionY;
		
		printf("\a");
		
		score += apple.scoreFactor;
		randomizeApplePosition(&apple, &snake, &screen);
	}
	
	if(currentCollider == wallCollider ||currentCollider == snakeBodyCollider)	// Player hit wall or snake itself
	{
		dead = true;
		
		restoreSnakePreviousLocation(&snake);
	}
}

// Takes advantage of Alnitak's code. Checks if player has pressed any WASD key
// Does not block, if no key is down the game will continue.
// Chosen direction is stored in the direction -variable
void handleInput() {
	set_conio_terminal_mode();
	
	if(kbhit())
	{
		char c = getch();
		
		if(c == keyUp && snake.direction != down)
			snake.direction = up;
		else if (c==keyDown && snake.direction != up)
			snake.direction = down;
		else if (c==keyLeft  && snake.direction != right)
			snake.direction = left;
		else if (c==keyRight  && snake.direction != left)
			snake.direction = right;
		
	}
	
	reset_terminal_mode();
}

// Function: Somewhat complex function, that handless all drawing with putchar. Uses drawobject.h
void draw(void) {

	system("clear");
	
	bool bg = true;
	
	for(int y = 0; y < screen.positionY + screen.height; y++)
	{	
		if(y < screen.positionY)
		{
			for(int x = 0; x < screen.positionX + screen.width + screen.positionX; x++)
			{
				drawWall();
			}
		}
		else
		{
			
			for(int x = 0; x < screen.positionX + screen.width; x++)
			{
				if(x < screen.positionX)
				{
					drawWall();
				}
				else
				{
					for(int s = 0; s < snake.maxLength; s++)
					{
						if(snake.location[s][0] == x && snake.location[s][1] == y)
						{
							drawSnake();
							bg = false;
						}
					}
					
					if(apple.positionX == x && apple.positionY == y)
					{
						drawApple();
					}
					else if(bg)
						drawBackground();
				}
				
				bg = true;
			}
			
			for(int x = 0; x < screen.positionX; x++)
			{
				drawWall();
			}
		}	
		
		printf("\n");
	}
	
	for(int x = 0; x < screen.positionX+screen.width+screen.positionX; x++)
	{
		drawWall();
	}
	printf("\n");
	
	int head = getSnakeHead(snake);
	
	drawScoreboard(score, head+1);
	
	if(dead)
		printf("\nYikes! You died.");
	
	printf("\n\n\n\n");
	
}

// Checks if snake has collided with anything
enum Collider getCollider(void) {
	
	int head = getSnakeHead(snake);
	
	if(apple.positionX == snake.location[head][0] && apple.positionY == snake.location[head][1])
	{
		return appleCollider;
	}
	
	if(snake.location[head][0] < screen.positionX
	||snake.location[head][0] > screen.positionX+screen.width-1
	||snake.location[head][1] < screen.positionY
	||snake.location[head][1] > screen.positionY+screen.height-1)
	{
		return wallCollider;
	}
	
	for(int i = 0; i < snake.maxLength; i++)
	{
		if(i == head)
			break;
		
		if(snake.location[head][0] == snake.location[i][0] && snake.location[head][1] == snake.location[i][1])
		{
			return snakeBodyCollider;
		}
	}
	
	return nullCollider;
}