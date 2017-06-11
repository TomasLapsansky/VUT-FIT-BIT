/***
* Projekt1 IZP, FIT VUT Brno
* Autor: Tomáš Lapšanský
* Final date: 4.11.2016
***/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> //strtol
#include <ctype.h> //isprint

//Prototypes
void argx(void);
void argsn(int S, int N);
void args(int S);
void argr(void);
void print_help(void);

int stringcmp(char* str1, char* str2); //strcmp
void changeOne(int* i);
int isnum(char *str);

int main(int argc, char *argv[])
{
	if(argc == 1) //without argv
	{
		argsn(0, 0);
	}
	else //with argv
	{
		if((argc == 2) && stringcmp(argv[1], "-x")) //X flag
		{
			argx();
		} else if((argc == 5) && (stringcmp(argv[1], "-s")) && (isnum(argv[2])) && (stringcmp(argv[3], "-n")) && (isnum(argv[4]))) //S-N flag with 5 flags
		{
			if(*argv[4] == '0') //if N = 0, end
				return 0;
			int S = strtol(argv[2], (char **)NULL, 10); //convert argv to int
			int N = strtol(argv[4], (char **)NULL, 10);
			argsn(S, N);
		} else if((argc == 5) && (stringcmp(argv[1], "-n")) && (isnum(argv[2])) && (stringcmp(argv[3], "-s")) && (isnum(argv[4]))) //S-N flag with 5flags - inverted
		{
			if(*argv[2] == '0') //if N = 0, end
				return 0;
			int N = strtol(argv[2], (char **)NULL, 10); //convert argv to int
			int S = strtol(argv[4], (char **)NULL, 10);
			argsn(S, N);
		} else if((argc == 2) && stringcmp(argv[1], "-r")) //R flag
		{
			//printf("argr\n");
			argr();
		} else if((argc == 3) && (stringcmp(argv[1], "-S")) && (isnum(argv[2]))) //S flag
		{
			int S = strtol(argv[2], (char **)NULL, 10);
			args(S);
		} else
		{
			print_help();
			return 0;
		}
	}

	return 0;
}

void argx(void) //translate and print with -x
{
	int c;

	c = getchar();
	do
	{
		printf("%02x", c);
	} while((c = getchar()) != EOF);

	printf("\n");

	return;
}

void argsn(int adress, int scale) //printin with S N or without
{
	bool bscale = false; //true if N-flag is defined
	
	char buffer[16]; //buffer
	int i; //variable for buffer
	
	if(adress < 0) //adress cant be lower than 0
	{
		return;
	}
	
	if(scale != 0)
	{
		if(scale < 0) //scale cant be lower than 0
		{
			return;
		}
		bscale = true;
	}
	
	
	for(int j = 0; j < adress; j++) //S slip
	{
		int c = getchar();
		if(c == '\0')
			return;
	}
	
	do
	{
		for(int j = 0; j <= 15; j++) //clearin buffer
			buffer[j] = '\0';
			
		i = 0;
		
		if(bscale == 1)
		{
			while((i <= 15) && (scale > 0) && ((buffer[i] = getchar()) != EOF)) //loadin buffer with scale
			{
				i++;
				scale--;
			}
		} else
		{
			while((i <= 15) && ((buffer[i] = getchar()) != EOF)) //loadin buffer without scale
			{
				i++;
			}
		}
		
		printf("%08x  ", adress); //print adress
		
		adress += 16; //add hex adress
		
		for(int j = 0; j <= 15; j++) //printin hex-char
		{
			if((isprint(buffer[j]) == 0) && (buffer[j] != '\n')) //unprintable + \n print
				printf("   ");
			else
				printf("%02x ", buffer[j]); //hex-char
			
			if(j == 7)
				printf(" "); //for extra space
		}
		
		printf(" |");
		for(int i = 0; i <= 15; i++) //printin 16-string
		{
			
			if((buffer[i] == EOF) || (buffer[i] == '\0'))
				printf(" ");
			else if(isprint(buffer[i]) == 0)
				printf(".");
			else
				printf("%c", buffer[i]);
		}
		printf("|\n");
	} while((buffer[i] != EOF) && (((bscale == true) && (scale > 0)) || (bscale == false))); //end if EOF or end of scale
	
return;
}

void args(int S) //printin strings larger than S
{
	bool found = false;
	char c = 0;
	char buffer[S + 1];
	
	buffer[S] = '\0';
	
	while(c != EOF) //first writin to buffer
	{
		found = false; //if unprintable or !isblank was found
		
		for(int i = 0; i < S; i++)
		{
			c = getchar();
			
			if((isprint(c) == 0) && (isblank(c) == 0))
			{
				found = true;
				break;
			}
				
			buffer[i] = c;
		}
		
		if(found && (c != EOF))
		{
			continue;
		} else if(c == EOF)
		{
			break;
		}
			printf("%s", buffer);
		
		while((isprint(c = getchar()) != 0) || (isblank(c) != 0)) //direct printin
		{
			printf("%c", c);
		}
		
		printf("\n");
	}
}

void argr(void) //printin chars with -r
{
	char buffer[2] = {0, 0}; //buffer for XX
	int i = 0;

	while((buffer[i] = getchar()) != EOF) //loadin chars
	{
		if((buffer[i] == ' ') || (isprint(buffer[i]) == 0)) //if unprinted or blank char, changeOne and continue
		{
			if(i == 1) //one char was now loaded
			{
				changeOne(&i);
				if((buffer[0] != 0) || (buffer[1]) != 10)
					printf("%c", buffer[i]);
			}
			continue;
		}
		
		if((buffer[i] >= 48) && (buffer[i] <= 57)) //changing char adress to adress
			buffer[i] -= 48;
		else if((buffer[i] >= 97) && (buffer[i] <= 122))
			buffer[i] -= 87;
		
		if(i == 1) //if 2 chars were loaded and they are different from 0a, print them
		{
			printf("%c", ((buffer[0] * 16) + buffer[1]));
		}
		
		changeOne(&i);
	}

	return;
}

void changeOne(int* i) //switchin between 1 and 0
{
	if(*i == 1)
		*i = 0;
	else *i = 1;
}

int stringcmp(char* str1, char* str2) //strcmp
{
	int i = 0;

	while((str1[i] != '\0') || (str2[i] != '\0'))
	{
		if(str1[i] != str2[i])
			return 0; //non-equal
		
		i++;
	}

return 1; //equal
}

void print_help(void)
{
	printf("Wrong argument/s.\n");
	printf("-----------------\n");
	printf("Use -r to convert HEX to CHAR\n");
	printf("Use -x to convert CHAR to HEX\n");
	printf("Use [-S A] to print Strings larger than A\n");
	printf("Use no-arguments to convert String to 16xHEX\n");
	printf("Use [-s A] and [-n B] to skip and display translated String\n");
}

int isnum(char *str) //return 1 if string is number, else return 0
{
	int i = 0;
	
	while(str[i] != '\0') //comparing str[idx]
	{
		if(isdigit(str[i]) == 0)
			return 0;
		i++;
	}
	
	return 1;
}
