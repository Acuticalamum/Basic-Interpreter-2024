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

/*trasfer is the function to change str into Statement* */
Statement* transfer(const std::string &str) {
  TokenScanner scanner;
  scanner.ignoreWhitespace();
  scanner.scanNumbers();
  scanner.setInput(str);
  std::string indicator, token;
  Statement *statement = nullptr;
  Expression *expr1 = nullptr, *expr2 = nullptr;
  indicator = scanner.nextToken();
  TokenType tokentype = scanner.getTokenType(indicator);
  if(tokentype == NUMBER) {
    indicator = scanner.nextToken();
  }
  if(indicator == "REM") {
    return new RemStatement;
  }
  else if(indicator == "LET") {
    try {
      statement = new LetStatement(scanner);
      return statement;
    } catch (ErrorException &ex) {
      delete statement;
      throw ex;
    }
  }
  else if(indicator == "PRINT") {
    try {
      expr1 = parseExp(scanner);
      statement = new PrintStatement(expr1);
      return statement;
    } catch (ErrorException &ex) {
      delete statement;
      throw ex;
    }
  }
  else if(indicator == "INPUT") {
    try {
      expr1 = parseExp(scanner);
      if(expr1 -> getType() == IDENTIFIER) {
        token = expr1 -> toString();
        statement = new InputStatement(token);
        delete expr1;
        return statement;
      } else {
        error("SYNTAX ERROR");
      }
    } catch(ErrorException &ex) {
      delete statement;
      delete expr1;
      throw ex;
    }
  }
  else if(indicator == "END") {
    return new EndStatement;
  }
  else if(indicator == "IF") {
    
  }
  else if(indicator == "GOTO") {
    token = scanner.nextToken();
    if(scanner.hasMoreTokens()) {
      error("SYNTAX ERROR");
    }
    try {
      int lineNumber = stringToInteger(token);
      statement = new GotoStatement(lineNumber);
      return statement;
    } catch (ErrorException &ex) {
      delete statement;
      throw ex;
    }
  }
  else error("SYNTAX ERROR");
}

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
        try {
          statement -> execute(state, program);
        } catch (ErrorException &ex) {
          std::cout << ex.getMessage() << std::endl;
        }
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
