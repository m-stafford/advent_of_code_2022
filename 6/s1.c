#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print_bits(unsigned long long n){
  unsigned long long i; 

  i = 1ULL << (sizeof(n)*8-1);

  while (i > 0) {
    if (n & i)
      printf("1"); 
    else 
      printf("0"); 

    i >>= 1;
  }
}

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

  int cnt = llist_count(*head);

  if (n == 1 || (*head)->tail == (*head)) {
    return pop(head);
  }

  tail = (*head)->tail;

  for (int i = 0; i < n; i++) {
    node = (*head)->tail; 
    (*head)->tail = node->prev;
  }

  if (n == cnt)
    (*head) = NULL;
  else
    (*head)->tail->next = NULL;

  node->prev = NULL;
  node->tail = tail;

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

void queue(llist ** head, llist * e) {
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

void push(llist ** head, llist * e) {
  if (*head) {
    e->next = (*head);
    e->prev = NULL;
    e->tail = (*head)->tail;
    (*head)->prev = e;
  }
  (*head) = e;
}

llist *dequeue(llist **head) {
  llist *node = NULL; 
  if (*head) {
    node = (*head);

    if ((*head)->tail != (*head)) {
      (*head)->next->tail = (*head)->tail;
      (*head) = (*head)->next;
      (*head)->prev = NULL;
    } else {
      (*head) = NULL;
    }
  }

  return node;
}


int main() {
  FILE *fp = fopen("data.txt", "r");

  size_t lc = line_count(fp);
  size_t linecap = 0;
  size_t llen = 0;

  char *line = NULL;

  
  while ((llen = getline(&line, &linecap, fp)) != -1) {
    unsigned int msg = 0x00000000;
    unsigned int set_bits;
    static const int S[] = {1, 2, 4, 8, 16};
    static const int B[] = {0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF};

    llist *stack = NULL;

    for (int i = 0; i < llen; i++) {
      llist *node = (llist *)malloc(sizeof(llist));
      
      node->value = line[i];
      node->next = NULL;
      node->prev = NULL;
      node->tail = node;

      queue(&stack, node);
    }
       
    int l_cnt = llist_count(stack);
    llist *ptr = stack;

    for (int i = 0; i < l_cnt; i++) {

      llist *sub_ptr = ptr;
      int j = 14;
      msg = 0x0;
      while(sub_ptr != NULL && j > 0) {
        msg |= 1 << (sub_ptr->value - 0x60);
        sub_ptr = sub_ptr->prev;
        j--;
      }
      // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
      set_bits = msg - ((msg >> 1) & B[0]);
      set_bits = ((set_bits >> S[1]) & B[1]) + (set_bits & B[1]);
      set_bits = ((set_bits >> S[2]) + set_bits) & B[2];
      set_bits = ((set_bits >> S[3]) + set_bits) & B[3];
      set_bits = ((set_bits >> S[4]) + set_bits) & B[4];
      if (set_bits == 14) {
        printf("msg: %d\n", i+1);
        break;
      }

      ptr = ptr->next;
    }
      
  }
  
  return 0;
}
