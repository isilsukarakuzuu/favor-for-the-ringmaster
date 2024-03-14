#include <stdio.h>

#include "tokenizer.h"
#include "parser.h"

//TODO: answer questions, get data from sentences, store data

int main()
{
  char input[MAX];
  while (true)
  {
    printf(">> ");
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
        print_sentence(sentences);
        printf("OK\n");
      }
    }
  }
}