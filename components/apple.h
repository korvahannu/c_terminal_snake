// Randomizes apple position, duh!
void randomizeApplePosition(int * appleX, int * appleY, Snake * snake, Screen * screen) {
	
	bool acceptPosition = false;
	
	do
	{
		acceptPosition = true;
		*appleX = (*screen).positionX + (rand()%(*screen).width);
		*appleY = (*screen).positionY + (rand()%(*screen).height);
		
		for(int i = 0; i < (*snake).maxLength; i++)
		{
			if((*snake).location[i][0]==-1)
				break;
			
			if(*appleX == (*snake).location[i][0] && *appleY == (*snake).location[i][1])
				acceptPosition = false;
		}
	}
	while(!acceptPosition);
}