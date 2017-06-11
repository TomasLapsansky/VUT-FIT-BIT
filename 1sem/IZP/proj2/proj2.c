/*
 Tomas Lapsansky
 IZP Projekt 2 - iteracne vypocty
 2016
 FIT VUTBR
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <ctype.h>

#include <math.h>

//prototypes
double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
double taylor_pow(double x, double y, unsigned int n);
double taylorcf_pow(double x, double y, unsigned int n);

double mylog(double x);
double mypow(double x, double y);

double absolut(double x);
void help(void);
int isint(double x);
int ispair(double x);

int main(int argc, char* argv[])
{
	if((argc == 4) && (strcmp("--log", argv[1]) == 0) /* doplnit cisla */) /// <----------------------------------- LOG
	{
		if(strtod(argv[3], NULL) <= 0) //iterations
		{
			help();
			return 0;
		} else if(strcmp("INFINITY", argv[2]) == 0)
		{
			printf("         log(%5.4g) = %.12g\n", strtod(argv[2], NULL), log(strtod(argv[2], NULL)));
			printf("   cfrac_log(%5.4g) = %.12g\n", strtod(argv[2], NULL), INFINITY);
			printf("  taylor_log(%5.4g) = %.12g\n", strtod(argv[2], NULL), INFINITY);
			
			return 0;
		}
		
		printf("         log(%5.4g) = %.12g\n", strtod(argv[2], NULL), log(strtod(argv[2], NULL)));
		printf("   cfrac_log(%5.4g) = %.12g\n", strtod(argv[2], NULL), cfrac_log(strtod(argv[2], NULL), strtod(argv[3], NULL)));
		printf("  taylor_log(%5.4g) = %.12g\n", strtod(argv[2], NULL), taylor_log(strtod(argv[2], NULL), strtod(argv[3], NULL)));
	} else if((argc == 5) && (strcmp("--pow", argv[1]) == 0) /* doplnit cisla */) /// <----------------------------------- POW
	{
		if((strcmp("INFINITY", argv[2]) == 0) && (strcmp("-INFINITY", argv[3]) == 0))
		{
			{
				printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
				printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
				printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
				
				return 0;
			}
		}
		
		if((strcmp("-INFINITY", argv[2]) == 0) && (strcmp("INFINITY", argv[3]) == 0))
		{
			{
				printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(-INFINITY, INFINITY));
				printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
				printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
				
				return 0;
			}
		}
		
		if((strcmp("-INFINITY", argv[2]) == 0) && (strcmp("-INFINITY", argv[3]) == 0))
		{
			{
				printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(-INFINITY, -INFINITY));
				printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
				printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
				
				return 0;
			}
		}
		
		if(strcmp("NAN", argv[2]) == 0) // NAN ˆ X
		{
			printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), NAN);
			printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), NAN);
			
			return 0;
		}
		
		if(strcmp(argv[2], "1") == 0) // X == 1
		{
			printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 1.0);
			printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 1.0);
			
			return 0;
		}
		
		if(strcmp("NAN", argv[3]) == 0) // X ^ NAN
		{
			printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), NAN);
			printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), NAN);
			
			return 0;
		}
		
		if((strcmp("-INFINITY", argv[2]) == 0) && (strtod(argv[3], NULL) != 0.0)) //argument -INFINITY
		{
			double temp = strtod(argv[3], NULL);
			if(temp > 0)
			{
				if(isint(temp) != 1)
				{
					printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
					printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
					printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
				} else {
					if(ispair(temp))
					{
						printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
						printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
						printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
					} else {
						printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
						printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), -INFINITY);
						printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), -INFINITY);
					}
				}
			} else {
				if(isint(temp) != 1)
				{
					printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
					printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
					printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
				} else {
					if(ispair(temp))
					{
						printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
						printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
						printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
					} else {
						printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
						printf("  taylor_pow(%3.2g, %3.2g) = -0\n", strtod(argv[2], NULL), strtod(argv[3], NULL));
						printf("taylorcf_pow(%3.2g, %3.2g) = -0\n", strtod(argv[2], NULL), strtod(argv[3], NULL));
					}
				}
			}
			
			return 0;
		}
		
		if((strtod(argv[3], NULL) < 0) && (strtod(argv[2], NULL) != 0) && (strcmp("-INFINITY", argv[3]) != 0))//negative exponent != INF && a != 0
		{
			printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), taylor_pow(1 / (strtod(argv[2], NULL)), -(strtod(argv[3], NULL)), strtod(argv[4], NULL)));
			printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), taylorcf_pow(1 / (strtod(argv[2], NULL)), -(strtod(argv[3], NULL)), strtod(argv[4], NULL)));
			
			return 0;
		}
		if((strcmp("INFINITY", argv[3]) == 0) && (strtod(argv[2], NULL) != 0)) //INFINITY exponent && a != 0
		{
			printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
			printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
			
			return 0;
		}
		
		if((strtod(argv[3], NULL) != 0) && (strcmp("INFINITY", argv[2]) == 0)) //INFINITY argument && x != 0
		{
			printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
			printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
			
			return 0;
		}
		
		if((strtod(argv[2], NULL) != 0) && (strcmp("-INFINITY", argv[3]) == 0)) //exponent -INFINITY
		{
			printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
			printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
			
			return 0;
		}
		
		if(strtod(argv[4], NULL) <= 0) //iterations
		{
			help();
			return 0;
		}
		
		printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
		printf("  taylor_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), taylor_pow(strtod(argv[2], NULL), strtod(argv[3], NULL), strtod(argv[4], NULL)));
		printf("taylorcf_pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), taylorcf_pow(strtod(argv[2], NULL), strtod(argv[3], NULL), strtod(argv[4], NULL)));
	} else if((argc == 3) && (strcmp("--mylog", argv[1]) == 0) /* doplnit cisla */) /// <----------------------------------- MYLOG
	{
		if(strcmp("INFINITY", argv[2]) == 0)
		{
			printf("         log(%5.4g) = %.7g\n", strtod(argv[2], NULL), log(strtod(argv[2], NULL)));
			printf("       mylog(%5.4g) = %.7g\n", strtod(argv[2], NULL), INFINITY);
		} else
		{
			printf("         log(%5.4g) = %.7g\n", strtod(argv[2], NULL), log(strtod(argv[2], NULL)));
			printf("       mylog(%5.4g) = %.7g\n", strtod(argv[2], NULL), mylog(strtod(argv[2], NULL)));
		}
	} else if((argc == 4) && (strcmp("--mypow", argv[1]) == 0) /* doplnit cisla */) /// <----------------------------------- MYPOW
	{
		if((strcmp("INFINITY", argv[2]) == 0) && (strcmp("-INFINITY", argv[3]) == 0))
		{
			{
				printf("         pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
				printf("       mypow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
				
				return 0;
			}
		}
		
		if((strcmp("-INFINITY", argv[2]) == 0) && (strcmp("INFINITY", argv[3]) == 0))
		{
			{
				printf("         pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(-INFINITY, INFINITY));
				printf("       mypow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
				
				return 0;
			}
		}
		
		if((strcmp("-INFINITY", argv[2]) == 0) && (strcmp("-INFINITY", argv[3]) == 0))
		{
			{
				printf("         pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(-INFINITY, -INFINITY));
				printf("       mypow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
				
				return 0;
			}
		}
		
		if(strcmp("NAN", argv[2]) == 0) // NAN ˆ X
		{
			printf("         pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("  taylor_pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), NAN);
			printf("taylorcf_pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), NAN);
			
			return 0;
		}
		
		if(strcmp(argv[2], "1") == 0) // X == 1
		{
			printf("         pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("  taylor_pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 1.0);
			printf("taylorcf_pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 1.0);
			
			return 0;
		}
		
		if(strcmp("NAN", argv[3]) == 0) // X ^ NAN
		{
			printf("         pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("  taylor_pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), NAN);
			printf("taylorcf_pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), NAN);
			
			return 0;
		}
		
		if((strcmp("-INFINITY", argv[2]) == 0) && (strtod(argv[3], NULL) != 0.0)) //argument -INFINITY
		{
			double temp = strtod(argv[3], NULL);
			if(temp > 0)
			{
				if(isint(temp) != 1)
				{
					printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
					printf("       mypow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
				} else {
					if(ispair(temp))
					{
						printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
						printf("       mypow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
					} else {
						printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
						printf("       mypow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), -INFINITY);
					}
				}
			} else {
				if(isint(temp) != 1)
				{
					printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
					printf("       mypow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
				} else {
					if(ispair(temp))
					{
						printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
						printf("       mypow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
					} else {
						printf("         pow(%3.2g, %3.2g) = %.12g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
						printf("       mypow(%3.2g, %3.2g) = -0\n", strtod(argv[2], NULL), strtod(argv[3], NULL));
					}
				}
			}
			
			return 0;
		}
		
		if((strtod(argv[3], NULL) < 0) && (strtod(argv[2], NULL) != 0) && (strcmp("-INFINITY", argv[3])))//negative exponent != INF && a != 0
		{
			printf("         pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("       mypow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), mypow(1 / (strtod(argv[2], NULL)), -(strtod(argv[3], NULL))));
			
			return 0;
		}
		if((strcmp("INFINITY", argv[3]) == 0) && (strtod(argv[2], NULL) != 0)) //INFINITY exponent && a != 0
		{
			printf("         pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("       mypow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
			
			return 0;
		}
		
		if((strtod(argv[3], NULL) != 0) && (strcmp("INFINITY", argv[2]) == 0)) //INFINITY argument && x != 0
		{
			printf("         pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("       mypow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), INFINITY);
			
			return 0;
		}
		
		if((strtod(argv[2], NULL) != 0) && (strcmp("-INFINITY", argv[3]) == 0))
		{
			printf("         pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
			printf("       mypow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), 0.0);
			
			return 0;
		}
		
		printf("         pow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), pow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
		printf("       mypow(%3.2g, %3.2g) = %.7g\n", strtod(argv[2], NULL), strtod(argv[3], NULL), mypow(strtod(argv[2], NULL), strtod(argv[3], NULL)));
	} else
		help();
	
	return 0;
}

double taylor_log(double x, unsigned int n) //for x < 1
{
	if(x == 1)
	{
		return 0.0;
	} else if(x == 0)
	{
		return -INFINITY;
	} else if((0 < x) && (x < 1))
	{
		x = 1.0 - x; //for convert to log(1 - x)
		double log = 0;
		double prelog = 1;
		
		for(unsigned int ni = 1; ni <= n; ni++)
		{
			prelog = prelog * x;
		
			log = log - prelog/ni;
		}
		
		return log;
	} else if(x > 1)
	{
		double sum = 0;
		double part = 1;
		
		for(unsigned int ni = 1; ni <= n; ni++)
		{
			part = part * ((x - 1)/x);
			
			sum = sum + part/ni;
		}
		
		return sum;
	} else if(x == INFINITY)
	{
		return INFINITY;
	}
	
	return NAN;
}

double cfrac_log(double x, unsigned int n)
{
	if(x == 0)
	{
		return -INFINITY;
	} else if(x < 0)
	{
		return NAN;
	}
		
	double cf = 1.0;
	double z = (x - 1)/(x + 1);
	
	for(unsigned int ni = n; ni > 0; ni--)
	{
		cf =  (2 * ni - 1) - (((ni * ni) * (z * z)) / cf);
	}
	
	return (2 * z) / cf;
}

double taylor_pow(double x, double y, unsigned int n)
{
	double pow = 1;
	double part = 1;
	
	if (x == 0)
	{
		if (y == 0)
			return 1;
		else if (y < 0)
			return INFINITY;
		
		return 0;
	} else if((x < 0) && (isint(y)))
	{
		for(int i = 1; i <= y; i++)
			pow = pow * x;

		return pow;
	} else if((x < 0) && !(isint(y)))
		return NAN;
	else if(y == 0)
		return 1;
	
	for(unsigned int ni = 1; ni <= n; ni++)
	{
		part = part * ((y * taylor_log(x, n)) / ni);
		pow = pow + part;
	}
	
	return pow;
}

double taylorcf_pow(double x, double y, unsigned int n)
{
	double pow = 1;
	double part = 1;
	
	if (x == 0)
	{
		if (y == 0)
			return 1;
		else if (y < 0)
			return INFINITY;
		
		return 0;
	} else if((x < 0) && (isint(y)))
	{
		for(int i = 1; i <= y; i++)
			pow = pow * x;
		
		return pow;
	} else if((x < 0) && !(isint(y)))
		return NAN;
	else if(y == 0)
		return 1;
	
	for(unsigned int ni = 1; ni <= n; ni++)
	{
		part = part * ((y * cfrac_log(x, n)) / ni);
		pow = pow + part;
	}
	
	return pow;
}

double mylog(double x)
{
	if(x == 1)
	{
		return 0.0;
	} else if(x == 0)
	{
		return -INFINITY;
	}else if((0 < x) && (x < 1))
	{
		x = 1.0 - x; //for convert to log(1 - x)
		double eps = 10e-8;
		
		double before_log = -1; //for first iter.
		double log = 0;
		double prelog = 1;
		unsigned int ni = 1;
		
		while(absolut(log - before_log) > eps)
		{
			prelog = prelog * x;
			
			before_log = log;
			log = log - prelog/ni;
			
			ni++;
		}
		
		return log;
	} else if(x > 1)
	{
		double eps = 10e-8;
		
		double before_log = -1; //for first iter.
		double log = 0;
		double part = 1;
		unsigned int ni = 1;
		
		while(absolut(log - before_log) > eps)
		{
			part = part * ((x - 1)/x);
			
			before_log = log;
			log = log + part/ni;
			
			//printf("%d\n", ni);
			ni++;
		}
		
		return log;
	}
	
	return NAN;
}

double mypow(double x, double y)
{
	double eps = 10e-8;
	unsigned int ni = 1;
	
	double cf_pow = 1;
	double cf_part = 1;
	double cf_before_pow = 0;
	
	double pow = 1;
	double part = 1;
	double before_pow = 0;
	
	if (x == 0)
	{
		if (y == 0)
			return 1;
		else if (y < 0)
			return INFINITY;
		
		return 0;
	} else if((x < 0) && (isint(y)))
	{
		for(int i = 1; i <= y; i++)
			pow = pow * x;
		
		return pow;
	} else if((x < 0) && !(isint(y)))
		return NAN;
	else if(y == 0)
		return 1;
	
	while((absolut(cf_before_pow - cf_pow) > eps) && (absolut(before_pow - pow) > eps))
	{
		cf_before_pow = cf_pow;
		cf_part = cf_part * ((y * cfrac_log(x, 200)) / ni);
		cf_pow = cf_pow + cf_part;
		
		before_pow = pow;
		part = part * ((y * taylor_log(x, 200)) / ni);
		pow = pow + part;
		ni++;
	}
	
	if(absolut(cf_before_pow - cf_pow) <= eps)
	{
		printf("////Zlomkom\n");
		return cf_pow;
	} else
		printf("////Taylor\n");
		return pow;
	
}

//more effective with mylog
/*
double mypow(double x, double y)
{
	double eps = 10e-8;
	unsigned int ni = 1;
	
	double pow = 1;
	double part = 1;
	double before_pow = 0;
	
	while(absolut(before_pow - pow) > eps)
	{
		before_pow = pow;
		part = part * ((y * mylog(x)) / ni);
		pow = pow + part;
		ni++;
	}
	
	return pow;
 }
 */

double absolut(double x)
{
	if(x >= 0)
		return x;
	else
		return -x;
}

void help(void)
{
	printf("For log, use --log X N\n");
	printf("For pow, use --pow X N\n");
	printf("For mylog, use --mylog X Y N\n");
	printf("For mypow, use --mypow X Y N\n");
}

int isint(double x) //return 1 if x could be int number
{
	x = absolut(x);
	
	while(1)
	{
		if(x < 0)
			return 0;
		else if(x == 0)
			return 1;
		
		x = x - 1;
	}
}

int ispair(double x)
{
	x = absolut(x);
	
	if(((int)x % 2) == 0)
		return 1;
	else
		return 0;
}
