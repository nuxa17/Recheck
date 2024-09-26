#include <string.h>
#include <stdlib.h>
#include "dump.h"

Dump::Dump(){
    file = NULL;
    md5 = NULL;
    sha1 = NULL;
}

Dump::Dump(const char* f){
    file = (char*) malloc(strlen(f) + 1);
    strcpy(file, f);
    md5 = NULL;
    sha1 = NULL;
}

Dump::~Dump(){
    free(file);
    free(md5);
    free(sha1);
}

char* Dump::get_file(){
    return file;
}

void Dump::set_file(const char* f){
    free(file);
    file = (char*) malloc(strlen(f) + 1);
    strcpy(file, f);
}

char* Dump::get_md5(){
    return md5;
}

void Dump::set_md5(const char* m){
    free(md5);
    md5 = (char*) malloc(strlen(m) + 1);
    strcpy(md5, m);
}

char* Dump::get_sha1(){
    return sha1;
}

void Dump::set_sha1(const char* s){
    free(sha1);
    sha1 = (char*) malloc(strlen(s) + 1);
    strcpy(sha1, s);
}
