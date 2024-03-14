#include "parser.h"

void print_sentence(sentence *sentences)
{
  int i = 0;
  while (sentences[i].type != END_SENTENCE)
  {
    printf("Type: %d\n", sentences[i].type);
    int j = 0;
    while (sentences[i].subjects[j].type != END)
    {
      printf("Subject: %s\n", sentences[i].subjects[j].value);
      j++;
    }
    j = 0;
    while (sentences[i].verbs[j].type != END)
    {
      printf("Verb: %s\n", sentences[i].verbs[j].value);
      j++;
    }
    j = 0;
    while (sentences[i].items[j].type != END)
    {
      printf("Item: %s\n", sentences[i].items[j].value);
      j++;
    }
    j = 0;
    while (sentences[i].to_from[j].type != END)
    {
      printf("To/From: %s\n", sentences[i].to_from[j].value);
      j++;
    }
    j = 0;
    while (sentences[i].location[j].type != END)
    {
      printf("Location: %s\n", sentences[i].location[j].value);
      j++;
    }
    i++;
  }
}

void init_sentence(sentence *sentence, sentence_type type)
{
  sentence->type = type;
  sentence->subjects = malloc(MAX * sizeof(token));
  sentence->verbs = malloc(MAX * sizeof(token));
  sentence->items = malloc(MAX * sizeof(token));
  sentence->to_from = malloc(MAX * sizeof(token));
  sentence->location = malloc(MAX * sizeof(token));

  sentence->subjects[0].type = END;
  sentence->verbs[0].type = END;
  sentence->items[0].type = END;
  sentence->to_from[0].type = END;
  sentence->location[0].type = END;
}

token *parse_subjects(token *current_token, token *subject_array_token)
{
  while (current_token->type == ENTITIY)
  {
    *subject_array_token = *current_token;
    current_token++;
    subject_array_token++;
    if (current_token->type != AND)
    {
      break;
    }
    if ((current_token + 1)->type == ENTITIY)
    {
      current_token++;
    }
  }
  subject_array_token->type = END;
  return current_token;
}

token *parse_items(token *current_token, token *item_array_token)
{
  while (current_token->type == NUMBER && (current_token + 1)->type == ENTITIY)
  {
    *item_array_token = *current_token;
    current_token++;
    item_array_token++;
    *item_array_token = *current_token;
    current_token++;
    item_array_token++;
    if (current_token->type != AND)
    {
      break;
    }
    if ((current_token + 1)->type == NUMBER)
    {
      current_token++;
    }
  }
  item_array_token->type = END;
  return current_token;
}

token *parse_action(token *current_token, sentence *current_sentence)
{
  token *subject_array_token = &current_sentence->subjects[0];
  token *verb_array_token = &current_sentence->verbs[0];
  token *item_array_token = &current_sentence->items[0];
  token *to_from_array_token = &current_sentence->to_from[0];
  token *location_array_token = &current_sentence->location[0];

  if (current_token->type != ENTITIY)
  {
    return NULL;
  }

  current_token = parse_subjects(current_token, subject_array_token);

  if (current_token->type == SELL && (current_token + 1)->type == NUMBER && (current_token + 2)->type == ENTITIY)
  {
    *verb_array_token = *current_token;
    current_token++;
    verb_array_token++;
    current_token = parse_items(current_token, item_array_token);
    if (current_token->type == TO && (current_token + 1)->type == ENTITIY)
    {
      *verb_array_token = *current_token;
      current_token++;
      verb_array_token++;
      *to_from_array_token = *current_token;
      current_token++;
      to_from_array_token++;
    }
  }
  else if (current_token->type == BUY && (current_token + 1)->type == NUMBER && (current_token + 2)->type == ENTITIY)
  {
    *verb_array_token = *current_token;
    current_token++;
    verb_array_token++;
    current_token = parse_items(current_token, item_array_token);
    if (current_token->type == FROM && (current_token + 1)->type == ENTITIY)
    {
      *verb_array_token = *current_token;
      current_token++;
      verb_array_token++;
      *to_from_array_token = *current_token;
      current_token++;
      to_from_array_token++;
    }
  }
  else if (current_token->type == GO && (current_token + 1)->type == TO)
  {
    *verb_array_token = *current_token;
    current_token++;
    verb_array_token++;
    *verb_array_token = *current_token;
    current_token++;
    verb_array_token++;
    *location_array_token = *current_token;
    current_token++;
    location_array_token++;
  }
  else
  {
    return NULL;
  }

  if (current_token->type != END && current_token->type != AND && current_token->type != IF)
  {
    return NULL;
  }

  verb_array_token->type = END;
  to_from_array_token->type = END;
  location_array_token->type = END;
  return current_token;
}

