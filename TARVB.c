#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "pilha.h"
#include "lista.h"

const int t = 2;

typedef struct ArvB{
  int nchaves, folha, *chave;
  struct ArvB **filho;
}TAB;


TAB *Cria(int t){
  TAB* novo = (TAB*)malloc(sizeof(TAB));
  novo->nchaves = 0;
  novo->chave =(int*)malloc(sizeof(int*)*((t*2)-1));
  novo->folha=1;
  novo->filho = (TAB**)malloc(sizeof(TAB*)*t*2);
  int i;
  for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
  return novo;
}


TAB *Libera(TAB *a){
  if(a){
    if(!a->folha){
      int i;
      for(i = 0; i <= a->nchaves; i++) Libera(a->filho[i]);
    }
    free(a->chave);
    free(a->filho);
    free(a);
  }
  return NULL;
}


void Imprime(TAB *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      Imprime(a->filho[i],andar+1);
      for(j=0; j<=andar; j++) printf("   ");
      printf("%d\n", a->chave[i]);
    }
    Imprime(a->filho[i],andar+1);
  }
}


TAB *Busca(TAB* x, int ch){
  TAB *resp = NULL;
  if(!x) return resp;
  int i = 0;
  while(i < x->nchaves && ch > x->chave[i]) i++;
  if(i < x->nchaves && ch == x->chave[i]) return x;
  if(x->folha) return resp;
  return Busca(x->filho[i], ch);
}


TAB *Inicializa(){
  return NULL;
}


