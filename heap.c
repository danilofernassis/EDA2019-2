#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "arvore.h"

int pai(int ind)
{
  if(ind <= 0)
    return -1;
  return ((ind-1)/2);
}

int esq(int ind)
{
  return ((2*ind) + 1);
}

int dir(int ind)
{
  return ((2*ind) + 2);
}

void max_heapfy(int *vet, int n, int pos)
{
  int e = esq(pos), d = dir(pos), maior = pos;
  if((e < n) && (vet[e] > vet[maior]))
    maior = e;
  if((d < n) && (vet[d] > vet[maior]))
    maior = d;
  if(maior != pos)
  {
    int aux = vet[pos];
    vet[pos] = vet[maior];
    vet[maior] = aux;
    max_heapfy(vet, n, maior);
  }
}

void build_maxheap(int *vet, int n)
{
  int ult_pai = pai(n-1);
  int i;
  for(i = ult_pai; i >= 0; i--)
    max_heapfy(vet, n, i);
}

void heapsort_max(int *vet, int n)
{
  build_maxheap(vet, n);
  int i;
  for(i = n-1; i != 0; i--)
  {
    int temp = vet[0];
    vet[0] = vet[i];
    vet[i] = temp;
    max_heapfy(vet, i, 0);
  }
}

void min_heapfy(int *vet, int n, int pos)
{
  int e = esq(pos), d = dir(pos), menor = pos;
  if(e < n)
    if(vet[e] < vet[menor])
      menor = e;
  if(d < n)
    if(vet[d] < vet[menor])
      menor = d;
  if(menor != pos)
  {
    int aux = vet[pos];
    vet[pos] = vet[menor];
    vet[menor] = aux;
    min_heapfy(vet, n, menor);
  }
}

void build_minheap(int *vet, int n)
{
  int ult_pai = pai(n-1);
  int i;
  for(i = ult_pai; i >=0; i--)
    min_heapfy(vet, n, i);
}

void heapsort_min(int *vet, int n)
{
  build_minheap(vet, n);
  int i;
  for(i = n-1; i > 0; i--)
  {
    int temp = vet[0];
    vet[0] = vet[i];
    vet[i] = temp;
    min_heapfy(vet, i, 0); 
  }
  int j = n-1;
  for(i = 0; i < (n/2); i++)
  {
    int aux = vet[j];
    vet[j] = vet[i];
    vet[i] = aux;
    j--;
  }
}

int verifica_maxheap(int *vet, int n)
{
  int ult_pai = pai(n-1);
  int i;
  for(i = ult_pai; i >= 0 ; i--)
  {
    int e = esq(i), d = dir(i);
    if((e < n) && (vet[e] > vet[i]))
        return 0;
    if((d < n) && (vet[d] > vet[i]))
        return 0;
  }
  return 1;
}

int verifica_minheap(int *vet, int n)
{
  int ult_pai = pai(n-1);
  int i;
  for(i = ult_pai; i >= 0 ; i--)
  {
    int e = esq(i), d = dir(i);
    if((e < n) && (vet[e] < vet[i]))
        return 0;
    if((d < n) && (vet[d] < vet[i]))
        return 0;
  }
  return 1;
}

int chave_minima(int *vet, int n)
{
  int ult_pai = pai(n-1);
  int i, resp = vet[0];
  for(i = ult_pai; i >= 0; i--)
  {
    int e = esq(i), d = dir(i);
    if((e < n) && (vet[e] < resp))
        resp = vet[e];
    if((d < n) && (vet[d] < resp))
        resp = vet[d];
  }
  return resp;
}

int chave_maxima(int *vet, int n)
{
  int ult_pai = pai(n-1);
  int i, resp = vet[0];
  for(i = ult_pai; i >= 0; i--)
  {
    int e = esq(i), d = dir(i);
    if((e < n) && (vet[e] > resp))
        resp = vet[e];
    if((d < n) && (vet[d] > resp))
        resp = vet[d];
  }
  return resp;
}

void preenche_arv(TAB *a, int *vet, int n, int i)
{
  if(i >= n)
    return;
  int e = esq(i), d = dir(i);

  if(e < n)
    a->esq = cria_TAB(vet[e], NULL, NULL);
  if(d < n)
    a->dir = cria_TAB(vet[d], NULL, NULL);

  preenche_arv(a->esq, vet, n, e);
  preenche_arv(a->dir, vet, n, d);
}


TAB *vet2arv(int *vet, int n)
{
  TAB *a = cria_vazia();
  a = cria_TAB(vet[0], NULL, NULL);
  preenche_arv(a, vet, n, 0);
  return a;
}


void preenche_vet(TAB *a, int *vet, int n, int i)
{
  if(i >= n)
    return;
  int e = esq(i), d = dir(i);

  if(e < n)
    vet[e] = a->esq->info;
  if(d < n)
    vet[d] = a->dir->info;

  preenche_vet(a->esq, vet, n, e);
  preenche_vet(a->dir, vet, n, d); 
}

int conta(TAB *a)
{
  if(!a)
    return 0;
  return 1+conta(a->esq)+conta(a->dir);
}

int *arv2vet(TAB *a)
{
  if(!a)
    return NULL;
  int n = conta(a);
  int *vet = (int*)malloc(n*sizeof(int));
  vet[0] = a->info;
  preenche_vet(a, vet, n, 0);
  return vet;
}

void arv2vet2(TAB *a, int *vet, int n)
{
  if(!a)
    return;
  vet[0] = a->info;
  preenche_vet(a, vet, n, 0);

}

void imprime_vet(int *vet, int n)
{
  int i;
  for(i=0; i < n; i++)
    printf("%d ", vet[i]);
  printf("\n");
}

void remove_filhos(int *vet, int *n, int i)
{
  int e = esq(i), d = dir(i);
  if(d < *n)
  {
    int j = d+1;
    while(j < *n)
    {
      vet[j-1] = vet[j];
      j++;
    }
    *n = *n - 1;
  }
  if(e < *n)
  {
    int j = e+1;
    while(j < *n)
    {
      vet[j-1] = vet[j];
      j++;
    }
    *n = *n -1;
  }
}




int main(void) {
  int i, n=10;
 
  int vet[] = {10, 8, 9, 6, 7, 2, 4, 3, 1, 5};
  //int vet[] = {23, 17, 14, 7, 13, 10, 1, 5, 6, 12};
  imprime_vet(vet, n);
  heapsort_min(vet, n);
  imprime_vet(vet, n);

    
  return 0;
}
