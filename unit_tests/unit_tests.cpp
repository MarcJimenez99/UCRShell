#include "gtest/gtest.h"
#include "input.h"
#include "command.h"
#include "FullCommand.h"
#include "semicolon.h"
#include "and.h"
#include "test.h"
#include "or.h"
#include "Base.hpp"
#include "connector.hpp"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

TEST(SingleCommand, CommandEcho) {
    char inputArray [] = "echo hello";
    Input* input = new Input(inputArray);
    FullCommand* command = new FullCommand();
    command->parse(0, 0, input);
    testing::internal::CaptureStdout();
    command->execute();
    string output = testing::internal::GetCapturedStdout();
    string expectedOutput = "hello\n";
    EXPECT_EQ(expectedOutput, output);
}

TEST(SingleCommand, CommandLS) {
    char inputArray [] = "ls -a";
    Input* input = new Input(inputArray);
    FullCommand* command = new FullCommand();
    command->parse(0,0,input);
    testing::internal::CaptureStdout();
    command->execute();
    string output = testing::internal::GetCapturedStdout();
    string expectedOutput = ".\n..\na.out\nbin\nCMakeCache.txt\nCMakeFiles\ncmake_install.cmake\nCMakeLists.txt\n.git\n.gitignore\n.gitmodules\ngoogletest\nimages\nintegration_tests\nlib\nMakefile\nnames.txt\nREADME.md\nrshell\nsrc\ntest\nunit_tests\n";
    EXPECT_EQ(expectedOutput, output);
}


TEST(SingleCommand, MKDirAndRM) {
    char inputArray [] = "mkdir hello";
    Input* input = new Input(inputArray);
    FullCommand* command = new FullCommand();
    command->parse(0, 0, input);
    command->execute();
    
    char input2Array [] = "ls -a";
    Input* inputCheck = new Input(input2Array);
    FullCommand* command2 = new FullCommand();
    command2->parse(0, 0, inputCheck);
    testing::internal::CaptureStdout();
    command2->execute();
    string output = testing::internal::GetCapturedStdout();
    string expectedOutput = ".\n..\na.out\nbin\nCMakeCache.txt\nCMakeFiles\ncmake_install.cmake\nCMakeLists.txt\n.git\n.gitignore\n.gitmodules\ngoogletest\nhello\nimages\nintegration_tests\nlib\nMakefile\nnames.txt\nREADME.md\nrshell\nsrc\ntest\nunit_tests\n";
    EXPECT_EQ(expectedOutput, output);
    
    char input3Array [] = "rm -rf hello";
    Input* input3 = new Input(input3Array);
    FullCommand* command3  = new FullCommand();
    command3->parse(0, 0, input3);
    command3->execute();
    
    testing::internal::CaptureStdout();
    command2->execute();
    string output2 = testing::internal::GetCapturedStdout();
    string expectedOutput2 = ".\n..\na.out\nbin\nCMakeCache.txt\nCMakeFiles\ncmake_install.cmake\nCMakeLists.txt\n.git\n.gitignore\n.gitmodules\ngoogletest\nimages\nintegration_tests\nlib\nMakefile\nnames.txt\nREADME.md\nrshell\nsrc\ntest\nunit_tests\n";
    EXPECT_EQ(expectedOutput2, output2); 
}

TEST(SingleCommand, WrongCommand) {
    char inputArray [] = "wrong hello";
    Input* input = new Input(inputArray);
    FullCommand* command = new FullCommand();
    command->parse(0, 0, input);
    testing::internal::CaptureStdout();
    command->execute();
    string output = testing::internal::GetCapturedStdout();
    string expectedOutput = "";
    EXPECT_EQ("", output);
}

