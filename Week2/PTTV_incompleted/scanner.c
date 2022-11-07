/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"

extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];

/***************************************************************/

void skipBlank()
{
  while ((currentChar != EOF) && ((charCodes[currentChar] == CHAR_SPACE) || (charCodes[currentChar] == CHAR_ENDOFLINE))) // check to prevent invalid index (-1) of array and ensure continue looping to remove all blank
  {
    readChar();
  }
}

void skipComment()
{
  // modify :
  // Single-line comment : Starts with //, end at end of line.
  int flag = 0;                               // flag var to check valid comment format
  while ((currentChar != EOF) && (flag != 1)) // check to prevent invalid index (-1) of array, flag var to ch
  {
    switch (charCodes[currentChar])
    {
    case CHAR_ENDOFLINE:
      flag = 1; // end of line - end of comment
      break;

    default:
      flag = 0;
      break;
    }
    readChar();
  }
  if (flag != 1) // unclosed comment => error
    error(ERR_ENDOFCOMMENT, lineNo, colNo);
}

Token *readIdentKeyword(void)
{
  Token *token = makeToken(TK_NONE, lineNo, colNo);
  int stringIndexCount = 1;

  token->string[0] = (char)currentChar;
  if (charCodes[currentChar] != CHAR_LETTER && charCodes[currentChar] != CHAR_UNDERSCORE) // Modify:  The first character of an identifier should be either a letter or an underscore.
  {
    error(ERR_INVALIDSYMBOL, token->lineNo, token->colNo);
    return token;
  }
  readChar();

  while ((currentChar != EOF) &&                                                                                                             // check to prevent invalid index (-1) of array
         ((charCodes[currentChar] == CHAR_LETTER) || (charCodes[currentChar] == CHAR_DIGIT) || (charCodes[currentChar] == CHAR_UNDERSCORE))) // A valid identifier can have letters (both uppercase and lowercase letters), digits and underscores.
  {
    if (stringIndexCount < MAX_IDENT_LEN)                    // Modify: There is no rule on how long an identifier can be. However, only 15 characters are significant
      token->string[stringIndexCount++] = (char)currentChar; // append char to string
    readChar();                                              // read next character
  }
  token->string[stringIndexCount] = '\0';

  TokenType checkedType = checkKeyword(token->string); // check if string is belong to program keyword
                                                       // Modify: cannot use keywords as identifiers
  token->tokenType = checkedType != TK_NONE ? checkedType : TK_IDENT;

  return token;
}

Token *readConstChar(void)
{
  Token *token = makeToken(TK_CHAR, lineNo, colNo);

  readChar();

  int flag = 0;

  if (currentChar == EOF)
  {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }

  int stringIndexCount = 0;
  int continueFlag = 0;
  while ((currentChar != EOF) && // check to prevent invalid index (-1) of array
         currentChar >= PRINTABLE_CHARCODE_lOWERBOUND && currentChar <= PRINTABLE_CHARCODE_UPPERBOUND)
  {
    continueFlag = 0;
    if (flag == 1 && charCodes[currentChar] != CHAR_SINGLEQUOTE)
    {
      break;
    }
    if (charCodes[currentChar] == CHAR_SINGLEQUOTE)
    {
      flag++;
      if (flag == 4)
      {
        token->string[stringIndexCount++] = 39;
        flag = 0;
      }
      continueFlag = 1;
    }
    if (continueFlag == 0)
    {
      token->string[stringIndexCount++] = (char)currentChar;
    }
    readChar();
    if (stringIndexCount > MAX_STRING_LEN)
    {
      token->tokenType = TK_NONE;
      error(ERR_STRINGCONSTANTTOOLONG, token->lineNo, token->colNo);
      return token;
    }
  }
  token->string[stringIndexCount] = '\0';

  if (flag == 1) // case valid constant char
  {
    readChar();
    return token;
  }
  else // unclosed constChar
  {
    token->tokenType = TK_NONE;
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  }
}

