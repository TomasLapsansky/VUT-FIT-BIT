/**
 * Vysoké učení technické v Brne, Fakulta informačních technologií
 * Tomáš Lapšanský (xlapsa00)
 *
 * Projekt ISA - Nástroje monitorující a generující zprávy jednoduchých distance-vector protokolů
 * 2018/2019
 */

#include "sniffer.h"
#include "print.h"

/*  Returns
 * 0 - OK
 * 1 - Arguments error
 * 2 - Sniffing device error
 * 3 - Filter error
 * 4 - Invalid packet captured
 */

int main(int argc, char const * argv[])
{
	/* Variables */
	const char * dev = NULL;

	/* Arguments */
	if (argc == 3) {
		if (strcmp(argv[1], "-i") == 0) {
			dev = argv[2];
		} else {
			msg_stderr("Wrong parameter!");
            print_help_sniffer();
			return 1;
		}
	} else {
		msg_stderr("Wrong number of arguments!");
        print_help_sniffer();
		return 1;
	}

	/* Turning on sniffer */
	return sniffer(dev);

	/* return 0; */
}

/*  Source:
 * http://www.tcpdump.org/pcap.html
 **/
