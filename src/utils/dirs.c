#include "headers/dirs.h"

bool file_exists(const char *filename) {
    FILE *fp = fopen(filename, "r");
    bool is_exist = false;
    if (fp != NULL) {
        is_exist = true;
        fclose(fp);
    }
    else{
        is_exist = false;
        fclose(fp);
    }
    return is_exist;
}

void SetupArgonDirs() {
    system("mkdir /tmp/argon/builds");
}

#include <dirent.h>
#include <stdbool.h>

bool is_dir_empty(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        return true;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' &&
           (entry->d_name[1] == '\0' ||
            (entry->d_name[1] == '.' && entry->d_name[2] == '\0'))) {
            continue; // skip "." and ".."
        }

        closedir(dir);
        return false;
    }

    closedir(dir);
    return true; 
}


bool directory_exists(const char *path) {
    struct stat st;

    if (stat(path, &st) != 0) {
        return false;
    }

    return S_ISDIR(st.st_mode);
}


char* get_repo_name(const char *pkg) {
    const char *slash = strrchr(pkg, '/');
    const char *name = slash ? slash + 1 : pkg;

    size_t len = strlen(name);
    const char *suffix = ".git";
    size_t suffix_len = strlen(suffix);

    if (len > suffix_len && strcmp(name + len - suffix_len, suffix) == 0) {
        len -= suffix_len;
    }

    char *result = malloc(len + 1);
    if (!result) return NULL;

    memcpy(result, name, len);
    result[len] = '\0';

    return result; 
}