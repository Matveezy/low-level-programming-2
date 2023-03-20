%{
#include "../struct/struct.h"
#include <stdio.h>
#include "parser.h"
%}

%option yylineno noyywrap debug nounput

%%
[ \t]+                {}
[\n\r]                  {}
"true"                  {yylval.bool_value = true; return TOKBOOL;}
"false"                 {yylval.bool_value = false; return TOKBOOL;}
"&&"                    {yylval.log_op = AND_OPERATION; return LOGOP;}
"||"                    {yylval.log_op = OR_OPERATION; return LOGOP;}
"LIKE"                  {yylval.const_op = LIKE; return CONSTOP;}
">="                    {yylval.const_op = GTE; return CONSTOP;}
">"                     {yylval.const_op = GT; return CONSTOP;}
"<="                    {yylval.const_op = LTE; return CONSTOP;}
"<"                     {yylval.const_op = LT; return CONSTOP;}
"=="                    {yylval.const_op = EQUALS; return CONSTOP;}
"!="                    {yylval.const_op = NEQUALS; return CONSTOP;}
FOR                     return TOKFOR;
FILTER                  return TOKFILTER;
RETURN                  return TOKRETURN;
IN                      return TOKIN;
\"                      return QUOTE;
\{                      return OBRACE;
\}                      return EBRACE;
%                       return TOKPERCENT;
;                       return SEMICOLON;
":"                     return COLON;
","                     return COMMA;
\"[^\"]*\"              {yylval.string = strdup(yytext); return TOKSTRING; }
[a-zA-Z][a-zA-Z0-9.]*   {yylval.string = strdup(yytext); return ID; }
-?[0-9]+                {yylval.int_value = atoi(yytext); return INT_TOKEN;}
-?[0-9]+\.[0-9]+        {yylval.float_value = atof(yytext);  return FLOAT_TOKEN;}
%%