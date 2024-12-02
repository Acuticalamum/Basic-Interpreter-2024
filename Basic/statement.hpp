/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include <string>
#include <sstream>
#include "evalstate.hpp"
#include "exp.hpp"
#include "Utils/tokenScanner.hpp"
#include "program.hpp"
#include "parser.hpp"
#include "Utils/error.hpp"
#include "Utils/strlib.hpp"

class Program;

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(EvalState &state, Program &program) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

 class RemStatement : public Statement {
   public :
     RemStatement() {}
     ~RemStatement() {}
     virtual void execute(EvalState &state, Program &program) override {
       program.nextCurrentLine();
     }
 };

 class LetStatement : public Statement {
   public :
     LetStatement::LetStatement(TokenScanner &scanner) {
       expr = parseExp(scanner);
     }
     ~LetStatement() {
        delete expr;
     }
     virtual void execute(EvalState &state, Program &program) override {
        expr -> eval(state);
        program.nextCurrentLine();
     }
   private :
     Expression* expr;
 };

 class PrintStatement : public Statement {
   public :
     PrintStatement(Expression* _expr) {
       expr = _expr;
     }
     ~PrintStatement() {
        delete expr;
     }
     virtual void execute(EvalState &state, Program &program) override {
       std::cout << expr -> eval(state) << std::endl;
       program.nextCurrentLine();
     }
   private :
     Expression* expr;
 };

 class InputStatement : public Statement {
   public :
     InputStatement(std::string _name) {
       name = _name;
     }
     virtual void execute(EvalState &state, Program &program) override {
       int Input;
       std::cin >> Input;
       std::cout << "  ?  " << std::endl;
       state.setValue(name, Input);
       program.nextCurrentLine();
     }
   private :
     std::string name;
 };

 class IfStatement : public Statement {
   public :
     IfStatement(Expression* _expr1, std::string _cmp, Expression* _expr2, int _gotoNumber) {
       expr1 = _expr1;
       expr2 = _expr2;
       cmp = _cmp;
       gotoNumber = _gotoNumber;
     }
     ~IfStatement() {
        delete expr1;
        delete expr2;
     }
     virtual void execute(EvalState &state, Program &program) override {
       //to do
     }
   private : 
     Expression *expr1, *expr2;
     int gotoNumber;
     std::string cmp;
 };

 class EndStatement : public Statement {
   public :
     EndStatement() {}
     virtual void execute(EvalState &state, Program &program) override {
       program.endProgram();
       program.nextCurrentLine();
     }
 };

 class GotoStatement : public Statement {
   public :
     GotoStatement(int _lineNumber) {
       lineNumber = _lineNumber;
     }
     ~GotoStatement() {}
     virtual void execute(EvalState &state, Program &program) override {
       program.switchCurrentLine(lineNumber);
     }
   private : 
     int lineNumber;
 };

Statement* transfer(const std::string &x); 

#endif