Token *readNumber(void)
{
  Token *token = makeToken(TK_NUMBER, lineNo, colNo);
  int check = 0;
  while ((currentChar != EOF) && (charCodes[currentChar] == CHAR_DIGIT))
  {
    if ((check * 10) / 10 != check || check * 10 + (currentChar - '0') < 0) // convert to base 10 number, check number is too large
    {
      error(ERR_NUMBERTOOLARGE, token->lineNo, token->colNo);
      break;
    }
    check = check * 10 + (currentChar - '0');
    readChar();
  }
  sprintf(token->string, "%d%c", check, '\0');
  token->value = check;

  return token;
}

Token *getToken(void)
{
  Token *token;
  int Ln, Col;

  if (currentChar == EOF) // check to prevent invalid index (-1) of array
    return makeToken(TK_EOF, lineNo, colNo);

  switch (charCodes[currentChar])
  {
  case CHAR_SPACE:
    skipBlank();
    return getToken();
  case CHAR_ENDOFLINE:
    skipBlank();
    return getToken();
  case CHAR_LETTER:
    return readIdentKeyword();
  case CHAR_UNDERSCORE:
    return readIdentKeyword();
  case CHAR_DIGIT:
    return readNumber();
  case CHAR_PLUS:
    token = makeToken(SB_PLUS, lineNo, colNo);
    readChar();
    return token;
  case CHAR_MINUS:
    token = makeToken(SB_MINUS, lineNo, colNo);
    readChar();
    return token;
  case CHAR_TIMES:
    token = makeToken(SB_TIMES, lineNo, colNo);
    readChar();
    return token;
  case CHAR_SLASH:
    Ln = lineNo;
    Col = colNo;
    readChar();
    if (currentChar == EOF) // check to prevent invalid index (-1) of array
    {
      token = makeToken(SB_LPAR, Ln, Col);
      return token;
    }
    switch (charCodes[currentChar])
    {
    case CHAR_SLASH: // Modify:  single line comment with //
      readChar();
      skipComment();
      return getToken();
    default:
      token = makeToken(SB_LPAR, lineNo, colNo);
      return token;
    }
  case CHAR_EQ:
    token = makeToken(SB_EQ, lineNo, colNo);
    readChar();
    return token;
  case CHAR_COMMA:
    token = makeToken(SB_COMMA, lineNo, colNo);
    readChar();
    return token;
  case CHAR_SEMICOLON:
    token = makeToken(SB_SEMICOLON, lineNo, colNo);
    readChar();
    return token;

  case CHAR_PERIOD:
    Ln = lineNo;
    Col = colNo;
    token = makeToken(SB_PERIOD, Ln, Col);
    readChar();
    if (currentChar != EOF && charCodes[currentChar] == CHAR_RPAR)
    {
      token = makeToken(SB_RSEL, Ln, Col);
      readChar();
    }
    return token;

  case CHAR_COLON:
    Ln = lineNo;
    Col = colNo;

    token = makeToken(SB_COLON, Ln, Col);
    readChar();
    if (currentChar != EOF && charCodes[currentChar] == CHAR_EQ) // case assign
    {
      token = makeToken(SB_ASSIGN, Ln, Col);
      readChar();
    }
    return token;

  case CHAR_GT:
    Ln = lineNo;
    Col = colNo;

    token = makeToken(SB_GT, Ln, Col);
    readChar();
    if (currentChar != EOF && charCodes[currentChar] == CHAR_EQ)
    {
      token = makeToken(SB_GE, Ln, Col);
      readChar();
    }
    return token;

  case CHAR_LT:
    Ln = lineNo;
    Col = colNo;

    readChar();

    if (currentChar == EOF) // check to prevent invalid index (-1) of array
    {
      token = makeToken(SB_LPAR, Ln, Col);
      return token;
    }

    switch (charCodes[currentChar])
    {
    case CHAR_EQ:
      token = makeToken(SB_LE, Ln, Col);
      readChar();
      return token;
    case CHAR_GT:
      token = makeToken(SB_NEQ, Ln, Col); // Modify : Relop: not equal to <>
      readChar();
      return token;

    default:
      token = makeToken(SB_LT, lineNo, colNo);
      return token;
    }

  case CHAR_RPAR:
    token = makeToken(SB_RPAR, lineNo, colNo);
    readChar();
    return token;

  case CHAR_LPAR:
    Ln = lineNo;
    Col = colNo;

    readChar();

    if (currentChar == EOF) // check to prevent invalid index (-1) of array
    {
      token = makeToken(SB_LPAR, Ln, Col);
      return token;
    }

    switch (charCodes[currentChar])
    {
    case CHAR_PERIOD:
      token = makeToken(SB_LSEL, Ln, Col);
      readChar();
      return token;

    default:
      token = makeToken(SB_LPAR, lineNo, colNo);
      return token;
    }

  case CHAR_SINGLEQUOTE:
    return readConstChar();

  default:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar();
    return token;
  }
}

