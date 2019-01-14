/**
 * Vysoké učení technické v Brne, Fakulta informačních technologií
 * Tomáš Lapšanský (xlapsa00)
 *
 * Projekt ISA - Nástroje monitorující a generující zprávy jednoduchých distance-vector protokolů
 * 2018/2019
 */

#include "sniffer.h"
#include "print.h"

/**
 * Print message to stderr
 */
void msg_stderr(char * message) {

	if (message != NULL) {
		fprintf(stderr, "%s\n", message);
	}
}

/**
 * Print help for myripresponse
 */
void print_help_response(void) {
	fprintf(stdout, "Usage:\t./myripresponse -i <env> -r <IPv6>/[16-128] {-n <IPv6>} {-m [0-16]} {-t [0-65535]}\n");
}

/**
 * Print help for myripsniffer
 */
void print_help_sniffer(void) {
    fprintf(stdout, "Usage:\t./myripsniffer -i <env>\n");
}

/**
 * Print mac address in correct format
 */
void print_mac(const u_int8_t * address) {
	
	int i = 0;
	for (i = 0; i < ETH_ALEN; i++) {
		if (address[i] > 16) {
			fprintf(stdout, "%x", address[i]);
		} else {
			fprintf(stdout, "0%x", address[i]);
		}
		if (i != ETH_ALEN - 1)
			fprintf(stdout, ":");
	}
}

/**
 * Print ipv4
 */
void print_ip(const u_int32_t address) {

	struct in_addr ip;

	ip.s_addr = address;

	fprintf(stdout, "%s", inet_ntoa(ip));
}

/**
 * Print ipv6
 */
void print_ipv6(struct in6_addr addr) {

	/* source: http://www.qnx.com/developers/docs/6.5.0SP1.update/com.qnx.doc.neutrino_lib_ref/i/inet_ntop.html */

	char to_print[INET6_ADDRSTRLEN];

	inet_ntop(AF_INET6, addr.s6_addr32, to_print, sizeof(to_print));

	fprintf(stdout, "%s", to_print);

}

/**
 * Print start of packet to stdout
 */
void print_packet_start(int count) {

	if (count != 1) {
		fprintf(stdout, "\n-------------------------------\n");
		fprintf(stdout, "\n%d. packet:\n", count);
	} else {
		fprintf(stdout, "%d. packet:\n", count);
	}
}

/**
 * Print ethernet header
 */
void print_ether_header(const struct ether_header * ether) {

	fprintf(stdout, "\nETHERNET HEADER\n");
	fprintf(stdout, "src:\t\t");
	print_mac(ether->ether_shost);
	fprintf(stdout, "\n");
	fprintf(stdout, "dest:\t\t");
	print_mac(ether->ether_dhost);
	fprintf(stdout, "\n");
	fprintf(stdout, "type:\t\t0x%04x\n", ntohs(ether->ether_type));
}

/**
 * Print ip header
 */
void print_ip_header(struct iphdr * ip_header) {

	fprintf(stdout, "\nIP HEADER\n");
	fprintf(stdout, "version:\t%d\n", ip_header->version);
	fprintf(stdout, "src:\t\t");
	print_ip(ip_header->saddr);
	fprintf(stdout, "\n");
	fprintf(stdout, "dest:\t\t");
	print_ip(ip_header->daddr);
	fprintf(stdout, "\n");
}

/**
 * Print ipv6 header
 */
void print_ipv6_header(struct ip6_hdr * ip_header) {

	fprintf(stdout, "\nIP HEADER\n");
	fprintf(stdout, "version:\t%d\n", ntohl(ip_header->ip6_vfc) >> 28);
	fprintf(stdout, "src:\t\t");
	print_ipv6(ip_header->ip6_src);
	fprintf(stdout, "\n");
	fprintf(stdout, "dest:\t\t");
	print_ipv6(ip_header->ip6_dst);
	fprintf(stdout, "\n");
}

/**
 * Print udp header
 */
