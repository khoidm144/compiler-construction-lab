/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __ERROR_H__
#define __ERROR_H__

typedef enum {
  ERR_ENDOFCOMMENT,
  ERR_IDENTTOOLONG,
  ERR_INVALIDCHARCONSTANT,
  ERR_INVALIDSYMBOL,
  ERR_NUMBERTOOLARGE,
  ERR_INVALIDINDENT,
  ERR_STRINGCONSTANTTOOLONG
} ErrorCode;


#define ERM_ENDOFCOMMENT "End of comment expected!"
#define ERM_IDENTTOOLONG "Identification too long!"
#define ERM_INVALIDCHARCONSTANT "Invalid const char!"
#define ERM_INVALIDSYMBOL "Invalid symbol!"
#define ERM_NUMBERTOOLARGE "Number constant is too large!"
#define ERM_INVALIDINDENT "Invalid indent!"
#define ERM_STRINGCONSTANTTOOLONG "String constant too long !"

void error(ErrorCode err, int lineNo, int colNo);

#endif