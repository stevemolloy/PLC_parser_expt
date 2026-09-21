#ifndef LEXER_H
#define LEXER_H

#include "nob.h"

typedef enum TokenType {
  TT_UNKNOWN = 0,
  TT_SIG,
  TT_EQTEST,
  TT_AND,
  TT_OR,
  TT_BOOL,
  TT_OPAREN,
  TT_CPAREN,
} TokenType;

typedef struct Token {
  String_View text;
  TokenType ttype;
} Token;

typedef struct Tokens {
  Token *items;
  size_t count;
  size_t capacity;
} Tokens;

const char *tokentype_text(TokenType ttype);
TokenType classify_token(Token token);
int is_not_space_or_paren(int x);
Tokens lex_string_view(String_View stmt_sv);

#endif
