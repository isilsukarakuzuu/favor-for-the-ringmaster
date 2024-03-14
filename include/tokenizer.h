#ifndef TOKENIZER
#define TOKENIZER

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 1024

typedef enum
{
  ENTITIY,
  SELL,
  BUY,
  GO,
  TO,
  FROM,
  AND,
  AT,
  HAS,
  IF,
  LESS,
  MORE,
  THAN,
  NUMBER,
  INVALID,
  EXIT,
  WHERE,
  TOTAL,
  WHO,
  QUESTION_MARK,
  END,
  NOBODY,
  NOTHING,
  NOWHERE
} token_type;

typedef struct
{
  token_type type;
  char *value;
} token;

token_type get_token_type(char *word);
bool contains_invalid_token(token *tokens);
token *tokenize(char *input);
int find_first_token(token *tokens, token_type type);

#endif