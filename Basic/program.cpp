/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"



Program::Program() {
  CurrentLine = -1;
  Ended = false;
  lines.clear();
  for(auto it = map_statement.begin(); it != map_statement.end(); it++) {
    delete it -> second;
  }
  map_statement.clear();
  map_string.clear();
}

Program::~Program() {
  CurrentLine = -1;
  Ended = false;
  lines.clear();
  for(auto it = map_statement.begin(); it != map_statement.end(); it++) {
    delete it -> second;
  }
  map_statement.clear();
  map_string.clear();
}

void Program::clear() {
    // Replace this stub with your own code
    //todo
    map_string.clear();
    for(auto it = map_statement.begin(); it != map_statement.end(); it++) {
      delete it -> second;
    }
    map_statement.clear();
    lines.clear();
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    // Replace this stub with your own code
    //todo
    map_string[lineNumber] = line;
    lines.insert(lineNumber);
    Statement* statement = transfer(line);
    setParsedStatement(lineNumber, statement);
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if(map_string.count(lineNumber)) {
       //auto iter = map_string.find(lineNumber);
       map_string.erase(lineNumber);
       lines.erase(lineNumber);
       if (map_statement.count(lineNumber)) {
         delete map_statement[lineNumber];
         map_statement.erase(lineNumber);
       }
    }
}

std::string Program::getSourceLine(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if(map_string.count(lineNumber)) {
       return map_string[lineNumber];
    }
    return "";
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    // Replace this stub with your own code
    //todo
    if(map_string.count(lineNumber)) {
      delete map_statement[lineNumber];
    }
    map_statement[lineNumber] = stmt;
}

//void Program::removeSourceLine(int lineNumber) {

Statement *Program::getParsedStatement(int lineNumber) {
   // Replace this stub with your own code
   //todo
   if(map_statement.count(lineNumber)) {
     return map_statement[lineNumber];
   }
   return nullptr;
}

int Program::getFirstLineNumber() {
    // Replace this stub with your own code
    //todo
    if(lines.empty()) return -1;
    auto iter = lines.begin();
    return *iter;
}

int Program::getNextLineNumber(int lineNumber) {
    // Replace this stub with your own code
    //todo
    if(lineNumber == -1) return -1;
    auto iter = lines.find(lineNumber);
    iter++;
    if(iter == lines.end()) return -1;
    return *iter;
}

void Program::endProgram() {
    Ended = true; 
}

void Program::switchCurrentLine(int lineNumber) {
    if(map_statement.count(lineNumber)) {
      CurrentLine = lineNumber;
    }
    else error("LINE NUMBER ERROR");
}

void Program::nextCurrentLine() {
    if(CurrentLine == -1) return;
    //std::cout << CurrentLine << std::endl;
    CurrentLine = getNextLineNumber(CurrentLine);
}

void Program::run_the_program(EvalState &state) {
    Ended = false;
    CurrentLine = getFirstLineNumber();
    while(CurrentLine != -1) {
        try {
            if(Ended) break;
            getParsedStatement(CurrentLine) -> execute(state, *this);
            if(Ended) break;
        } catch(ErrorException &ex) {
            nextCurrentLine();
            std::cout << ex.getMessage() << std::endl;
        }
    }
}
//more func to add
//todo


