#include<stdio.h>
#include<stdlib.h>

typedef struct {
    int id;
    int gravidade;
} Paciente;

long long swaps_totais = 0;

//verificar se A tem prioridade sobre B
//Maior gravidade ou, se gravidade igual, menor id

int prioridade(Paciente a, Paciente b){
    if(a.gravidade > b.gravidade){
        return 1;
    }
    if(a.gravidade == b.gravidade && a.id < b.id){
        return 1;
    }
    return 0;
}

void troca(Paciente *a, Paciente *b){
    if(a->id != b->id || a->gravidade != b->gravidade){
        Paciente temp = *a;
        *a = *b;
        *b = temp;
        swaps_totais++;
    }
}

void mediana_tres(Paciente A[], int p, int r){
    int mid = p + (r - p)/2;

    if(prioridade(A[mid], A[p])){
        troca(&A[mid], &A[p]);
    }
    if(prioridade(A[r], A[p])){
        troca(&A[r], &A[p]);
    }
    if(prioridade(A[r], A[mid])){
        troca(&A[r], &A[mid]);
    }

    //coloca a mediana (que está em mid) na posição de pivo (fim)
    troca(&A[mid], &A[r]);
}

int particao(Paciente A[], int p, int r){
    mediana_tres(A, p, r);

    Paciente pivo = A[r];
    int i = p - 1;

    for(int j = p; j < r; j++){
        if(prioridade(A[j], pivo)){
            i++;
            troca(&A[i], &A[j]);
        }
    }
    troca(&A[i+1], &A[r]);
    return i+1;
}

void quickSort(Paciente A[], int p, int r){
    if(p < r){
        int q = particao(A, p, r);
        quickSort(A, p, q-1);
        quickSort(A, q+1, r);
    }
}

int main(){
    int n;

    if(scanf("%d", &n) != 1){
        return 0;
    }

    Paciente *pacientes = (Paciente*)malloc(n * sizeof(Paciente));
    for(int i = 0; i < n; i++){
        scanf("%d %d", &pacientes[i].id, &pacientes[i].gravidade);
    }

    quickSort(pacientes, 0, n-1);
    printf("\n");
    for(int i = 0; i < n; i++){
        printf("%d\n", pacientes[i].id);
    }
    printf("Swaps: %lld \n", swaps_totais);

    free(pacientes);

    return 0;
}