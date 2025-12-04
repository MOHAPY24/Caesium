#pragma once
#include "stdlib.h"
#include <string.h>
#include "src/cli/headers/cli.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

extern char **environ;

extern const char* getDomain(InstallArgs args);
extern int runCommand(const char* cmd);
extern char *extract_repo_path(const char *pkg);
extern bool handleExistingDir(const char* BuildDir);
extern bool has_dot(const char *s);
extern int cloneRepo(const InstallArgs *pkg, const char* branch, const char* BuildDir);
extern int applyPatches(const char* BuildDir, const char* patchesDir);
extern int buildWithMake(const char *BuildDir, const char *cflags, const char *libs);