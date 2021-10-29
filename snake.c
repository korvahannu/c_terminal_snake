/*
***C Terminal Snake version 1
@ Hannu Korvala 29.10.2021

***What?
This is a snake game made using C programming language and is run on your linux terminal. Should be fully functional.
Works with Ubuntu 21.04 (actually Zorian OS 16 Core). Other compatibility has not been checked.

***How to compile and use:
1. Open terminal in the folder that has snake.c and terminalstyle.h
2. type gcc snake.c -o snake
3. type ./snake

That should work! If terminal says to install gcc, do as it says :)!

***To do:
- Support for more apples
- Support to disable wall death (teleport snake to the other side)
- Proper main menu
- Maybe two players?
- More customization options
- Current welcome screen sucks and bugs may appear if player inputs non-supported answers

***terminalstyle.h
https://github.com/korvahannu/terminalstyle

***Big thanks to Alnitak for this code
https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "terminalstyle.h"
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>

enum Direction {
	none,
	up,
	down,
	left,
	right
};


void initialize(void);
void initializeSnake(void);
void welcomeScreen(void);

void update(void);
void updateInput(void);
void updateSnakePosition(void);
int checkCollision(void);

void draw(void);
void drawSnake(void);
void drawWall(void);
void drawBackground(void);
void drawApple(void);

void randomizeApplePosition(void);
int getSnakeHead(void);
void saveSnakePreviousLocation(void);
void restoreSnakePreviousLocation();

void swapInt(int *, int *);

/*
Credit to Alnitak for the 4 functions below
https://stackoverflow.com/questions/448944/c-non-blocking-keyboard-input
*/
void reset_terminal_mode(void);
void set_conio_terminal_mode(void);
int kbhit(void);
int getch(void);
//---------end of Alnitak's functions


// Game settings

// snake settings. Note: snakeMax, snake[x] and snakePreviousLocation[x] have to have the same value!
int snakeMax = 1000;
int snake[1000][2] = {-1};
int snakePreviousLocation[1000][2] = {-1};
int snakeStartLength = 10;
enum Direction direction;

// Initialize screen settings
int scrX = 1;	// where the screen will be drawn in the x axis
int scrY = 1;	// same as above but y axis
int scrW = 60;	// play-area width
int scrH = 15;	// play-area height
int scrExtra = 1;	// how much extra border we draw outside the play area

int updateSpeed = 60000;	// Game speed, too fast may result in graphical glitches. Lower is faster

// Movement
char keyUp = 'w';
char keyDown = 's';
char keyLeft = 'a';
char keyRight = 'd';

int apples = 0;
bool dead = false;

int apple[2];	// Apple location


// Function: Some things need to be initialized before we start up the game
// This is the first function to be called in the main -function
void initialize(void) {
	
	srand(time(NULL));
	
	welcomeScreen();
	
	initializeSnake();
	
	randomizeApplePosition();
	direction = right;
}

// Function: Shows player the credit screen and lets the player customize
// playarea width, height and the speed of the game
void welcomeScreen(void) {
	
	system("clear");
	
	terminalStyle(TEXT_EFFECT_BOLD);
	terminalStyle(TEXT_BLUE);
	
	printf("C Snake by Hannu Korvala");
	resetTerminalStyle();
	terminalStyle(TEXT_EFFECT_ITALIC);
	terminalStyle(TEXT_RED);
	printf("\nVersion 1");
	resetTerminalStyle();
	
	printf("\n\nUse WASD to move around.");
	printf("\npress enter to continue . . .");
	getchar();
	system("clear");
	
	printf("Type desired screen width (recommended 60) > ");
	scanf("%d", &scrW);
	printf("\nType desired screen height (recommended 15) > ");
	scanf("%d", &scrH);
	printf("\nType desired game update speed in ms (recommended 60000) > ");
	scanf("%d", &updateSpeed);
	
	
}

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

