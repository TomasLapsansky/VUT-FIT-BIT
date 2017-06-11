#include "io.h"

int get_word(char* s, int max, FILE *f)
{
	char string[max]; ///< max - 1 (limit) + '/0'
	
	int index = 0;
	char c;
	
	while(1)
	{
		c = fgetc(f);
		if(isspace(c))
		{
			break;
		}
	
		string[index] = c;
		
		if(index >= (max - 2))
		{
			index++;
			
			fprintf(stderr, "Long word!\n");
			
			while(1)
			{
				c = fgetc(f);
				if(isspace(c))
				{
					break;
				}
			}
			
			break;
			
		} else
			index++;
	}
	
	string[index] = 0;
	
	strcpy(s, string);
	
	return (index);
}
