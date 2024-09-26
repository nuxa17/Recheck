#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

#include "openssl/evp.h"

#include "pugixml.hpp"
#include "dump.h"
#include "hash.h"

const char* DAT_FOLDER = "dat"; // Maximum length = BUFF_LEN - 2  
const int BUFF_LEN = 1024;

void print_usage();
int find_game(Dump *dump, int s, char *name);
const char * find_dump_name(pugi::xml_node datafile, Dump *dump);

int main(int argc, char *argv[]){
    if (argc == 1){
        print_usage();
        exit(0);
    }

    bool rename = false;
    int i = 1; 
    
    //parse options
    if (strcmp(argv[1], "-r") == 0 || strcmp(argv[1], "--rename") == 0){
        rename = true;
        i++;
    }
    
    if (argc == i) {
        print_usage();
        exit(0);
    }
    
    Dump* dump = new Dump();
    char name[BUFF_LEN];

    for (i; i < argc; i++) {
        // TODO: check if it's a file or a folder
        dump->set_file(argv[i]);
        if (!hash_dump(dump)) {
            printf("The file %s has not been processed.\n", argv[i]);
            continue;
        }

        printf("MD5: %s\nSHA-1: %s\n", dump->get_md5(), dump->get_sha1());
        printf("Searching DATs...\n");

        switch(find_game(dump, BUFF_LEN, name)) {
            case 0:
                printf("Game not found.\n");
                break;
            case 1:
                printf("Name: %s\n", name);
                if (rename){
                    // TODO: rename
                }
                break;
            default:
                break;
        }
        
    }

    delete dump;
    return 0;
}

void print_usage(){
    printf("Usage: recheck.exe [Options...] [Paths...]\n\n"
        "(NOT IMPLEMENTED) If a directory is passed as argument, all the files will be checked (including folders recursively).\n\n"
        "Options:\n"
        "    --rename, -r\n"
        "        (NOT IMPLEMENTED) If the game is found, renames the files to match the DAT.\n");
}

/* Parameter 's' indicates the maximum length of 'name' (counting the terminating null).
Returns 1 if found, 0 if not found and -1 on error.*/
int find_game(Dump *dump, int s, char *name){
    pugi::xml_document doc;
    pugi::xml_parse_result result;

    DIR *dat_folder;
    if(!(dat_folder = opendir(DAT_FOLDER))){
        perror("Error opening 'dat' folder");
        return -1;
    }

    char path[BUFF_LEN];
    int path_len = snprintf(path, BUFF_LEN, "%s/", DAT_FOLDER);

    const char *buff;
    int found = 0;

    struct dirent *entry;
    while (entry = readdir(dat_folder)){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue;
        }

        if(BUFF_LEN < path_len + strlen(entry->d_name) + 1){
            printf("Warning(%s): Path length too large. Skipping...\n", entry->d_name);
            continue;
        }

        result = doc.load_file(strcat(path, entry->d_name));
        if (!result){
            printf("Warning(dats/%s): %s. Skipping...\n", entry->d_name, result.description());
            path[path_len] = '\0';
            continue;
        }
        
        if(buff = find_dump_name(doc.child("datafile"), dump)){
            if(strlen(buff) >= s){
                printf("Error: name '%s' too large.\n", buff);
            }
            else{
                snprintf(name, s, "%s", buff);
                found = 1;
                break;
            }
        }
        
        path[path_len] = '\0';
    }

    closedir(dat_folder);
    return found;
}

/* The returned name is stored dinamically, it should be copied!
If the game is not found, 'name' is set to NULL.
*/
const char * find_dump_name(pugi::xml_node datafile, Dump *dump){
    for (pugi::xml_node game = datafile.child("game"); game; game = game.next_sibling()){
        for (pugi::xml_node rom = game.child("rom"); rom; rom = rom.next_sibling("rom")){
            if(strcmp(rom.attribute("md5").value(), dump->get_md5()) == 0 &&
                strcmp(rom.attribute("sha1").value(), dump->get_sha1()) == 0)
            {
                return rom.attribute("name").value();
            }
        }
    }
    return NULL;
}
