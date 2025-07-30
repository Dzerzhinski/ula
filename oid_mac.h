#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>

/*
#include <ifaddrs.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>
#include <string.h>
#include <strings.h>
#include <net/if.h>
*/

#define MAC_LEN  6u
#define OID_LEN  8u

/*
 * Convenience function, prints hexadecimal oid or mac with colon 
 *    separators.  Takes pointer to oid/mac, length of oid/mac.
 */
void print_id(uint8_t * id, size_t len);

/*
 * Validate interface name length.  Interface name length must be 
 *    in range (0, IFNAMSIZ) excluding null terminator.  Return 
 *    length of the interface name string, including null terminator, 
 *    if valid.  Return 0 if invalid.
 */
size_t validate_if_name(char * name_if); 

/*
 * Helper function.  Takes the name and type of an interface (e.g. "eth0", 
 *    AF_PACKET) and returns a sockaddr struct with the address.  Uses the 
 *    first matching interface returned by getifaddrs().  Returns NULL if 
 *    none found, or input parameters didn't pass basic validation check. 
 *    If successful, allocated memory is the responsibility of the caller.
 */
struct sockaddr * find_addr(char * name_if, sa_family_t family, size_t saddr_len);

/*
 * Helper function.  Finds the MAC address for an interface, given the 
 *    interface name.  Returns NULL if none is found, or the interface 
 *    name fails to pass basic validation checks.  If successful, 
 *    allocated memory is the responsibility of the caller.
 */
uint8_t * find_mac_addr(char * name_if);

/*
 * Generates OID from MAC address, following RFC 3513. Returns NULL if 
 *    passed a NULL pointer as input argument.  Allocates memory if 
 *    successful, which must be freed by caller.
 */
uint8_t * oid_from_mac(uint8_t * mac);


/*
 * Generates an OID from the MAC address of a named interface, per RFC 3513.
 *    Returns 0 if input argument is not valid, the interface can't be 
 *    accessed, or there is an error.  
 */
uint64_t generate_oid(char * name_if);


