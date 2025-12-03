#include "headers/checks.h"

char* command_exists(const char *cmd) {
    if (access(cmd, X_OK) == 0) {
        return strdup(cmd);
    }

    char *path = getenv("PATH");
    if (!path) return NULL;

    char *path_copy = strdup(path);
    char *token = strtok(path_copy, ":");

    while (token) {
        char fullpath[4096];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", token, cmd);

        if (access(fullpath, X_OK) == 0) {
            free(path_copy);
            return strdup(fullpath);
        }

        token = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL;
}

char* get_admin_command() {
    char *cmd;
    cmd = command_exists("doas");
    if (cmd) return cmd;
    cmd = command_exists("sudo");
    if (cmd) return cmd;

    return NULL;
}