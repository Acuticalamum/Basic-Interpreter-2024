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

/**/

RemStatement::RemStatement() {}
RemStatement::~RemStatement() {}
void RemStatement::execute(EvalState &state, Program &program) {
  program.nextCurrentLine();
}

/**/

LetStatement::LetStatement(TokenScanner &scanner) {
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
  std::string Input;
  std::cout << " ? ";
  getline(std::cin, Input);
  for(int i = 0; i < Input.size(); i++) {
    if(i == 0 && Input[0] == '-' && Input.size() > 1) continue;
    if(Input[i] < '0' || Input[i] > '9') {
      std::cout << "INVALID NUMBER" << std::endl;
      execute(state, program);
      return;
    }
  }
  int val = stringToInteger(Input);
  state.setValue(name, val);
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
  try {
    int lval = expr1 -> eval(state);
    int rval = expr2 -> eval(state);
    if(cmp == "<") {
      if(lval < rval) {
        try {
          program.switchCurrentLine(gotoNumber);
        } catch (ErrorException &ex) {
          throw ex;
        }
      }
      else {
        program.nextCurrentLine();
      }
    }
    if(cmp == ">") {
      if(lval > rval) {
        try {
          program.switchCurrentLine(gotoNumber);
        } catch (ErrorException &ex) {
          throw ex;
        }
      }
      else {
        program.nextCurrentLine();
      }
    }
    if(cmp == "=") {
      if(lval == rval) {
        try {
          program.switchCurrentLine(gotoNumber);
        } catch (ErrorException &ex) {
          throw ex;
        }
      }
      else {
        program.nextCurrentLine();
      }
    }
  } catch (ErrorException &ex) {
    throw ex;
  }
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
  try {
    program.switchCurrentLine(lineNumber);
  } catch (ErrorException &ex) {
    throw ex;
  }
}
