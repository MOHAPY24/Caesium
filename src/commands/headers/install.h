#pragma once
#include "stdlib.h"
#include <string.h>
#include "src/cli/headers/cli.h"

extern const char* getDomain(InstallArgs args);
extern int runCommand(const char* cmd);
extern char *extract_repo_path(const char *pkg);
extern bool handleExistingDir(const char* BuildDir);
extern bool has_dot(const char *s);
extern int cloneRepo(InstallArgs pkg, const char* branch, const char* BuildDir);