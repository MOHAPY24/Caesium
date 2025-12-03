#include "headers/cli.h"

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

bool handleExistingDir(buildDir string)