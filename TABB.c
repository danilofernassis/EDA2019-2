/*
- copia arvore
- espelho da arvore
- maior, menor elemento
- arvores iguais
- retira pares
- colore arvore
- qte no interno e folha
- imprime intervalo
- retorna em lista o caminho ate determinado elemento
- retorna elemento sucessor de elemento x
- retorna elemento antecessor de elemento x
- remove pai de determinado elemento
- verifica se é avl
- altura arvore
- retorna nivel de determinado elemento
*/

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
}TABB;

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

void printTree(TABB *root, Trunk *prev, int isLeft)
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

TABB *cria_vazia(void)
{
  return NULL;
}

TABB *insere_TABB(TABB *a, int x)
{
  if(!a)
  {
    TABB *novo = (TABB*)malloc(sizeof(TABB));
    novo->info = x;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
  }
  else
  {
    if(a->info > x)
      a->esq = insere_TABB(a->esq, x);
    else if (a->info < x)
      a->dir = insere_TABB(a->dir, x);
    return a;
  }
}

int arv_vazia(TABB *a)
{
  return a==NULL;
}

void libera_arv(TABB *a)
{
  if(!arv_vazia(a))
  {
    libera_arv(a->esq);
    libera_arv(a->dir);
    free(a);
  }

}

//Questão 1***********************************************
TABB *copia(TABB *a)
{
  if(!a)
    return a;
  TABB *novo = (TABB*)malloc(sizeof(TABB));
  novo->info = a->info;
  novo->esq = copia(a->esq);
  novo->dir = copia(a->dir);
  return novo;
}
//********************************************************

//Questão 2***********************************************
TABB *espelho(TABB *a)
{
  if(!a)
    return NULL;
  TABB *novo = (TABB*)malloc(sizeof(TABB));
  novo->info = a->info;
  novo->esq = espelho(a->dir);
  novo->dir = espelho(a->esq);
  return novo;
}
//********************************************************

//Questão 3***********************************************
TABB *maior(TABB *a)
{
  if(!a)
    return a;
  TABB *resp = a;
  if(a->dir)
      resp = maior(a->dir);
  return resp;
}
//********************************************************

//Questão 4***********************************************
TABB *menor(TABB *a)
{
  if(!a)
    return a;
  TABB *resp = a;
  if(a->esq)
      resp = menor(a->esq);
  return resp;
}
//********************************************************

//Questão 5***********************************************
int igual(TABB *a1, TABB *a2)
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
TABB  *remove_no(TABB *a, int x)
{
  if(arv_vazia(a))
    return a;

  if(x < a->info)
    a->esq = remove_no(a->esq, x);
  if(x > a->info)
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
      TABB *temp = a;
      if(a->esq)
        a = a->esq;
      else
        a = a->dir;
      free(temp);
    }
    else
    {
      TABB *temp = a->esq;
      while(temp->dir)
        temp = temp->dir;
      a->info = temp->info;
      temp->info = x;
      a->esq = remove_no(a->esq, x);
    }
  }
  return a;
}

TABB* retira_pares(TABB* a)
{
  if(!a)
    return a;
  a->esq=retira_pares(a->esq);
  a->dir=retira_pares(a->dir);
  if((a->info%2)==0)
  a = remove_no(a,a->info);
  return a;
}

//Questão 7***********************************************
int altura_arv(TABB *a)
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

void colore(TABB *a)
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

void insere_cor(TABB *a, int x)
{
  if(!a)
    return;
  
  if(x%2)
    a->cor = 0;
  else
    a->cor = 1;
  insere_cor(a->esq, x+1);
  insere_cor(a->dir, x+1);
}

void colore_2(TABB *a)
{
  insere_cor(a, 0);
}

//Questão 8***********************************************
int nf(TABB *a)
{
  if(!a)
    return 0;
  if(!a->esq && !a->dir)
    return 1;
  return nf(a->esq) + nf(a->dir);
}
//********************************************************

//Questão 9***********************************************
int ni(TABB *a)
{
  if(!a || (!a->esq && !a->dir))
    return 0;
  return 1 + ni(a->esq) + ni(a->dir);
}
//********************************************************