TEST(MultipleCommand, ManyCommands) {
    char inputArray [] = "ls; echo hello && echo 1 || echo 2; echo 3";
    Input* input = new Input(inputArray);
    FullCommand* command = new FullCommand();
    command->parse(0, 0, input);
    testing::internal::CaptureStdout();
    command->execute();
    string output = testing::internal::GetCapturedStdout();
    string expectedOutput = "a.out\nbin\nCMakeCache.txt\nCMakeFiles\ncmake_install.cmake\nCMakeLists.txt\ngoogletest\nimages\nintegration_tests\nlib\nMakefile\nnames.txt\nREADME.md\nrshell\nsrc\ntest\nunit_tests\nhello\n1\n3\n";
    EXPECT_EQ(expectedOutput, output);
}

TEST(SingleCommand, Quotes) {
    char inputArray [] = "echo \"hello && world\"";
    Input* input = new Input(inputArray);
    FullCommand* command = new FullCommand();
    command->parse(0, 0, input);
    testing::internal::CaptureStdout();
    command->execute();
    string output = testing::internal::GetCapturedStdout();
    string expectedOutput = "hello && world\n";
    EXPECT_EQ(expectedOutput, output);
}
TEST(MultipleCommand, QuotesWithConnectors) {
    char inputArray [] = "echo \"hello && world\"; echo \"Basket && ball\"";
    Input* input = new Input(inputArray);
    FullCommand* execute = new FullCommand();
    execute->parse(0, 0, input);
    testing::internal::CaptureStdout();
    execute->execute();
    string output = testing::internal::GetCapturedStdout();
    string expectedOutput = "hello && world\nBasket && ball\n";
    EXPECT_EQ(expectedOutput, output);
}
TEST(MultipleCommand, QuotesWithConnectors2) {
	char inputArray [] = "echo \"hello || world\" || echo \"shouldn't print\" && echo \"also no print\"";
	Input* input = new Input(inputArray);
	FullCommand* execute = new FullCommand();
	execute->parse(0, 0, input);
	testing::internal::CaptureStdout();
	execute->execute();
	string output = testing::internal::GetCapturedStdout();
	string expectedOutput = "hello || world\n";
	EXPECT_EQ(expectedOutput, output);
}
TEST(SingleCommand, Comment) {
	char inputArray [] = "echo hello # world";
	Input* input = new Input(inputArray);
	FullCommand* execute = new FullCommand();
	execute->parse(0, 0, input);
	testing::internal::CaptureStdout();
	execute->execute();
	string output = testing::internal::GetCapturedStdout();
	string expectedOutput = "hello\n";
	EXPECT_EQ(expectedOutput, output);   
}
TEST(SingleCommand, CommentWithQuotes) {
	char inputArray [] = "echo \"hello # world\"";
	Input* input = new Input(inputArray);
	FullCommand* command = new FullCommand();
	command->parse(0, 0, input);
	testing::internal::CaptureStdout();
	command->execute();
	string output = testing::internal::GetCapturedStdout();
	string expectedOutput = "hello # world\n";
	EXPECT_EQ(expectedOutput, output);
}

TEST(test1, bracketE){
	char inputArray [] = "[ -e src/input.cpp ]";
	Input* input = new Input(inputArray);
	FullCommand* command = new FullCommand();
	command->parse(0,0,input);
	testing::internal::CaptureStdout();
	command->execute();
	string output = testing::internal::GetCapturedStdout();
	string expectedOutput = "(TRUE)\n";
	EXPECT_EQ(expectedOutput, output);
}
TEST(test2, bracketF){
	char inputArray [] = "[ -f src/stuff.cpp ]";
        Input* input = new Input(inputArray);
        FullCommand* command = new FullCommand();
        command->parse(0,0,input);
        testing::internal::CaptureStdout();
        command->execute();
        string output = testing::internal::GetCapturedStdout();
        string expectedOutput = "(FALSE)\n";
        EXPECT_EQ(expectedOutput, output);
}