void print_udp_header(struct udphdr * udp) {

	fprintf(stdout, "\nUDP HEADER\n");
	fprintf(stdout, "src port:\t%d\n", ntohs(udp->source));
	fprintf(stdout, "dest port:\t%d\n", ntohs(udp->dest));
	fprintf(stdout, "len:\t\t%d\n", ntohs(udp->len));
	fprintf(stdout, "checksum:\t0x%x\n", ntohs(udp->check));
}

/**
 * Print rip header
 */
void print_rip_header(struct riphdr * rip) {

	fprintf(stdout, "\nRIP HEADER\n");
	switch (rip->command) {
		case 1:
			fprintf(stdout, "command:\trequest\n");
			break;
		case 2:
			fprintf(stdout, "command:\tresponse\n");
			break;
		/* RFC1058 page 19 */
		/* To be droped
		case 3:
			fprintf(stdout, "command:\ttraceon\n");
			break;
		case 4:
			fprintf(stdout, "command:\ttraceoff\n");
			break;
		case 5:
			fprintf(stdout, "command:\treserved\n");
			break;
		*/
		default:
			msg_stderr("Wrong RIP header command");
			return;
	}
	fprintf(stdout, "version:\tRIPv%d\n", rip->version);
}

/**
 * Print ripng header
 */
void print_ripng_header(struct riphdr * rip) {

	fprintf(stdout, "\nRIP HEADER\n");
	switch(rip->command) {
		case 1:
			fprintf(stdout, "command:\trequest\n");
			break;
		case 2:
			fprintf(stdout, "command:\tresponse\n");
			break;
		default:
			msg_stderr("Wrong RIPng header command");
			return;
	}
	fprintf(stdout, "version:\tRIPng\n");
}

/**
 * Print ripv1 entry
 */
void print_ripv1_entry(struct rip_entry * rip, int rip_entry_cnt) {

	struct rip_entry * rip_entry = rip;

	int i = 0;
	for (i = 0; i < rip_entry_cnt; i++) {

		if(i > 0) {
			rip_entry = (struct rip_entry *) ((u_char *)rip_entry + RIP_ENTRY_SIZE);
		} else {
			fprintf(stdout, "\t-----------------------\n");
		}

		switch (ntohs(rip_entry->type.entry.afi)) {
			case 0:
				fprintf(stdout, "\tAFI:\tunspecified\n");
				break;
			case 2:
				fprintf(stdout, "\tAFI:\tIP\n");
				break;
			default:
				msg_stderr("\tWrong AFI TODO");
				//return;
		}
		fprintf(stdout, "\tIP:\t");
		print_ip(rip_entry->type.entry.ip);
		fprintf(stdout, "\n");
		fprintf(stdout, "\tmetric:\t%d\n", ntohl(rip_entry->type.entry.metric));
		fprintf(stdout, "\t-----------------------\n");
	}
}

/**
 * Print ripv2 entry
 */
