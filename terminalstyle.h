/*
terminalstyle.h takes use of ANSI escape sequences and provides an easier verison of making your basic C terminal application look better with colors and other features.

Usage:
#include "terminalstyle.h" at the start of your script file and have this file in the same folder

**********FUNCTIONS AND USAGE**********
void terminalStyle(char *);		Call this function to set an invidual style change. You can use the definitions below to help
					e.g. terminalStyle(TEXT_RED); terminalStyle(TEXT_EFFECT_ITALIC); would result in red text that is italic
					
void resetTerminalStyle(void);		Resets the terminal style to default
void clearTerminal(void);		Clear terminal with system("clear")
void terminalMoveCursor(int, int);	Move terminal cursor
void terminalStyleManual(char *);	Manuallu set your escape code. For example: 1;31 is bold red text
void printColorTable(void);		INDEV ONLY! Prints colortable that you can use with terminalStyle255()
void terminalStyle255(int, int);	changes terminal style to your liking using rbg codes between 1-255, first parameter is text color and second is background

void terminalRectangle(int, int, char *, char *);	// Draw a text rectangle
void terminalTextRectangle(int, int, char, char);	// Draw a rectangle using colors

cursor stuff:
void hideTerminalCursor(void);		Hide cursor
void showTerminalCursor(void);		show cursor
void toggleTerminalCursor(void);	toggle cursor
void saveCursorState(void);		save cursor location
void restoreCursorState(void);		sets cursor back to previously saved location
***************************************

Hannu Korvala 2021
Information got from fnky @ https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
*/

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DVC_DEFAULT "\033[0m"

// - These definitions can be used with terminalStyle(char *)
#define TEXT_BLACK "30"
#define TEXT_RED "31"
#define TEXT_GREEN "32"
#define TEXT_YELLOW "33"
#define TEXT_BLUE "34"
#define TEXT_PURPLE "35"
#define TEXT_CYAN "36"
#define TEXT_GREY "37"

#define BACKGROUND_BLACK "40"
#define BACKGROUND_RED "41"
#define BACKGROUND_GREEN "42"
#define BACKGROUND_YELLOW "43"
#define BACKGROUND_BLUE "44"
#define BACKGROUND_PURPLE "45"
#define BACKGROUND_CYAN "46"
#define BACKGROUND_GREY "47"

#define TEXT_EFFECT_BOLD "1"
#define TEXT_EFFECT_DIM "2"
#define TEXT_EFFECT_ITALIC "3"
#define TEXT_EFFECT_UNDERLINE "4"
#define TEXT_EFFECT_FLASH "5"
#define TEXT_EFFECT_INVERSE "7"
#define TEXT_EFFECT_HIDDEN "8"
#define TEXT_EFFECT_STRIKETHROUGH "9"
// -

void terminalStyle(char *);
void resetTerminalStyle(void);
void clearTerminal(void);
void terminalMoveCursor(int, int);
void terminalStyleManual(char *);
void applyChanges(void);
void printColorTable(void);
void terminalStyle255(int, int);
void hideTerminalCursor(void);
void showTerminalCursor(void);
void toggleTerminalCursor(void);
void terminalRectangle(int, int, char *, char *);
void terminalTextRectangle(int, int, char, char);
void saveCursorState(void);
void restoreCursorState(void);
void terminalRawInput(void);
void terminalCookedInput(void);

char text_color[25];
char background_color[25];
char text_effect = 'x';
int cursor = 1;

int cursorLocation[2] = {0, 0};

// Function: Call this function to add a specific style to your terminal text
// @param: Your style as char *, designed to be used with the definitions such as TEXT_RED
// @return: null
void terminalStyle(char * style) {
	
	if(style[1] == '\0') {
		text_effect = style[0];
	}
	else if(style[0] == '3') {
		strcpy(text_color, style);
	}
	else if(style[0] == '4') {
		strcpy(background_color, style);
	}
	
	applyChanges();
	
}

// Function: Call this function to print a color table. Designed to be used to help with function terminalStyle255
// @param: null
// @return: null
void printColorTable(void) {

	printf("\n\n*******PRINTING COLOR TABLE*******\n\n");
	int currentColor = 1;
	
	for(; currentColor < 256; currentColor++)
	{
		printf("\033[38;5;%dm", currentColor);
		printf("[%d]", currentColor);
	}
	
	for(currentColor = 1; currentColor < 256; currentColor++)
	{
		resetTerminalStyle();
		printf("\033[48;5;%dm", currentColor);
		printf("[%d]", currentColor);
	}
	
	resetTerminalStyle();

	printf("\n\n*******END OF COLOR TABLE*******\n\n");
	
	
}

// Function: Call this function to set own escape sequence style
// @param: escape sequence, such as 1;31 for bold red text
// @return: null
// Does no work well with terminalStyle and terminalStyle255, but gives more flexibility. Always reset terminal style and dont mix and match!
void terminalStyleManual(char * style) {
	
	resetTerminalStyle();
	printf("%s%s%c", "\033[", style, 'm');
	
}

// Function: Call this function to change your text and background color with 0-255 rbg colors. Use printColorTable to make this easier
// @param: int 1 => text color (1-255), int 2 => background color (1-255)
// @return: null
// Does no work well with terminalStyle and terminalStyleManual, but gives more flexibility. Always reset terminal style and dont mix and match!
void terminalStyle255(int text, int background) {
	resetTerminalStyle();
	printf("\033[38;5;%dm", text);
	printf("\033[48;5;%dm", background);
}

// Basic cursor functions
void hideTerminalCursor() {
	printf("\033[?25l");
	cursor = 0;
}

void showTerminalCursor() {
	printf("\033[?25h");
	cursor = 1;
}

