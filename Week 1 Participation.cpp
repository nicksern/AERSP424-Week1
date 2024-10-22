#include <iostream>
int i;
int weird_func()
{
    i++;
    return 10;
}
int main()
{
    i = 0;
    i = i + 1 - weird_func();    
    std::cout << i << std::endl;
    i = 0;
    i = i - weird_func();
    std::cout << i << std::endl;
    i = 0;
    i -= weird_func();
    std::cout << i << std::endl;
    return 0;
}