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

void print_list(llist * list, char * pre) {
  llist * node = list;
  printf("%s: ", pre);
  if (node != NULL) 
    while (node != NULL) {
      printf("%c",node->value);

      if (node->next != NULL) 
        printf(",");

      node = node->next;
    }
  printf("\n");
}

int llist_count(llist *head) {
  int cnt = (head) ? 1 : 0; 
  llist * p = head;

  while(p->next) {
    p = p->next;
    cnt++;
  }

  return cnt;
}

llist * pop(llist ** head) {
  llist *node;

  if ((*head)->tail != (*head)) {
    node = (*head)->tail;

    if ((*head)->tail->prev)
      (*head)->tail->prev->next = NULL;

    (*head)->tail = (*head)->tail->prev;

    return node;
  } else {
    node = (*head);

    (*head) = NULL;
    
    return node;
  }
}

llist *pop_stack(llist ** head, int n) {
  llist *node;
  llist *tail;

  printf("0x%x\n", head);
  int cnt = llist_count(*head);

  if (n > cnt) printf("!!!!! ");
  printf("%d req, %d avail\n", n, cnt);

  if (n == 1 || (*head)->tail == (*head)) {
    printf("drop to pop\n");
    printf("drop s = %x p = %x, n = %x, t = %x, v = %c\n", (*head), (*head)->prev, (*head)->next, (*head)->tail, (*head)->value);
    return pop(head);
  }

  tail = (*head)->tail;

  printf("@@tail ss = %x\n", tail);
  for (int i = 0; i < n; i++) {
    printf("##head s = %x p = %x, n = %x, t = %x, v = %c\n", (*head), (*head)->prev, (*head)->next, (*head)->tail, (*head)->value);
    node = (*head)->tail; 
    printf("!!node s = %x p = %x, n = %x, t = %x, v = %c\n", node, node->prev, node->next, node->tail, node->value);
    (*head)->tail = node->prev;
  }

  if (n == cnt)
    (*head) = NULL;
  else
    (*head)->tail->next = NULL;

  node->prev = NULL;
  node->tail = tail;

  printf("N p = %x, n = %x, t = %x, v = %c\n", node->prev, node->next, node->tail, node->value);
  print_list(node, "NODE");

  return node;
}

void add_stack(llist ** head, llist * e) {
  if (*head) {
    e->prev = (*head)->tail;
    (*head)->tail->next = e;
    (*head)->tail = e->tail;
    e->tail = NULL;
  } else {
    (*head) = e;
    /*
    (*head)->tail = e;
    (*head)->tail = e->tail;
    e->tail = e;
    e->prev = NULL;
    e->tail->next = NULL;
    */
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
    (*head)->prev = NULL;
    (*head)->next = NULL;
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

    llist *node = NULL;
    printf("=============================\n");
    printf("=>>>>> move %d to %d from %d\n", units, dest, source);
    print_list(stack[source-1], "SOURCE");
    print_list(stack[dest-1], "DEST");

    if (units == 1) {
      node = pop(&stack[source-1]);
      add_top(&stack[dest-1], node);
    } else {
      node = pop_stack(&stack[source-1], units);
      add_stack(&stack[dest-1], node);
    }

    print_list(stack[source-1], "SOURCE");
    print_list(stack[dest-1], "DEST");
  }

  for (int i = 0; i < 9; i++) {
    if(stack[i])
      printf("%c", stack[i]->tail->value);
    else
      printf(" ");
  }

  printf("\n");
  
  return 0;
}
