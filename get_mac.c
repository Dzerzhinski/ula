#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "oid_mac.h"

int main()
{
   uint8_t * mac;
   uint64_t oid;
   char name_if[] = "wlp1s0";

   mac = find_mac_addr(name_if);
   printf("\n");
   printf("%s: ", name_if);
   print_id(mac, MAC_LEN);
   printf("\n");
   free(mac);

   oid = generate_oid(name_if);
   print_id((uint8_t *)(&oid), OID_LEN);
   printf("\n");
   
   

   /*
   fprintf(stderr, "starting main.\n");
   struct ifaddrs ** ifaces = malloc(sizeof(struct ifaddrs *));
   getifaddrs(ifaces);
   fprintf(stderr, "starting check\n");
   for(struct ifaddrs * ifa = *ifaces; ifa != NULL; ifa = ifa->ifa_next) {
      fprintf(stderr, "next\n");
      printf("%s: ", ifa->ifa_name);
      switch(ifa->ifa_addr->sa_family) {
         case AF_INET:
            printf("AF_INET ");
            printf("%s", inet_ntoa(((struct sockaddr_in *)(ifa->ifa_addr))->sin_addr));
            break;
         case AF_INET6:
            printf("AF_INET6");
            break;
         case AF_LLC: 
            printf("AF_LLC");
            printf("%x", (unsigned int)(ifa->ifa_addr->sa_data[0]));
            break;
         case AF_PACKET:
            printf("AF_PACKET: ");
            fprintf(stderr, "found af packet\n");
            struct sockaddr_ll * sock = (struct sockaddr_ll *)(ifa->ifa_addr);
            uint8_t len = sock->sll_halen;
            fprintf(stderr, "length: %d\n", (int)(len));
            for(int i = 0; i < len; i++) {
               fprintf(stderr, "%x:", (int)((sock->sll_addr)[i]));
            }
            fprintf(stderr, "\n");
                     
            uint8_t * foo = ifa->ifa_data;
            fprintf(stderr, "\n");
            for(int i = 0; i < 16; i++) {
               fprintf(stderr, "%02x:", (int)(((uint8_t*)(ifa->ifa_addr))[i]));
            }
            fprintf(stderr, "\n");

            if(foo != NULL) {
               printf("%x\n", (int)(*foo));
            }

            if(ifa->ifa_addr == NULL) {
               fprintf(stderr, "NULL sockaddr\n");
            }


            //size_t len = sock->sll_halen;
            fprintf(stderr, "%ld\n", len);
            if(len > 8) {
               fprintf(stderr, "mac address length wrong.");
               break;
            }
            if(len > 0) {
               printf("%x", sock->sll_addr[0]);
               for(int i = 1; i < len; i++) {
                  printf(":%x", sock->sll_addr[i]);
               }
            }
            break;
         default: 
            printf("Other: %d: ", ifa->ifa_addr->sa_family);
            printf("%x", ifa->ifa_addr->sa_data[0]);
      }

      printf("\n");
   }
   */
}
