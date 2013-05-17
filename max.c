#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
#define A1 "abc"
#define A5 "abcdefghklmnoprst"
#define ALPH A5
#define PSWD_LEN 5
char *hash;

typedef enum {BM_ITER, BM_REC} brute_mode_t;
typedef struct {
  char *alphabet;
  char password[PSWD_LEN + 1];
  int passwordLength;
  int alphLength;
  brute_mode_t brute_mode;
} context_t;

void iterative (context_t *context) {
  int letterIndex[context->passwordLength];

  context->password[context->passwordLength] = 0 ;
  int i = 0;

  for (i = 0;i < context->passwordLength;++i) {
    letterIndex[i] = 0;
  }
  int j = 0;
  for (i=0;j<context->passwordLength;++i){
    for(j = 0;j < context->passwordLength;++j) {
      context->password[j] = context->alphabet[letterIndex[j]];
    }
    if(!strcmp(crypt(context->password,hash), hash))
    printf("%s \n",context->password);
    j = 0;
    
    while (j < context->passwordLength) {
      letterIndex[j]++;
      if (letterIndex[j] == context->alphLength) {
        letterIndex[j] = 0;
        ++j;
      } else break;
    }
  }
}
 
void recursive (context_t *context, int pos) {
  if (pos == context->passwordLength){
      if(!strcmp(crypt(context->password,hash), hash))
      printf("%s\n", context->password);
      return;
  }
  int i;
  for (i = 0; i < context->alphLength; i++) {
      context->password[pos] = context->alphabet[i];
      recursive(context, pos + 1);
  }
  return;
}

void brute_all(context_t *context) {
  switch (context->brute_mode) {
    case BM_REC:
      printf ("Рекурсивный алгоритм запущен\n");
      recursive(context, 0);
      break;
    case BM_ITER:
      printf ("Итеративный алгоритм запущен\n");
      iterative(context);
      break;
  }
}

void parse_args(context_t *context, int argc, char *argv[]) {
  int opt;
  int optint=1;
  while ((opt = getopt(argc, argv, "ri")) != -1) {
    optint++;
    switch( opt ) {
      case 'r':
        context->brute_mode = BM_REC;
        break;
      case 'i':
        context->brute_mode = BM_ITER;
        break;
    }
  }
  if (argc==optint){
    printf("Введите хэш\n");
    exit(1);    
    }	
    else {
    hash = argv[optint];
    }
}


int main(int argc, char * argv[]){
  context_t context = {
  .alphabet = ALPH,
  .passwordLength = PSWD_LEN,
  .alphLength = (int) strlen(ALPH),
 };
  parse_args(&context, argc, argv);
  brute_all(&context);
  return (EXIT_SUCCESS);
}
