/**
 * Vysoké učení technické v Brne, Fakulta informačních technologií
 * Tomáš Lapšanský (xlapsa00)
 *
 * Projekt ISA - Nástroje monitorující a generující zprávy jednoduchých distance-vector protokolů
 * 2018/2019
 */

#define ETH_SIZE 14			/* size of ethernet header for bit shifting */
#define RIP_ENTRY_SIZE 20	/* size of rip entry structure for shifting */
#define IPV6_SIZE 40		/* size of ipv6 header structure */

struct riphdr {
	u_int8_t command;
	u_int8_t version;
	u_int16_t mbz;
};

/* RIPv1 - RFC 1058 page 18
 * https://tools.ietf.org/html/rfc1058
 *
 * RIPv2 - RFC 2453 page 31
 * https://tools.ietf.org/html/rfc2453
 **/
struct rip_entry {
	union {
		/* RIPv1 struct
		struct {
			u_int16_t afi;
			u_int16_t mbz0;
			u_int32_t ip;
			u_int32_t mbz1;
		 	u_int32_t mbz2;
		 	u_int32_t metric;
		} ripv1;*/
		struct {
			u_int16_t afi;
			u_int16_t tag;
			u_int32_t ip;
			u_int32_t mask;
			u_int32_t nhop;
			u_int32_t metric;
		} entry;	/* ripv2 */
		union {
			struct {
				u_int16_t afi;
				u_int16_t type;
				char authentication[16];
			} plain;	/* also end of md5 */
			struct {
				u_int16_t afi;
				u_int16_t type;
				u_int16_t lenght;
				u_int8_t id;
				u_int8_t data_len;
				u_int32_t sequence;
				u_int32_t mbz0;
				u_int32_t mbz1;
			} md5;
		} auth;
	} type;
};

/* RIPng - RFC2080 page 5
 * https://tools.ietf.org/html/rfc2080
 **/
 struct ripng_entry {
	 u_int8_t prefix[16];
	 u_int16_t tag;
	 u_int8_t len;
	 u_int8_t metric;
 };
