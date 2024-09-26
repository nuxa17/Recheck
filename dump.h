#ifndef HEADER_DUMP
#define HEADER_DUMP

class Dump {
public:
    Dump();
    Dump(const char*);
    ~Dump();

    char* get_file();
    void set_file(const char*);
    char* get_md5();
    void set_md5(const char*);
    char* get_sha1();
    void set_sha1(const char*);

private:
    char *file;
    char *md5;
    char *sha1;
};

#endif