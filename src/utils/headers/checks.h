#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char* command_exists(const char *cmd);
extern char* get_admin_command();