#include <iostream>
#include <thread>
#include <chrono>


int main()
{
    using namespace std::chrono_literals;
    char srcString[] = { "Old String" };

    while (true) {
        std::cout << srcString << std::endl;
        std::this_thread::sleep_for(2s);
    }

    return 0;
}


