/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            getline(std::cin, input);
            if (input.empty())
                continue;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    std::string token;
    TokenType tokentype = STRING;
    if(scanner.hasMoreTokens()) {
      token = scanner.nextToken();
      tokentype = scanner.getTokenType(token);
    }
    else return;
    
    /*dealing with number-prefixed lines*/
    if(tokentype == NUMBER) {
      int lineNumber = stringToInteger(token);
      try {
        if(scanner.hasMoreTokens()) program.addSourceLine(lineNumber, line);
        else program.removeSourceLine(lineNumber);
      } catch(ErrorException &ex) {
        std::cout << ex.getMessage() << std::endl;
      }
    }

    /*dealing with special orders*/
    else if(token == "RUN") {
      try {
        program.run_the_program(state);
      } catch (ErrorException &ex) {
        std::cout << ex.getMessage() << std::endl;
      }
      return;
    }
    else if(token == "LIST") {
      int key = program.getFirstLineNumber();
      while(key != -1) {
        std::cout << program.getSourceLine(key) << std::endl;
        key = program.getNextLineNumber(key);
      }
      return;
    }
    else if(token == "CLEAR") {
      program.clear();
      state.Clear();
      return;
    }
    else if(token == "QUIT") {
      exit(0);
    }
    else if(token == "HELP") {
      std::cout << "Damn the BASIC Interpreter!!!" << std::endl;
      return;
    }

    /*dealing with other lines*/
    else if(tokentype == WORD) {
      if(token == "LET" || token == "PRINT" || token == "INPUT") {
        Statement* statement = transfer(line);
        statement -> execute(state, program);
        delete statement;
      }
      else {
        std::cout << "SYNTAX ERROR" << std::endl;
      }
      //SYNTAX ERROR
    }
    else std::cout << "SYNTAX ERROR" << std::endl;
    //todo
}
