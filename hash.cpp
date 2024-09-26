#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include "openssl/evp.h"

#include "hash.h"

bool hash_dump(Dump *dump){
    OpenSSL_add_all_algorithms();
    
    printf("Next file: %s.\n", dump->get_file());

    FILE *file = fopen(dump->get_file(), "rb");
    if (!file){
        perror("Error opening file.");
        return false;
    }

    printf("Hashing...\n");

    EVP_MD_CTX *mdctx_md5 = EVP_MD_CTX_new();
    EVP_MD_CTX *mdctx_sha1 = EVP_MD_CTX_new();
    const EVP_MD *md_md5;
    const EVP_MD *md_sha1;
    unsigned char hash_md5[EVP_MAX_MD_SIZE];
    unsigned char hash_sha1[EVP_MAX_MD_SIZE];
    unsigned int md5_len;
    unsigned int sha1_len;

    md_md5 = EVP_get_digestbyname("MD5");
    md_sha1 = EVP_get_digestbyname("SHA1");

    // TODO: Error management cleanup 

    if (!EVP_DigestInit_ex2(mdctx_md5, md_md5, NULL) ||
        !EVP_DigestInit_ex2(mdctx_sha1, md_sha1, NULL))
    {
        printf("Digest initialization failed.\n");
        EVP_MD_CTX_free(mdctx_md5);
        EVP_MD_CTX_free(mdctx_sha1);
        return false;
    }

    size_t buffer_size = 81920;
    char buffer[buffer_size];
    size_t count;
    
    while (!feof(file)) {
        count = fread(buffer, 1, buffer_size, file);
        if (ferror(file)) {
            fprintf(stderr, "fread() failed: %zu\n", count);

            EVP_MD_CTX_free(mdctx_md5);
            EVP_MD_CTX_free(mdctx_sha1);
            fclose(file);
            return false;
        }
        if (!EVP_DigestUpdate(mdctx_md5, buffer, count) ||
            !EVP_DigestUpdate(mdctx_sha1, buffer, count))
        {
            printf("Digest update failed.\n");

            EVP_MD_CTX_free(mdctx_md5);
            EVP_MD_CTX_free(mdctx_sha1);
            fclose(file);
            return false;
        }
    }

    fclose(file);

    if (!EVP_DigestFinal_ex(mdctx_md5, hash_md5, &md5_len) ||
        !EVP_DigestFinal_ex(mdctx_sha1, hash_sha1, &sha1_len))
    {
        printf("Digest finalization failed.\n");
        EVP_MD_CTX_free(mdctx_md5);
        EVP_MD_CTX_free(mdctx_sha1);
        return false;
    }

    EVP_MD_CTX_free(mdctx_md5);
    EVP_MD_CTX_free(mdctx_sha1);

    // 1 byte = 8 bits
    // 1 hex character = 4 bits
    // On each byte, there are 2 hex characters

    char hash[EVP_MAX_MD_SIZE*2 + 1];
    char *ptr = hash;

    for (int i = 0; i < md5_len; i++){
        sprintf(ptr, "%02x", hash_md5[i]);
        ptr = ptr + 2;
    }
    hash[md5_len*2] = '\0';
    dump->set_md5(hash);
    
    ptr = hash;
    for (int i = 0; i < sha1_len; i++){
        sprintf(ptr, "%02x", hash_sha1[i]);
        ptr = ptr + 2;
    }
    hash[sha1_len*2] = '\0';
    dump->set_sha1(hash);
    
    return true;
}