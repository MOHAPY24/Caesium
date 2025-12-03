#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


extern bool file_exists(const char *filename);
extern void SetupArgonDirs();
extern char* get_repo_name(const char *pkg);