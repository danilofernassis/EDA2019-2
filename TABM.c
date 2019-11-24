/*
- retorna o anterior do elemento x;
- retorna o posterior do elemento x;
- imprime intervalo;
- imprime maiores que x;
- imprime menores que x;
- retorna lista de intervalo
- retorna pilha de intervalo
- retorna menor elemento
- retorna maior elemento
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "pilha.h"
#include "lista.h"

const int t = 2;

typedef struct arvbm{
  int nchaves, folha, *chave;
  struct arvbm **filho, *prox;
}TABM;


TABM *cria(int t){
  TABM* novo = (TABM*)malloc(sizeof(TABM));
  novo->nchaves = 0;
  novo->chave =(int*)malloc(sizeof(int)*((t*2)-1));
  novo->folha = 1;
  novo->filho = (TABM**)malloc(sizeof(TABM*)*t*2);
  novo->prox = NULL;
  int i;
  for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
  return novo;
}


TABM *inicializa(void){
  return NULL;
}

void libera(TABM *a){
  if(a){
    if(!a->folha){
      int i;
      for(i = 0; i <= a->nchaves; i++) libera(a->filho[i]);
    }
    free(a->chave);
    free(a);
  }
}

TABM *busca(TABM *a, int mat){
  if (!a) return NULL;
  int i = 0;
  while ((i < a->nchaves) && (mat > a->chave[i])) i++;
  if ((i < a->nchaves) && (a->folha) && (mat == a->chave[i])) return a;
  if (a-> folha) return NULL;
  if (a->chave[i] == mat) i++;
  return busca(a->filho[i], mat);
}

void imprime(TABM *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      imprime(a->filho[i],andar+1);
      for(j=0; j<=andar; j++) printf("   ");
      printf("%d\n", a->chave[i]);
    }
    imprime(a->filho[i],andar+1);
  }
}

TABM *divisao(TABM *x, int i, TABM* y, int t){
  TABM *z = cria(t);
  z->folha = y->folha;
  int j;
  if(!y->folha){
    z->nchaves = t-1;
    for(j=0;j<t-1;j++) z->chave[j] = y->chave[j+t];
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }
  else {
    z->nchaves = t; //z possuir� uma chave a mais que y se for folha
    for(j=0;j < t;j++) z->chave[j] = y->chave[j+t-1];//Caso em que y � folha, temos q passar a info para o n� da direita
    y->prox = z;
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
  x->chave[i-1] = y->chave[t-1];
  x->nchaves++;
  return x;
}


TABM *insere_nao_completo(TABM *x, int mat, int t){
  int i = x->nchaves-1;
  if(x->folha){
    while((i>=0) && (mat < x->chave[i])){
      x->chave[i+1] = x->chave[i];
      i--;
    }
    x->chave[i+1] = mat;
    x->nchaves++;
    return x;
  }
  while((i>=0) && (mat < x->chave[i])) i--;
  i++;
  if(x->filho[i]->nchaves == ((2*t)-1)){
    x = divisao(x, (i+1), x->filho[i], t);
    if(mat > x->chave[i]) i++;
  }
  x->filho[i] = insere_nao_completo(x->filho[i], mat, t);
  return x;
}

TABM *insere(TABM *T, int mat, int t){
  if(busca(T, mat)) return T;
  if(!T){
    T=cria(t);
    T->chave[0] = mat;
    T->nchaves=1;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TABM *S = cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    S = divisao(S,1,T,t);
    S = insere_nao_completo(S, mat, t);
    return S;
  }
  T = insere_nao_completo(T, mat, t);
  return T;
}

TLSE *junta_lista(TLSE *l1, TLSE *l2)
{
  if(!l1)
    return l2;
  if(!l2)
    return l1;
  TLSE *p = l1;
  while(p->prox)
    p = p->prox;
  p->prox = l2;
  return l1;
}

TLSE *lista_intervalo(TABM *a, int x, int y)
{
  if(!a)
    return NULL;
  TLSE *l = NULL;
  int i = 0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  while((i < a->nchaves)  && (a->chave[i] < y))
  {
    TLSE *aux = lista_intervalo(a->filho[i], x, y);
    l = junta_lista(aux, l);
    if(a->folha)
      l = lst_insere(l, a->chave[i]);
    i++;
  }
  TLSE *aux = lista_intervalo(a->filho[i], x, y);
  l = junta_lista(aux, l);
  return l;
}

TLSE *lista_maiores(TABM *a, int x)
{
  if(!a)
    return NULL;
  TLSE *l = NULL;
  int i=0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  while(i < a->nchaves)
  {
    TLSE *aux = lista_maiores(a->filho[i], x);
    l = junta_lista(aux, l);
    if(a->folha && (a->chave[i] > x))
      l = lst_insere(l, a->chave[i]);
    i++;
  }
  TLSE *aux = lista_maiores(a->filho[i], x);
  l = junta_lista(aux, l);
  return l;
}

void junta_pilha(TP *p1, TP *p2)
{
  if(vazia_pilha(p2))
    return;
  TP *aux = cria_pilha();
  while(!vazia_pilha(p2))
    push(aux, pop(p2));
  while(!vazia_pilha(aux))
    push(p1, pop(aux));
  libera_pilha(aux);
}

TP *pilha_intervalo(TABM *a, int x, int y)
{
  if(!a)
  {
    TP *vazia = cria_pilha();
    return vazia;
  }
  TP *p = cria_pilha();
  int i=0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  while((i < a->nchaves) && (a->chave[i] < y))
  {
    TP *aux = pilha_intervalo(a->filho[i], x, y);
    junta_pilha(p, aux);
    if(a->folha)
      push(p, a->chave[i]);
    libera_pilha(aux);
    i++;
  }
  TP *aux = pilha_intervalo(a->filho[i], x, y);
  junta_pilha(p, aux);
  libera_pilha(aux);
  return p;
}

void imprime_intervalo(TABM *a, int x, int y)
{
  if(!a)
    return;
  int i=0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  while((i < a->nchaves) && (a->chave[i] < y))
  {
    imprime_intervalo(a->filho[i], x, y);
    if(a->folha)
      printf("%d->", a->chave[i]);
    i++;
  }
  imprime_intervalo(a->filho[i], x, y);
}

void imprime_intervalo2(TABM *a, int x, int y)
{
  if(!a)
    return;
  int i = 0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  if(a->folha)
  {
    TABM *aux = a;
    while(aux)
    {
      int j;
      for(j = 0; j < aux->nchaves; j++)
      {
        if((aux->chave[j] > x) && (aux->chave[j] < y))
          printf("%d->", aux->chave[j]);
        else if (aux->chave[j] >= y)
          return;
      }
      aux = aux->prox;
    }
  }
  else
    imprime_intervalo2(a->filho[i], x, y);
}

void compara(TABM *a, int x, int *resp)
{
  if(!a)
    return;
  int i=0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  
  if((i < a->nchaves) && (a->chave[i] == x))
  {
    if((*resp == INT_MAX) && (a->folha))
      *resp = a->chave[i];
    i++;
  }

  if((i < a->nchaves) && (a->chave[i] > x) && (a->folha))
  {
    if(*resp == x)
      *resp = a->chave[i];
    else
    {
      if(*resp > a->chave[i])
        *resp = a->chave[i];
    }
  }

  if(a->folha)
    return;
  else
  {
    compara(a->filho[i], x, resp);
    if(i < a->nchaves)
      compara(a->filho[i+1], x, resp);
  }
}

int sucessor(TABM *a, int x)
{
  if(!a)
  {
    printf("Arvore vazia\n");
    exit(1);
  }
  int resp = INT_MAX;
  compara(a, x, &resp);
  if(resp == INT_MAX)
  {
    printf("Valor é maior valores da arvore\n");
    exit(1);
  }
  else
    return resp;
}

void compara_ant(TABM *a, int x, int *resp)
{
  if(!a)
    return;
  
  int i=0;
  while((i < a->nchaves) && (a->chave[i] < x))
  {
    if((*resp == x) && (a->folha))
      *resp = a->chave[i];
    else
      if((*resp < a->chave[i]) && (a->folha))
        *resp = a->chave[i];
    i++;
  }
  if((i < a->nchaves) && (a->chave[i] == x) && (a->folha))
    if(*resp == INT_MIN)
      *resp = a->chave[i];

  if(a->folha)
    return;
  else
  {
    compara_ant(a->filho[i], x, resp);
    if(i > 0)
      compara(a->filho[i-1], x, resp);
  }
}

int antecessor(TABM *a, int x)
{
  if(!a)
  {
    printf("Arvore vazia\n");
    exit(1);
  }
  int resp = INT_MIN;
  compara_ant(a, x, &resp);
  if(resp == INT_MIN)
  {
    printf("Valor é menor valores da arvore\n");
    exit(1);
  }
  else
    return resp;
}

int maior(TABM *a)
{
  if(!a)
    return INT_MIN;
  if(a->folha)
    return a->chave[a->nchaves - 1];
  else
    return maior(a->filho[a->nchaves]);
}

int menor(TABM *a)
{
  if(!a)
    return INT_MAX;
  if(a->folha)
    return a->chave[0];
  else
    return menor(a->filho[0]);
}

void imprime_maiores(TABM *a, int x)
{
  if(!a)
    return;
  int i = 0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  while((i < a->nchaves) && (a->chave[i] > x))
  {
    imprime_maiores(a->filho[i], x);
    if(a->folha)
      printf("%d->", a->chave[i]);
    i++;
  }
  if(a->folha)
    return;
  imprime_maiores(a->filho[i], x);
}

void imprime_maiores2(TABM *a, int x)
{
  if(!a)
    return;
  int i = 0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  if(a->folha)
  {
    TABM *aux = a;
    while(aux)
    {
      int j;
      for(j = 0; j < aux->nchaves; j++)
      {
        if(aux->chave[j] > x)
          printf("%d->", aux->chave[j]);
      }
      aux = aux->prox;
    }
  }
  else
    imprime_maiores2(a->filho[i], x);
}

void imprime_menores(TABM *a, int x)
{
  if(!a)
    return;
  int i = 0;
  while((i < a->nchaves) && (a->chave[i] < x))
  {
    imprime_menores(a->filho[i], x);
    if(a->folha)
      printf("%d->", a->chave[i]);
    i++;
  }
  if(a->folha)
    return;
  imprime_menores(a->filho[i], x);
}

void imprime_menores2(TABM *a, int x)
{
  if(!a)
    return;
  if(a->folha)
  {
    TABM *aux = a;
    int i = 0;
    while(aux)
    {
      for(i = 0; i < aux->nchaves; i++)
      {
        if(aux->chave[i] < x)
          printf("%d->", aux->chave[i]);
        else
          return;
      }
      aux = aux->prox;
    }
  }
  else
    imprime_menores2(a->filho[0], x);
}

TLSE *lista_menores(TABM *a, int x)
{
  if(!a)
    return NULL;
  TLSE *l = NULL;
  TABM *p = a;
  while(!p->folha)
    p = p->filho[0];
  while(p)
  {
    int i = 0;
    for(i = 0; (i < p->nchaves) && (p->chave[i] < x); i++)
      l = lst_insere(l, p->chave[i]);
    if((i < p->nchaves) && (p->chave[i] >= x))
      p = NULL;
    else
      p = p->prox;
  }
  return l;
}

TLSE *lista_maiores2(TABM *a, int x)
{
  if(!a)
    return NULL;
  TLSE *l = NULL;
  int i = 0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  if(a->folha)
  {
    TABM *p = a;
    int j = 0;
    while(p)
    {
      for(j = 0; j < p->nchaves; j++)
        if(p->chave[i] > x)
          l = lst_insere(l, p->chave[j]);
      p = p->prox;
    }
    return l;
  }
  return lista_maiores2(a->filho[i], x);
}

TLSE *lista_intervalo2(TABM *a, int x, int y)
{
  if(!a)
    return NULL;
  int i = 0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  if(a->folha)
  {
    TABM *p = a;
    TLSE *l = NULL;
    int j = 0;
    while(p)
    {
      for(j = 0; (j < p->nchaves) && (p->chave[j] < y); j++)
        if(p->chave[j] > x)
          l = lst_insere(l, p->chave[j]);
      if((j < p->nchaves) && (p->chave[j] >= y))
        p = NULL;
      else
        p = p->prox;
    }
    return l;
  }
  return lista_intervalo2(a->filho[i], x, y);
}

int sucessor2(TABM *a, int x)
{
  if(!a)
    return INT_MAX;
  int i = 0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  if(a->folha)
  {
    TABM *p = a;
    int j = 0;
    int resp = INT_MAX;
    while(p)
    {
      for(j = 0; j < p->nchaves; j++)
      {
        if(p->chave[j] == x)
          if(resp == INT_MAX)
            resp = p->chave[j];

        if(p->chave[j] > x)
        {
          if(resp == x)
            resp = p->chave[j];
          else
            if(resp > p->chave[j])
              resp = p->chave[j];
        }
      }
      if((resp != INT_MAX) && (resp != x))
        p = NULL;
      else
        p = p->prox;
    }
    return resp;
  }
  return sucessor2(a->filho[i], x);
}

int antecessor2(TABM *a, int x)
{
  if(!a)
    return INT_MIN;
  int i = 0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if(a->folha)
  {
    int j = 0;
    int resp = INT_MIN;
 
    for(j = 0; j < a->nchaves; j++)
    { 
      if(a->chave[j] < x)
      {
        if(resp == x)
          resp = a->chave[j];
        else
          if(resp < a->chave[j])
            resp = a->chave[j];
      }
      if(a->chave[j] == x)
        if(resp == INT_MIN)
          resp = a->chave[j];
    }
    return resp;
  }
  return antecessor2(a->filho[i], x);
}
 


int main(void){
  TABM * arvore = inicializa();
  int num = 0, from, to;
  while(num != -1){
    printf("Digite um numero para adicionar. 0 para imprimir e -1 para sair\n");
    scanf("%i", &num);
    if(num == -1){
      printf("\n");
      imprime(arvore,0);
      libera(arvore);
      return 0;
    }
    /*
    else if(num == -9){
      scanf("%d", &from);
      arvore = retira(arvore, from, t);
      Imprime(arvore,0);
    }
    */
    else if(num == -5){
      printf("\n");
    //  TLSE *l = lista_intervalo(arvore, 13, 33);
    //  imprime_lst(l);
    //  TP *p = pilha_intervalo(arvore, 13, 33);
    //  imprime_pilha(p);
    //  TLSE *l = lista_maiores(arvore, 29);
    //  imprime_lst(l);
      imprime_intervalo(arvore, 12, 90);
      libera(arvore);
      return 0;
    }
    else if(!num){
      printf("\n");
      imprime(arvore,0);
    }
    else arvore = insere(arvore, num, t);
    printf("\n\n");
  }
}
