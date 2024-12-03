/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"
#include "program.hpp"

/* Implementation of the Statement class */

int stringToInt(std::string str);

Statement::Statement() = default;

Statement::~Statement() = default;

//todo


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
  else error("SYNTAX ERROR");
}

/**/

RemStatement::RemStatement() {}
RemStatement::~RemStatement() {}
void RemStatement::execute(EvalState &state, Program &program) {
  program.nextCurrentLine();
}

/**/

LetStatement::LetStatement(TokenScanner &scanner) {
  //modify key words
  expr = parseExp(scanner);
}
LetStatement::~LetStatement() {
  delete expr;
}
void LetStatement::execute(EvalState &state, Program &program) {
  expr -> eval(state);
  program.nextCurrentLine();
}

/**/

PrintStatement::PrintStatement(Expression* _expr) {
  expr = _expr;
}
PrintStatement::~PrintStatement() {
  delete expr;
}
void PrintStatement::execute(EvalState &state, Program &program) {
  std::cout << expr -> eval(state) << std::endl;
  program.nextCurrentLine();
}

/**/

InputStatement::InputStatement(std::string _name) {
  name = _name;
}
void InputStatement::execute(EvalState &state, Program &program) {
  int Input;
  std::cin >> Input;
  std::cout << "  ?  " << std::endl;
  state.setValue(name, Input);
  program.nextCurrentLine();
}

/**/

IfStatement::IfStatement(Expression* _expr1, std::string _cmp, Expression* _expr2, int _gotoNumber) {
  expr1 = _expr1;
  expr2 = _expr2;
  cmp = _cmp;
  gotoNumber = _gotoNumber;
}
IfStatement::~IfStatement() {
  delete expr1;
  delete expr2;
}
void IfStatement::execute(EvalState &state, Program &program) {
  //to do
}

/**/

EndStatement::EndStatement() {}
void EndStatement::execute(EvalState &state, Program &program) {
  program.endProgram();
  program.nextCurrentLine();
}

/**/

GotoStatement::GotoStatement(int _lineNumber) {
  lineNumber = _lineNumber;
}
void GotoStatement::execute(EvalState &state, Program &program) {
  program.switchCurrentLine(lineNumber);
}