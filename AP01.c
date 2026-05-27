#include<stdio.h>
#include<stdlib.h>

//estrutura da struct
typedef struct pont{
    int id;
    struct pont *prox;
    struct pont *ant;
} point;

//variáveis globais (a fim de evitar ponteiros duplos no código)
point* cabeca = NULL;
point* final = NULL;
int nep_tot = 0;


//Função para inserção ordenada 

void insere(int x){
    point* novo = (point*)malloc(sizeof(point));
    novo->id = x;
    novo->ant = NULL;
    novo->prox = NULL;

    //caso a lista esteja vazia 
    if(cabeca == NULL){
        cabeca = novo;
        final = novo;
        return;
    }
    //Novo id (x nesse caso) <= cabeça->ID (insere no inicio)
    if(x <= cabeca->id){
        novo->prox = cabeca;
        cabeca->ant = novo;
        cabeca = novo;
        return;
    }
    //Novo id (x nesse caso) >= final->ID (insere no fim)
    if(x >= final->id){
        novo->ant = final;
        final->prox = novo;
        final = novo;
        return;
    }
    //inserção no meio pela regra da proximidade 
    int dist_c = x - cabeca->id;
    int dist_f = final->id - x;

    point* atual;

    //busca a partir da cabeça
    if(dist_c <= dist_f){
        atual = cabeca;
        //"anda pra frente" até achar a posição certa 
        while(atual != NULL && atual->id < x){
            atual = atual->prox;
            nep_tot++;
        }
        novo->prox =atual;
        novo->ant = atual->ant;
        if(atual->ant != NULL){
            atual->ant->prox = novo;
        }
        atual->ant = novo;
    }
    //busca a partir do final
    else{
        atual = final;
        //retrocede até achar a posição certa 
        while(atual != NULL && atual->id > x){
            atual = atual->ant;
            nep_tot++;
        }
        //ajuste de ponteiros para inserir depois do nó 'atual'
        novo->ant = atual;
        novo->prox = atual->prox;
        if(atual->prox != NULL){
            atual->prox->ant = novo;
        }
        atual->prox = novo;
    }  
}

/*função principal e saída*/

int  main(){
    
    int n, id_lido;

    //entrada: N e depois os IDs 
    if(scanf("%d", &n) != 1){
        return 0;
    }

    for(int i=0; i<n; i++){
        if(scanf("%d", &id_lido) == 1){
            insere(id_lido);
        }
    }

    //saída: lista ordenada 
    point* temp = cabeca;
    while(temp != NULL){
        printf("%d", temp->id);
        if(temp->prox != NULL){
            printf(" ");
        }
        temp = temp->prox;
    }

    //saída do NEP total    
    printf("\n%d\n", nep_tot);

    //liberação de memória por boa prática
    temp = cabeca;
    while(temp != NULL){
        point* aux = temp;
        temp = temp->prox;
        free(aux);
    }
    
    return 0;
}