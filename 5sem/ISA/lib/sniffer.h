/**
 * Vysoké učení technické v Brne, Fakulta informačních technologií
 * Tomáš Lapšanský (xlapsa00)
 *
 * Projekt ISA - Nástroje monitorující a generující zprávy jednoduchých distance-vector protokolů
 * 2018/2019
 */

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

int sniffer(const char * dev);
void packet_handler(u_char * args, const struct pcap_pkthdr * header, const u_char * packet);
int packet_ether_handler(struct ether_header * ether);
int packet_ip_handler(const u_char * packet);
int packet_ipv6_handler(const u_char * packet);
int packet_udp_handler(struct udphdr * udp);
int packet_rip_handler(struct riphdr * rip, int rip_entry_cnt);
int packet_ripng_handler(struct riphdr * rip, int rip_entry_cnt);
