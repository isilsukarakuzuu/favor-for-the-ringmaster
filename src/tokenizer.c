#include "tokenizer.h"

token_type get_token_type(char *word)
{
  if (strcmp(word, "sell") == 0)
  {
    return SELL;
  }
  if (strcmp(word, "buy") == 0)
  {
    return BUY;
  }
  if (strcmp(word, "go") == 0)
  {
    return GO;
  }
  if (strcmp(word, "to") == 0)
  {
    return TO;
  }
  if (strcmp(word, "from") == 0)
  {
    return FROM;
  }
  if (strcmp(word, "and") == 0)
  {
    return AND;
  }
  if (strcmp(word, "at") == 0)
  {
    return AT;
  }
  if (strcmp(word, "has") == 0)
  {
    return HAS;
  }
  if (strcmp(word, "if") == 0)
  {
    return IF;
  }
  if (strcmp(word, "less") == 0)
  {
    return LESS;
  }
  if (strcmp(word, "more") == 0)
  {
    return MORE;
  }
  if (strcmp(word, "than") == 0)
  {
    return THAN;
  }
  if (strcmp(word, "exit") == 0)
  {
    return EXIT;
  }
  if (strcmp(word, "where") == 0)
  {
    return WHERE;
  }
  if (strcmp(word, "total") == 0)
  {
    return TOTAL;
  }
  if (strcmp(word, "who") == 0)
  {
    return WHO;
  }
  if (strcmp(word, "?") == 0)
  {
    return QUESTION_MARK;
  }
  if (strcmp(word, "NOBODY") == 0)
  {
    return NOBODY;
  }
  if (strcmp(word, "NOTHING") == 0)
  {
    return NOTHING;
  }
  if (strcmp(word, "NOWHERE") == 0)
  {
    return NOWHERE;
  }

  // if all characters are letters or underscores, its an entity

  int i = 0;
  while (word[i] != '\0')
  {
    if (!isalpha(word[i]) && word[i] != '_')
    {
      break;
    }
    i++;
    if (word[i] == '\0')
    {
      return ENTITIY;
    }
  }

  // if all characters are digits, its a number
  i = 0;
  while (word[i] != '\0')
  {
    if (!isdigit(word[i]))
    {
      break;
    }
    i++;
    if (word[i] == '\0')
    {
      return NUMBER;
    }
  }

  return INVALID;
}

bool contains_invalid_token(token *tokens)
{
  int i = 0;
  while (tokens[i].type != END)
  {
    if (tokens[i].type == INVALID)
    {
      return true;
    }
    i++;
  }
  return false;
}

token *tokenize(char *input)
{
  token *tokens = malloc(MAX * sizeof(token));

  int i = 0;
  int token_index = 0;
  while (input[i] != '\0')
  {
    char word[MAX];
    int j = 0;
    while (input[i] != ' ' && input[i] != '\0')
    {
      word[j] = input[i];
      i++;
      j++;
    }
    word[j] = '\0';
    if (strlen(word) == 0)
    {
      i++;
      continue;
    }
    tokens[token_index].value = malloc(strlen(word) + 1);
    strcpy(tokens[token_index].value, word);
    tokens[token_index++].type = get_token_type(word);
    if (input[i] != '\0')
    {
      i++;
    }
  }
  tokens[token_index].type = END;
  if (contains_invalid_token(tokens))
  {
    return NULL;
  }
  return tokens;
}

int find_first_token(token *tokens, token_type type)
{
  int i = 0;
  while (tokens[i].type != END)
  {
    if (tokens[i].type == type)
    {
      return i;
    }
    i++;
  }
  if (type == END)
  {
    return i;
  }
  return -1;
}
