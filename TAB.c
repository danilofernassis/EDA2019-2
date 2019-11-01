#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef struct ab
{
  int info;
  int altura;
  int cor;
  int fb;
  struct ab *esq;
  struct ab *dir;
}TAB;

/*********************************************************/
/*-------------------------------------------------------*/
typedef struct Trunk
{
    struct Trunk *prev;
    char *str;

}Trunk;

Trunk *cria_trunk(Trunk *prev, char *str)
{
  Trunk *novo = (Trunk*)malloc(sizeof(Trunk));
  novo->prev = prev;
  novo->str = str;
  return novo;
}

void showTrunks(Trunk *p)
{
    if (p == NULL)
        return;

    showTrunks(p->prev);

    printf("%s", p->str);
}

void printTree(TAB *root, Trunk *prev, int isLeft)
{
    if (root == NULL)
        return;
    
    char *prev_str = "    ";
    Trunk *trunk = cria_trunk(prev, prev_str);

    printTree(root->dir, trunk, 1);

    if (!prev)
        trunk->str = "---";
    else if (isLeft)
    {
        trunk->str = ".---";
        prev_str = "   |";
    }
    else
    {
        trunk->str = "`---";
        prev->str = prev_str;
    }

    showTrunks(trunk);
    printf("%d\n", root->info);

    if (prev)
        prev->str = prev_str;
    trunk->str = "   |";

    printTree(root->esq, trunk, 0);
}
/*-------------------------------------------------------*/
/*********************************************************/

TAB *cria_vazia(void)
{
  return NULL;
}

TAB *cria_TAB(int c, TAB *esq, TAB *dir)
{
  TAB *novo = (TAB*)malloc(sizeof(TAB));
  novo->info = c;
  novo->esq = esq;
  novo->dir = dir;
  return novo;
}

int arv_vazia(TAB *a)
{
  return a==NULL;
}

void libera_arv(TAB *a)
{
  if(!arv_vazia(a))
  {
    libera_arv(a->esq);
    libera_arv(a->dir);
    free(a);
  }

}

//Questão 1***********************************************
TAB *copia(TAB *a)
{
  if(!a)
    return a;
  TAB *novo = (TAB*)malloc(sizeof(TAB));
  novo->info = a->info;
  novo->esq = copia(a->esq);
  novo->dir = copia(a->dir);
  return novo;
}
//********************************************************

//Questão 2***********************************************
TAB *espelho(TAB *a)
{
  if(!a)
    return NULL;
  TAB *novo = (TAB*)malloc(sizeof(TAB));
  novo->info = a->info;
  novo->esq = espelho(a->dir);
  novo->dir = espelho(a->esq);
  return novo;
}
//********************************************************

//Questão 3***********************************************
TAB *maior(TAB *a)
{
  if(!a)
    return a;
  TAB *resp = a;
  TAB *esq = maior(a->esq);
  TAB *dir = maior(a->dir);

  if(esq)
    if(resp->info < esq->info)
      resp = esq;
  if(dir)
    if(resp->info < dir->info)
      resp = dir;
  return resp;
}
//********************************************************

//Questão 4***********************************************
TAB *menor(TAB *a)
{
  if(!a)
    return a;
  TAB *resp = a;
  TAB *esq = menor(a->esq);
  TAB *dir = menor(a->dir);

  if(esq)
    if(resp->info > esq->info)
      resp = esq;
  if(dir)
    if(resp->info > dir->info)
      resp = dir;
  return resp;
}
//********************************************************

//Questão 5***********************************************
int igual(TAB *a1, TAB *a2)
{
  if(!a1 && !a2)
    return 1;
  else if((!a1 && a2) || (a1 && !a2) || (a1->info != a2->info))
    return 0;
  int esq = igual(a1->esq, a2->esq);
  int dir = igual(a1->dir, a2->dir);
  return (esq && dir);
}
//********************************************************

