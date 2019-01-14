/**
 * Vysoké učení technické v Brne, Fakulta informačních technologií
 * Tomáš Lapšanský (xlapsa00)
 *
 * Projekt ISA - Nástroje monitorující a generující zprávy jednoduchých distance-vector protokolů
 * 2018/2019
 */

#include "response.h"
#include "sniffer.h"	/* for structures */
#include "print.h"

int main(int argc, char **argv) {

	int c;	/* flags */
	/* required args */
	char *i = NULL;
	char *r = NULL;
	/* implicit values */
	int m = 1;
	char *n = "::";
	int t = 0;

	while((c = getopt(argc, argv, "i:r:m:n:t:")) != -1) {

		switch (c) {
			case 'i':
				i = optarg;
				break;
			case 'r':
				r = optarg;
				break;
			case 'm':
				m = strtol(optarg, NULL, 10);
				break;
			case 'n':
				n = optarg;
				break;
			case 't':
				t = strtol(optarg, NULL, 10);
				break;
			default:
				print_help_response();
				return 1;
		}

	}

	if(i == NULL || r == NULL) {
		print_help_response();
		return 1;
	}

	return response(i, r, m, n, t);

	//return 0;
}
