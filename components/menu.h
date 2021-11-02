// Function: Shows player the credit screen and lets the player customize
// playarea width, height and the speed of the game
void welcomeScreen(void) {
	
	system("clear");
	
	terminalStyle(TEXT_EFFECT_BOLD);
	terminalStyle(TEXT_BLUE);
	terminalStyle(TEXT_EFFECT_UNDERLINE);
	
	printf("C Snake by Hannu Korvala");
	resetTerminalStyle();
	terminalStyle(TEXT_EFFECT_ITALIC);
	terminalStyle(TEXT_RED);
	printf("\nVersion 1");
	resetTerminalStyle();
	
	printf("\n\nUse");
	terminalStyle(TEXT_EFFECT_BOLD);
	printf(" WASD ");
	resetTerminalStyle();
	printf("to move around.\n");

	printf("Press enter to start game.");

	getchar();
	
	
}