TEST(testtwo, bracketD){
        char inputArray [] = "[ -d src ]";
        Input* input = new Input(inputArray);
        FullCommand* command = new FullCommand();
        command->parse(0,0,input);
        testing::internal::CaptureStdout();
        command->execute();
        string output = testing::internal::GetCapturedStdout();
        string expectedOutput = "(TRUE)\n";
        EXPECT_EQ(expectedOutput, output);
}
TEST(test3, wordnoFlag){
        char inputArray [] = "test src/Paren.cpp";
        Input* input = new Input(inputArray);
        FullCommand* command = new FullCommand();
        command->parse(0,0,input);
        testing::internal::CaptureStdout();
        command->execute();
        string output = testing::internal::GetCapturedStdout();
        string expectedOutput = "(TRUE)\n";
        EXPECT_EQ(expectedOutput, output);
}
TEST(test4, wordF){
        char inputArray [] = "test -f src/command.h";
        Input* input = new Input(inputArray);
        FullCommand* command = new FullCommand();
        command->parse(0,0,input);
        testing::internal::CaptureStdout();
        command->execute();
        string output = testing::internal::GetCapturedStdout();
        string expectedOutput = "(TRUE)\n";
        EXPECT_EQ(expectedOutput, output);
}
TEST(test5, wordE){
        char InputArray [] = "test -e src/main.cpp";
        Input* input = new Input(InputArray);
        FullCommand* command = new FullCommand();
        command->parse(0,0,input);
        testing::internal::CaptureStdout();
        command->execute();
        string output = testing::internal::GetCapturedStdout();
        string expectedOutput = "(TRUE)\n";
        EXPECT_EQ(expectedOutput, output);
}
TEST(test6, wordD){
	char InputArray [] = "test -d integration_tests";
	Input* input = new Input(InputArray);
	FullCommand* command = new FullCommand();
	command->parse(0,0,input);
	testing::internal::CaptureStdout();
	command->execute();
	string output = testing::internal::GetCapturedStdout();
	string expectedOutput = "(TRUE)\n";
	EXPECT_EQ(expectedOutput, output);
}
TEST(SingleCommand, SingleParen) {
	char inputArray [] = "(echo hello)";
	Input* input = new Input(inputArray);
        FullCommand* command = new FullCommand();
        command->parse(0,0,input);
        testing::internal::CaptureStdout();
        command->execute();
        string output = testing::internal::GetCapturedStdout();
        string expectedOutput = "hello\n";
        EXPECT_EQ(expectedOutput, output);	
}
TEST(SingleCommand, MultipleParen) {
	char inputArray [] = "(echo 1; (echo 1; echo 2))";
	Input* input = new Input(inputArray);
        FullCommand* command = new FullCommand();
        command->parse(0,0,input);
        testing::internal::CaptureStdout();
        command->execute();
        string output = testing::internal::GetCapturedStdout();
        string expectedOutput = "1\n1\n2\n";
        EXPECT_EQ(expectedOutput, output); 
}
TEST(SingleCommand, MultipleParen2) {
	char inputArray [] = "(echo 1 && (echo 2 || echo 3))";
        Input* input = new Input(inputArray);
        FullCommand* command = new FullCommand();
        command->parse(0,0,input);
        testing::internal::CaptureStdout();
        command->execute();
        string output = testing::internal::GetCapturedStdout();
        string expectedOutput = "1\n2\n";
        EXPECT_EQ(expectedOutput, output);	
}
TEST(SingleCommand, MultipleParen3) {
        char inputArray [] = "(echo 1 && (echo 2 || echo 3); (echo 4 && echo 5))";
        Input* input = new Input(inputArray);
        FullCommand* command = new FullCommand();
        command->parse(0,0,input);
        testing::internal::CaptureStdout();
        command->execute();
        string output = testing::internal::GetCapturedStdout();
        string expectedOutput = "1\n2\n4\n5\n";
        EXPECT_EQ(expectedOutput, output);
}


int main(int argc, char**argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
