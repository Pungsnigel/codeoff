//
//  main.cpp
//  codeoff
//
//  Created by Piggy on 2014-02-12.
//  Copyright (c) 2014 Piggy. All rights reserved.
//

#include <unistd.h>
#include <iostream>

void startProcess(int *in, int *out, const char *cmd);

int main(int argc, const char * argv[])
{
    using namespace std;
    int toFirst[2],  fromFirst[2];
    int toSecond[2], fromSecond[2];
    
    if (argc != 3)      // User didn't specify enough arguments
    {
        cout << "Usage: " << argv[0] << " cmd1 cmd2"  << endl;
        exit(1);
    }
    
    if (pipe(toFirst) == -1 || pipe(fromFirst) == -1 || pipe(toSecond) == -1 || pipe(fromSecond) == -1)
    {
        cerr << "Failed to pipe";
        exit(EXIT_FAILURE);
    }
    
    switch(fork())
    {
        case -1: cerr << "Failed to fork";       break;              // error
        case 0: startProcess(toFirst, fromFirst, argv[1]); break;     // Child proccess
        default:                                                     // Main proccess
            switch(fork())
            {
                case -1: cerr << "Failed to fork";          break;  // error
                case 0: startProcess(toSecond, fromSecond, argv[2]);  break;  // child proccess
                default:                                                    // Main proccess
                    sleep(2);
                    break;
            }
            break;
    }
    
    
    return 0;
}

void startProcess(int *in, int *out,const char *cmd)
{
    dup2(in[0], 0);
    dup2(out[1], 1);
    close(in[1]);    // Child process should not read from input-stream of main
    close(out[0]);   // Child process should not write from output-stream of main
    execl(cmd,cmd, (char*) NULL);
}

