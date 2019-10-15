#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

const int NUMBER_OF_SUBPROCESS = 3;

struct Process{
    pid_t pid;
    int type;
    int number;
    char file_name[20];
};
queue<Process> QUEUE;
void add_to_queue(pid_t pid, int type, int number, char* filename){
    Process pt1;
    pt1.pid = pid;
    pt1.type = type;
    pt1.number = number;
    strcpy(pt1.file_name, filename);
    QUEUE.push(pt1);
}
void create_process(int type, int number){
    char number_chr[10] = "", filename[20] = "", buf[20] = "";
    strcat(filename,"file_#");
    sprintf(buf,"%d",number);
    strcat(filename,buf);
    strcat(filename,".txt");

    pid_t proc_pid;
    switch(type){
        case 1:
            proc_pid = fork();
            cout << "process number #" << number << " has been created" << endl;
            if (proc_pid == 0){
                kill(getpid(), SIGSTOP);
                execl("a1", "a1", filename, "20", NULL);
                exit(0);
            }else if (proc_pid > 0){
                sleep(1);
                add_to_queue(proc_pid, type, number, filename);

                cout << "process of type " << type << ", what have number " << number << " has been pushed to queue" << endl;
            }
            break;
        case 2:
            proc_pid = fork();
            cout << "process number #" << number << " has been created" << endl;
            if (proc_pid == 0){
                kill(getpid(), SIGSTOP);
                execl("a2", "a2", filename, "20", NULL);
                exit(0);
            }else if (proc_pid == 1){
                sleep(1);

                add_to_queue(proc_pid, type, number, filename);
                cout << "process of type " << type << ", what have number " << number << " has been pushed to queue" << endl;
            }
            break;
        case 3:
            proc_pid = fork();
            cout << "process number #" << number << " has been created" << endl;
            if (proc_pid == 0){
                kill(getpid(), SIGSTOP);
                execl("a3", "a3", filename, "20", NULL);
                exit(0);
            }else if (proc_pid == 1){
                sleep(1);
                add_to_queue(proc_pid, type, number, filename);
                cout << "process of type " << type << ", what have number " << number << " has been pushed to queue" << endl;
            }
            break;
    }

}

int main(){
    int type_for_add_to_queue = 1;
    for (int i = 0; i < NUMBER_OF_SUBPROCESS; i++){
        create_process(1, i);
    }
    for (int i = 0; i < NUMBER_OF_SUBPROCESS; i++){
        kill(QUEUE.front().pid, SIGCONT);
        QUEUE.pop();
    }
    return 0;
}
