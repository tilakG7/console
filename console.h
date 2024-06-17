/** console.h
* Console class which allows for simultaneous input and output.
*
* The bottom of the console prompts the user for input while output is displayed
* above.
* More info in the README file.
* 
*/
#pragma once

#include <mutex>
#include <string>

using namespace std;

/**
* Singleton class to prompt and read information from the user.
* Also supports writing to the terminal.
* @note Singleton pattern ensures that only 1 object of this class exists. It
* can be accessed by the getInstance() function.
*/
class Console {
public:
    /** 
    * Initializes the Console application when called for the first time
    * @return a reference to the Console singleton object
    */
    static Console& getInstance();

    /**
    * Prompts the user and waits for user to input data until a new line 
    * character has been detected
    * @note This is a blocking function. It will not return until the user has
    * entered their message.
    * @param prompt - a message to prompt the user with. Ex: "Enter your age>"
    * @return the string the user inputted. Ex: "My age is 43"
    */
    string read(const string &prompt);

    /**
    * Writes data to the console, without overwriting the user prompt and user
    * input
    * @param s - string to write to the console
    */
    void write(const string &s);

    // Delete copy constructor, copy assignment and move assignment to support
    // Singleton pattern
    Console(Console &) = delete;
    Console& operator=(const Console &) = delete;
    Console&& operator=(Console &&) = delete;
private:

    // private constructor to support Singleton pattern
    Console();

    // Values for special ASCII characters
    static constexpr int kDelete = 0x7F;
    static constexpr int kNewLine = 0x0A;
    mutex m_console_mtx;   // mutex for ownership of shared member variables and cout
    string m_prompt;       // prompt to display to the user
    string m_user_input;   // stores what the user has typed so far
};