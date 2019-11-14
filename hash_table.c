#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_TAM 4

typedef struct aluno
{
  int mat;
  float cr;
  struct aluno *prox;
}TA;

typedef TA *Hash[MAX_TAM];

int hash(int mat, int n)
{
  return mat%n;
}

void inicializa(Hash tab, int n)
{
  int i;
  for(i = 0; i < n; i++)
    tab[i] = NULL;
}

TA *busca(Hash tab, int n, int mat)
{
  int ind = hash(mat, n);
  TA *resp = tab[ind];
  while((resp) && (resp->mat < mat))
    resp = resp->prox;
  if((resp) && (resp->mat == mat))
    return resp;
  else
    return NULL;
}



void insere_ord(Hash tab, int n, int mat, float cr)
{
  int ind = hash(mat, n);
  TA *ant = NULL, *p = tab[ind];
  while((p) && (p->mat < mat))
  {
    ant = p;
    p = p->prox;
  }
  if((p) && (p->mat == mat))
  {
    p->cr = cr;
    return;
  }
  TA *novo = (TA*)malloc(sizeof(TA));
  novo->mat = mat;
  novo->cr = cr;
  if(!ant)
    tab[ind] = novo;
  else
    ant->prox = novo;
  novo->prox = p;
}

float retira(Hash tab, int n, int mat)
{
  int ind = hash(mat, n);
  TA *ant = NULL, *p = tab[ind];
  while((p) && (p->mat < mat))
  {
    ant = p;
    p = p->prox;
  }
  if((!p) || (p->mat > mat))
    return INT_MIN;
  if(!ant)
    tab[ind] = p->prox;
  else
    ant->prox = p->prox;
  float cr = p->cr;
  free(p);
  return cr;
}

void libera(Hash tab, int n)
{
  if(tab)
  {
    int i;
    for(i = 0; i < n; i++)
    {
      TA *p = tab[i];
      while(p)
      {
        TA *temp = p;
        p = p->prox;
        free(temp);
      }
      tab[i]=NULL;
    }
  }
}

void imprime_hash(Hash tab, int n)
{
  if(tab)
  {
    int i;
    for(i = 0; i < n; i++)
    {
      TA *p = tab[i];
      while(p)
      {
        printf("[%d|%.1f]->", p->mat, p->cr);
        p = p->prox;
      }
      printf("\n------------------------------------------------------------------------\n");
    }
  }
}

void remove_todos_x(Hash tab, int n, int x)
{
  if(tab)
  {
    int i;
    for(i = 0; i < n; i++)
    {
      TA *p = tab[i], *ant = NULL;
      while(p)
      {
        if(p->cr == x)
        {
          if(!ant)
            tab[i] = p->prox;
          else
            ant->prox = p->prox;
          TA *temp = p;
          p = p->prox;
          free(temp);
        }
        else
        {
          ant = p;
          p = p->prox;
        }
      }
    }
  }
}

void remove_grupo(Hash tab, int n)
{
  if(tab)
  {
    int i;
    for(i = 0; i < n; i++)
    {
      TA *p = tab[i], *ant = NULL;
      while(p)
      {
        if(!(p->mat%2))
        {
          TA *temp = p;
          if(!ant)
            tab[i] = p->prox;
          else
            ant->prox = p->prox;
          p = p->prox;
          free(temp);
        }
        else
        {
          ant = p;
          p = p->prox;
        }
      }
    }
  }
}

void remove_ant_dep(Hash tab, int n , int mat)
{
  int ind = hash(mat, n);
  TA *p = tab[ind], *ant = NULL;
  while(p)
  {
    if(p->mat == mat)
    {
      if(ant)
        retira(tab, n, ant->mat);
      if(p->prox)
        retira(tab, n, p->prox->mat);
    }
    ant = p;
    p = p->prox;
  }
}

void remove_antes_depois(Hash tab, int n, int x)
{
  if(tab)
  {
    int i;
    for(i = 0; i < n; i++)
    {
      TA *p = tab[i], *ant = NULL, *ant_ant = NULL;
      while(p)
      {
        if(p->mat == x)
        {
          if((p == tab[i]) || (p->prox == NULL))
          {
            ant_ant = ant;
            ant = p;
            p = p->prox;
          }
          else if(ant == tab[i])
          {
            ant->mat = p->mat;
            ant->cr = p->cr;

            TA *temp1 = p;
            TA *temp2 = p->prox;
            p = p->prox->prox;
            
            ant->prox = p;
            free(temp1);
            free(temp2);

          }
          else
          {
            TA *temp1 = ant;
            TA *temp2 = p->prox;

            ant = p;
            p = p->prox->prox;
            ant_ant->prox = ant;
            ant->prox = p;

            free(temp1);
            free(temp2);
          }
        }
        else
        {
          ant_ant = ant;
          ant = p;
          p = p->prox;
        }
      }
    }
  }
}

TA **nova_tabela(Hash tab, int n, float cr)
{
  TA **nova = (TA**)malloc(n*sizeof(TA*));
  int i;
  for(i = 0; i < n; i++)
    nova[i] = NULL;
  for(i = 0; i < n; i++)
  {
    TA *p = tab[i];
    while(p)
    {
      if(p->cr >=  cr)
      {
        insere_ord(nova, n, p->mat, p->cr);
        int aux = p->mat;
        p = p->prox;
        retira(tab, n, aux);
      }
      else
        p = p->prox;
    }
  }
  return nova;
}

int main(void) {

  Hash tab;
  inicializa(tab, MAX_TAM);
  int i;
  for(i=0; i <= 30; i++)
    insere_ord(tab, MAX_TAM, i, i*0.5);
  
  imprime_hash(tab, MAX_TAM);

  TA **resp = nova_tabela(tab, MAX_TAM, 6.0);
  imprime_hash(tab, MAX_TAM);

  printf("\n****************************\n");
  imprime_hash(resp, MAX_TAM);




}
