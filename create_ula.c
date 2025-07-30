#include <openssl/evp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "oid_mac.h"
#include "ntp_unix.h"

#define KEY_LEN 16u
#define TIME_LEN 8u
//#define OID_LEN 8u
#define GID_LEN 40u

int main()
{

   uint8_t * key;
   uint8_t * hash;
   uint8_t * gid;
   uint64_t time, oid;


   time = get_ntp_time();
   if(time == 0) {
      fprintf(stderr, "Can't get time.\n");
      exit(EXIT_FAILURE);
   }
   oid = generate_oid("wlp1s0");
   if(oid == 0) {
      fprintf(stderr, "Can't generate oid.\n");
      exit(EXIT_FAILURE);
   }

   key = malloc(KEY_LEN);
   if(key == NULL) {
      perror("Can't malloc for key value");
      exit(EXIT_FAILURE);
   }

   memcpy(key, &time, TIME_LEN);
   memcpy(key + TIME_LEN, &oid, OID_LEN);

   EVP_MD_CTX * md_ctx = NULL;
   EVP_MD * sha1 = EVP_MD_fetch(NULL, "SHA1", NULL);
   if(sha1 == NULL) {
      fprintf(stderr, "Can't setup hash provider.\n");
      exit(EXIT_FAILURE);
   }

   md_ctx = EVP_MD_CTX_new();
   if(md_ctx == NULL) {
      fprintf(stderr, "Can't setup hash function.\n");
      exit(EXIT_FAILURE);
   }

   if(!(EVP_DigestInit(md_ctx, sha1))) {
      fprintf(stderr, "Can't setup hash function.\n");
      exit(EXIT_FAILURE);
   }

   if(!(EVP_DigestUpdate(md_ctx, key, KEY_LEN))) {
      fprintf(stderr, "Error computing hash.\n");
      exit(EXIT_FAILURE);
   }

   ssize_t hash_len = EVP_MD_CTX_get_size(md_ctx);
   if(hash_len < 1) {
      fprintf(stderr, "Error getting hash length\n");
      exit(EXIT_FAILURE);
   }

   hash = malloc(hash_len);
   if(hash == NULL) {
      perror("Error allocating hash.");
      exit(EXIT_FAILURE);
   }

   unsigned int final_hash_len;

   if(!(EVP_DigestFinal(md_ctx, hash, &final_hash_len))) {
      fprintf(stderr, "Error computing hash.\n");
      exit(EXIT_FAILURE);
   }

   if(final_hash_len != hash_len) {
      fprintf(stderr, "Hash buffer size error!\n");
      exit(EXIT_FAILURE);
   }

   free(key);
   EVP_MD_CTX_free(md_ctx);

   gid = malloc(GID_LEN);
   if(gid == NULL) {
      fprintf(stderr, "malloc error for global id\n");
      exit(EXIT_FAILURE);
   }

   size_t gid_offset = hash_len - GID_LEN;
   memcpy(gid, hash + gid_offset, GID_LEN);

   free(hash);

   printf("\n");
   printf("GID: ");
   printf("%x", (int)(gid[0]));
   for(int i = 1; i < GID_LEN; i++) {
      printf(":%x", gid[i]);
   }
   printf("\n");

   free(gid);

   exit(EXIT_SUCCESS);
}










