#include <stdio.h>
#include <stdlib.h>

//0. Primeira função pra fazer o merge e o número de quedas drásticas
long long int merge_conta(int arr[], int esquerda, int meio, int direita){
    long long int contagem_quedas = 0;

    //1. Parte para separar os elementos do vetor

    //1.1. Tamanho das metades
    int n1 = meio - esquerda + 1;
    int n2 = direita - meio;

    //1.2. Criar vetores temporários para auxiliar
    int *L = (int*)malloc(n1*sizeof(int));
    int *R = (int*)malloc(n2*sizeof(int));

    //1.3 Copiar os dados do vetor original para o auxiliar temporário
    for(int i = 0; i < n1; i++){
        L[i] = arr[esquerda + i];
    }
    for(int j = 0; j < n2; j++){
        R[j] = arr[meio + 1 + j];
    }

    //2. Contagem rápida

    //2.1 Ponteiro para varrer o vetor da direita
    int j_conta = 0;

    for(int i = 0; i < n1; i++){
        //Condição do exercício (V[i] > 2xV[j])
        while(j_conta < n2 && L[i] > 2LL * R[j_conta]){
            j_conta++; //Avança na direita
        }
        contagem_quedas += j_conta;
    }

    //3. Merge clássico 

    int i = 0; //Indice do subvetor da esquerda (L)
    int j = 0; //Indice do subvetor da direita (R)
    int k = esquerda; //Indice incial do vetor fundido

    // Compara o topo das duas "pilhas" e pega o menor
    while(i < n1 && j < n2){
        if(L[i] <= R[j]){
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    //3.1. Limpeza: se sobraram elementos na pilha da esquerda, copia todos
    while(i<n1){
        arr[k] = L[i];
        i++;
        k++;
    }

    //3.2. Limpeza: se sobraram elementos na pilha da direita, copia todos
    while(j<n2){
        arr[k] = R[j];
        j++;
        k++;
    }
    
    //4. Libera memória temporária (muito importante pra evitar vazamentos)
    free(L);
    free(R);

    //5. Retorna a contagem de quedas para usarmos na saída depois
    return contagem_quedas;
}

//6. Divisão recursiva
long long int mergeSort_conta(int arr[], int esquerda, int direita){
    long long int quedas_totais = 0;

    if(esquerda < direita){
        int meio = esquerda + (direita - esquerda) / 2;

        //7. Soma das quedas drásticas

        //7.1. Quedas achadas na metade esquerda
        quedas_totais += mergeSort_conta(arr, esquerda, meio);

        //7.2. Quedas achadas na metade direita
        quedas_totais += mergeSort_conta(arr, meio + 1, direita);

        //7.3. Soma as quedas cruzadas entre as metades
        quedas_totais += merge_conta(arr, esquerda, meio, direita);
    }

    return quedas_totais;
}

int main(){
    int n;

    if(scanf("%d", &n) != 1){
        return 1;
    }

    //Alocação dinâmica do vetor principal 
    int *arr = (int*)malloc(n * sizeof(int));

    for(int i = 0; i < n; i++){
        scanf("%d", &arr[i]);
    }

    long long int total_quedas = mergeSort_conta(arr, 0, n - 1);

    //Saída seca (lld foi usado para imprimir long long (anotação para lembra sobre long long))
    printf("%lld\n", total_quedas);

    for(int i = 0; i < n; i++){
        printf("%d", arr[i]);
        //Para não deixar espaço sobrando no final
        if(i < n-1){
            printf(" ");
        }
    }
    printf("\n");

    free(arr);

    return 0;
}