/******************************************************************/

void printToken(Token *token)
{

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType)
  {
  case TK_NONE:
    printf("TK_NONE\n");
    break;
  case TK_IDENT:
    printf("TK_IDENT(%s)\n", token->string);
    break;
  case TK_NUMBER:
    printf("TK_NUMBER(%s)\n", token->string);
    break;
  case TK_CHAR:
    printf("TK_CHAR(\'%s\')\n", token->string);
    break;
  case TK_EOF:
    printf("TK_EOF\n");
    break;

  case KW_PROGRAM:
    printf("KW_PROGRAM\n");
    break;
  case KW_CONST:
    printf("KW_CONST\n");
    break;
  case KW_TYPE:
    printf("KW_TYPE\n");
    break;
  case KW_VAR:
    printf("KW_VAR\n");
    break;
  case KW_INTEGER:
    printf("KW_INTEGER\n");
    break;
  case KW_CHAR:
    printf("KW_CHAR\n");
    break;
  case KW_ARRAY:
    printf("KW_ARRAY\n");
    break;
  case KW_OF:
    printf("KW_OF\n");
    break;
  case KW_FUNCTION:
    printf("KW_FUNCTION\n");
    break;
  case KW_PROCEDURE:
    printf("KW_PROCEDURE\n");
    break;
  case KW_BEGIN:
    printf("KW_BEGIN\n");
    break;
  case KW_END:
    printf("KW_END\n");
    break;
  case KW_CALL:
    printf("KW_CALL\n");
    break;
  case KW_IF:
    printf("KW_IF\n");
    break;
  case KW_THEN:
    printf("KW_THEN\n");
    break;
  case KW_ELSE:
    printf("KW_ELSE\n");
    break;
  case KW_WHILE:
    printf("KW_WHILE\n");
    break;
  case KW_DO:
    printf("KW_DO\n");
    break;
  case KW_FOR:
    printf("KW_FOR\n");
    break;
  case KW_TO:
    printf("KW_TO\n");
    break;

  case SB_SEMICOLON:
    printf("SB_SEMICOLON\n");
    break;
  case SB_COLON:
    printf("SB_COLON\n");
    break;
  case SB_PERIOD:
    printf("SB_PERIOD\n");
    break;
  case SB_COMMA:
    printf("SB_COMMA\n");
    break;
  case SB_ASSIGN:
    printf("SB_ASSIGN\n");
    break;
  case SB_EQ:
    printf("SB_EQ\n");
    break;
  case SB_NEQ:
    printf("SB_NEQ\n");
    break;
  case SB_LT:
    printf("SB_LT\n");
    break;
  case SB_LE:
    printf("SB_LE\n");
    break;
  case SB_GT:
    printf("SB_GT\n");
    break;
  case SB_GE:
    printf("SB_GE\n");
    break;
  case SB_PLUS:
    printf("SB_PLUS\n");
    break;
  case SB_MINUS:
    printf("SB_MINUS\n");
    break;
  case SB_TIMES:
    printf("SB_TIMES\n");
    break;
  case SB_SLASH:
    printf("SB_SLASH\n");
    break;
  case SB_LPAR:
    printf("SB_LPAR\n");
    break;
  case SB_RPAR:
    printf("SB_RPAR\n");
    break;
  case SB_LSEL:
    printf("SB_LSEL\n");
    break;
  case SB_RSEL:
    printf("SB_RSEL\n");
    break;
  }
}

int scan(char *fileName)
{
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF)
  {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(int argc, char *argv[])
{
  if (argc <= 1)
  {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR)
  {
    printf("Can\'t read input file!\n");
    return -1;
  }

  return 0;
}