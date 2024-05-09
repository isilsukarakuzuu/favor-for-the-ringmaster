#ifndef SUBJECTS
#define SUBJECTS

#include "tokenizer.h"
#include <stdio.h>

typedef struct item
{
  char name[MAX_WORD_LENGTH];
  int quantity;
} item;

typedef struct subject
{
  char name[MAX_WORD_LENGTH];
  char location[MAX_WORD_LENGTH];
  item current_items[MAX_ITEMS];
  char temp_location[MAX_WORD_LENGTH];
  item temp_current_items[MAX_ITEMS];
} subject;

extern subject subjects[MAX_SUBJECTS];

int find_subject(char *name);
int find_item(char *name, item *current_items);
void place_subjects(token *subject_array_token);
void execute_updates();
void delete_updates();
void print_subjects();
void init_items(item items[]);
void init_subjects();

#endif