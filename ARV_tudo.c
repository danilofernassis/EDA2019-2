#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


typedef struct arv
{
  int info;
  int altura;
  int cor;
  int fb;
  struct arv *esq;
  struct arv *dir;
}ARV;

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

void printTree(ARV *root, Trunk *prev, int isLeft)
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

ARV *cria_vazia(void)
{
  return NULL;
}

ARV *cria_arv(int c, ARV *esq, ARV *dir)
{
  ARV *novo = (ARV*)malloc(sizeof(ARV));
  novo->info = c;
  novo->esq = esq;
  novo->dir = dir;
  return novo;
}

int arv_vazia(ARV *a)
{
  return a==NULL;
}

void libera_arv(ARV *a)
{
  if(!arv_vazia(a))
  {
    libera_arv(a->esq);
    libera_arv(a->dir);
    free(a);
  }

}

int altura_arv(ARV *a)
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

int qte_no_arv(ARV *a)
{
  if(arv_vazia(a))
    return 0;
  else
    return 1 + qte_no_arv(a->esq) + qte_no_arv(a->dir);
}

void imprime_arv(ARV *a)
{
  printTree(a, NULL, 0);
}


ARV *copia(ARV *a)
{
  if(!a)
    return a;
  ARV *novo = (ARV*)malloc(sizeof(ARV));
  novo->info = a->info;
  novo->esq = copia(a->esq);
  novo->dir = copia(a->dir);
  return novo;
}


ARV *busca(ARV *a, int x)
{
  if(!a)
    return a;
  if(a->info == x)
    return a;
  ARV *resp = busca(a->esq, x);
  if(resp)
    return resp;
  else
    return busca(a->dir, x);
}

int arvore_vazia(ARV *a)
{
  return a == NULL;
}

ARV  *remove_no(ARV *a, int x)
{
  if(arvore_vazia(a))
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
      ARV *temp = a;
      if(a->esq)
        a = a->esq;
      else
        a = a->dir;
      free(temp);
    }
    else
    {
      ARV *temp = a->esq;
      while(temp->dir)
        temp = temp->dir;
      a->info = temp->info;
      temp->info = x;
      a->esq = remove_no(a->esq, x);
    }
  }
  return a;
}

ARV* retira_pares(ARV* a)
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

void colore(ARV *a)
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

int nf(ARV *a)
{
  if(!a)
    return 0;
  if(!a->esq && !a->dir)
    return 1;
  return nf(a->esq) + nf(a->dir);
}

int ni(ARV *a)
{
  if(!a || (!a->esq && !a->dir))
    return 0;
  return 1 + ni(a->esq) + ni(a->dir);
}

int igual(ARV *a, ARV *b)
{
  if(!a && !b)
    return 1;
  if(a && b)
  {
    if(a->info != b->info)
      return 0;
    else
    {
      int esq = igual(a->esq, b->esq);
      int dir = igual(a->dir, b->dir);
      return esq == dir;
    }
  }
  return 0;
}

int igual_2(ARV *a, ARV *b)
{
  if(!a && !b)
    return 1;
  else if((!a && b) || (a && !b) || (a->info != b->info))
    return 0;
  int esq = igual_2(a->esq, b->esq);
  int dir = igual_2(a->dir, b->dir);
  return (esq && dir);
}

int estritamente_binario(ARV *a)
{
  if(!a)
    return 1;
  if((!a->esq && a->dir) || (a->esq && !a->dir))
    return 0;
  else
  {
    int esq = estritamente_binario(a->esq);
    int dir = estritamente_binario(a->dir);
    return (esq && dir);
  }
  
}

int completa(ARV *a)
{
  if(!a)
    return 1;
  int resp = completa(a->esq) - completa(a->dir);
  if((resp == -1) || (resp == 1) || (resp == 0))
  {
    if(resp == -1)
      return (completa(a->esq) - completa(a->dir))*(-1);
    else
      return completa(a->esq) - completa(a->dir);

  }
    
  else
    return 0;
}


ARV *espelho(ARV *a)
{
  if(!a)
    return NULL;
  ARV *novo = (ARV*)malloc(sizeof(ARV));
  novo->info = a->info;
  novo->esq = espelho(a->dir);
  novo->dir = espelho(a->esq);
  return novo;

}
void ins_altura(ARV *a)
{
  if(!a)
    return;
  a->altura = altura_arv(a);
  ins_altura(a->esq);
  ins_altura(a->dir);
}

int soma(ARV *a)
{
  if(!a)
    return 0;
  return a->info + soma(a->esq) + soma(a->dir);
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

void imprime_intervalo(ARV *a, int x, int y)
{
  if(!a)
    return;
  if(a->info < x)
    imprime_intervalo(a->dir, x, y);
  else if (a->info > y)
    imprime_intervalo(a->esq, x, y);
  else
  {
    printf("%d->", a->info);
    imprime_intervalo(a->esq, x, y);
    imprime_intervalo(a->dir, x, y);
  }

}

int cont_nivel(ARV *a, int n)
{
  if(!a)
    return 0;
  if(n == 0)
    return 1;
  return cont_nivel(a->dir, n-1) + cont_nivel(a->esq, n-1);
}



int main(void) {
  ARV *arv = cria_vazia(),
      *arv_teste = NULL,
      *arv_teste1 = cria_arv('A', cria_arv('B', NULL, cria_arv('D', NULL, NULL)), cria_arv('F', NULL, cria_arv('G', NULL, NULL))),


      *arv1 = cria_arv(68, cria_vazia(), cria_vazia()), 
      *arv2 = cria_arv(66, cria_vazia(), arv1), 
      *arv3 = NULL,
      *arv4 = cria_arv(71, cria_vazia(), cria_vazia()),
      *arv5 = cria_arv(70, arv3, arv4),
      *arv6 = cria_arv(65, arv2, arv5);

  ARV *arv_balanceada = arv = cria_arv(10, cria_arv(9, cria_arv(7, cria_arv(13, NULL, NULL), cria_arv(5, NULL, NULL)), cria_arv(8, cria_arv(4, NULL, NULL), cria_arv(1, NULL, NULL))), cria_arv(11, cria_arv(21, cria_arv(17, NULL, NULL), cria_arv(20, NULL, NULL)), cria_arv(22, cria_arv(15, NULL, NULL), cria_arv(23, NULL, NULL)))); 

  arv = cria_arv(15, cria_arv(10, cria_arv(8, cria_arv(5, NULL, NULL), cria_arv(9, NULL, NULL)), cria_arv(12, cria_arv(11, NULL, NULL), cria_arv(13, NULL, cria_arv(14, NULL, NULL)))), cria_arv(23, cria_arv(19, cria_arv(18, cria_arv(17, NULL, NULL), NULL), cria_arv(22, NULL, NULL)), cria_arv(26, NULL, cria_arv(28, NULL, NULL))));

  
  
  imprime_arv(arv_teste1);
  
  printf("%d\n", nf(arv_teste1));
  
  
}