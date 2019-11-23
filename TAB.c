/*
- copia arvore
- espelho da arvore
- maior, menor elemento
- arvores iguais
- retira pares
- colore arvore
- qte no interno e folha
- imprime intervalo
- remove pai de determinado elemento
- remove filhos de determinado elemento
- conta qte de no
- verifica se é abb (vetor, lista, pilha)
- verifica se é avl
- verifica se nivel impar tem um filho e se par tem 2 ou 0 filhos
- retorna nivel de determinado elemento
- troca arvore da direta com a esquerda
- altura arvore
- verifica se todo filho a esq é folha
- verifica se todo no so tem um filho
- retorna elemento sucessor de elemento x
- retorna elemento antecessor de elemento x
- retorna em lista o caminho ate determinado elemento
- verifica se duas arvores possuem os mesmos valores independento formato arvore
- lista nivel;
- pilha nivel;
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

TAB *remove_pai(TAB *a, int x)
{
  if(!a)
    return a;
  if(a->esq)
    if(a->esq->info == x)
      a = remove_no(a, a->info);
  if(a->dir)
    if(a->dir->info == x)
      a = remove_no(a, a->info);
  a->esq = remove_pai(a->esq, x);
  a->dir = remove_pai(a->dir, x);
  return a;
}

TAB *busca(TAB *a, int x)
{
  if(!a)
    return NULL;
  if(a->info == x)
    return a;
  TAB *resp = busca(a->esq, x);
  if(resp)
    return resp;
  return busca(a->dir, x);
}

TAB *remove_pai_filhos(TAB *a, int x)
{
  if(!a)
    return NULL;
  TAB *localiza = busca(a, x);
  if(!localiza)
    return a;
  else
  {
    if(localiza->esq)
      a = remove_no(a, localiza->esq->info);
    if(localiza->dir)
      a = remove_no(a, localiza->dir->info);
    a = remove_pai(a, x);
  }
  return a;
}

int conta_no(TAB *a)
{
  if(!a)
    return 0;
  return 1+conta_no(a->esq)+conta_no(a->dir);
}

void preenche_vet(TAB *a, int *vet, int *i)
{
  if(!a)
    return;
  preenche_vet(a->esq, vet, i);
  vet[*i] = a->info;
  (*i)++;
  preenche_vet(a->dir, vet, i);
}

int eabb(TAB *a)
{
  if(!a)
    return 1;
  int n = conta_no(a);
  int vet[n], i=0;
  preenche_vet(a, vet, &i);
  for(i=1; i < n; i++)
    if(vet[i] < vet[i-1])
      return 0;
  return 1;
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

TLSE *preenche_lista(TAB *a)
{
  if(!a)
    return NULL;
  TLSE *resp = NULL;
  TLSE *esq = preenche_lista(a->esq);
  resp = junta_listas(resp, esq);
  resp = lst_insere_fim(resp, a->info);
  TLSE *dir = preenche_lista(a->dir);
  resp = junta_listas(resp, dir);
  return resp;
}

int eabb_lista(TAB *a)
{
  if(!a)
    return 1;
  TLSE *l = preenche_lista(a);
  TLSE *p = l->prox, *ant = l;
  while(p->prox)
  {
    if(p->info < ant->info)
      return 0;
    ant = p;
    p = p->prox;
  }
  return 1;
}

TP *junta_pilhas(TP *p1, TP *p2)
{
  if(vazia_pilha(p2))
    return p1;
  TP *aux = cria_pilha();
  while(!vazia_pilha(p2))
    push(aux, pop(p2));
  while(!vazia_pilha(aux))
    push(p1, pop(aux));
  libera_pilha(aux);
  return p1;
}

TP *preenche_pilha(TAB *a)
{
  if(!a)
  {
    TP *vazia = cria_pilha();
    return vazia;
  }
  TP *resp = cria_pilha();
  TP *esq = cria_pilha();
  esq = preenche_pilha(a->esq);
  resp = junta_pilhas(resp, esq);
  push(resp, a->info);
  libera_pilha(esq);
  TP *dir = preenche_pilha(a->dir);
  resp = junta_pilhas(resp, dir);
  libera_pilha(dir);
  return resp;
}

int eabb_pilha(TAB *a)
{
  if(!a)
    return 1;
  TP *p = cria_pilha();
  p = preenche_pilha(a);
  imprime_pilha(p);
  int x = pop(p);
  while(!vazia_pilha(p))
  {
    int y = pop(p);
    if(x < y)
      return 0;
    x = y;
  }
  return 1;
}

int testa(TAB *a, int n)
{
  if(!a)
    return 1;
  if((n%2)==0)
    if((a->esq && !a->dir) || (!a->dir && a->esq))
      return 0;
  if((n%2)!=0)
    if((a->esq && a->dir) || (!a->dir && !a->esq))
      return 0;
  return testa(a->esq, n+1)*testa(a->dir, n+1);
}

int verifica(TAB *a)
{
  if(!a)
    return 1;
  return testa(a, 0);
}

int max(TAB *a)
{
  if(!a)
    return INT_MIN;
  int resp = a->info;
  int esq = max(a->esq);
  int dir = max(a->dir);
  if(esq > resp)
    resp = esq;
  if(dir > resp)
    resp = dir;
  return resp;
}

int soma(TAB *a)
{
  if(!a)
    return 0;
  return a->info + soma(a->esq) + soma(a->dir);
}

int pares(TAB *a)
{
  if(!a)
    return 0;
  if(!(a->info % 2))
    return 1 + pares(a->esq) + pares(a->dir);
  else
    return pares(a->esq) + pares(a->dir);
}

int conta_nivel(TAB *a, int x, int n)
{
  if(!a)
    return INT_MIN;
  if(a->info == x)
    return n;
  int resp = conta_nivel(a->esq, x, n+1);
  if(resp != INT_MIN)
    return resp;
  return conta_nivel(a->dir, x, n+1);
}

int nivel_x(TAB *a, int x)
{
  if((!a) || (!busca(a, x)))
    return INT_MIN;
  return conta_nivel(a, x, 0);
}


void troca(TAB *a)
{
  if(!a)
    return;
  troca(a->esq);
  troca(a->dir);
  TAB *temp = a->esq;
  a->esq = a->dir;
  a->dir = temp;
}

int maximo(int a, int b)
{
  if(a > b)
    return a;
  else
    return b;
}

int altura(TAB *a)
{
  if(!a)
    return -1;
  return 1+maximo(altura(a->esq), altura(a->dir));
}

int eavl(TAB *a)
{
  if(!a)
    return 1;
  int fb = altura(a->esq) - altura(a->dir);
  if((fb < -1) || (fb > 1))
    return 0;
  return eavl(a->esq)*eavl(a->dir);
}

void fator_bal(TAB *a)
{
  if(!a)
    return;
  a->fb = altura(a->esq) - altura(a->dir);
  fator_bal(a->esq);
  fator_bal(a->dir);
}

TLSE *caminho(TAB *a, int x)
{
  TLSE *l = NULL;
  l = lst_insere(l, a->info);
  if(a->info == x)
    return l;
  if(busca(a->esq, x))
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

TLSE *ancestral(TAB *a, int x)
{
  if(!a || !busca(a, x))
    return NULL;
  TLSE *l = caminho(a, x);
  return l;
}

void compara(TAB *a, int x, int *resp)
{
  if(!a)
    return;
  if(a->info == x)
  {
    if(*resp == INT_MAX)
      *resp = a->info;
  }

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

  compara(a->esq, x, resp);
  compara(a->dir, x, resp);
}

int sucessor(TAB *a, int x)
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
    printf("Valor superior aos valores da arvore\n");
    exit(1);
  }
  else
    return resp;
}

void compara_ant(TAB *a, int x, int *resp)
{
  if(!a)
    return;
  
  if(a->info == x)
  {
    if(*resp == INT_MIN)
      *resp = a->info;
  }
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
  compara_ant(a->esq, x, resp);
  compara_ant(a->dir, x, resp);
}

int antecessor(TAB *a, int x)
{
  if(!a)
  {
    printf("Arvore vazia");
    exit(1);
  }
  int resp = INT_MIN;
  compara_ant(a, x, &resp);
  if(resp == INT_MIN)
  {
    printf("Valor inferior aos valores da arvore\n");
    exit(1);
  }
  else
    return resp;
}

//verifica se todo filho da esquerda é um nó folha.
int teste(TAB *a)
{
  if(!a)
    return 0;
  int resp = 1;
  TAB *p = a;
  while(p)
  {
    if(!p->esq)
      return 0;
    if(p->esq)
      if((p->esq->esq)||(p->esq->dir))
        return 0;
    p = p->dir;
  }
  return resp;
}

//verifica se todos os nos da arvores possuem apenas um filho ou nenhum.
int zz(TAB *a)
{
  if(!a)
    return 0;
  TAB *p = a;
  while(p->esq || p->dir)
  {
    if(p->esq && p->dir)
      return 0;
    else if(p->esq)
      p = p->esq;
    else
      p = p->dir;
  }
  return 1;
}


TLSE *caminho(TAB *a, int x)
{
  TLSE *l = NULL;
  l = lst_insere(l, a->info);
  if(a->info == x)
    return l;
  if(busca(a->esq, x))
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

TLSE *ancestral(TAB *a, int x)
{
  if(!a || !busca(a, x))
    return NULL;
  TLSE *l = caminho(a, x);
  return l;
}

TP *caminho_pilha(TAB *a, int x)
{
  TP *p = cria_pilha();
  push(p, a->info);
  if(a->info == x)
    return p;
  if(busca(a->esq, x))
  {
    TP *esq = caminho_pilha(a->esq, x);
    p = junta_pilhas(p, esq);
  }
  else
  {
    TP *dir = caminho_pilha(a->dir, x);
    p = junta_pilhas(p, dir);
  }
  return p;
}

TP *ancestral_pilha(TAB *a, int x)
{
  if(!a || !busca(a,x))
  {
    TP *vazia = cria_pilha();
    return vazia;
  }
  TP *p = caminho_pilha(a, x);
  return p;
}

//verifica se duas arvores possuem os mesmos valores independente da forma que eles estao organizados na arvore.
int mi(TAB *a1, TAB *a2)
{
  if((!a1 && a2) || (!a1 && a2))
    return 0;
  //verifica se as arvores possuem as mesmas qtdes de nos.
  int cont1 = conta_no(a1);
  int cont2 = conta_no(a2);
  if(cont1 != cont2)
    return 0;
  int vet1[cont1], vet2[cont1];
  int i = 0, j = 0;
  //pega os valores da arvore e preenche os vetores
  preenche_vet(a1, vet1, &i);
  preenche_vet(a2, vet2, &j);
  //ordena os dois vetores
  for(i = 0; i < cont1; i++)
    for(j = i+1; j < cont1; j++)
    {
      if(vet1[j] < vet1[i])
      {
        int temp = vet1[i];
        vet1[i] = vet1[j];
        vet1[j] = temp;
      }

      if(vet2[j] < vet2[i])
      {
        int temp = vet2[i];
        vet2[i] = vet2[j];
        vet2[j] = temp;
      }
    }
  
  int resp = 1;
  //verifica se os dois vetores sao iguais
  for(i = 0; i < cont1; i++)
    if(vet1[i] != vet2[i])
      resp = 0;
  return resp;
}
TLSE *lista_nivel(TAB *a, int n)
{
  if(!a)
    return NULL;
  if(n == 0)
  {
    TLSE *l = NULL;
    l = lst_insere(l, a->info);
    return l;
  }
  else
    return junta_listas(lista_nivel(a->esq, n-1), lista_nivel(a->dir, n-1)); 
}

TP *pilha_nivel(TAB *a, int n)
{
  if(!a)
  {
    TP *vazia = cria_pilha();
    return vazia;
  }
  if(n == 0)
  {
    TP *p = cria_pilha();
    push(p, a->info);
    return p;
  }
  else
    return junta_pilhas(pilha_nivel(a->esq, n-1), pilha_nivel(a->dir, n-1));
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