// Game update function. Checks collision, moves snake etc.
void update(void) {
	
	
	saveSnakePreviousLocation();
	
	updateInput();
	updateSnakePosition();
	
	if(checkCollision() == 1) // 1 => apple got
	{
		restoreSnakePreviousLocation();
		
		// Make the apples position the new snakes head
		int head = getSnakeHead();
		snake[head+1][0] = apple[0];
		snake[head+1][1] = apple[1];
		
		printf("\a");
		
		apples++;
		randomizeApplePosition();
	}
	
	if(checkCollision() == 2 ||checkCollision() == 3)	// Player hit wall or snake itself
	{
		dead = true;
		
		restoreSnakePreviousLocation();
	}
}

// Takes advantage of Alnitak's code. Checks if player has pressed any WASD key
// Does not block, if no key is down the game will continue.
// Chosen direction is stored in the direction -variable
void updateInput() {
	set_conio_terminal_mode();
	
	if(kbhit())
	{
		char c = getch();
		
		if(c == keyUp && direction != down)
			direction = up;
		else if (c==keyDown && direction != up)
			direction = down;
		else if (c==keyLeft  && direction != right)
			direction = left;
		else if (c==keyRight  && direction != left)
			direction = right;
		
	}
	
	reset_terminal_mode();
}

// Updates snakes position using the direction -variable
void updateSnakePosition() {
	
	int snakeHead = getSnakeHead();
	int tempPosition[2] = {-1, -1};
	
	
	if(direction == right)
	{
		tempPosition[0] = snake[snakeHead][0];
		tempPosition[1] = snake[snakeHead][1];
		
		snake[snakeHead][0]++;
		
		for(int i = snakeHead-1; i >= 0; i--)
		{
			swapInt(&snake[i][0], &tempPosition[0]);
			swapInt(&snake[i][1], &tempPosition[1]);
		}
	}
	else if(direction == left)
	{
		tempPosition[0] = snake[snakeHead][0];
		tempPosition[1] = snake[snakeHead][1];
		
		snake[snakeHead][0]--;
		
		for(int i = snakeHead-1; i >= 0; i--)
		{
			swapInt(&snake[i][0], &tempPosition[0]);
			swapInt(&snake[i][1], &tempPosition[1]);
		}
	}
	else if (direction == up)
	{
		tempPosition[0] = snake[snakeHead][0];
		tempPosition[1] = snake[snakeHead][1];
		
		snake[snakeHead][1]--;
		
		for(int i = snakeHead-1; i >= 0; i--)
		{
			swapInt(&snake[i][0], &tempPosition[0]);
			swapInt(&snake[i][1], &tempPosition[1]);
		}
	}
	else if (direction == down)
	{
		tempPosition[0] = snake[snakeHead][0];
		tempPosition[1] = snake[snakeHead][1];
		
		snake[snakeHead][1]++;
		
		for(int i = snakeHead-1; i >= 0; i--)
		{
			swapInt(&snake[i][0], &tempPosition[0]);
			swapInt(&snake[i][1], &tempPosition[1]);
		}
	}
	

}

// Function: Saves the current snake position to snakePreviousLocation
void saveSnakePreviousLocation(void) {
	for(int i = 0; i < snakeMax; i++)
	{
		snakePreviousLocation[i][0] = snake[i][0];
		snakePreviousLocation[i][1] = snake[i][1];
	}
}

// Function: Sets the snakes position back to previously saved snakePreviousLocation
void restoreSnakePreviousLocation(void) {
	for(int i = 0; i < snakeMax; i++)
	{
		snake[i][0] = snakePreviousLocation[i][0];
		snake[i][1] = snakePreviousLocation[i][1];
	}
}

