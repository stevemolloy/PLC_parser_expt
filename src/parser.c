#include "parser.h"
#include "lexer.h"

Token *peek(Parser *p) {
  if (p->pos >= p->tokens->count)
    return NULL;
  return &p->tokens->items[p->pos];
}

Token *next(Parser *p) {
  Token *t = peek(p);
  if (t)
    p->pos++;
  return t;
}

Node *new_node(NodeType t) {
  Node *n = malloc(sizeof(Node));
  assert(n != NULL);
  n->ntype = t;
  return n;
}

int infix_bp(Token *t, BinOpType *out) {
  switch (t->ttype) {
  case TT_OR:
    *out = BO_OR;
    return 1;
  case TT_AND:
    *out = BO_AND;
    return 2;
  case TT_EQTEST:
    *out = BO_EQ;
    return 3;
  case TT_UNKNOWN:
  case TT_SIG:
  case TT_BOOL:
  case TT_OPAREN:
  case TT_CPAREN:
  default: return -1;
  }
}

Node *parse_expr(Parser *p, int min_bp);

Node *parse_primary(Parser *p) {
  Token *t = next(p);
  if (!t) {
    fprintf(stderr, "ERROR: Unexpected end of input\n");
    return NULL;
  }

  switch (t->ttype) {
  case TT_SIG: {
    Node *n = new_node(NT_SIGNAL);
    n->as.signal.name = t->text;
    return n;
  }
  case TT_BOOL: {
    Node *n = new_node(NT_BOOL);
    n->as.boolnode.text = t->text;
    n->as.boolnode.value = sv_eq(t->text, sv_from_cstr("1"));
    return n;
  }
  case TT_OPAREN: {
    Node *inner = parse_expr(p, 0);
    if (!inner)
      return NULL;
    Token *close = next(p);
    if (!close || close->ttype != TT_CPAREN) {
      fprintf(stderr, "ERROR: expected \")\"");
      return NULL;
    }
    return inner;
  }
  case TT_UNKNOWN:
  case TT_EQTEST:
  case TT_AND:
  case TT_OR:
  case TT_CPAREN:
  default:
    fprintf(stderr, "ERROR: Unexpected token\n");
    return NULL;
  }
  
  return NULL;
}

Node *parse_expr(Parser *p, int min_bp) {
  Node *lhs = parse_primary(p);
  if (lhs == NULL)
    return NULL;

  for (;;) {
    Token *op = peek(p);
    if (!op)
      break;

    BinOpType bt;
    int bp = infix_bp(op, &bt);
    if (bp < 0 || bp < min_bp)
      break;

    next(p);
    Node *rhs = parse_expr(p, bp + 1);
    if (!rhs)
      return NULL;

    Node *n = new_node(NT_BINOP);
    n->as.binop.btype = bt;
    n->as.binop.lhs = lhs;
    n->as.binop.rhs = rhs;
    lhs = n;
  }
  
  return lhs;
}

Node *parse_statement(Tokens *tokens) {
  Parser p = {.tokens = tokens};
  Node *root = parse_expr(&p, 0);
  if (root && peek(&p)) {
      root = NULL;
  }
  if (!root) fprintf(stderr, "ERROR: Parser error!");
  return root;
}

void print_node(Node *n, int depth) {
  printf("%*s", depth * 2, " ");

  switch (n->ntype) {
  case NT_SIGNAL:
    printf("SIGNAL " SV_Fmt "\n", SV_Arg(n->as.signal.name));
    break;
  case NT_BOOL:
    printf("BOOL " SV_Fmt "\n", SV_Arg(n->as.boolnode.text));
    break;
  case NT_BINOP:
    const char *names[] = {"AND", "OR", "EQ"};
    printf("%s\n", names[n->as.binop.btype]);
    print_node(n->as.binop.lhs, depth + 1);
    print_node(n->as.binop.rhs, depth + 1);
    break;
  }  
}
