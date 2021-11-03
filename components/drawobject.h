// You must include terminalstyle.h in your main.c!

char * snakeStyle = BACKGROUND_BLACK;
char snakeChar = ' ';

char * wallStyle = BACKGROUND_RED;
char wallChar = ' ';

char * backgroundStyle = "DVC_DEFAULT";
char backgroundChar = ' ';

char * appleStyle = BACKGROUND_GREEN;
char appleChar = 'A';

// To make modifying the game look mor easy, these draw(something) functions should help
void drawSnake(void) {
	terminalStyle(snakeStyle);
	putchar(snakeChar);
	resetTerminalStyle();
}

void drawWall(void) {
	terminalStyle(wallStyle);
	putchar(wallChar);
	resetTerminalStyle();
}

void drawBackground(void) {
	terminalStyle(backgroundStyle);
	putchar(backgroundChar);
	resetTerminalStyle();
}

void drawApple(void) {
	terminalStyle(appleStyle);
	putchar(appleChar);
	resetTerminalStyle();
}

void drawScoreboard(int score, int snakeLength) {
	terminalStyle(TEXT_GREY);
	terminalStyle(TEXT_EFFECT_BOLD);
	printf("Score: %d", score);
	resetTerminalStyle();
}