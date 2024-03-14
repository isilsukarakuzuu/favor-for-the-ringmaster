#ifndef PARSER
#define PARSER

#include <stdio.h>
#include "tokenizer.h"

typedef enum
{
  ACTION,
  CONDITION,
  END_SENTENCE
} sentence_type;

typedef struct
{
  sentence_type type;
  token *subjects;
  token *verbs;
  token *items;
  token *to_from;
  token *location;
} sentence;

void print_sentence(sentence *sentences);
void init_sentence(sentence *sentence, sentence_type type);
token *parse_subjects(token *current_token, token *subject_array_token);
token *parse_items(token *current_token, token *item_array_token);
token *parse_action(token *current_token, sentence *current_sentence);
token *parse_condition(token *current_token, sentence *current_sentence);
sentence *parse_sentence(token *tokens);
char *parse_question(token *tokens);

#endif