TAB *Divisao(TAB *x, int i, TAB* y, int t){
  TAB *z=Cria(t);
  z->nchaves= t - 1;
  z->folha = y->folha;
  int j;
  for(j=0;j<t-1;j++) z->chave[j] = y->chave[j+t];
  if(!y->folha){
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
  x->chave[i-1] = y->chave[t-1];
  x->nchaves++;
  return x;
}


TAB *Insere_Nao_Completo(TAB *x, int k, int t){
  int i = x->nchaves-1;
  if(x->folha){
    while((i>=0) && (k<x->chave[i])){
      x->chave[i+1] = x->chave[i];
      i--;
    }
    x->chave[i+1] = k;
    x->nchaves++;
    return x;
  }
  while((i>=0) && (k<x->chave[i])) i--;
  i++;
  if(x->filho[i]->nchaves == ((2*t)-1)){
    x = Divisao(x, (i+1), x->filho[i], t);
    if(k>x->chave[i]) i++;
  }
  x->filho[i] = Insere_Nao_Completo(x->filho[i], k, t);
  return x;
}


TAB *Insere(TAB *T, int k, int t){
  if(Busca(T,k)) return T;
  if(!T){
    T=Cria(t);
    T->chave[0] = k;
    T->nchaves=1;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TAB *S = Cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    S = Divisao(S,1,T,t);
    S = Insere_Nao_Completo(S,k,t);
    return S;
  }
  T = Insere_Nao_Completo(T,k,t);
  return T;
}


TAB* remover(TAB* arv, int ch, int t){
  if(!arv) return arv;
  int i;
  printf("Removendo %d...\n", ch);
  for(i = 0; i<arv->nchaves && arv->chave[i] < ch; i++);
  if(i < arv->nchaves && ch == arv->chave[i]){ //CASOS 1, 2A, 2B e 2C
    if(arv->folha){ //CASO 1
      printf("\nCASO 1\n");
      int j;
      for(j=i; j<arv->nchaves-1;j++) arv->chave[j] = arv->chave[j+1];
      arv->nchaves--;
      return arv;      
    }
    if(!arv->folha && arv->filho[i]->nchaves >= t){ //CASO 2A
      printf("\nCASO 2A\n");
      TAB *y = arv->filho[i];  //Encontrar o predecessor k' de k na árvore com raiz em y
      while(!y->folha) y = y->filho[y->nchaves];
      int temp = y->chave[y->nchaves-1];
      arv->filho[i] = remover(arv->filho[i], temp, t); 
      //Eliminar recursivamente K e substitua K por K' em x
      arv->chave[i] = temp;
      return arv;
    }
    if(!arv->folha && arv->filho[i+1]->nchaves >= t){ //CASO 2B
      printf("\nCASO 2B\n");
      TAB *y = arv->filho[i+1];  //Encontrar o sucessor k' de k na árvore com raiz em y
      while(!y->folha) y = y->filho[0];
      int temp = y->chave[0];
      y = remover(arv->filho[i+1], temp, t); //Eliminar recursivamente K e substitua K por K' em x
      arv->chave[i] = temp;
      return arv;
    }
    if(!arv->folha && arv->filho[i+1]->nchaves == t-1 && arv->filho[i]->nchaves == t-1){ //CASO 2C
      printf("\nCASO 2C\n");
      TAB *y = arv->filho[i];
      TAB *z = arv->filho[i+1];
      y->chave[y->nchaves] = ch;          //colocar ch ao final de filho[i]
      int j;
      for(j=0; j<t-1; j++)                //juntar chave[i+1] com chave[i]
        y->chave[t+j] = z->chave[j];
      for(j=0; j<=t; j++)                 //juntar filho[i+1] com filho[i]
        y->filho[t+j] = z->filho[j];
      y->nchaves = 2*t-1;
      for(j=i; j < arv->nchaves-1; j++)   //remover ch de arv
        arv->chave[j] = arv->chave[j+1];
      for(j=i+1; j <= arv->nchaves; j++)  //remover ponteiro para filho[i+1]
        arv->filho[j] = arv->filho[j+1];
      arv->filho[j] = NULL; //Campello
      arv->nchaves--;
      arv->filho[i] = remover(arv->filho[i], ch, t);
      return arv;   
    }   
  }

  TAB *y = arv->filho[i], *z = NULL;
  if (y->nchaves == t-1){ //CASOS 3A e 3B
    if((i < arv->nchaves) && (arv->filho[i+1]->nchaves >=t)){ //CASO 3A
      printf("\nCASO 3A: i menor que nchaves\n");
      z = arv->filho[i+1];
      y->chave[t-1] = arv->chave[i];   //dar a y a chave i da arv
      y->nchaves++;
      arv->chave[i] = z->chave[0];     //dar a arv uma chave de z
      int j;
      for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
        z->chave[j] = z->chave[j+1];
      //z->chave[j] = 0; Rosseti
      y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
      for(j=0; j < z->nchaves; j++)       //ajustar filhos de z
        z->filho[j] = z->filho[j+1];
      z->nchaves--;
      arv->filho[i] = remover(arv->filho[i], ch, t);
      return arv;
    }
    if((i > 0) && (!z) && (arv->filho[i-1]->nchaves >=t)){ //CASO 3A
      printf("\nCASO 3A: i igual a nchaves\n");
      z = arv->filho[i-1];
      int j;
      for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
        y->chave[j] = y->chave[j-1];
      for(j = y->nchaves+1; j>0; j--)             //encaixar lugar dos filhos da nova chave
        y->filho[j] = y->filho[j-1];
      y->chave[0] = arv->chave[i-1];              //dar a y a chave i da arv
      y->nchaves++;
      arv->chave[i-1] = z->chave[z->nchaves-1];   //dar a arv uma chave de z
      y->filho[0] = z->filho[z->nchaves];         //enviar ponteiro de z para o novo elemento em y
      z->nchaves--;
      arv->filho[i] = remover(y, ch, t);
      return arv;
    }
    if(!z){ //CASO 3B
      if(i < arv->nchaves && arv->filho[i+1]->nchaves == t-1){
        printf("\nCASO 3B: i menor que nchaves\n");
        z = arv->filho[i+1];
        y->chave[t-1] = arv->chave[i];     //pegar chave [i] e coloca ao final de filho[i]
        y->nchaves++;
        int j;
        for(j=0; j < t-1; j++){
          y->chave[t+j] = z->chave[j];     //passar filho[i+1] para filho[i]
          y->nchaves++;
        }
        if(!y->folha){
          for(j=0; j<t; j++){
            y->filho[t+j] = z->filho[j];
          }
        }
        for(j=i; j < arv->nchaves-1; j++){ //limpar referências de i
          arv->chave[j] = arv->chave[j+1];
          arv->filho[j+1] = arv->filho[j+2];
        }
        arv->nchaves--;
        arv = remover(arv, ch, t);
        return arv;
      }
      if((i > 0) && (arv->filho[i-1]->nchaves == t-1)){ 
        printf("\nCASO 3B: i igual a nchaves\n");
        z = arv->filho[i-1];
        if(i == arv->nchaves)
          z->chave[t-1] = arv->chave[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
        else
          z->chave[t-1] = arv->chave[i];   //pegar chave [i] e poe ao final de filho[i-1]
        z->nchaves++;
        int j;
        for(j=0; j < t-1; j++){
          z->chave[t+j] = y->chave[j];     //passar filho[i+1] para filho[i]
          z->nchaves++;
        }
        if(!z->folha){
          for(j=0; j<t; j++){
            z->filho[t+j] = y->filho[j];
          }
        }
        arv->nchaves--;
        arv->filho[i-1] = z;
        arv = remover(arv, ch, t);
        return arv;
      }
    }
  }  
  arv->filho[i] = remover(arv->filho[i], ch, t);
  return arv;
}


TAB* retira(TAB* arv, int k, int t){
  if(!arv || !Busca(arv, k)) return arv;
  return remover(arv, k, t);
}

void imprime_intervalo(TAB *a, int x, int y)
{
  if(!a)
    return;
  int i=0;
  while((a->chave[i] < x) && (i < a->nchaves))
    i++;
  if((i < a->nchaves) && (a->chave[i]==x))
    i++;
    
  while((i < a->nchaves) && (a->chave[i] < y))
  {
    imprime_intervalo(a->filho[i], x, y);
    printf("%d->", a->chave[i]); 
    i++;
  }
  imprime_intervalo(a->filho[i], x, y);
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

TLSE *insere_intervalo2(TAB *a, int x, int y)
{
  if(!a)
    return NULL;
  TLSE *l = NULL;
  int i = 0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  while((i < a->nchaves) && (a->chave[i] < y))
  {
    TLSE *aux1 = insere_intervalo2(a->filho[i], x, y);
    l = junta_lista(l, aux1);
    l = lst_insere_fim(l, a->chave[i]);
    i++;
  }
  TLSE *aux2 = insere_intervalo2(a->filho[i], x, y);
  l = junta_lista(l, aux2);
  return l;
}

TLSE *insere_intervalo(TAB *a, int x, int y)
{
  if(!a)
    return NULL;
  int i=0;
  TLSE *l = NULL;
  
  while((a->chave[i] < y) && (i < a->nchaves))
  {
    if(a->chave[i] > x)
    {
      TLSE *aux = insere_intervalo(a->filho[i], x, y);
      l = junta_lista(l, aux);
      l = lst_insere_fim(l, a->chave[i]);
    }
    i++;
  }
  TLSE* aux = insere_intervalo(a->filho[i], x, y);
  l = junta_lista(l, aux);

  return l;
}

void junta_pilha(TP *p1, TP *p2)
{
  if(!vazia_pilha(p2))
  {
    TP *aux = cria_pilha();
    while(!vazia_pilha(p2))
      push(aux, pop(p2));
    while(!vazia_pilha(aux))
      push(p1, pop(aux));
  }
}

TP *insere_inter_pilha2(TAB *a, int x, int y)
{
  if(!a)
  {
    TP *vazia = cria_pilha();
    return vazia;
  }
  TP *p = cria_pilha();
  int i = 0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
    i++;
  while((i < a->nchaves) && (a->chave[i] < y))
  {
    TP *aux1 = insere_inter_pilha2(a->filho[i], x, y);
    junta_pilha(p, aux1);
    push(p, a->chave[i]);
    i++;
  }
  TP *aux2 = insere_inter_pilha2(a->filho[i], x, y);
  junta_pilha(p, aux2);
  return p; 
}

TP *insere_inter_pilha(TAB *a, int x, int y)
{
  if(!a)
  {
    TP *vaz = cria_pilha();
    return vaz;
  }
  int i=0;
  TP *p = cria_pilha();
  while((a->chave[i] < y) && (i < a->nchaves))
  {
    if(a->chave[i] > x)
    {
      TP *aux = insere_inter_pilha(a->filho[i], x, y);
      junta_pilha(p, aux);
      push(p, a->chave[i]);
      libera_pilha(aux);
    }
    i++;
  }
  TP *aux1 = insere_inter_pilha(a->filho[i], x, y);
  junta_pilha(p, aux1);
  libera_pilha(aux1);

  return p;
}

TLSE *insere_maior(TAB *a, int x)
{
  if(!a)
    return NULL;
  int i=0;
  TLSE *l = NULL;

  while((a->chave[i] <= x) && (i < a->nchaves))
    i++;
  
  while((i < a->nchaves))
  {
    TLSE *aux = insere_maior(a->filho[i], x);
    l = junta_lista(l, aux);
    if(a->chave[i] > x)
      l = lst_insere_fim(l, a->chave[i]);
    i++;
  }
  TLSE *aux = insere_maior(a->filho[i], x);
  l = junta_lista(l, aux);

  return l;
}

TP *insere_maior_pilha(TAB *a, int x)
{
  if(!a)
  {
    TP *vaz = cria_pilha();
    return vaz;
  }
  int i=0;
  TP *p = cria_pilha();
  while((a->chave[i] <= x) && (i < a->nchaves))
    i++;

  while(i < a->nchaves)
  {
    TP *aux = insere_maior_pilha(a->filho[i], x);
    junta_pilha(p, aux);
    if(a->chave[i] > x)
      push(p, a->chave[i]);
    libera_pilha(aux);
    i++;
  }
  TP *aux1 = insere_maior_pilha(a->filho[i], x);
  junta_pilha(p, aux1);
  libera_pilha(aux1);

  return p;
}

TLSE *lista_menor(TAB *a, int x)
{
  if(!a)
    return NULL;
  int i=0;
  TLSE *l = NULL;
  while((i < a->nchaves) && (a->chave[i] <= x))
  {
    TLSE *aux = lista_menor(a->filho[i], x);
    l = junta_lista(l, aux);
    if(a->chave[i] < x)
      l = lst_insere_fim(l, a->chave[i]);
    i++;
  }
  TLSE *temp = lista_menor(a->filho[i], x);
  l = junta_lista(l, temp);

  return l;
}

TP *pilha_menor(TAB *a, int x)
{
  if(!a)
  {
    TP *vaz = cria_pilha();
    return vaz;
  }
  int i=0;
  TP *p = cria_pilha();
  while((a->chave[i] <= x) && (i < a->nchaves))
  {
    TP *aux = pilha_menor(a->filho[i], x);
    junta_pilha(p, aux);
    if(a->chave[i] < x)
      push(p, a->chave[i]);
    libera_pilha(aux);
    i++;
  }
  TP *aux1 = pilha_menor(a->filho[i], x);
  junta_pilha(p, aux1);
  libera_pilha(aux1);

  return p;
}

void compara(TAB *a, int x, int *resp)
{
  if(!a)
    return;
  
  int i=0;
  while((i < a->nchaves) && (a->chave[i] < x))
    i++;
  if((i < a->nchaves) && (a->chave[i] == x))
  {
    if((*resp) == INT_MAX)
      *resp = a->chave[i];
    i++;
  }
  
  if((i < a->nchaves) && (a->chave[i] > x))
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
    
  compara(a->filho[i], x, resp);  
}

int sucessor(TAB *a, int x)
{
  if(!a)
    exit(1);
  int resp = INT_MAX;
  compara(a, x, &resp);
  if(resp == INT_MAX)
  {
    printf("Valor superior valores da arvore\n");
    exit(1);
  }
  else
    return resp;
}

void compara_ant(TAB *a, int x, int *resp)
{
  if(!a)
    return;
  
  int i=0;
  while((i < a->nchaves) && (a->chave[i] < x))
  { 
    if(*resp == x)
      *resp = a->chave[i];
    else
    {
      if(*resp < a->chave[i])
        *resp = a->chave[i];
    }
    i++;
  }
  if((i < a->nchaves) && (a->chave[i] == x))
  {
    if((*resp) == INT_MIN)
      *resp = a->chave[i];
  }

  if(a->folha)
    return;
  compara_ant(a->filho[i], x, resp);
    
}

int antecessor(TAB *a, int x)
{
  if(!a)
    exit(1);
  int resp = INT_MIN;
  compara_ant(a, x, &resp);
  if(resp == INT_MIN)
  {
    printf("Valor inferior valores da arvore\n");
    exit(1);
  }
  else
    return resp;
}




int main(int argc, char *argv[]){
  TAB * arvore = Inicializa();
  int num = 0, from, to;
  while(num != -1){
    printf("Digite um numero para adicionar. 0 para imprimir. -9 para remover e -1 para sair\n");
    scanf("%i", &num);
    if(num == -9){
      scanf("%d", &from);
      arvore = retira(arvore, from, t);
      Imprime(arvore,0);
    }
    else if(num == -1){
      printf("\n");
      Imprime(arvore,0);
      Libera(arvore);
      return 0;
    }
    else if(num == -5){
      printf("\n");
  
    //  TLSE *l = insere_intervalo2(arvore, 12, 90);
    //  imprime_lst(l);
    //  imprime_pilha(p);
    //TLSE *l = lista_menor(arvore, 22);
    //imprime_lst(l);
    //TP *p = insere_inter_pilha2(arvore, 12, 90);
    //imprime_pilha(p);
   //   imprime_intervalo(arvore, 12, 90);
      int n;
      scanf("%d", &n);
      printf("%d\n", antecessor(arvore, n));
    }
    else if(!num){
      printf("\n");
      Imprime(arvore,0);
    }
    else arvore = Insere(arvore, num, t);
    printf("\n\n");
  }
}
