#include "lexer.h"

const char *tokentype_text(TokenType ttype) {
  switch (ttype) {
  case TT_UNKNOWN:	return "TT_UNKNOWN";
  case TT_SIG:		return "TT_SIG";
  case TT_EQTEST:	return "TT_EQ";
  case TT_AND:		return "TT_AND";
  case TT_OR:		return "TT_OR";
  case TT_BOOL:		return "TT_BOOL";
  case TT_OPAREN:	return "TT_OPAREN";
  case TT_CPAREN:	return "TT_CPAREN";
  default:		return "ERROR: Mem corruption?";
  }
}

TokenType classify_token(Token token) {
  String_View ttext = token.text;
  assert(ttext.count > 0);
  if (sv_eq(ttext, sv_from_cstr("="))) return TT_EQTEST;
  else if (sv_eq(ttext, sv_from_cstr("AND"))) return TT_AND;
  else if (sv_eq(ttext, sv_from_cstr("OR"))) return TT_OR;
  else if (sv_eq(ttext, sv_from_cstr("("))) return TT_OPAREN;
  else if (sv_eq(ttext, sv_from_cstr(")"))) return TT_CPAREN;
  else if (sv_eq(ttext, sv_from_cstr("0")) || sv_eq(ttext, sv_from_cstr("1")))
    return TT_BOOL;
  else if (ttext.data[0] == '(') return TT_OPAREN;
  else return TT_SIG;
}

int is_not_space_or_paren(int x) {
  return !((x=='(') || (x==')') || isspace(x));
}

Tokens lex_string_view(String_View stmt_sv) {
  Tokens tokens = {0};
  while (stmt_sv.count > 0) {
    Token next_token = {0};
    if ((stmt_sv.data[0] == '(') || (stmt_sv.data[0] == ')')) {
      next_token.text = sv_chop_left(&stmt_sv, 1);
    }
    else
      next_token.text = nob_sv_chop_while(&stmt_sv, is_not_space_or_paren);

    next_token.ttype = classify_token(next_token);

    da_append(&tokens, next_token);
    
    stmt_sv = sv_trim(stmt_sv);
  }
  return tokens;
}
