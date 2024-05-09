#include <stdio.h>

#include "parser.h"

void update_subjects(sentence *current_sentence)
{
  token *subjects_array = current_sentence->subjects;
  token *verbs = current_sentence->verbs;
  token *items = current_sentence->items;
  token *to_from = current_sentence->to_from;
  token *location = current_sentence->location;

  // check whether the verb arrays first element is sell, buy, or go
  if (verbs->type == SELL)
  {
    if ((verbs + 1)->type == TO)
    {
      // check if all subjects has enough of each item to sell
      while (subjects_array->type != END)
      {
        int index = find_subject(subjects_array->value);
        int i = 1;
        while (items[i - 1].type != END && items[i].type != END)
        {
          int item_index = find_item(items[i].value, subjects[index].temp_current_items);
          if (item_index == -1)
          {
            return;
          }
          else
          {
            int quantity = atoi(items[i - 1].value);
            if (subjects[index].temp_current_items[item_index].quantity < quantity)
            {
              return;
            }
          }
          i += 2;
        }
        subjects_array++;
      }
      subjects_array = current_sentence->subjects;
      // iterate through the subjects array and update their temp current items
      // find to_from value and update the subjects location
      int to_from_index = find_subject(to_from->value);

      while (subjects_array->type != END)
      {
        int index = find_subject(subjects_array->value);
        int i = 1;
        while (items[i - 1].type != END && items[i].type != END)
        {
          int item_index = find_item(items[i].value, subjects[index].temp_current_items);
          int quantity = atoi(items[i - 1].value);
          subjects[index].temp_current_items[item_index].quantity -= quantity;
          int to_from_item_index = find_item(items[i].value, subjects[to_from_index].temp_current_items);
          if (to_from_item_index != -1)
          {
            subjects[to_from_index].temp_current_items[to_from_item_index].quantity += quantity;
          }
          else
          {
            // add the item to the to_from subject
            int to_from_item_index = 0;
            while (subjects[to_from_index].temp_current_items[to_from_item_index].name[0] != '\0')
            {
              to_from_item_index++;
            }
            strcpy(subjects[to_from_index].temp_current_items[to_from_item_index].name, items[i].value);
            subjects[to_from_index].temp_current_items[to_from_item_index].quantity = quantity;
          }
          i += 2;
        }
        subjects_array++;
      }
    }
    else
    {
      // iterate through the subjects array and update their current items
      while (subjects_array->type != END)
      {
        int index = find_subject(subjects_array->value);
        int i = 1;
        while (items[i - 1].type != END && items[i].type != END)
        {
          int item_index = find_item(items[i].value, subjects[index].temp_current_items);
          if (item_index != -1)
          {
            int quantity = atoi(items[i - 1].value);
            subjects[index].temp_current_items[item_index].quantity -= quantity;
            if (subjects[index].temp_current_items[item_index].quantity < 0)
            {
              subjects[index].temp_current_items[item_index].quantity += quantity;
              delete_updates();
              return;
            }
          }
          i += 2;
        }
        subjects_array++;
      }
    }
  }
  else if (verbs->type == BUY)
  {
    if ((verbs + 1)->type == FROM)
    {
      // find how many subjects are there
      int subjects_count = 0;
      while (subjects_array->type != END)
      {
        subjects_count++;
        subjects_array++;
      }
      subjects_array = current_sentence->subjects;
      // check if to_from subject has enough of each item to sell
      int to_from_index = find_subject(to_from->value);
      int i = 1;
      while (items[i - 1].type != END && items[i].type != END)
      {
        int item_index = find_item(items[i].value, subjects[to_from_index].temp_current_items);
        if (item_index == -1)
        {
          return;
        }
        else
        {
          int quantity = atoi(items[i - 1].value);
          if (subjects[to_from_index].temp_current_items[item_index].quantity < subjects_count * quantity)
          {
            return;
          }
        }
        i += 2;
      }
      subjects_array = current_sentence->subjects;
      // iterate through the subjects array and update their temp current items
      while (subjects_array->type != END)
      {
        int index = find_subject(subjects_array->value);
        int i = 1;
        while (items[i - 1].type != END && items[i].type != END)
        {
          int item_index = find_item(items[i].value, subjects[index].temp_current_items);
          int to_from_item_index = find_item(items[i].value, subjects[to_from_index].temp_current_items);
          int quantity = atoi(items[i - 1].value);
          subjects[to_from_index].temp_current_items[to_from_item_index].quantity -= quantity;
          if (item_index == -1)
          {
            item_index = 0;
            while (subjects[index].temp_current_items[item_index].name[0] != '\0')
            {
              item_index++;
            }
            strcpy(subjects[index].temp_current_items[item_index].name, items[i].value);
            subjects[index].temp_current_items[item_index].quantity = quantity;
          }
          else
          {
            subjects[index].temp_current_items[item_index].quantity += quantity;
          }
          i += 2;
        }
        subjects_array++;
      }
    }
    else
    {
      // iterate through the subjects array and update their current items
      while (subjects_array->type != END)
      {
        int index = find_subject(subjects_array->value);
        int i = 1;
        while (items[i - 1].type != END && items[i].type != END)
        {
          int item_index = find_item(items[i].value, subjects[index].temp_current_items);
          if (item_index == -1)
          {
            item_index = 0;
            while (subjects[index].temp_current_items[item_index].name[0] != '\0')
            {
              item_index++;
            }
            strcpy(subjects[index].temp_current_items[item_index].name, items[i].value);
            subjects[index].temp_current_items[item_index].quantity = atoi(items[i - 1].value);
          }
          else
          {
            int quantity = atoi(items[i - 1].value);
            subjects[index].temp_current_items[item_index].quantity += quantity;
          }
          i += 2;
        }
        subjects_array++;
      }
    }
  }
  else if (verbs->type == GO)
  {
    // iterate through the subjects array and update their location
    while (subjects_array->type != END)
    {
      int index = find_subject(subjects_array->value);
      strcpy(subjects[index].temp_location, location->value);
      subjects_array++;
    }
  }
}