//Questão 6***********************************************
TAB  *remove_no(TAB *a, int x)
{
  if(arv_vazia(a))
    return a;

  if((a->info != x) && (a->esq))
    a->esq = remove_no(a->esq, x);
  if((a->info != x) && (a->dir))
    a->dir = remove_no(a->dir, x);
  
  if(a->info == x)
  {
    if(!a->esq && !a->dir)
    {
      free(a);
      return NULL;
    }
    else if((!a->esq && a->dir) || (a->esq && !a->dir))
    {
      TAB *temp = a;
      if(a->esq)
        a = a->esq;
      else
        a = a->dir;
      free(temp);
    }
    else
    {
      TAB *temp = a->esq;
      while(temp->dir)
        temp = temp->dir;
      a->info = temp->info;
      temp->info = x;
      a->esq = remove_no(a->esq, x);
    }
  }
  return a;
}

TAB* retira_pares(TAB* a)
{
  if(a)
  {
    a->esq=retira_pares(a->esq);
    a->dir=retira_pares(a->dir);
    if((a->info%2)==0)
      a = remove_no(a,a->info);
  }
  return a;
}

//Questão 7***********************************************
int altura_arv(TAB *a)
{
  if(arv_vazia(a))
    return -1;
  
  int tam_esq = altura_arv(a->esq);
  int tam_dir = altura_arv(a->dir);

  if(tam_esq > tam_dir)
    return 1 + tam_esq;
  else
    return 1 + tam_dir;
}

void colore(TAB *a)
{
  if(!a)
    return;
  int altura = altura_arv(a);
  if(altura%2)
    a->cor = 0;
  else
    a->cor = 1;
  colore(a->esq);
  colore(a->dir);
}
//********************************************************

//Questão 8***********************************************
int nf(TAB *a)
{
  if(!a)
    return 0;
  if(!a->esq && !a->dir)
    return 1;
  return nf(a->esq) + nf(a->dir);
}
//********************************************************

//Questão 9***********************************************
int ni(TAB *a)
{
  if(!a || (!a->esq && !a->dir))
    return 0;
  return 1 + ni(a->esq) + ni(a->dir);
}
//********************************************************

void imprime_TAB(TAB *a)
{
  printTree(a, NULL, 0);
}

void imprime_intervalo(TAB *a, int x, int y)
{
  if(!a)
    return;
  imprime_intervalo(a->esq, x, y);
  if((a->info >= x) && (a->info <= y))
    printf("%d->", a->info);
  imprime_intervalo(a->dir, x, y);
}


int main(void) {
  TAB *arv = cria_vazia(),
      *arv_teste = NULL,
      *arv_teste1 = cria_TAB('A', cria_TAB('B', NULL, cria_TAB('D', NULL, NULL)), cria_TAB('F', NULL, cria_TAB('G', NULL, NULL))),


      *arv1 = cria_TAB(68, cria_vazia(), cria_vazia()), 
      *arv2 = cria_TAB(66, cria_vazia(), arv1), 
      *arv3 = NULL,
      *arv4 = cria_TAB(71, cria_vazia(), cria_vazia()),
      *arv5 = cria_TAB(70, arv3, arv4),
      *arv6 = cria_TAB(65, arv2, arv5);

  TAB *arv_balanceada = arv = cria_TAB(10, cria_TAB(9, cria_TAB(7, cria_TAB(13, NULL, NULL), cria_TAB(5, NULL, NULL)), cria_TAB(8, cria_TAB(4, NULL, NULL), cria_TAB(1, NULL, NULL))), cria_TAB(11, cria_TAB(21, cria_TAB(17, NULL, NULL), cria_TAB(20, NULL, NULL)), cria_TAB(22, cria_TAB(15, NULL, NULL), cria_TAB(23, NULL, NULL)))); 

  arv = cria_TAB(15, cria_TAB(10, cria_TAB(8, cria_TAB(5, NULL, NULL), cria_TAB(9, NULL, NULL)), cria_TAB(12, cria_TAB(11, NULL, NULL), cria_TAB(13, NULL, cria_TAB(14, NULL, NULL)))), cria_TAB(23, cria_TAB(19, cria_TAB(18, cria_TAB(17, NULL, NULL), NULL), cria_TAB(22, NULL, NULL)), cria_TAB(26, NULL, cria_TAB(28, NULL, NULL))));

  
  
  imprime_TAB(arv);
  imprime_intervalo(arv, 11, 18);
  
  //printf("%d\n", igual(arv6, arv_teste1));



}