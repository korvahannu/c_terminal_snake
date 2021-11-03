typedef struct {
	int positionX;
	int positionY;
	int scoreFactor;
} Apple;

void randomizeApplePosition(Apple * apple, Snake * snake, Screen * screen) {
	
	bool acceptPosition = false;
	
	do
	{
		acceptPosition = true;
		(*apple).positionX = (*screen).positionX + (rand()%(*screen).width);
		(*apple).positionY = (*screen).positionY + (rand()%(*screen).height);
		
		for(int i = 0; i < (*snake).maxLength; i++)
		{
			if((*snake).location[i][0]==-1)
				break;
			
			if((*apple).positionX  == (*snake).location[i][0] && (*apple).positionY == (*snake).location[i][1])
				acceptPosition = false;
		}
	}
	while(!acceptPosition);
}
