/**
 * Vysoké učení technické v Brne, Fakulta informačních technologií
 * Tomáš Lapšanský (xlapsa00)
 *
 * Projekt ISA - Nástroje monitorující a generující zprávy jednoduchých distance-vector protokolů
 * 2018/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef NET_HEADERS
#define NET_HEADERS
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>
#include <pcap.h>
#include <arpa/inet.h>
#endif

#ifndef RIP_H
#define RIP_H
#include "rip.h"
#endif

/* sniffer */

void print_help_sniffer(void);
void msg_stderr(char* message);
void print_mac(const u_int8_t * address);
void print_ip(const u_int32_t address);
void print_ipv6(struct in6_addr addr);

void print_packet_start(int count);
void print_ether_header(const struct ether_header * ether);
void print_ip_header(struct iphdr * ip_header);
void print_ipv6_header(struct ip6_hdr * ip_header);
void print_udp_header(struct udphdr * udp);

void print_rip_header(struct riphdr * rip);
void print_ripng_header(struct riphdr * rip);

void print_ripv1_entry(struct rip_entry * rip, int rip_entry_cnt);
void print_ripv2_entry(struct rip_entry * rip, int rip_entry_cnt);
void print_ripng_entry(struct ripng_entry * rip, int rip_entry_cnt);

/* response */
void print_help_response(void);
