#include "subjects.h"

subject subjects[MAX_SUBJECTS];

int find_subject(char *name)
{
  int i = 0;
  while (subjects[i].name[0] != '\0')
  {
    if (strcmp(subjects[i].name, name) == 0)
    {
      return i;
    }
    i++;
  }
  return -1;
}

int find_item(char *name, item *current_items)
{
  int i = 0;
  while (current_items[i].name[0] != '\0')
  {
    if (strcmp(current_items[i].name, name) == 0)
    {
      return i;
    }
    i++;
  }
  return -1;
}

void place_subjects(token *subject_array_token)
{
  // traverse all subjects using subject_array_token and if the subject is not found, initialize it
  while (subject_array_token->type != END)
  {
    int index = find_subject(subject_array_token->value);
    if (index == -1)
    {
      int i = 0;
      while (subjects[i].name[0] != '\0')
      {
        i++;
      }
      strcpy(subjects[i].name, subject_array_token->value);
    }
    subject_array_token++;
  }
}

void execute_updates()
{
  // copy temp values to current values
  int i = 0;
  while (subjects[i].name[0] != '\0')
  {
    strcpy(subjects[i].location, subjects[i].temp_location);
    int j = 0;
    while (subjects[i].temp_current_items[j].name[0] != '\0')
    {
      strcpy(subjects[i].current_items[j].name, subjects[i].temp_current_items[j].name);
      subjects[i].current_items[j].quantity = subjects[i].temp_current_items[j].quantity;
      j++;
    }
    i++;
  }
}

void delete_updates()
{
  // copy current values to temp values
  int i = 0;
  while (subjects[i].name[0] != '\0')
  {
    strcpy(subjects[i].temp_location, subjects[i].location);
    int j = 0;
    while (subjects[i].temp_current_items[j].name[0] != '\0')
    {
      strcpy(subjects[i].temp_current_items[j].name, subjects[i].current_items[j].name);
      subjects[i].temp_current_items[j].quantity = subjects[i].current_items[j].quantity;
      j++;
    }
    i++;
  }
}

void print_subjects()
{
  int i = 0;
  while (subjects[i].name[0] != '\0')
  {
    printf("Subject: %s\n", subjects[i].name);
    printf("Location: %s\n", subjects[i].location);
    int j = 0;
    while (subjects[i].current_items[j].name[0] != '\0')
    {
      printf("Item: %s\n", subjects[i].current_items[j].name);
      printf("Quantity: %d\n", subjects[i].current_items[j].quantity);
      j++;
    }
    printf("***********\n");
    i++;
  }
}

void init_items(item items[])
{
  int i = 0;
  while (i < MAX_ITEMS)
  {
    for (int j = 0; j < MAX_WORD_LENGTH; j++)
    {
      items[i].name[j] = '\0';
    }
    items[i].quantity = 0;
    i++;
  }
}

void init_subjects()
{
  int i = 0;
  while (i < MAX_SUBJECTS)
  {
    for (int j = 0; j < MAX_WORD_LENGTH; j++)
    {
      subjects[i].name[j] = '\0';
      subjects[i].location[j] = '\0';
      subjects[i].temp_location[j] = '\0';
    }
    init_items(subjects[i].current_items);
    init_items(subjects[i].temp_current_items);
    i++;
  }
}
