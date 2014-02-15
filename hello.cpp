#include <iostream>
#include <string>
#include <unistd.h>

int main ()
{
    std::cerr << "In child process!" << std::endl;
    sleep(3);
    std::string msg;
    if(getline(std::cin,msg))
    {
        std::cout << "Child process read: " << msg;
    } else
    {
        std::cout << "Failed to read from input!" << std::endl;
    }
    std::cout.flush();
    std::cerr << "Exiting child process" << std::endl;
    return 0;
}
