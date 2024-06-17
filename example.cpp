#include <thread>
#include <string>
#include "console.h"

using namespace std;

// Periodically writes to console
// Note that writing to console will not disrupt the user if they are typing a message
void periodicLoggingTask() {
    Console &c = Console::getInstance();

    int count = 0;
    while(true) {
        c.write("Elapsed time = " + std::to_string(count++) + " seconds");
        std::this_thread::sleep_for(1s);
    }
}

// Request input from user and display what they typed
void getUserInput() {
    Console &c = Console::getInstance();
    while(true) {
        string user_input = c.read("Enter something > ");
        c.write("User entered: " + user_input);
    }
}

int main() {
    thread t1(periodicLoggingTask);
    thread t2(getUserInput);
    t1.join();
    t2.join();
    return 0;
}