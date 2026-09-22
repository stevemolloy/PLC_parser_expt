#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

#define NOB_IMPLEMENTATION
#include "nob.h"

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

  Node *root = parse_statement(&tokens);
  if (root == NULL)
    return 1;

  print_node(root, 0);

  free(tokens.items);
  
  return 0;
}
