//
//  main.cpp
//  codeoff
//
//  Created by Piggy on 2014-02-12.
//  Copyright (c) 2014 Piggy. All rights reserved.
//

#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdlib>

void startProcess(int *in, int *out, const char *cmd);

int main(int argc, const char * argv[])
{
    int toFirst[2],  fromFirst[2];
    int toSecond[2], fromSecond[2];
    
    if (argc != 3)      // User didn't specify enough arguments
    {
        std::cout << "Usage: " << argv[0] << " cmd1 cmd2"  << std::endl;
        exit(1);
    }
    
    if (pipe(toFirst) == -1 || pipe(fromFirst) == -1 || pipe(toSecond) == -1 || pipe(fromSecond) == -1)
    {
        std::cerr << "Failed to pipe";
        exit(EXIT_FAILURE);
    }
    
    startProcess(toFirst, fromFirst, argv[1]);
    startProcess(toSecond, fromSecond, argv[2]);

    const char *msg = "Hello from Main program! \n";
    char buff [1024];
    std::cout << "Writing to external programs" << std::endl;
    write(toFirst[1], msg, strlen(msg));
    write(toSecond[1], msg, strlen(msg));
    sleep(4);
    read(fromFirst[0], buff, 1024);
    std::cout << "Main prog read: " << buff << "from first process" << std::endl;
    read(fromSecond[0], buff, 1024);
    std::cout << "Main prog read: " << buff << "from second process" << std::endl;
    std::cout <<"Bertil" << std::endl;
    return 0;
}

void startProcess(int *in, int *out,const char *cmd)
{
    switch(fork())
    {
    case -1:
        std::cerr << "Failed to fork";
        break;
    case 0:
        dup2(in[0], 0);
        dup2(out[1], 1);
        close(in[1]);    // Child process should not read from input-stream of main
        close(out[0]);   // Child process should not write from output-stream of main
        execl("/bin/sh","sh","-c",cmd,NULL);
        std::cerr << "Something went wrong in excecuting command: " << cmd << std::endl;
        break;
    default:
        return;
    }
}

