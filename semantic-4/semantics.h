/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__

#include "symtab.h"

void checkFreshIdent(char *name);
Object* checkDeclaredIdent(char *name);//check declared identifiers: (identifier is already declared or not. If declared return identifier object, else return null.)
Object* checkDeclaredConstant(char *name);// check declared constants.
Object* checkDeclaredType(char *name);// check declared identifiers.
Object* checkDeclaredVariable(char *name);// check declared variables.
Object* checkDeclaredFunction(char *name);//check declared functions.
Object* checkDeclaredProcedure(char *name);// check declared procedure.
Object* checkDeclaredLValueIdent(char *name);// check declared LValue.


void checkIntType(Type* type);
void checkCharType(Type* type);
void checkArrayType(Type* type);
void checkBasicType(Type* type);
void checkTypeEquality(Type* type1, Type* type2);

#endif