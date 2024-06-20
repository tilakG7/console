/** console.cpp
* Defines functions to implement the Console class.
* More documentation on the functions in the header file.
*/
#include "console.h"

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;

Console& Console::getInstance() {
    static Console c;
    return c;
}

string Console::read(const string &prompt) {
    // set the prompt and write it to std::cout
    {
        unique_lock<mutex> lck{m_console_mtx}; // acquire mutex for access to std::cout and m_prompt
        m_prompt = prompt;
        std::cout << m_prompt;
    }

    while(true) {
        int next_char = getchar(); // wait for user to enter next character
    
        unique_lock<mutex> lck{m_console_mtx}; // acquire mutex for access to std::cout and member variables

        bool user_done_typing = false;
        switch(next_char) {
            case kDelete:
                // delete the last character the user typed, if any
                if(m_user_input.size()) {
                    m_user_input.pop_back();
                    // move cursor back one character
                    // overwrite character with empty space
                    // move cursor back one character again
                    std::cout << "\b \b"; 
                }
                break;
            case kNewLine:
                // the user hit "enter"
                user_done_typing = true;
                break;
            default:
                // store and display the character user typed
                m_user_input.append(1, static_cast<char>(next_char)); // append 1 next_char
                std::cout << static_cast<char>(next_char);
        }
        std::cout.flush(); // avoid buffering

        if(user_done_typing) {
            std::cout << std::endl;
            string to_return = m_user_input;
            // reset the prompt and user input strings
            m_user_input.clear();
            m_prompt.clear();
            return to_return;
        }
    }
}

void Console::write(const string &s) {
    unique_lock<mutex> lck(m_console_mtx); // acquire mutex for access to std::cout and m_prompt
    
    const size_t current_input_size = m_prompt.size() + m_user_input.size();
    
    // if a read() is not in progress, simply write the string to cout
    if(!current_input_size) {
        std::cout << s << std::endl;
        return;
    }

    // Since a read is in progress, the prompt and user input is being displayed
    // at the bottom of the terminal. We must erase the prompt and user input 
    // and write the string. Now that the string has been written, we can restore
    // the prompt and user input that we erased.
    // The erase and restoration of the user prompt and user input occurs 
    // extremely fast

                // move cursor to the start of the prompt and user input
    std::cout << '\r'  
                // overwrite the remaining line with empty character space
                << std::setfill(' ') 
                << std::setw(current_input_size) 
                << ""
                // move cursor to start of line again
                << '\r';
    std::cout << std::setw(0)
                // write the intended message
                << s
                << std::endl
                // write the user prompt and input again
                << m_prompt 
                << m_user_input
                << std::flush;
}

Console::Console() {
    struct termios old_attr;
    tcgetattr(STDIN_FILENO, &old_attr);
    struct termios new_attr = old_attr;
    // clear ICANON and ECHO flag
    // clearing ICANON bit - input from the user will be available immediately
    //                       after it is typed instead of being buffered
    // clearing ECHO bit   - input is NOT written back to the terminal 
    //                       automatically (will be done manually)
    new_attr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
}
    
