# Console
## Description
The Console class allows for simultaneous input and output from a terminal. 

It allows 1 thread to read data from `std::cin` and another thread to output
data to `std::cout` without jumbling the 2 streams that are displayed to the 
user.

## Quick Example
Say you have 2 threads in a peer-to-peer messaging application:
1. Logger - logs incoming messages from other users
2. User Input thread - asks the user for a message to send to other users

Console class ensures the output from Logger thread does not interfere w/ the 
User Input thread. In the following example, note how a log arrives as the user
is inputting a message to respond to their friends. Sad.

<table>
<tr>
<td> <b>With</b> Console </td> <td> <b>Without Console</b> </td>
</tr>
<tr>
<td> 

```
Logger: Jenny: Hey! Are you in or out?
Logger: John : I'm in, hbu Will?

Enter message, Will> Yeah I'll be there!
```

</td>
<td>

```
Logger: Jenny: Hey! Are you in or out?

Enter message, Will> Ye
Logger: John : I'm in, hbu Will?
ah I'll be there!
```

</td>

</table>

Compile and run example.cpp for a more hands-on example.

## Manual

### Read from Console
`Console::getInstance().read(my_prompt)`
* Pass in a prompt to the `read` function to prompt the user for data
* Returns the string the user inputted after user hits Enter.

### Write to Console
`Console::getInstance().write(my_message)`
* Pass in a string to the `write` function to write the output to `std::cout`