bool check_condition(sentence *current_sentence)
{
  token *subjects_array = current_sentence->subjects;
  token *verbs = current_sentence->verbs;
  token *items = current_sentence->items;
  token *to_from = current_sentence->to_from;
  token *location = current_sentence->location;

  if (verbs->type == HAS && (verbs + 1)->type == MORE && (verbs + 2)->type == THAN && (verbs + 3)->type == END)
  {
    while (subjects_array->type != END)
    {
      int index = find_subject(subjects_array->value);
      int i = 1;
      while (items[i - 1].type != END && items[i].type != END)
      {
        int item_index = find_item(items[i].value, subjects[index].current_items);
        if (item_index == -1)
        {
          return false;
        }
        if (atoi(items[i - 1].value) >= subjects[index].current_items[item_index].quantity)
        {
          return false;
        }
        i += 2;
      }
      subjects_array++;
    }
    return true;
  }
  if (verbs->type == HAS && (verbs + 1)->type == LESS && (verbs + 2)->type == THAN && (verbs + 3)->type == END)
  {
    while (subjects_array->type != END)
    {
      int index = find_subject(subjects_array->value);
      int i = 1;
      while (items[i - 1].type != END && items[i].type != END)
      {
        int item_index = find_item(items[i].value, subjects[index].current_items);
        if (item_index == -1)
        {
          if (atoi(items[i - 1].value) == 0)
          {
            return false;
          }
          i += 2;
          continue;
        }
        if (atoi(items[i - 1].value) <= subjects[index].current_items[item_index].quantity)
        {
          return false;
        }
        i += 2;
      }
      subjects_array++;
    }
    return true;
  }
  if (verbs->type == HAS && (verbs + 1)->type == END)
  {
    // iterate through the subjects array and check if the subjects have the items
    while (subjects_array->type != END)
    {
      int index = find_subject(subjects_array->value);
      int i = 1;
      while (items[i - 1].type != END && items[i].type != END)
      {
        int item_index = find_item(items[i].value, subjects[index].current_items);
        if (item_index == -1)
        {
          if (atoi(items[i - 1].value) != 0)
          {
            return false;
          }
          i += 2;
          continue;
        }
        if (atoi(items[i - 1].value) != subjects[index].current_items[item_index].quantity)
        {
          return false;
        }
        i += 2;
      }
      subjects_array++;
    }
    return true;
  }
  if (verbs->type == AT && (verbs + 1)->type == END)
  {
    // iterate through the subjects array and check if the subjects are at the location
    while (subjects_array->type != END)
    {
      int index = find_subject(subjects_array->value);
      if (strcmp(subjects[index].location, location->value) != 0)
      {
        return false;
      }
      subjects_array++;
    }
    return true;
  }
  return false;
}

void evaluate_sentences(sentence *sentences)
{
  sentence *current_sentence = sentences;
  while (current_sentence->type != END_SENTENCE)
  {
    while (current_sentence->type == ACTION)
    {
      place_subjects(current_sentence->subjects);
      place_subjects(current_sentence->to_from);
      update_subjects(current_sentence);
      current_sentence++;
    }
    bool condition = true;
    while (current_sentence->type == CONDITION)
    {
      condition = condition & check_condition(current_sentence);
      current_sentence++;
    }
    if (condition)
    {
      execute_updates();
    }
    else
    {
      delete_updates();
    }
  }
}

int main()
{
  char input[MAX];
  init_subjects();

  while (true)
  {
    fflush(stdout);
    printf(">> ");
    fflush(stdout);
    fgets(input, MAX, stdin);
    input[strcspn(input, "\n")] = 0;

    token *tokens = tokenize(input);

    if (tokens == NULL)
    {
      printf("INVALID\n");
      continue;
    }

    if (tokens[0].type == EXIT)
    {
      if (tokens[1].type == END)
        break;
      printf("INVALID\n");
      continue;
    }

    int is_question = find_first_token(tokens, QUESTION_MARK);

    // if the last token is a question mark, its a question
    if (is_question != -1 && tokens[is_question + 1].type == END)
    {
      char *answer = parse_question(tokens);
      if (answer == NULL)
      {
        printf("INVALID\n");
      }
      else
      {
        printf("%s\n", answer);
      }
    }

    // if the last token is not a question mark, its a sentence
    else
    {
      sentence *sentences = parse_sentence(tokens);
      if (sentences == NULL)
      {
        printf("INVALID\n");
      }
      else
      {
        evaluate_sentences(sentences);
        printf("OK\n");
      }
    }
  }
}