void print_ripv2_entry(struct rip_entry * rip, int rip_entry_cnt) {

	struct rip_entry * rip_entry = rip;
	bool auth = false;

	int i = 0;
	for (i = 0; i < rip_entry_cnt; i++) {

		if(i > 0) {
			rip_entry = (struct rip_entry *) ((u_char *)rip_entry + RIP_ENTRY_SIZE);
		} else {
			fprintf(stdout, "\t-----------------------\n");

			/* Authentication check, available only for the first packet */
			if(ntohs(rip_entry->type.auth.plain.afi) == 0xFFFF) {
				auth = true;
			}
		}

		if(!auth) {

			switch (ntohs(rip_entry->type.entry.afi)) {
				case 0:
					fprintf(stdout, "\tAFI:\tunspecified\n");
					break;
				case 2:
					fprintf(stdout, "\tAFI:\tIP\n");
					break;
				default:
					msg_stderr("\tWrong AFI TODO");
					return;
			}

			fprintf(stdout, "\ttag:\t%d\n", ntohs(rip_entry->type.entry.tag));
			fprintf(stdout, "\tIP:\t");
			print_ip(rip_entry->type.entry.ip);
			fprintf(stdout, "\n");
			fprintf(stdout, "\tmask:\t");
			print_ip(rip_entry->type.entry.mask);
			fprintf(stdout, "\n");
			fprintf(stdout, "\tn-hop:\t");
			print_ip(rip_entry->type.entry.nhop);
			fprintf(stdout, "\n");
			fprintf(stdout, "\tmetric:\t%d\n", ntohl(rip_entry->type.entry.metric));
			fprintf(stdout, "\t-----------------------\n");

		} else {

			switch(ntohs(rip_entry->type.auth.plain.type)) {
				case 0: /* unspecified */
					fprintf(stdout, "\tAFI:\t0xFFFF (authentication)\n");
					fprintf(stdout, "\ttype:\t%d\n", ntohs(rip_entry->type.auth.plain.type));
					break;
				case 2:	/* plain text */
					fprintf(stdout, "\tAFI:\t0xFFFF (authentication)\n");
					fprintf(stdout, "\ttype:\t%d\n", ntohs(rip_entry->type.auth.plain.type));
					fprintf(stdout, "\tmsg:\t%s\n", rip_entry->type.auth.plain.authentication);
					fprintf(stdout, "\t-----------------------\n");
					break;
				case 3: /* keyed message digest */
					fprintf(stdout, "\tAFI:\t0xFFFF (authentication)\n");
					fprintf(stdout, "\ttype:\t%d\n", ntohs(rip_entry->type.auth.md5.type));
					fprintf(stdout, "\toffset:\t%d\n", ntohs(rip_entry->type.auth.md5.lenght));
					fprintf(stdout, "\tid:\t%d\n", rip_entry->type.auth.md5.id);
					fprintf(stdout, "\td-len:\t%d\n", rip_entry->type.auth.md5.data_len);
					fprintf(stdout, "\tseq:\t%d\n", ntohl(rip_entry->type.auth.md5.sequence));

					struct rip_entry * auth_message = (struct rip_entry *) ((u_char *)rip_entry + RIP_ENTRY_SIZE*(rip_entry_cnt-1));
					rip_entry_cnt--;

					fprintf(stdout, "\tdata:\t");
					unsigned int j = 0;
					for(j = 0; j < 16; j++) {
						fprintf(stdout, "%02hhx", auth_message->type.auth.plain.authentication[j]);
					}
					fprintf(stdout, "\n\t-----------------------\n");

					break;
			}

			auth = false;
		}
	}
}

/**
 * Print ripng entry
 */
void print_ripng_entry(struct ripng_entry * rip, int rip_entry_cnt) {

	struct ripng_entry * ripng_entry = rip;
	struct in6_addr prefix_hdr;

	int i = 0;
	for(i = 0; i < rip_entry_cnt; i++ ){

		if(i > 0) {
			ripng_entry = (struct ripng_entry *) ((u_char *)ripng_entry + RIP_ENTRY_SIZE);
		} /* else {
			fprintf(stdout, "\t-----------------------\n");
		} Part of the complex output */

		memcpy(prefix_hdr.s6_addr, ripng_entry->prefix, 16);

		/* 	Complex output
		fprintf(stdout, "\tprefix:\t");
		print_ipv6(prefix_hdr);
		fprintf(stdout, "\n");
		fprintf(stdout, "\ttag:\t0x%04x\n", ntohs(ripng_entry->tag));
		fprintf(stdout, "\tlen:\t%d\n", ripng_entry->len);
		fprintf(stdout, "\tmetric:\t%d\n", ripng_entry->metric);
		fprintf(stdout, "\t-----------------------\n");
		*/

		/* Simplified output */
		fprintf(stdout, "\tIPv6 Prefix: ");
		print_ipv6(prefix_hdr);
		fprintf(stdout, "/%d ", ripng_entry->len);
		fprintf(stdout, "Metric: %d ", ripng_entry->metric);
		fprintf(stdout, "Tag: 0x%04x\n", ntohs(ripng_entry->tag));
	}
}
