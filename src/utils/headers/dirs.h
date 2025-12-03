#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

extern bool file_exists(const char *filename);
extern void SetupArgonDirs();
extern char* get_repo_name(const char *pkg);
extern bool directory_exists(const char *path);
extern bool is_dir_empty(const char *path);