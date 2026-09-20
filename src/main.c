#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define NOB_IMPLEMENTATION
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

const char *tokentype_text(TokenType ttype) {
  switch (ttype) {
  case TT_UNKNOWN:	return "TT_UNKNOWN";
  case TT_SIG:		return "TT_SIG";
  case TT_EQTEST:	return "TT_EQ";
  case TT_AND:		return "TT_AND";
  case TT_OR:		return "TT_OR";
  case TT_BOOL:		return "TT_BOOL";
  case TT_OPAREN:       return "TT_OPAREN";
  case TT_CPAREN:       return "TT_CPAREN";
  default:		return "ERROR: Mem corruption?";
  }
}

typedef struct Token {
  String_View text;
  TokenType ttype;
} Token;

typedef struct Tokens {
  Token *items;
  size_t count;
  size_t capacity;
} Tokens;


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

int main(void) {
  const char *stmt =
      "N_R3_VAC_PLC01_IP_RFD_S = 0 AND N_R3_VAC_PLC01_VGC_RFD_S = 0 AND "
      "N_R3_WAT_PLC01_RFD_S = 0 AND N_R3_DIA_PLC01_RFD_S = 0 AND "
      "N_R3_DIA_PLC01_LIB_RFD_S = 0 AND N_R3_VAC_PLC01_FEBEAM_RFD_S = 0 AND "
      "N_R3_VAC_PLC01_COMMS_RFD_S = 0 AND N_R3_VAC_PLC01_OTHERS_RFD_S = 0 AND "
      "(B_R3_VAC_PLC01_RFDMP_LOCAL_HB OR B_R3_VAC_FASTSCAN_RESET_HB)";

  String_View stmt_sv = sv_from_cstr(stmt);
  stmt_sv = sv_trim(stmt_sv);
  if (stmt_sv.count <= 0) {
    printf("ERROR: The string to parse is empty.");
    return 1;
  }

  Tokens tokens = lex_string_view(stmt_sv);

  da_foreach(Token, t, &tokens) {
    printf("Token: %s : \"" SV_Fmt "\"\n",
           tokentype_text(t->ttype), SV_Arg(t->text));
  }
  
  return 0;
}

