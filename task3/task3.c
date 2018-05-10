#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <malloc.h>
#include <wait.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "LineParser.h"
pid_t parent;
char pathName[PATH_MAX];
void execute(cmdLine *pCmdLine,int* fdin);


void connectFdInPipe(int* fdin){
    if (fdin!=NULL) {
        assert(close(fdin[1]) != -1);
        assert(dup2(fdin[0], STDIN_FILENO) != -1);
        assert(close(fdin[0]) != -1);
    }
}
void connectFdOutPipe(int* fdout){
    if(fdout!=NULL){
        assert(close(fdout[0])!=-1);
        assert(dup2(fdout[1],STDOUT_FILENO)!=-1);
        assert(close(fdout[1])!=-1);
    }
}


void childProccess(cmdLine *pCmdLine, int* fdout){
    if (pCmdLine->inputRedirect!=NULL) {
        assert(close(0)!=-1);
        assert(dup2(fileno(fopen(pCmdLine->inputRedirect, "r")), 0) != -1);
    }
    if (pCmdLine->outputRedirect!=NULL) {
        assert(close(1)!=-1);
        assert(dup2(fileno(fopen(pCmdLine->outputRedirect, "w")), 1) != -1);
    }
    if(pCmdLine->next!=NULL) {
        execute(pCmdLine->next, fdout);
        connectFdOutPipe(fdout);
    }
    assert(execvp(pCmdLine->arguments[0],pCmdLine->arguments)!=-1);
}

void ParentProccess(int childId, cmdLine *pCmdLine,int* fdout){
    if (fdout!=NULL) {
        assert(close(fdout[0]) != -1);
        assert(close(fdout[1]) != -1);
    }
    if (parent==getpid()) {
        int status;
        assert(waitpid(childId, &status, 0)!=-1);
    }
}

void execute(cmdLine *pCmdLine,int* fdin){
    int fdout[2];
    if(pCmdLine->next!=NULL){
        assert(pipe(fdout)!=-1);
    }
    int child_pid;
    switch (child_pid=fork()){
        case -1:
            perror("fork failed");
            exit(-1);
        case 0:
            connectFdInPipe(fdin);
            childProccess(pCmdLine,(pCmdLine->next!=NULL)?fdout:NULL);
            break;
        default:
            ParentProccess(child_pid,pCmdLine,(pCmdLine->next!=NULL)?fdout:NULL);
    }

}
int main() {
    int i=1;
    while (i){
        parent = getpid();
        size_t size = PATH_MAX;
        assert(getcwd(pathName,size)!=NULL);
        printf("%s",pathName);
        printf(">");
        char line[2048];
        sync();
        fflush(stdin);
        assert(fgets(line,2048,stdin)!=NULL);
        cmdLine *pCmdLine;
        pCmdLine = parseCmdLines(line);
        if(strcmp(pCmdLine->arguments[0],"quit")==0){
            freeCmdLines(pCmdLine);
            break;
        }
        execute(pCmdLine,NULL);
        freeCmdLines(pCmdLine);
    }
}