enum Direction {
	none,
	up,
	down,
	left,
	right
};

typedef struct {
	int startLength;
	int maxLength;
	int location[1000][2];
	int previousLocation[1000][2];
	enum Direction direction;
} Snake;

void initializeSnake(Snake * snake, Screen * screen)
{
	int firstPos = 0;

	(*snake).startLength = 10;
	(*snake).maxLength = 1000;
	(*snake).direction = right;

	for(int i = 0; i < (*snake).maxLength; i++)
	{
		(*snake).location[i][0] = -1;
		(*snake).location[i][1] = -1;
		(*snake).previousLocation[i][0] = -1;
		(*snake).previousLocation[i][1] = -1;
	}
	
	if((*snake).startLength < 1)
		(*snake).startLength = 1;
	
	(*snake).location[0][0] = (*screen).width/2;
	(*snake).location[0][1] = (*screen).height/2;
	firstPos = (*snake).location[0][0];
	
	for(int i = 1; i < (*snake).startLength; i++)	// Initialize rest of snake
	{
		(*snake).location[i][0] = ++firstPos;
		(*snake).location[i][1] = (*snake).location[0][1];
	}
}

// Gets the snakes head index
// Biggest snake[x] with a value that is not -1 represents snakes head
int getSnakeHead(Snake snake) {
	
	int head = 0;
	
	for(int i = 0; i < snake.maxLength; i++)
	{
		if(snake.location[i][0] != -1)
		{
			head = i;
		}
	}
	
	return head;
}

// Function: Saves the current snake position to snakePreviousLocation
void saveSnakePreviousLocation(Snake * snake) {
	for(int i = 0; i < (*snake).maxLength; i++)
	{
		(*snake).previousLocation[i][0] = (*snake).location[i][0];
		(*snake).previousLocation[i][1] = (*snake).location[i][1];
	}
}

// Function: Sets the snakes position back to previously saved snakePreviousLocation
void restoreSnakePreviousLocation(Snake * snake) {
	for(int i = 0; i < (*snake).maxLength; i++)
	{
		(*snake).location[i][0] = (*snake).previousLocation[i][0];
		(*snake).location[i][1] = (*snake).previousLocation[i][1];
	}
}

// Moves the snake once in the direction that it's facing
void moveSnakeOnce(Snake * snake) {
	int snakeHead = getSnakeHead((*snake));
	int tempPosition[2] = {-1, -1};
	
	
	if((*snake).direction == right)
	{
		tempPosition[0] = (*snake).location[snakeHead][0];
		tempPosition[1] = (*snake).location[snakeHead][1];
		
		(*snake).location[snakeHead][0]++;
		
		for(int i = snakeHead-1; i >= 0; i--)
		{
			swapInt(&(*snake).location[i][0], &tempPosition[0]);
			swapInt(&(*snake).location[i][1], &tempPosition[1]);
		}
	}
	else if((*snake).direction == left)
	{
		tempPosition[0] = (*snake).location[snakeHead][0];
		tempPosition[1] = (*snake).location[snakeHead][1];
		
		(*snake).location[snakeHead][0]--;
		
		for(int i = snakeHead-1; i >= 0; i--)
		{
			swapInt(&(*snake).location[i][0], &tempPosition[0]);
			swapInt(&(*snake).location[i][1], &tempPosition[1]);
		}
	}
	else if ((*snake).direction == up)
	{
		tempPosition[0] = (*snake).location[snakeHead][0];
		tempPosition[1] = (*snake).location[snakeHead][1];
		
		(*snake).location[snakeHead][1]--;
		
		for(int i = snakeHead-1; i >= 0; i--)
		{
			swapInt(&(*snake).location[i][0], &tempPosition[0]);
			swapInt(&(*snake).location[i][1], &tempPosition[1]);
		}
	}
	else if ((*snake).direction == down)
	{
		tempPosition[0] = (*snake).location[snakeHead][0];
		tempPosition[1] = (*snake).location[snakeHead][1];
		
		(*snake).location[snakeHead][1]++;
		
		for(int i = snakeHead-1; i >= 0; i--)
		{
			swapInt(&(*snake).location[i][0], &tempPosition[0]);
			swapInt(&(*snake).location[i][1], &tempPosition[1]);
		}
	}
}