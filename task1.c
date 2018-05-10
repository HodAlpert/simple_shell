#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
#include <malloc.h>
#include <wait.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "LineParser.h"
char pathName[PATH_MAX];


int getSize(char* array){
    int i=0;
    for(;array[i]!='\0';i++){

    }
    return i;
}
char* concat(char* first, char* second){
    int firstLength = getSize(first);
    int secondLength = getSize(second);
    char *result = calloc((size_t)firstLength+secondLength+1,1);
    int i=0;
    for(;i<firstLength;i++){
        result[i] = first[i];
    }
//    result[i] = ' ';
//    i++;
    int j=0;
    for(;i<firstLength+secondLength+1;i++,j++){
        result[i]=second[j];

    }
    result[i]='\0';
    return result;
}
void childProccess(cmdLine *pCmdLine){
    if (pCmdLine->inputRedirect!=NULL) {
        close(0);
        assert(dup2(fileno(fopen(pCmdLine->inputRedirect, "r")), 0) != -1);
    }
    if (pCmdLine->outputRedirect!=NULL) {
        close(1);
        assert(dup2(fileno(fopen(pCmdLine->outputRedirect, "w")), 1) != -1);
    }
    assert(execvp(pCmdLine->arguments[0],pCmdLine->arguments)!=-1);
}
void ParentProccess(int childId, cmdLine *pCmdLine){
    if (pCmdLine->blocking=='\1') {
        int status;
        assert(waitpid(childId, &status, 0)!=-1);
    }
}
void executeHelper(cmdLine *pCmdLine){
    int child_pid;
//    int childInput[2];
//    int childOutput[2];
//    assert(pipe(childInput)!=-1);
//    assert(pipe(childOutput)!=-1);
    switch (child_pid=fork()){
        case -1:
            perror("fork failed");
            exit(-1);
        case 0:
            childProccess(pCmdLine);
            break;
        default:
            ParentProccess(child_pid,pCmdLine);
    }
}

void execute(cmdLine *pCmdLine){
    for (;pCmdLine!=NULL;pCmdLine=pCmdLine->next){
        executeHelper(pCmdLine);
    }

}
    int main() {
    int i=1;
    while (i){
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
        execute(pCmdLine);
        freeCmdLines(pCmdLine);
    }
}