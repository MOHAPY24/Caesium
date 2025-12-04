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

int applyPatches(const char* BuildDir, const char* patchesDir){
    if(patchesDir == NULL){
        return NULL;
    }
    if(!directory_exists(patchesDir)){
        perror("patches directory does not exist.");
        return 0;
    }
    int cmd;
    cmd = snprintf(cmd, sizeof(cmd), "cd %s && for patch in %s/*.patch; do git apply \"$patch\"; done", BuildDir, patchesDir);
    return runCommand(cmd); 
}



int buildWithMake(const char *buildDir, const char *cflags, const char *libs) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        if (chdir(buildDir) != 0) {
            perror("chdir");
            exit(1);
        }

        int envCount = 0;
        while (environ[envCount]) envCount++;

        char **newEnv = malloc((envCount + 3) * sizeof(char *));
        if (!newEnv) {
            perror("malloc");
            exit(1);
        }

        for (int i = 0; i < envCount; i++) {
            newEnv[i] = environ[i];
        }

        int idx = envCount;

        if (cflags && cflags[0] != '\0') {
            size_t len = strlen(cflags) + 8;
            newEnv[idx] = malloc(len);
            snprintf(newEnv[idx++], len, "CFLAGS=%s", cflags);
        }

        if (libs && libs[0] != '\0') {
            size_t len = strlen(libs) + 9;
            newEnv[idx] = malloc(len);
            snprintf(newEnv[idx++], len, "LDFLAGS=%s", libs);
        }

        newEnv[idx] = NULL;

        char *argv[] = { "make", NULL };
        execvpe("make", argv, newEnv);
        perror("execvpe");
        exit(1);
    }

    int status = 0;
    waitpid(pid, &status, 0);

    return status;
}

int buildWithCargo(const char* buildDir) {
    char command[512];

    snprintf(command, sizeof(command),
             "cd \"%s\" && cargo build --release",
             buildDir);

    return runCommand(command);
}

int buildWithCMake(const char* buildDir) {
    char buildPath[512];
    char cmd[1024];

    snprintf(buildPath, sizeof(buildPath), "%s/build", buildDir);

    snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", buildPath);
    if(runCommand(cmd) != 0) {
        return -1;
    }

    snprintf(cmd, sizeof(cmd),
             "cd \"%s\" && cmake ..",
             buildPath);
    if(runCommand(cmd) != 0) {
        return -2;
    }

    snprintf(cmd, sizeof(cmd),
             "cd \"%s\" && make",
             buildPath);
    if(runCommand(cmd) != 0) {
        return -3;
    }

    return 0;
}

int buildWithConfigure(const char* buildDir) {
    char cmd[1024];

    snprintf(cmd, sizeof(cmd),
             "cd \"%s\" && ./configure",
             buildDir);

    if(runCommand(cmd) != 0) {
        return -1;
    }

    snprintf(cmd, sizeof(cmd),
             "cd \"%s\" && make",
             buildDir);

    if(runCommand(cmd) != 0) {
        return -2;
    }

    return 0;
}

int buildWithZig(const char* buildDir) {
    char cmd[1024];

    snprintf(cmd, sizeof(cmd),
             "cd \"%s\" && zig build",
             buildDir);

    return runCommand(cmd);
}

int buildWithShellScript(const char* buildDir) {
    char scriptPath[1024];
    char cmd[2048];

    snprintf(scriptPath, sizeof(scriptPath), "%s/build.sh", buildDir);

    if (chmod(scriptPath, 0755) != 0) {
        perror("chmod");
        return -1;
    }

    snprintf(cmd, sizeof(cmd), "cd \"%s\" && ./build.sh", buildDir);

    return runCommand(cmd);
}

