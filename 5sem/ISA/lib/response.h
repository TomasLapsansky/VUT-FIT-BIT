/**
 * Vysoké učení technické v Brne, Fakulta informačních technologií
 * Tomáš Lapšanský (xlapsa00)
 *
 * Projekt ISA - Nástroje monitorující a generující zprávy jednoduchých distance-vector protokolů
 * 2018/2019
 */

#include <unistd.h>	/* getopt */
#include <stdlib.h>	/* string convert */

#ifndef RIP_H
#define RIP_H
#include "rip.h"
#endif

#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>

#define PACKET_SIZE sizeof(struct riphdr) + 2 * sizeof(struct ripng_entry)

int response(char *interface, char *r, int m, char *n, int t);
char * set_packet(char *ipmask, int metric, char *nexthop, int tag);
void parse_ip(char *ipmask, struct in6_addr * prefix, int * len);
