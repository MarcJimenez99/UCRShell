# CS 100 Programming Project
 Winter 2019 
 
> Authors: Marc Jimenez (862049962) , Neha Gupta (862049507)

# Introduction
In this program our goal is to develop a command shell called rshell. This program will be able to take in an input from our user from standard output and execute the appropriate commands.

# Specifications

  1. Print a commad prompt (e.g. '$')
  2. Read in a line of commands(s) (and connectors(s)) from standard input
  3. Execute the appropriate commands through the use of __fork__, __execvp__, and __waitpid__
  4. Ability to interact with both quotes(" ") and the hashtag symbol(#) to alter the output of certain commands.
  5. Able to check for file and directory existence through the use of the test command and brackets
  6. The precedence operator () will change the precedence of the execution of commands
  7. Be able to handle input redirection, output redirection, and piping

## Connectors

Connectors will be implemented in our pattern through the use of three different classes known as **_and_**, **_or_**, and **_semicolon_** which all derive from the same class known as **_connector_**. 

## Method

The program will implement this through a composite pattern from which the classes known as **_FullCommand_**, **_connector_**, **_Paren_**, **_Test_**, **_MultCmd_**, **_command_** , **_SymbolTree_** are derived from a shared base interface class known as **_base_**. There are six classes which derive from **_connector_** called **_and_** , **_or_** , **_semicolon_** , **_MoreInput_** , **_LessInput_** , **_pipe_** , **_doubleOut_** . We have an additional class called **_input_** which stores the client's input and parses through the char* array. Our client will first create a new **_input_** object and call the function known as _getInput()_. They will be then prompted to input a command that we will then tokenize using _strtok()_ into two seperate vectors encapsulated in the **_input_** class that holds the commmands and one that holds the connectors. From there they will create a new **_FullCommand_** object that they will immediately call two functions on it, known as _parse()_ and _execute_. This will take in the vectors from the object **_input_** and create two new vectors of type **_base_** pointer that holds **_commands_** and **_connectors_**. From there the client will finally call execute on their **_FullCommand_** object and in turn execute the correct output using both vectors. In our program when our user's input consists of a <,>,|,>> a **_SymbolTree_** is created to direct output or recieve input from a file. 

# Diagram

![sample UML Diagram](https://github.com/MarcJimenez99/UCRShell/blob/master/images/Assignment4Uml.jpeg)

# Parsing

![Parse](https://github.com/MarcJimenez99/UCRShell/blob/master/images/parse.jpeg)

# Classes

Our class group is **_FullCommand_** , **_connector_**, **_command_**, **_Paren_**, **_Test_**, **_MultCmd_** which all inherit from the single base class of **_base_**. On top of that we have three classes that derive from the **_connector_** class called **_and_**, **_semicolon_**, and **_or_**. 

The base class **_base_** will carry functions to help us identify whether the client has included comments which are preceded by hastags, which indicate comments must be ignored. The base class encapsulates a bool function called flag which is used in other classes to indicate how and when to implement the connectors. 

The **_input_** class will have private a char* array, two string type vectors called vectorInput and Connectors. Along with two functions called populateVector and populateConnectors. In this class through parsing we split our input array into two seperate vectors one for commands and input, and the other for connectors. 
 
The **_FullCommand_** will have two private vectors type Base* called commandLine and connectors. In addition, to the functions held by the Base class, the execute class also implements functions populateExecute and populateExecuteConnectors of type void. 

The class **_command_** will hold an encapsulated vector of type string named singleCommand which is iterating through the client's input and searching for hashtags. The command class is responsible for testing whether the user's commands are exectuable. The commands are exectued based on the syscalls waitpid, fork, and execvp. After each syscall perror is called in any case of an error in execution. The final part is the exit call which exits our program if called upon by the client.

The class **_connector_** holds a protected base pointer named _first_ and _next_ . The base pointer first will point to the first command located directly to the left of the connector in the command line. The base pointer next will point to the command after the connector unless no command exists there. 

The class **_and_** is derived from the class Connector which is derived from Base. So the and class functions replicate the ones that exist in the Base class. The flags return true or false to indicate whether the command can be executed. If the connector is a "&&" then the next command is executed only if first exceeds. 

The class **_or_** is another class derived from Connector. The execute function in the or class returns a true or false to indicate whether the command should execute. If its a "||" then the next command is executed only if the first one fails.

The class **_semicolon_** derived from Connector always executes the next command in the input. 

The class **_test_** derived from Base checks whether the file or directory exists based on the flag and returns a true or false. 

The class **_Paren_** derived from Base changes the precedence of the execution of commands,connectors, and grouped commands.

The class **_MultCmd_** has a vector of type Base* called CommandLine which identifies whether the input has multiple commands. This class derives from the Base class and has functionality which keeps tracks of what is inside the precedence operator (). 

The class **_MoreInput_** has the redirection (>) symbol and appends input to the file and if something already exists it replaces its contents.

The class **_LessOutput_**  has the redirection (<) symbol and uses the input from a file as input instead of the keyboard.

The class **_pipe_** connects the standard input of one command to the standard input of another.

**_SymbolTree_** class builds a tree and performs the input and output redirection, and pipe if any of those exist in the command line. 

**_doubleOut_** class has the symbol (>>) which appends to a file rather than replace the contents it adds to what already exists.

# Test Command

In the test command it should be able to work with the command test and the brackets,[]. The flags -e, -f, -d check if there exists a directory, file, or both. If there exists no flag then it defaults to -e. The command should return true or false based on the output. We used the stat() function, S_ISDIR and S_ISREG to work with full directory paths and relative directory paths. The Test command should combine with other connectors and functionailty successfully. 

$ test -e src/input.cpp ==  $ test src/input.cpp -> returns (TRUE)

$ test -f stuff.cpp == $ [ -f stuff.cpp ] -> returns (FALSE)

# Precedence Operators

The precedence operators are parentheses ( ) which are used to change the precedence of the execution of commands. There can also be nested parentheses along with multiple sets. The rshell should also be able to recognize uneven parentheses and print an error message. 

$ (echo 2 && echo 3) || echo 5 

$ ((echo 4 || echo 2) && echo 6) 

$ ((((echo hello ))) - uneven set of parentheses


# Prototype/Research

```C++
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string>

using namespace std;

int main() {
        pid_t childpid = fork();
        int counter = 0;
        if (childpid == 0)  {
                waitpid(childpid, NULL, WNOHANG);
                cout << "We in the child process" << endl;
                char *cmd = "echo";
                char *argv[4];
                argv[0] = "echo";
                argv[1] = "hello";
                argv[2] = "world";
                argv[3] = NULL;
                execvp(cmd, argv);
        }
        else if (childpid > 0) {
                cout << "We in the parent process" << endl;
                waitpid(childpid, NULL, WNOHANG);
                counter++;
        }
        else {
                perror("fork() failed");
                return 1;
        }
        return 0;
}
```
fork()

We found that fork allows us to clone our current parent process creating a child process. As a clone this process will hold all of the same variables and data as the parent process, but it allows us to run a different branch of the program or even execute a different program. The child process will also run in parallel with the parent process allowing any changes to stay within the child.

waitpid()

We found that waitpid() allows our parent process to stop allowing it to wait for the child process to eventually terminate. This allows us to avoid zombie children processes that eat away at our memory eventually crashing our programs.

execvp()

We found that execvp() will be how we execute commands in our shell. It takes in two arguments first being the file we wish to execute and then an array of null-terminated strings that represent the appropriate arguments to the file. 

file descriptor

It is a number which identifies a file. The first three file descriptors are set by default at stdin(0), stdout(1), and stderr(2).

dup2()

This command is a system call which duplicates one file descriptor, and then deletes the old file descriptor.

pipe()

It is a form of redirection which sends the output of one program to another program


# Development and Testing Roadmap

1. Create base class **_base_** : Neha Gupta
2. Create derived class **_FullCommand_** : Marc Jimenez
3. Create derived class **_connector_** : Marc Jimenez
4. Create derived class **_command_** : Marc Jimenez
5. Write unit test single_command_tests.sh : Marc Jimenez
6. Write unit test multiple_command_tests.sh : Marc Jimenez 
7. Write unit test commented_command_tests.sh : Neha Gupta
8. Write unit test exit_command_tests.sh : Neha Gupta
9. Write unit test test_command_tests.sh : Marc Jimenez
10. Write unit test_symbolic_tests.sh : Neha Gupta
11. Write precedence_tests.sh : Marc Jimenez
12. Write input_redirection_tests.sh : Neha Gupta
13. Write output_redirection_tests.sh : Marc Jimenez
14. Write pipe_operator_tests.sh : Neha Gupta