void toggleTerminalCursor() {
	if(cursor)
		hideTerminalCursor();
	else
		showTerminalCursor();
}
//-

// TODO Does not work as intendent, but can be used for specific tasks
// Moves cursor to a set location on x and y axis using ANSI escape sequences
// @param: int x in x axis, int y in y axis
// @return null
void terminalMoveCursor(int x, int y) {

	if(x != 0) {
		if(x < 0)	
			printf("\033[%dD", abs(x)); // left
		else
			printf("\033[%dC", x); // right
		
		cursorLocation[0] = x;
	}
		
	if(y != 0) {
		if(y > 0)
			printf("\033[%dA", y);	// up
		else
			printf("\033[%dB", abs(y));  // down
		
		cursorLocation[1] = y;
	}
		
	
	
}

void clearTerminal(void) {
	system("clear");
}

// Function: Resets terminal style including the cursor styling
// @param: null
// @return null
void resetTerminalStyle() {
	cursor = 1;
	strcpy(text_color, "");
	strcpy(background_color, "");
	text_effect = 'x';
	showTerminalCursor();
	applyChanges();
}

// Applies the terminal changes. Used by terminalStyle and resetTerminalStyle
// Not meant to called outside these functions
// @param: null
// @return null
void applyChanges(void) {
	
	//if(text_color[0] == '\0') printf("n"); else printf("t");
	//if(background_color[0] == '\0') printf("n"); else printf("t");
	//if(text_effect == 'x') printf("n"); else printf("t");

	
	if(text_color[0] == '\0' && background_color[0] == '\0' && text_effect == 'x') // n n n
	{
		printf("%s", DVC_DEFAULT);
	}
	else if(text_color[0] != '\0' && background_color[0] != '\0' && text_effect == 'x') // t t n
	{
		printf("%s%s%c%s%c", "\033[", text_color, ';', background_color, 'm');
	}
	else if(text_color[0] != '\0' && background_color[0] == '\0' && text_effect == 'x') // t n n
	{
		printf("%s%s%c", "\033[", text_color, 'm');
	}
	else if(text_color[0] == '\0' && background_color[0] != '\0' && text_effect == 'x') // n t n
	{
		printf("%s%s%c", "\033[", background_color, 'm');
	}
	else if(text_color[0] == '\0' && background_color[0] != '\0' && text_effect != 'x') // n t t
	{
		printf("%s%c%c%s%c", "\033[", text_effect, ';', background_color, 'm');
	}
	else if(text_color[0] != '\0' && background_color[0] != '\0' && text_effect != 'x') // t t t
	{
		printf("%s%s%c%c%c%s%c", "\033[", text_color, ';', text_effect, ';', background_color, 'm');
	}
	else if(text_color[0] != '\0' && background_color[0] == '\0' && text_effect != 'x') // t n t
	{
		printf("%s%s%c%c%c", "\033[", text_color, ';', text_effect, 'm');
	}
	else if(text_color[0] == '\0' && background_color[0] == '\0' && text_effect != 'x') // n n t
	{
		printf("%s%c%c", "\033[", text_effect, 'm');
	}
}

// Function to call if you want to draw a rectangle using chars
// @params: int rectangle width, int rectangle height, char to fill the borders with, char to fill the empty space with
// example: 10, 10, 'x', ' '
// @return: null
// NOTE uses cursor location relatively
void terminalTextRectangle(int width, int height, char fill, char empty) {
	
	for(int i = 0; i < width+1; i++) {
		printf("%c", fill);
	}
	
	printf("\n");
	
	if(cursorLocation[0] > 0)
	{
		terminalMoveCursor(cursorLocation[0], 0);
	}

	for(int i = 0; i < height; i++)
	{
		for(int b = 0; b < width+1; b++) {
			if(b == 0 || b == width)
				printf("%c", fill);
			else
				printf("%c", empty);
		}
		
		printf("\n");
		
		if(cursorLocation[0] > 0)
		{
			terminalMoveCursor(cursorLocation[0], 0);
		}
	}
	
	for(int i = 0; i < width+1; i++) {
		printf("%c", fill);
	}
}

// Function to call if you want to draw a rectangle using colors
// @params: int rectangle width, int rectangle height, border color ,fill color
// example: 10, 10, BACKGROUND_RED, BACKGROUND_GREEN
// @return: null
// NOTE uses cursor location relatively
void terminalRectangle(int width, int height, char * fill, char * empty) {
	terminalStyle(fill);
	for(int i = 0; i < width+1; i++) {
		printf(" ");
	}
	
	printf("\n");
	
	if(cursorLocation[0] > 0)
	{
		terminalMoveCursor(cursorLocation[0], 0);
	}
	
	for(int i = 0; i < height; i++)
	{

		
		for(int b = 0; b < width+1; b++) {
			resetTerminalStyle();
			
			if(b == 0 || b == width)
			{
				terminalStyle(fill);
				printf(" ");
			}
			else
			{
				terminalStyle(empty);
				printf("%c", ' ');
			}
		}
		
		printf("\n");
		
		if(cursorLocation[0] > 0)
		{
			terminalMoveCursor(cursorLocation[0], 0);
		}
	}
	

	terminalStyle(fill);
	for(int i = 0; i < width+1; i++) {
		
		printf(" ");
	}
	resetTerminalStyle();
}

void saveCursorState() {
	printf("\0337");
}

void restoreCursorState() {
	printf("\0338");
}

// This function will make the terminal to immidietly accept input so an enter keypress is not required
void terminalRawInput()
{
	system ("/bin/stty raw");
}

void terminalCookedInput()
{
	system ("/bin/stty cooked");
}
