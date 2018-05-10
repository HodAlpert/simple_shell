#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <fcntl.h>


void childProccess2(int* fd){
    assert(close(fd[1])!=-1);
    assert(dup2(fd[0],STDIN_FILENO)!=-1);
    assert(close(fd[0])!=-1);
    char* command[] = {"tail","-n","2", 0};
    assert(execvp("tail",command)!=-1);



}
void childProccess1(int* fd){
    assert(close(fd[0])!=-1);
    assert(dup2(fd[1],STDOUT_FILENO)!=-1);
    assert(close(fd[1])!=-1);
    char* command[3] = {"ls","-l",0};
    assert(execvp("ls",command)!=-1);
}
void ParentProccess1(int childId,int* fd){
    int child_pid2;
    switch (child_pid2=fork()){
        case -1:
            perror("fork2 failed");
            exit(-1);
        case 0:
            childProccess2(fd);
            break;
        default:
            assert(close(fd[0])!=-1);//line 7
            assert(close(fd[1])!=-1);//line 7
            assert(wait(NULL)!=-1);
            assert(wait(NULL)!=-1);
    }
}

int main() {
    int fd[2];
    assert(pipe(fd)!=-1);
    int child_pid1;
    switch (child_pid1=fork()){
        case -1:
            perror("fork1 failed");
            exit(-1);
        case 0:
            childProccess1(fd);
            break;
        default:
            ParentProccess1(child_pid1,fd);
    }

}
