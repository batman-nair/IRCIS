#pragma once

#define DEF_CHAR_GROUP_DECL(char_group)		\
  bool is_ ## char_group ( char )

#define DEF_CHAR_GROUP(char_group, ch1, ...)			\
  bool is_ ## char_group ( char check ) {			\
    char arr[] = { ch1, ##__VA_ARGS__ };			\
    for ( char ch : arr ) { if ( ch == check) return true; }	\
    return false;						\
  }

namespace PTrain {
  // Character defines
#define MOVE_NORTH '^'
#define MOVE_SOUTH 'v'
#define MOVE_EAST '>'
#define MOVE_WEST '<'

#define ARITH_ADD '+'
#define ARITH_SUB '-'
#define ARITH_MUL '*'
#define ARITH_DIV '/'
#define ARITH_MOD '%'

#define PRINT_CHAR '#'
#define PRINT_ENDL '$'

#define STACK_PUSH '@'
#define STACK_POP '&'

#define STACK_MODE '"'
#define INT_MODE '\''

#define BLANK_DOT '.'
#define BLANK_SPC ' '

#define END_PROC '!'

  // Group defines
  DEF_CHAR_GROUP_DECL(arith);
  DEF_CHAR_GROUP_DECL(blank);
}
