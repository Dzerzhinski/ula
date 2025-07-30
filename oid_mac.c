#include <ifaddrs.h>             // has getifaddrs()
#include <sys/socket.h>          // pretty sure I need it?
//#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <net/ethernet.h>
#include <linux/if_packet.h>     // defines struct sockaddr_ll
#include <string.h>              // memcpy()
//#include <strings.h>
#include <net/if.h>              // defines IF_NAMESIZE


#include "oid_mac.h"

/*
 * Convenience function, prints hexadecimal oid or mac with colon
 *    separators.  Takes pointer to oid/mac, length of oid/mac.
 */
void print_id(uint8_t * id, size_t len)
{
   if(id == NULL) {
      return;
   }

   printf("%x", (int)(id[0]));
   for(int i = 1; i < len; i++) {
      printf(":%x", (int)(id[i]));
   }
   return;
}


/*
 * Validate interface name length.  Interface name length must be 
 *    in range (0, IFNAMSIZ) excluding null terminator.  Return 
 *    length of the interface name string, including null terminator, 
 *    if valid.  Return 0 if invalid.
 */
size_t validate_if_name(char * name_if) 
{
   size_t name_len;
   if(name_if == NULL) {
      return 0;
   }

   name_len = strnlen(name_if, IF_NAMESIZE);
   if((name_len < 1) || (name_len >= IF_NAMESIZE)) {
      return 0;
   }
   return name_len + 1;
}
   

/*
 * Helper function.  Takes the name and type of an interface (e.g. "eth0", 
 *    AF_PACKET) and returns a sockaddr struct with the address.  Uses the 
 *    first matching interface returned by getifaddrs().  Returns NULL if 
 *    none found, or input parameters didn't pass basic validation check. 
 *    If successful, allocated memory is the responsibility of the caller.
 */
struct sockaddr * find_addr(char * name_if, sa_family_t family, size_t saddr_len)
{
   size_t name_len;
   struct ifaddrs * faces;
   struct sockaddr * saddr = NULL;

   // validate name length 0 < name < IFNAMSIZ
   // returned length includes null terminator
   name_len = validate_if_name(name_if);
   if(name_len == 0) {
      return NULL;
   }

   if(getifaddrs(&faces) != 0) {
      perror("Can't access network interfaces.\n");
      return NULL;
   }

   for(struct ifaddrs * ifa = faces; ifa != NULL; ifa = ifa->ifa_next) {
      // check interface address family
      if(ifa->ifa_addr->sa_family == family) {
         // check interface name
         if(strncmp(ifa->ifa_name, name_if, name_len) == 0) {
            // allocate memory and copy
            saddr = malloc(saddr_len);
            if(saddr == NULL) {
               perror("malloc error in find_addr");
               return NULL;
            }
            memcpy(saddr, ifa->ifa_addr, saddr_len);
            break;
         }
      }
   }
   freeifaddrs(faces);
   return saddr;
}

/*
 * Helper function.  Finds the MAC address for an interface, given the 
 *    interface name.  Returns NULL if none is found, or the interface 
 *    name fails to pass basic validation checks.  If successful, 
 *    allocated memory is the responsibility of the caller.
 */
uint8_t * find_mac_addr(char * name_if)
{
   uint8_t * mac;
   struct sockaddr_ll * saddr;

   saddr = (struct sockaddr_ll *)(find_addr(name_if, AF_PACKET, sizeof(struct sockaddr_ll)));
   if(saddr == NULL) {
      return NULL;
   }

   if(saddr->sll_halen != MAC_LEN) {
      return NULL;
   }

   mac = malloc(MAC_LEN);
   if(mac == NULL) {
      perror("find_macs: can't allocate memeory.");
      return NULL;
   }
   memset(mac, 0, MAC_LEN);

   memcpy(mac, saddr->sll_addr, MAC_LEN);

   free(saddr);
   return mac;
}

/*
 * Generates OID from MAC address, following RFC 3513. Returns NULL if
 *    passed a NULL pointer as input argument.  Allocates memory if
 *    successful, which must be freed by caller.
 */
uint8_t * oid_from_mac(uint8_t * mac)
{
   uint8_t * oid;

   if(mac == NULL) {
      fprintf(stderr, "Invalid mac passed to oid_from_mac\n");
      return NULL;
   }

   oid = malloc(OID_LEN);
   if(oid == NULL) {
      perror("Can't malloc in oid_from_mac");
      return NULL;
   }
   memset(oid, 0, OID_LEN);

   memcpy(oid, mac, 3);
   memcpy(oid + 5, mac + 3, 3);
   oid[0] = oid[0] | 0x01;
   oid[3] = 0xFF;
   oid[4] = 0xFE;

   return oid;
}

/*
 * Generates an OID from the MAC address of a named interface, per RFC 3513.
 *    Returns 0 if input argument is not valid, the interface can't be
 *    accessed, or there is an error.
 */
uint64_t generate_oid(char * name_if)
{
   uint8_t * mac;
   uint8_t * oid_ptr;
   uint64_t  oid;

   mac = find_mac_addr(name_if);
   if(mac == NULL) {
      return 0;
   }

   oid_ptr = oid_from_mac(mac);
   free(mac);
   if(oid_ptr != NULL) {
      memcpy(&oid, oid_ptr, OID_LEN);
      free(oid_ptr);
   } else {
      oid = 0;
   }
   
   return oid;
}