void imprime_TABB(TABB *a)
{
  printTree(a, NULL, 0);
}

void imprime_caminho(ARV *a, int x)
{
  if(!a)
    return;
  printf("%d->", a->info);
  if(a->info > x)
    imprime_caminho(a->esq, x);
  if(a->info < x)
    imprime_caminho(a->dir, x);
}

void imprime_intervalo(TABB *a, int x, int y)
{
  if(!a)
    return;
  if(a->info == x)
  {
    printf("%d->", a->info);
    imprime_intervalo(a->dir, x, y);
  }
  if(a->info == y)
  {
    printf("%d->", a->info);
    imprime_intervalo(a->esq, x, y);
  }

  if(a->info < x)
    imprime_intervalo(a->dir, x, y);

  if(a->info > y)
    imprime_intervalo(a->esq, x, y);

  if((a->info > x) && (a->info < y))
  {
    imprime_intervalo(a->esq, x, y);
    printf("%d->", a->info);
    imprime_intervalo(a->dir, x, y);
  }

}

void compara(TABB *a, int x, int *resp)
{
  if(!a)
    return;
  
  if(a->info == x)
    if(*resp == INT_MAX)
      *resp = a->info;

  if(a->info > x)
  {
    if(*resp == x)
      *resp = a->info;
    else
    {
      if(*resp > a->info)
        *resp = a->info;
    }
  }

  if(x < a->info)
    compara(a->esq, x, resp);
  else
    compara(a->dir, x, resp);

}

int sucessor(TABB *a, int x)
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
    printf("Valor superior aos da arvore\n");
    exit(1);
  }
  else
    return resp;
}

void compara_ant(TABB *a, int x, int *resp)
{
  if(!a)
    return;
  
  if(a->info == x)
    if(*resp == INT_MIN)
      *resp = a->info;

  if(a->info < x)
  {
    if(*resp == x)
      *resp = a->info;
    else
    {
      if(*resp < a->info)
        *resp = a->info;
    }
  }

  if(x > a->info)
    compara_ant(a->dir, x, resp);
  else
    compara_ant(a->esq, x, resp);

}

int antecessor(TABB *a, int x)
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
    printf("Valor inferior aos da arvore\n");
    exit(1);
  }
  else
    return resp;
}

TABB *busca(TABB *a, int x)
{
  if(!a)
    return NULL;
  if(a->info == x)
    return a;
  if(a->info > x)
    return busca(a->esq, x);
  return busca(a->dir, x);
}

TLSE *junta_listas(TLSE *l1, TLSE *l2)
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

TLSE *caminho(TABB *a, int x)
{
  if(!a)
    return NULL;
  TLSE *l = NULL;
  l = lst_insere(l, a->info);
  if(a->info == x)
    return l;
  if(a->info > x)
  {
    TLSE *esq = caminho(a->esq, x);
    l = junta_listas(esq, l);
  }
  else
  {
    TLSE *dir = caminho(a->dir, x);
    l = junta_listas(dir, l);
  }
  return l;
}

TLSE *ancestral(TABB *a, int x)
{
  if((!a) || (!busca(a,x)))
    return NULL;
  TLSE *l = caminho(a,x);
  return l;
}

void junta_pilhas(TP *p1, TP *p2)
{
  if(!p2)
    return;
  TP *aux = cria_pilha();
  while(!vazia_pilha(p2))
    push(aux, pop(p2));
  while(!vazia_pilha(aux))
    push(p1, pop(aux));
  libera_pilha(p2);
  libera_pilha(aux);
}

TP *caminho_pilha(TABB *a, int x)
{
  if(!a)
  {
    TP *vazia = cria_pilha();
    return vazia;
  }
  TP *p = cria_pilha();
  push(p, a->info);
  if(a->info == x)
    return p;
  if(a->info > x)
  {
    TP *esq = caminho_pilha(a->esq, x);
    junta_pilhas(p, esq);
  }
  else
  {
    TP *dir = caminho_pilha(a->dir, x);
    junta_pilhas(p, dir);
  }
  return p;
}

