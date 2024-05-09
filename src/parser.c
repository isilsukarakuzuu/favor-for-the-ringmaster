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
  sentence->subjects = malloc(MAX_SUBJECTS * sizeof(token));
  sentence->verbs = malloc(5 * sizeof(token));
  sentence->items = malloc(MAX_ITEMS * sizeof(token));
  sentence->to_from = malloc(5 * sizeof(token));
  sentence->location = malloc(5 * sizeof(token));

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
  else if (current_token->type == GO && (current_token + 1)->type == TO && (current_token + 2)->type == ENTITIY)
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

bool check_duplicates(sentence *sentences)
{
  // check for duplicate names in subjects
  int i = 0;
  while (sentences[i].type != END_SENTENCE)
  {
    int j = 0;
    while (sentences[i].subjects[j].type != END)
    {
      int k = j + 1;
      while (sentences[i].subjects[k].type != END)
      {
        if (strcmp(sentences[i].subjects[j].value, sentences[i].subjects[k].value) == 0)
        {
          return true;
        }
        k++;
      }
      j++;
    }

    // check for duplicate names in to_from and subjects
    j = 0;
    while (sentences[i].to_from[j].type != END)
    {
      int k = 0;
      while (sentences[i].subjects[k].type != END)
      {
        if (strcmp(sentences[i].to_from[j].value, sentences[i].subjects[k].value) == 0)
        {
          return true;
        }
        k++;
      }
      j++;
    }
    // check for duplicate items
    j = 1;
    while (sentences[i].items[j - 1].type != END && sentences[i].items[j].type != END)
    {
      int k = j + 2;
      while (sentences[i].items[k - 1].type != END && sentences[i].items[k].type != END)
      {
        if (strcmp(sentences[i].items[j].value, sentences[i].items[k].value) == 0)
        {
          return true;
        }
        k += 2;
      }
      j += 2;
    }
    i++;
  }
  return false;
}

sentence *parse_sentence(token *tokens)
{
  sentence *sentences = malloc(MAX/8 * sizeof(sentence));
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
  if (check_duplicates(sentences))
  {
    return NULL;
  }
  return sentences;
}

char *parse_question(token *tokens)
{
  char *question = calloc(MAX, sizeof(char));
  if (tokens[0].type == WHO && tokens[1].type == AT && tokens[2].type == ENTITIY && tokens[3].type == QUESTION_MARK && tokens[4].type == END)
  {
    // iterate through subjects and find the ones with the location
    int i = 0;
    while (subjects[i].name[0] != '\0')
    {
      if (strcmp(subjects[i].location, tokens[2].value) == 0)
      {
        strcat(question, subjects[i].name);
        strcat(question, " and ");
      }
      i++;
    }
    // remove the last " and "
    question[strlen(question) - 5] = '\0';
    if (strlen(question) == 0)
    {
      return "NOBODY";
    }
    return question;
  }

  if (tokens[0].type == ENTITIY && tokens[1].type == WHERE && tokens[2].type == QUESTION_MARK && tokens[3].type == END)
  {
    // find token[0].value in subjects and return the location, if not found return "NOWHERE"
    int ind = find_subject(tokens[0].value);
    if (ind == -1)
    {
      return "NOWHERE";
    }
    if (strcmp(subjects[ind].location, "") == 0)
    {
      return "NOWHERE";
    }
    return subjects[ind].location;
  }
  token *current_token = &tokens[0];
  token *subject_array_token = malloc(MAX_SUBJECTS * sizeof(token));
  current_token = parse_subjects(current_token, subject_array_token);
  if (current_token != NULL && (current_token)->type == TOTAL && (current_token + 1)->type == ENTITIY && (current_token + 2)->type == QUESTION_MARK && (current_token + 3)->type == END)
  {
    int ans = 0;
    // iterate through subject array token and find the one with the name and return the total quantity
    int i = 0;
    while (subject_array_token[i].type != END)
    {
      int ind = find_subject(subject_array_token[i].value);
      if (ind != -1)
      {
        int item_index = find_item((current_token + 1)->value, subjects[ind].current_items);
        if (item_index != -1)
        {
          ans += subjects[ind].current_items[item_index].quantity;
        }
      }
      i++;
    }
    // convert ans to string
    sprintf(question, "%d", ans);
    return question;
  }
  if (current_token != NULL && (current_token)->type == TOTAL && (current_token + 1)->type == QUESTION_MARK && (current_token + 2)->type == END)
  {
    // if more than 1 subject, return null
    int i = 0;
    while (subject_array_token[i].type != END)
    {
      i++;
    }
    if (i > 1)
    {
      return NULL;
    }
    int ind = find_subject(subject_array_token[0].value);
    if (ind == -1)
    {
      return "NOTHING";
    }
    // print all items and their quantities without 0 quantities with " and " in between
    int j = 0;
    while (subjects[ind].current_items[j].name[0] != '\0')
    {
      if (subjects[ind].current_items[j].quantity != 0)
      {
        // first quantity then item name
        sprintf(question, "%s and %d %s", question, subjects[ind].current_items[j].quantity, subjects[ind].current_items[j].name);
      }
      j++;
    }
    // remove the first " and "
    question += 5;
    if (strlen(question) == 0)
    {
      return "NOTHING";
    }
    return question;
  }
  return NULL;
}
