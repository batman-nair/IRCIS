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
#define CH_NORTH '^'
#define CH_SOUTH 'v'
#define CH_EAST '>'
#define CH_WEST '<'

#define CH_ADD '+'
#define CH_SUB '-'
#define CH_MUL '*'
#define CH_DIV '/'
#define CH_MOD '%'

#define CH_PRINT '#'
#define CH_ENDL '$'

#define CH_PUSH '@'
#define CH_POP '&'

#define CH_STACK '"'
#define CH_INT '\''

#define CH_DOT '.'
#define CH_SPC ' '

#define CH_END '!'

  // Group defines
  DEF_CHAR_GROUP_DECL(arith);
  DEF_CHAR_GROUP_DECL(blank);
}
