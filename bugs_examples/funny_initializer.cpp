#include <stdio.h>
#include <iostream.h>

int main()
{
    char *buff = new char(500);

    std::cout << "Enter a string\n";
    std::cin >> buff;
    std::cout << "You entered: " << buff;
    return 1;
}
