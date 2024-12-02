/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"


/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

//todo

Statement* transfer(const std::string &str) {
  TokenScanner scanner;
  scanner.ignoreWhitespace();
  scanner.scanNumbers();
  scanner.setInput(str);
  std::string indicator, token;
  Statement *statement = nullptr;
  Expression *expr1 = nullptr, *expr2 = nullptr;
  indicator = scanner.nextToken();
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
        token = scanner.nextToken();
        statement = new InputStatement(token);
      } else {
        error("SYNTAX ERROR");
      }
    } catch(ErrorException &ex) {
      delete statement;
      throw ex;
    }
  }
  else if(indicator == "END") {
    return new EndStatement;
  }
  else if(indicator == "IF") {
    
  }
  else if(indicator == "GOTO") {
    
  }
  else error("SYNTAX MESSAGE");
}
