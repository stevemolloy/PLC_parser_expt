#ifndef _PARSER_H
#define _PARSER_H

#include "nob.h"
#include "lexer.h"

typedef struct Node Node;

typedef enum NodeType {
  NT_SIGNAL,
  NT_BOOL,
  NT_BINOP,
} NodeType;

typedef struct Signal { 
  String_View name;
} Signal;

typedef struct Boolnode {
  String_View text;
  bool value;
} Boolnode;

typedef enum BinOpType {
  BO_AND,
  BO_OR,
  BO_EQ,
} BinOpType;

typedef struct BinOp {
  BinOpType btype;
  Node *lhs;
  Node* rhs;
} BinOp;

typedef struct EqTest {
  String_View name;
  Node *lhs;
  Node *rhs;
} EqTest;

struct Node {
  NodeType ntype;
  union {
    Signal signal;
    Boolnode boolnode;
    BinOp binop;
    EqTest eqtest;
  } as;
};

typedef struct Parser {
  Tokens *tokens;
  size_t pos;
} Parser;

Token *peek(Parser *p);
Token *next(Parser *p);
Node *new_node(NodeType t);
int infix_bp(Token *t, BinOpType *out);
Node *parse_primary(Parser *p);
Node *parse_expr(Parser *p, int min_bp);
Node *parse_statement(Tokens *tokens);
void print_node(Node * n, int depth);
  
#endif
