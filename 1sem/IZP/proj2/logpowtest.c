#include <stdio.h>
#include <math.h>
#include <ctype.h>

int ispair(double);

int main(void)
{
	double x = 1;
	double y = -2;
	double z = -1;
	double xy = 2;

	printf("%d %d %d %d\n", ispair(x), ispair(y), ispair(z), ispair(xy));

	printf("%g %g\n", pow(-INFINITY, INFINITY), pow(-INFINITY, -INFINITY));
}

int ispair(double x)
{
	x = fabs(x);
	
	if(((int)x % 2) == 0)
		return 1;
	else
		return 0;
}
