#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define LENGTH 1024

char** tail_init(int lines);
void tail_deinit(char** strings, int lines);
void tail(FILE * stdfile, char** tails, int size);

int main(int argc, char* argv[])
{
	
	if(argc == 1) ///< file from stdin
	{
		
		char** tails = tail_init(10);
		
		tail(stdin, tails, 10);
		
		tail_deinit(tails, 10);
		
	} else if(argc > 4)
	{
		
		fprintf(stderr, "Wrong number of argumets!\n");
		return 1;
		
	} else if(argc == 2)
	{
		if(strcmp("-n", argv[1]) == 0)
		{
			fprintf(stderr, "Missing argument!\n");
			return 1;
		}
		
		char** tails = tail_init(10);
		
		FILE * fr = fopen(argv[1], "r");
		if(fr == NULL)
		{
			fprintf(stderr, "Wrong file!\n");
			return 1;
		}
		
		tail(fr, tails, 10);
		
		fclose(fr);
		
		tail_deinit(tails, 10);
		
	} else if(argc == 3)
	{
		if(strcmp("-n", argv[1]) != 0)
		{
			fprintf(stderr, "Wrong argumets!\n");
			return 1;
		}
		
		int size = strtol(argv[2], NULL, 10);
		
		if(isdigit(size) != 0)
		{
			fprintf(stderr, "-n Not a number!\n");
			return 1;
		}
		
		if(size == 0)
			return 0;
		
		if(size < 0)
		{
			fprintf(stderr, "Bad -n!\n");
			return 1;
		}
		
		char** tails = tail_init(size);
		
		tail(stdin, tails, size);
		
		tail_deinit(tails, size);
		
	} else if(argc == 4)
	{
	
		if(strcmp("-n", argv[1]) != 0)
		{
			fprintf(stderr, "Wrong argumets!\n");
			return 1;
		}
		
		int size = strtol(argv[2], NULL, 10);
		
		if(isdigit(size) != 0)
		{
			fprintf(stderr, "-n Not a number!\n");
			return 1;
		}
		
		if(size == 0)
			return 0;
		
		if(size < 0)
		{
			fprintf(stderr, "Bad -n!\n");
			return 1;
		}
		
		char** tails = tail_init(size);
		
		FILE * fr = fopen(argv[3], "r");
		if(fr == NULL)
		{
			fprintf(stderr, "Wrong file!\n");
			return 1;
		}
		
		tail(fr, tails, size);
		
		fclose(fr);
		
		tail_deinit(tails, size);
		
	}
	
	return 0;
}

char** tail_init(int lines)
{
	char** output;
	
	if((output = malloc(sizeof(char*) * lines)) == NULL)
	{
		fprintf(stderr, "Cant alloc!\n");
		exit(1);
	}
	
	for(int i = 0; i < lines; i++)
	{
		if((output[i] = malloc(sizeof(char) * LENGTH + 2)) == NULL) ///< /n + /0
		{
			fprintf(stderr, "Cant alloc!\n");
			exit(1);
		}
	}
	
	return output;
}

void tail_deinit(char** strings, int lines)
{
	for(int i = 0; i < lines; i++)
	{
		free(strings[i]);
	}
	
	free(strings);
}

void tail(FILE * stdfile, char** tails, int size)
{
	char read_string[LENGTH + 2];
	int count = 0;
	
	while(fgets(read_string, (LENGTH + 2), stdfile) != NULL)
	{
		bool err = true;
		
		for(int i = 0; i < LENGTH; i++)
		{
			if(read_string[i] == '\n')
			{
				err = false;
				break;
			}
		}
		
		if(err)
		{
			read_string[LENGTH] = '\n';
			read_string[LENGTH + 1] = '\0';
			fprintf(stderr, "Long line!\n");
		}
		
		if(count >= size)
		{	
			for(int i = 1; i < size; i++)
			{
				strcpy(tails[i - 1], tails[i]);
			}
			
			strcpy(tails[size - 1], read_string);
		} else
		{
			strcpy(tails[count], read_string);
			count++;
		}		
	}
	
	for(int i = 0; i < size; i++)
	{
		fprintf(stdout, "%s", tails[i]);
	}
}
