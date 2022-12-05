#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

size_t line_count(FILE *fp) {
  char *line = NULL;
  size_t linecap = 0;
  size_t lines = 0;

  while (getline(&line, &linecap, fp) != -1) {
    lines++;
  }
  rewind(fp);
  return lines;
}

typedef struct llist llist;

struct llist {
  llist *tail;
  llist *next;
  llist *prev;

  char value;
};

llist * pop(llist ** head) {
  if ((*head)->tail != (*head)) {
    llist *node = (*head)->tail;
    if ((*head)->tail->prev)
      (*head)->tail->prev->next = NULL;
    (*head)->tail = (*head)->tail->prev;
    return node;
  } else {
    llist *node = (*head);
    (*head) = NULL;
    return node;
  }
}

void add_top(llist ** head, llist * e) {
  if (*head) {
    e->prev = (*head)->tail;
    (*head)->tail->next = e;
    (*head)->tail = e;
  } else {
    (*head) = e;
    (*head)->tail = e;
  }
}

void push_bot(llist ** head, llist * e) {
  if (*head) {
    e->next = (*head);
    e->prev = NULL;
    e->tail = (*head)->tail;
    (*head)->prev = e;
  }
  (*head) = e;
}

void print_list(llist * list) {
  llist * node = list;
  if (node != NULL)
    while (node != NULL) {
      printf("%c",node->value);

      if (node->next != NULL) 
        printf(",");

      node = node->next;
    }
  printf("\n");
}

int main() {
  FILE *fp = fopen("data.txt", "r");

  size_t lc = line_count(fp);
  size_t linecap = 0;
  size_t llen = 0;

  char *line = NULL;

  char stack_map[64][64];

  for (int i = 0; i < 64; i++) {
    memset(stack_map[i], -1, sizeof(char)*64);
  }

  int y;
  int x;
  int k = 0;

  int col_pos[9];

  for (y = 0; (llen = getline(&line, &linecap, fp)) != -1 && line[0] != '\n'; y++) {
    for (x = 0; x < llen; x++) {
      stack_map[y][x] = line[x];
      
      if (isdigit(line[x])) {
        col_pos[k] = x;
        k++;
      }
    }
  }  

  llist *stack[9];

  for (int j = 0; j < sizeof(col_pos)/sizeof(int); j++) {
    stack[j] = NULL;

    for (int i = 0; i < y-1; i++) {
      char c = stack_map[i][col_pos[j]];
      if (isalpha(c)) {
        llist *node;
        node = (llist *)malloc(sizeof(llist));

        node->next = NULL;
        node->prev = NULL;
        node->tail = node;
        node->value = c;

        push_bot(&stack[j], node);
      }
    }
  }

  
  while ((llen = getline(&line, &linecap, fp)) != -1) {
    int units = -1;
    int source = -1;
    int dest = -1;

    char *word;
    char *bkrt;
    word = strtok_r(line, " ", &bkrt);
    while (word != NULL) {
      if ((strcmp("move", word) && strcmp("from", word) && strcmp("to", word) && strcmp(" ", word))) {
        if (units == -1)
          units = atoi(word);
        else if (source == -1)
          source = atoi(word);
        else
          dest = atoi(word);
      }
      word = strtok_r(NULL, " ", &bkrt);
    }

    for (; units > 0; units--) {
      llist *node = pop(&stack[source-1]);
      add_top(&stack[dest-1], node);
    }
  }
  for (int i = 0; i < 9; i++) {
    printf("%c", stack[i]->tail->value);
  }
  printf("\n");
  
  return 0;
}