// Function: Somewhat complex function, that handless all drawing of this snake game
void draw(void) {

	system("clear");
	
	bool bg = true;
	
	for(int y = 0; y < scrY + scrH; y++)
	{	
		if(y < scrY)
		{
			for(int x = 0; x < scrX + scrW + scrExtra; x++)
			{
				drawWall();
			}
		}
		else
		{
			
			for(int x = 0; x < scrX + scrW; x++)
			{
				if(x < scrX)
				{
					drawWall();
				}
				else
				{
					for(int s = 0; s < snakeMax; s++)
					{
						if(snake[s][0] == x && snake[s][1] == y)
						{
							drawSnake();
							bg = false;
						}
					}
					
					if(apple[0] == x && apple[1] == y)
					{
						drawApple();
					}
					else if(bg)
						drawBackground();
				}
				
				bg = true;
			}
			
			for(int x = 0; x < scrExtra; x++)
			{
				drawWall();
			}
		}	
		
		printf("\n");
	}
	
	for(int x = 0; x < scrX+scrW+scrExtra; x++)
	{
		drawWall();
	}
	printf("\n");
	
	int head = getSnakeHead();
	
	terminalStyle(BACKGROUND_YELLOW);
	terminalStyle(TEXT_GREY);
	terminalStyle(TEXT_EFFECT_BOLD);
	printf("Apples: %d . . . Snake length: %d", apples, head+1);
	resetTerminalStyle();
	
	if(dead)
		printf("\nYikes! You died.");
	
	printf("\n\n\n\n");
	
}

// Checks if snake has collided with anything
// Return: 0 means no collision, 1 means collision with apple, 2 means collision with a wall, 3 means collision with snakes body
int checkCollision(void) {
	
	int head = getSnakeHead();
	
	if(apple[0] == snake[head][0] && apple[1] == snake[head][1])
	{
		return 1;
	}
	
	if(snake[head][0] < scrX
	||snake[head][0] > scrX+scrW-1
	||snake[head][1] < scrY
	||snake[head][1] > scrY+scrH-1)
	{
		return 2;
	}
	
	for(int i = 0; i < snakeMax; i++)
	{
		if(i == head)
			break;
		
		if(snake[head][0] == snake[i][0] && snake[head][1] == snake[i][1])
		{
			return 3;
		}
	}
	
	return 0;
}

// Initializes our snake-boy
// BUG: this code does not take into account that if the playarea is too narrow, the snake spawns outside of the playarea and player is immidietly killed
void initializeSnake(void) {
	
	int firstPos = 0;

	for(int i = 0; i < snakeMax; i++)	// in snake[x][x] -1 means that that part of the snakes body does not exist
	{
		snake[i][0] = -1;
		snake[i][1] = -1;
	}
	
	
	
	if(snakeStartLength < 1)
		snakeStartLength = 1;
	
	snake[0][0] = scrW/2;
	snake[0][1] = scrH/2;
	firstPos = snake[0][0];
	
	for(int i = 1; i < snakeStartLength; i++)	// Initialize rest of snake
	{
		snake[i][0] = ++firstPos;
		snake[i][1] = snake[0][1];
	}
}

// Randomizes apple position, duh!
void randomizeApplePosition(void) {
	
	bool acceptPosition = false;
	
	do
	{
		acceptPosition = true;
		apple[0] = scrX + (rand()%scrW);
		apple[1] = scrY + (rand()%scrH);
		
		for(int i = 0; i < snakeMax; i++)
		{
			if(snake[i][0]==-1)
				break;
			
			if(apple[0] == snake[i][0] && apple[1] == snake[i][1])
				acceptPosition = false;
		}
	}
	while(!acceptPosition);
}

// To make modifying the game look mor easy, these draw(something) functions should help
void drawSnake(void) {
	terminalStyle(BACKGROUND_BLACK);
	printf(" ");
	resetTerminalStyle();
}

void drawWall(void) {
	terminalStyle(BACKGROUND_RED);
	printf(" ");
	resetTerminalStyle();
}

void drawBackground(void) {
	terminalStyle(BACKGROUND_GREY);
	printf(" ");
	resetTerminalStyle();
}

void drawApple(void) {
	terminalStyle(BACKGROUND_GREEN);
	printf(" ");
	resetTerminalStyle();
}
// end of draw(something) functions

void swapInt(int * swap1, int * swap2)
{
	int temp = *swap1;
	*swap1 = *swap2;
	*swap2 = temp;
}

// Gets the snakes head index
// Biggest snake[x] with a value that is not -1 represents snakes head
int getSnakeHead() {
	
	int head = 0;
	
	for(int i = 0; i < snakeMax; i++)
	{
		if(snake[i][0] != -1)
		{
			head = i;
		}
	}
	
	return head;
}


// THIS CODE BELOW IS NOT MADE BY ME, CHECK CREDITS ABOVE
struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}
