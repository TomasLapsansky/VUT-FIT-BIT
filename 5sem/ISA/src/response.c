/**
 * Vysoké učení technické v Brne, Fakulta informačních technologií
 * Tomáš Lapšanský (xlapsa00)
 *
 * Projekt ISA - Nástroje monitorující a generující zprávy jednoduchých distance-vector protokolů
 * 2018/2019
 */

#include "response.h"
#include "print.h"

/**
 * Parsing ipmask parameter to prefix and len
 */
void parse_ip(char *ipmask, struct in6_addr *prefix, int * len) {

	/* Setting for copy and working */
	char *ip_prefix = malloc(strlen(ipmask) + 1);
	char *ip_len = malloc(strlen(ipmask) + 1);

	/* Cleaning */
	bzero(ip_prefix, strlen(ipmask) + 1);
	bzero(ip_len, strlen(ipmask) + 1);
	bzero(prefix, sizeof(struct in6_addr));

	/* Copying */
	memcpy(ip_prefix, ipmask, strlen(ipmask));
	memcpy(ip_len, ipmask, strlen(ipmask));

	/* Cutting off prefix */
	char *len_parser = strchr(ip_len, '/');

	/* If len doesn't exist == 0 else len */
	if(len_parser == NULL) {
		*len = 0;
        inet_pton(AF_INET6, ip_prefix, prefix);
	} else {
		*len = strtol(len_parser + 1, NULL, 10);
        /* Cutting off prefix len */
        inet_pton(AF_INET6, strtok(ip_prefix, "/"), prefix);
	}

	/* Correcting */
	free(ip_prefix);
	free(ip_len);

}

/**
 * Setting up packet for sending
 */
char * set_packet(char *ipmask, int metric, char *nexthop, int tag) {

	struct riphdr rip_header;
	struct ripng_entry ripng, ripng_nexthop;

	/* Packet allocation */
	char * packet = malloc(PACKET_SIZE);

	/* Cleaning */
	bzero(packet, PACKET_SIZE);
	bzero(&rip_header, sizeof(struct riphdr));
	bzero(&ripng, sizeof(struct ripng_entry));
	bzero(&ripng_nexthop, sizeof(struct ripng_entry));

	/* Setting RIP header */
	rip_header.command = 2;
	rip_header.version = 1;
	//rip_header.mbz = 0;		CLEANED

	/* Setting RIP entry with nexthop */

		/* Parsing prefix and len nexthop */
		int len_nexthop;
		struct in6_addr prefix_nexthop;
		parse_ip(nexthop, &prefix_nexthop, &len_nexthop);

	memcpy(ripng_nexthop.prefix, &prefix_nexthop, sizeof(struct in6_addr));
	ripng_nexthop.tag = 0;
	ripng_nexthop.metric = 0xFF;
	ripng_nexthop.len = 0;

	/* Setting RIP entry */

		/* Parsing prefix and len */
		int len = 0;
		struct in6_addr prefix;
		parse_ip(ipmask, &prefix, &len);

	memcpy(ripng.prefix, &prefix, sizeof(struct in6_addr));
	ripng.tag = htons(tag);
	ripng.metric = metric;
	ripng.len = len;

	/* Copping to packet */
	memcpy(packet, &rip_header, sizeof(struct riphdr));
	memcpy(packet + sizeof(struct riphdr), &ripng_nexthop, sizeof(struct ripng_entry));
	memcpy(packet + sizeof(struct riphdr) + sizeof(struct ripng_entry), &ripng, sizeof(struct ripng_entry));

	/* Return packet */
	return packet;
}

/**
 * Main response function, setting up environement
 */
int response(char *interface, char *r, int m, char *n, int t) {

	char *packet;
	int s;
	struct sockaddr_in6 server_addr, client_addr;

	/* socket creation */
	s = socket(PF_INET6, SOCK_DGRAM, 0);
	if(s < 0) {
		perror("Socket open error\n");
		return 1;
	}

	/* Cleaning */
	bzero(&server_addr, sizeof(server_addr));
	bzero(&client_addr, sizeof(client_addr));

	/* Setting server details */
	server_addr.sin6_port = htons(521);
	server_addr.sin6_family = AF_INET6;
	inet_pton(AF_INET6, "FF02::9", &server_addr.sin6_addr);

	/* Setting client details */
	client_addr.sin6_port = htons(521);
	client_addr.sin6_family = AF_INET6;

	/* Creating packet to send*/
	packet = set_packet(r, m, n, t);

	/* Binding */
	if(bind(s, (struct sockaddr *) &client_addr, sizeof(client_addr)) < 0) {
		perror("Binding error\n");
		return 1;
	}
	setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, interface, strlen(interface));

    /* Source: http://man7.org/linux/man-pages/man3/if_nametoindex.3.html */
    unsigned int if_index = if_nametoindex(interface);
    setsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_IF, &if_index, sizeof(if_index));

    int hop = 255;
    setsockopt(s, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hop, sizeof(hop));

	/* Sending packet */
	if(sendto(s, packet, PACKET_SIZE, 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		perror("Send to error\n");
		return 1;
	}

	/* Save closing */
	free(packet);
	close(s);

	return 0;
}
