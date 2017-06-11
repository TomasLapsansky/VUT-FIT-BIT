#include "io.h"

int main(void)
{
	FILE * fr = fopen("subor2", "r");
	
	char string[15];
	
	int c = get_word(string, 15, fr);
	
	printf("%s %d\n", string, c);
	
	c = get_word(string, 15, fr);
	
	printf("%s %d\n", string, c);
	
	return 0;
}
