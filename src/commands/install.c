#include "headers/install.h"
#include "src/utils/headers/dirs.h"
#include "src/utils/headers/checks.h"


const char* getDomain(InstallArgs args){
    if(args.Codeberg) {
        return "godeberg.org";

    }
    else if(args.GitLab) {
        return "gitlab.com";
    }
    else {
        return "github.com";
    }
}

int runCommand(const char* cmd){
    return system(cmd);
}

static bool has_dot(const char *s) { 
    return strchr(s, '.') != NULL;
}

char *extract_repo_path(const char *pkg) {
    const char *p = strstr(pkg, "://");
    if (p) {
        pkg = p + 3;
    }

    char *copy = strdup(pkg);
    if (!copy) return NULL;

    char *parts[256];
    int count = 0;

    char *tok = strtok(copy, "/");
    while (tok && count < 256) {
        parts[count++] = tok;
        tok = strtok(NULL, "/");
    }


    int start = 0;
    for (int i = 0; i < count; i++) {
        if (has_dot(parts[i])) {
            start = i + 1;
        }
    }

    size_t total_len = 0;
    for (int i = start; i < count; i++) {
        total_len += strlen(parts[i]) + 1;
    }

    if (total_len == 0) {
        free(copy);
        return strdup("");
    }

    char *result = malloc(total_len);
    if (!result) {
        free(copy);
        return NULL;
    }

    result[0] = '\0';
    for (int i = start; i < count; i++) {
        strcat(result, parts[i]);
        if (i + 1 < count) strcat(result, "/");
    }

    free(copy);
    return result;
}

bool handleExistingDir(const char* BuildDir){
    if(!directory_exists(BuildDir)){
        return false;
    }
    if(is_dir_empty(BuildDir)) {
        return false;
    }
    printf("\nBuild directory '%s' already exists.\n", BuildDir);
    printf("Choose an option:");
	printf("  1. Use existing directory");
	printf("  2. Remove directory and re-clone");
    printf("  3. Abort installation");
	printf("Choice [1-3]: ");
    const char* choice;
    scanf(choice);
    if (choice == '1'){
        printf("Using existing directory...");
        return true;
    }
    else if(choice == '2'){
        printf("Removing directory and re-cloning...");
        char cmdr[1024];
        snprintf(cmdr, sizeof(cmdr), "rm -rf '%s'", BuildDir);
        system(cmdr);
        return false;
    }
    else{
        printf("Installation aborted");
        return false;
    }

}

int cloneRepo(const InstallArgs *pkg, const char* branch, const char* BuildDir){
    const char* domain = getDomain(*pkg);
    const char* repo_path = extract_repo_path(pkg->Packages[0]);
    int cmd;
    if(branch != NULL && branch[0] != '\0'){
        cmd = snprintf(cmd, sizeof(cmd), "git clone --branch %s https://%s/%s %s", branch, domain, repo_path, BuildDir);
    }
    else{
        cmd = snprintf(cmd, sizeof(cmd), "git clone https://%s/%s %s", domain, repo_path, BuildDir);
    }
    free((void*)repo_path);
    return runCommand(cmd);

}