token *parse_condition(token *current_token, sentence *current_sentence)
{
  token *subject_array_token = &current_sentence->subjects[0];
  token *verb_array_token = &current_sentence->verbs[0];
  token *item_array_token = &current_sentence->items[0];
  token *to_from_array_token = &current_sentence->to_from[0];
  token *location_array_token = &current_sentence->location[0];

  if (current_token->type != ENTITIY)
  {
    return NULL;
  }

  current_token = parse_subjects(current_token, subject_array_token);

  if (current_token->type == HAS)
  {
    *verb_array_token = *current_token;
    current_token++;
    verb_array_token++;
    if (current_token->type == MORE || current_token->type == LESS)
    {
      *verb_array_token = *current_token;
      current_token++;
      verb_array_token++;
      if (current_token->type != THAN)
      {
        return NULL;
      }
      *verb_array_token = *current_token;
      current_token++;
      verb_array_token++;
    }
    current_token = parse_items(current_token, item_array_token);
  }
  else if (current_token->type == AT)
  {
    *verb_array_token = *current_token;
    current_token++;
    verb_array_token++;
    *location_array_token = *current_token;
    current_token++;
    location_array_token++;
  }
  else
  {
    return NULL;
  }

  if (current_token->type != END && current_token->type != AND)
  {
    return NULL;
  }

  verb_array_token->type = END;
  to_from_array_token->type = END;
  location_array_token->type = END;
  return current_token;
}

sentence *parse_sentence(token *tokens)
{
  sentence *sentences = malloc(MAX * sizeof(sentence));
  sentence *current_sentence = &sentences[0];
  token *current_token = &tokens[0];

  while (current_token->type != END)
  {
    init_sentence(current_sentence, ACTION);
    current_token = parse_action(current_token, current_sentence);
    if (current_token == NULL)
    {
      return NULL;
    }
    current_sentence++;
    while (current_token->type == AND)
    {
      current_token++;
      init_sentence(current_sentence, ACTION);
      current_token = parse_action(current_token, current_sentence);
      if (current_token == NULL)
      {
        return NULL;
      }
      current_sentence++;
    }

    if (current_token->type == IF)
    {
      current_token++;
      init_sentence(current_sentence, CONDITION);
      current_token = parse_condition(current_token, current_sentence);
      if (current_token == NULL)
      {
        return NULL;
      }
      current_sentence++;
      token *temp_current_token = current_token;
      while (temp_current_token != NULL && temp_current_token->type == AND)
      {
        temp_current_token++;
        current_token++;
        init_sentence(current_sentence, CONDITION);
        temp_current_token = parse_condition(temp_current_token, current_sentence);
        if (temp_current_token == NULL)
        {
          break;
        }
        current_token = temp_current_token;
        current_sentence++;
      }
    }
  }
  current_sentence->type = END_SENTENCE;
  return sentences;
}

char *parse_question(token *tokens)
{
  if (tokens[0].type == WHO && tokens[1].type == AT && tokens[2].type == ENTITIY && tokens[3].type == QUESTION_MARK && tokens[4].type == END)
  {
    return tokens[2].value;
  }
  if (tokens[0].type == ENTITIY && tokens[1].type == WHERE && tokens[2].type == QUESTION_MARK && tokens[3].type == END)
  {
    return tokens[0].value;
  }
  token *current_token = &tokens[0];
  token *subject_array_token = malloc(MAX * sizeof(token));
  current_token = parse_subjects(current_token, subject_array_token);
  if (current_token != NULL && (current_token)->type == TOTAL && (current_token + 1)->type == ENTITIY && (current_token + 2)->type == QUESTION_MARK && (current_token + 3)->type == END)
  {
    return (current_token + 2)->value;
  }
  return NULL;
}