TP *ancestral_pilha(TABB *a, int x)
{
  if((!a) || (!busca(a, x)))
  {
    TP *vazia = cria_pilha();
    return vazia;
  }
  else
  {
    TP *p = caminho_pilha(a, x);
    return p;
  }
}

int maior_elem(TABB *a)
{
  if(!a)
    return INT_MIN;
  int resp = a->info;
  if(a->dir)
    resp = maior_elem(a->dir);
  return resp;
}

int menor_elem(TABB *a)
{
  if(!a)
    return INT_MIN;
  int resp = a->info;
  if(a->esq)
    resp = menor_elem(a->esq);
  return resp;
}

TABB *remove_pai(TABB*a, int x)
{
  if(!a)
    return NULL;
  if((a->esq) && (a->esq->info == x))
    a = remove_no(a, a->info);
  if((a->dir) && (a->dir->info == x))
    a = remove_no(a, a->info);
  if(a->info > x)
    a->esq = remove_pai(a->esq, x);
  else
    a->dir = remove_pai(a->dir, x);
  return a;
}

int maximo(int a, int b)
{
  if(a > b)
    return a;
  return b;
}

int altura(TABB *a)
{
  if(!a)
    return -1;
  return 1+(maximo(altura(a->esq), altura(a->dir)));
}

int testa_avl(TABB *a)
{
  if(!a)
    return 1;
  int fb = altura(a->esq) - altura(a->dir);
  if((fb > 1) || (fb < -1))
    return 0;
  else
    return testa_avl(a->esq)*testa_avl(a->dir);
}

int eavl(TABB *a)
{
  if(!a)
    return 0;
  return testa_avl(a);
}

int conta_nivel(TABB *a, int x, int n)
{
  if(!a)
    return INT_MIN;
  if(a->info == x)
    return n;
  if(a->info > x)
    return conta_nivel(a->esq, x, n+1);
  return conta_nivel(a->dir, x, n+1);
}

int nivelx(TABB *a, int x)
{
  if(!a)
    return INT_MIN;
  return conta_nivel(a, x, 0);
}



int main(void) {
  TABB *arv = cria_vazia();
  arv = insere_TABB(arv, 15);
  arv = insere_TABB(arv, 19);
  arv = insere_TABB(arv, 10);
  arv = insere_TABB(arv, 22);
  arv = insere_TABB(arv, 11);
  arv = insere_TABB(arv, 9);
  arv = insere_TABB(arv, 5);
  arv = insere_TABB(arv, 7);
  arv = insere_TABB(arv, 21);
  arv = insere_TABB(arv, 24);
  arv = insere_TABB(arv, 13);
  arv = insere_TABB(arv, 23);
  arv = insere_TABB(arv, 25);
  arv = insere_TABB(arv, 20);
  arv = insere_TABB(arv, 4);

  TABB *arv_balanceada = cria_vazia();
  arv_balanceada = insere_TABB(arv_balanceada, 8);
  arv_balanceada = insere_TABB(arv_balanceada, 4);
  arv_balanceada = insere_TABB(arv_balanceada, 12);
  arv_balanceada = insere_TABB(arv_balanceada, 2);
  arv_balanceada = insere_TABB(arv_balanceada, 6);
  arv_balanceada = insere_TABB(arv_balanceada, 10);
  arv_balanceada = insere_TABB(arv_balanceada, 14);
  arv_balanceada = insere_TABB(arv_balanceada, 1);
  arv_balanceada = insere_TABB(arv_balanceada, 3);
  arv_balanceada = insere_TABB(arv_balanceada, 5);
  arv_balanceada = insere_TABB(arv_balanceada, 7);
  arv_balanceada = insere_TABB(arv_balanceada, 9);
  arv_balanceada = insere_TABB(arv_balanceada, 11);
  arv_balanceada = insere_TABB(arv_balanceada, 13);
  arv_balanceada = insere_TABB(arv_balanceada, 15);


  imprime_TABB(arv);
  printf("\n");
  imprime_intervalo(arv, 7, 23);
  


}
