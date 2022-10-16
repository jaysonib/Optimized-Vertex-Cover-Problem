#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <limits.h>
#include <cstring>
#include <fstream>
#include <vector>

using namespace std;

int main (int argc, char **argv) {
    int s = 10;
    int n = 5;
    int l = 5;
    int c = 20;
    std::ifstream urandom("/dev/urandom");
    if (urandom.fail()) {
        cerr << "Error: cannot open /dev/urandom.";
        return 1;
    }

    for(int i=0;i<argc;i++){
        try{
            if(argv[i][0] == '-' && argv[i][1] == 's'){
                s = stoi(argv[++i]);
                if(s < 2){
                    cerr<<"Error: street numbers are invalid.";
                    return 2;
                }
            }
            else if(argv[i][0] == '-' && argv[i][1] == 'n'){
                n = stoi(argv[++i]);
                if(n < 1){
                    cerr<<"Error: line segments are not valid.";
                    return 3;
                }
            }
            else if(argv[i][0] == '-' && argv[i][1] == 'l'){
           l = stoi(argv[++i]);
                if(l<5){
                    cerr<<"Error: invalid wait time.";
                    return 4;
                }
            }
            else if(argv[i][0] == '-' && argv[i][1] == 'c'){
                c = stoi(argv[++i]);
                if(c<1){
                    cerr<<"Error: invalid c value.";
                    return 5;
                }
            }
        }catch(exception e){
            cerr<<"Error: occured in the stoi() coversion"<<endl;
            exit(0);
        }
    }


        vector <pid_t> kids;
        string line;
        //getline(cin,line);
        unsigned int num = 42;
        urandom.read((char *)&(num), sizeof(int));
        //l = (num% (l-4) + 5);

        int fd[2];
        int fd1[2];

        if(pipe(fd) == -1){
            cerr<<"Error: in creating pipe"<<endl;
            return 6;
        }
        if(pipe(fd1) == -1){
            cerr<<"Error: in creating pipe"<<endl;
            return 7;
        }


        pid_t pid1 = fork();
        if(pid1 < 0){
          cerr<<"Error: Fork is not being processed"<<endl;
            return 8;
        }
        if(pid1 == 0) {
            dup2(fd[1], STDOUT_FILENO);

            close(fd[0]);
            close(fd[1]);
            execv("./rgen",argv);
            //exit(0);
        }
        kids.push_back(pid1);
        pid_t pid2 = fork();
        if(pid2 < 0){
            cerr<<"Error: Fork2 is not being processed."<<endl;
            return 9;
        }
        if(pid2 == 0){
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            dup2(fd1[1],STDOUT_FILENO);
            close(fd1[0]);
            close(fd1[1]);
            execlp("python3","python3","ece650-a1.py","",NULL);
        }

        close(fd[0]);
        close(fd[1]);
        kids.push_back(pid2);
        pid_t pid3 = fork();
        string input = "";
        string input1 = "";
        if(pid3 < 0){
            cerr<<"Error: Fork2 is not being processed."<<endl;
            return 10;
        }
        if(pid3 == 0){


            dup2(fd1[1], STDOUT_FILENO);
            close(fd1[0]);
     close(fd1[1]);

            struct timeval tmo;
            fd_set readfds;
            l = (num% (l-4) + 5);

            /* wait only 5 seconds for user input */
            FD_ZERO(&readfds);
            FD_SET(0, &readfds);
            tmo.tv_sec = l;
            tmo.tv_usec = 0;

            switch (select(1, &readfds, NULL, NULL, &tmo)) {
                case -1:
                    break;
                case 0:
                    exit(0);
            }

            while(!cin.eof()){
                getline(cin, line);

                if(line == ""){  int status;
                 kill(getppid(),SIGTERM);
                waitpid(getppid(), &status, 0);break;}

                cout<<line<< endl;
             cout<<flush;
            }
        }
        kids.push_back(pid3);
        pid_t pid4 = fork();
        if(pid4 < 0){
            cerr<<"Error: Fork2 is not being processed."<<endl;
            return 12;
        }
        if(pid4 == 0){
            dup2(fd1[0],STDIN_FILENO);
            close(fd1[0]);
            close(fd1[1]);
            execv("./ece650-a2",argv);
        }
        kids.push_back(pid4);
        close(fd1[0]);
        close(fd1[1]);
        close(fd[0]);
        close(fd[1]);


        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        waitpid(pid3, NULL, 0);
        waitpid(pid4, NULL, 0);

        waitpid(-1,NULL, 0);

        for(pid_t k:kids){
            int status;
            kill(k,SIGTERM);
            waitpid(k, &status, 0);
        }


    return 0;
}

