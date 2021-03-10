// "Copyright 2020 Fatu Miruna Maria"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct info {
  char key[100];
  int appear;
};

unsigned int hashFuction(void *a, unsigned int size) {
  unsigned char * puchar_a = (unsigned char *) a;
  unsigned int hash = 5381;
  int c;

  while ((c = * puchar_a++))
    hash = ((hash << 5u) + hash) + c;

  // ne asiguram ca hash < size
  hash = hash % size;
  return hash;
}

void htInit(struct info * ht, unsigned int size) {
  unsigned int i;

  // se initializeaza fiecare camp din hashtable
  for (i = 0; i < size; i++) {
    memset(ht[i].key, '\0', sizeof(ht -> key));
    ht[i].appear = 0;
  }
}

void htPair(struct info * ht, char * key, unsigned int index) {
    int lg1 = strlen(key);

    // se actualizeaza cheia si valoarea
    snprintf(ht[index].key, lg1+1, "%s", key);
    ht[index].appear++;
}

void htCreate(struct info * ht, char * key, unsigned int size) {
  unsigned int index = hashFuction(key, size);
  int value;
  value = ht[index].appear;

  // bucket-ul este gol
  if (value == 0) {
    htPair(ht, key, index);
  }

  if (value != 0) {
     // se verifica cheia
    if (strcmp(ht[index].key, key) == 0) {
      ht[index].appear++;
    } else {
      // iteram pana gasim un bucket gol
      while (ht[index].appear != 0) {
        index++;
        // daca s-a ajuns la finalul hashtable-ului reluam de la inceput
        if (index >= size) {
          index = 0;
        }
        // un bucket gol a fost gasit
        if (ht[index].appear == 0) {
          htPair(ht, key, index);
          break;
        }
      }
    }
  }
}

void printHt(struct info * ht, unsigned int size) {
  for (unsigned int k = 0; k < size; k++) {
    if (ht[k].appear != 0) {
      printf("%s ", ht[k].key);
      printf("%d\n", ht[k].appear);
    }
  }
}

int main() {
  unsigned int idx = 0;
  char ch1;
  char fisier[] = "fis";

  // se deschide fisierul din care se vor citi datele
  FILE *in;
  if ((in = fopen(fisier, "w+")) == NULL) {
        fprintf(stderr , "Can't open %s" , fisier);
        return -1;
    }

  char word[100];

  // inseram datele in fisier pentru a determina ht_size
  while (1) {
    scanf("%s", word);
    ch1 = getc(stdin);
    if (ch1 == '\n') idx++;
    if (ch1 == EOF) break;
    fprintf(in, "%s\n", word);
  }

  unsigned int ht_size = idx;
  struct info ht[ht_size];
  htInit(ht, ht_size);

  char new_string[100];

  fseek(in, 0, SEEK_SET);

  // citim datele din fisier pentru a le introduce in hashtable
  while (fscanf(in, "%s", new_string) != EOF){
    htCreate(ht, new_string, ht_size);
      }

  printHt(ht, ht_size);
  fclose(in);
  return 0;
}
