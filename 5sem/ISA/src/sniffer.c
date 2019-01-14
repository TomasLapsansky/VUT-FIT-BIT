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

/* Packet header from <pcap.h> used in packet_info
 * struct pcap_pkthdr {
 * struct timeval ts; // time stamp
 * bpf_u_int32 caplen; // length of portion present
 * bpf_u_int32 len; // length this packet (off wire)
 * };
 */

/**
 * Handler for every catched packet
 */
void packet_handler(u_char * args, const struct pcap_pkthdr * header, const u_char * packet) {

	(void) args;
	(void) header;

	static int count = 0; /* Packet counter */
	int ip = 0;
	int ip_size = 0;
	struct ether_header * ether = (struct ether_header *) packet;
	struct udphdr * udp;
	struct riphdr * rip;
	int rip_entry_cnt;

	count++;
	print_packet_start(count);

	ip = packet_ether_handler(ether);   /* calling ethernet handler, returns type of ip address */

	if(ip == 4) {

		if((ip_size = packet_ip_handler(packet)) <= 0) { /* calling ip handler, returns size for future shifting */
			return;
		}

        /* saving udp and rip header pointers */
		udp = (struct udphdr *) (packet + ETH_SIZE + ip_size);
		rip = (struct riphdr *) (packet + ETH_SIZE + ip_size + sizeof(struct udphdr));

		if(packet_udp_handler(udp) != 0) {  /* calling udp handler */
			return;
		}

		if(((ntohs(udp->uh_ulen) - sizeof(struct udphdr) - sizeof(struct riphdr)) % RIP_ENTRY_SIZE) != 0) {
			msg_stderr("Wrong size of RIP entries");
			return;
		}
        /* saving calculated number of rip entries */
		rip_entry_cnt = (ntohs(udp->uh_ulen) - sizeof(struct udphdr) - sizeof(struct riphdr))/RIP_ENTRY_SIZE;

		if(packet_rip_handler(rip, rip_entry_cnt) != 0) {
			return;
		}

	} else if(ip == 6) {

		if(packet_ipv6_handler(packet) != 0) { /* calling ipv6 handler */
			return;
		}

        /* saving udp and rip header pointers */
		udp = (struct udphdr *) (packet + ETH_SIZE + IPV6_SIZE);
		rip = (struct riphdr *) (packet + ETH_SIZE + IPV6_SIZE + sizeof(struct udphdr));

		if(packet_udp_handler(udp) != 0) {  /* calling udp handler */
			return;
		}

		if(((ntohs(udp->uh_ulen) - sizeof(struct udphdr) - sizeof(struct riphdr)) % RIP_ENTRY_SIZE) != 0) {
			msg_stderr("Wrong size of RIP entries");
			return;
		}
        /* saving calculated number of rip entries */
		rip_entry_cnt = (ntohs(udp->uh_ulen) - sizeof(struct udphdr) - sizeof(struct riphdr))/RIP_ENTRY_SIZE;

		if(packet_ripng_handler(rip, rip_entry_cnt) != 0) {
			return;
		}
	}

	return;
}

/**
 * Ethernet handler
 */
int packet_ether_handler(struct ether_header * ether) {

	int return_type = 0;

    // decides return value for future
	switch (ntohs(ether->ether_type)) {
		case ETHERTYPE_IP:
			return_type = 4;
			break;
		case ETHERTYPE_IPV6:
			return_type = 6;
			break;
		default:
			msg_stderr("Wrong IP type in ethernet header");
			return -1;
	}

	print_ether_header(ether);
	return return_type;
}

/**
 * IPv4 handler
 */
int packet_ip_handler(const u_char * packet) {

	struct iphdr * ip_header = (struct iphdr *) (packet + ETH_SIZE);

	/* ip_header->ihl*4 == size of IP header */
	int ip_size = ip_header->ihl * 4;
	if(ip_size < 20) {
		msg_stderr("Wrong IP header length");
		return -1;
	}

	print_ip_header(ip_header);

	return ip_size;
}

/**
 * IPv6 handler
 */
int packet_ipv6_handler(const u_char * packet) {

	struct ip6_hdr * ip_header = (struct ip6_hdr *) (packet + ETH_SIZE);

	print_ipv6_header(ip_header);

	return 0;
}

/**
 * UDP handler
 */
int packet_udp_handler(struct udphdr * udp) {

	print_udp_header(udp);

	return 0;
}

/**
 * RIP handler
 */
int packet_rip_handler(struct riphdr * rip, int rip_entry_cnt) {

	print_rip_header(rip);

    // decides the version of RIP
	if(rip->version == 1) {
		print_ripv1_entry((struct rip_entry *) ((u_char *)rip + sizeof(struct riphdr)), rip_entry_cnt);
	} else if(rip->version == 2) {
		print_ripv2_entry((struct rip_entry *) ((u_char *)rip + sizeof(struct riphdr)), rip_entry_cnt);
	} else {
		msg_stderr("Wrong RIP version");
		return -1;
	}

	return 0;
}

/**
 * RIPng handler
 */
int packet_ripng_handler(struct riphdr * rip, int rip_entry_cnt) {

	print_ripng_header(rip);

	if(rip->version == 1) {
		print_ripng_entry((struct ripng_entry *) ((u_char *)rip + sizeof(struct riphdr)), rip_entry_cnt);
	} else {
		msg_stderr("Wrong RIPng version");
		return -1;
	}

	return 0;
}

/**
 * Main function of sniffer
 */
int sniffer(const char * dev) {

	/* Variables */
	pcap_t * handler;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program fp;
	char filter_exp[] = "port 520 or port 521"; /* RIP and RIPng ports */
	//char filter_exp[] = "port 80";
	bpf_u_int32 mask;
	bpf_u_int32 ip;

	/* Getting IPmask for device */
	if (pcap_lookupnet(dev, &ip, &mask, errbuf) == -1) {
		msg_stderr("Setup error: Can't look for ip or netmask of device");
		return 2;
	}

	/* Opening device for pcap */
	handler = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);

	/* Testing device for pcap */
	//handler = pcap_open_offline("test/RIPv2-auth.pcap", errbuf);

	if (handler == NULL) { /* Can't open device */
		fprintf(stderr, "Setup error: Can't open device %s : %s\n", dev, errbuf);
		return 2;
	}

	/* Checking support of link-layer header */

	/*if(pcap_datalink(hanlder) != DLT_EN10MB) {
	 * fprintf(stderr, "Device doesn't provide ethernet headers\n");
	 * return 2;
	 * }*/

	/* Compiling filter for sniffing */
	if (pcap_compile(handler, &fp, filter_exp, 0, ip) == -1) {
		msg_stderr("Setup error: Can't compile filter");
		return 3;
	}

	/* Setting previous compiled filter */
	if (pcap_setfilter(handler, &fp) == -1) {
		msg_stderr("Setup error: Can't apply filter to handler");
		return 3;
	}

	pcap_loop(handler, -1, packet_handler, NULL);

	/* Closing connection and freeing memory */
	pcap_freecode(&fp);
	pcap_close(handler);

	return 0;
} /* sniffer */
