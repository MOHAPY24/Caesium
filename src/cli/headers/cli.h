#pragma once
#include <stdbool.h>
#include <stdio.h>

typedef struct{
    char* Packages[1024];
    bool GitLab;
    bool Codeberg;
    bool Local;
    const char* Branch;
    const char* Patches;
    bool Yes; // Yes its called 'Yes'
    const char* PkgDeps;
} InstallArgs;

typedef enum {
    CommandInstall = 0,
    CommandUnkown = 1
} CommandType;