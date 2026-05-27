#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD 21
#define MAX_N    1000
#define ALPHA    26

typedef struct {
    char texto[MAX_WORD];
    int  peso;
} Palavra;

int calcularPeso(const char *palavra, int freq[]) {
    int peso = 0;
    int i;
    for (i = 0; palavra[i] != '\0'; i++) {
        peso += freq[palavra[i] - 'a'];
    }
    return peso;
}


int ehValida(const char *palavra, int freq[], int k) {
    int i;
    for (i = 0; palavra[i] != '\0'; i++) {
        if (freq[palavra[i] - 'a'] < k) {
            return 0;
        }
    }
    return 1;
}

int maior(Palavra *a, Palavra *b) {
    if (a->peso != b->peso) {
        return (a->peso > b->peso) ? 1 : -1;
    }
    int cmp = strcmp(a->texto, b->texto);
    if (cmp > 0) return  1;
    if (cmp < 0) return -1;
    return 0;
}

void trocar(Palavra *v, int i, int j) {
    Palavra tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

void heapify(Palavra *v, int n, int raiz) {
    int maior_idx = raiz;
    int esq = 2 * raiz + 1;
    int dir = 2 * raiz + 2;

    if (esq < n && maior(&v[esq], &v[maior_idx]) > 0) {
        maior_idx = esq;
    }

    if (dir < n && maior(&v[dir], &v[maior_idx]) > 0) {
        maior_idx = dir;
    }

    if (maior_idx != raiz) {
        trocar(v, raiz, maior_idx);
        heapify(v, n, maior_idx);
    }
}

void buildHeap(Palavra *v, int n) {
    int i;
    for (i = n / 2 - 1; i >= 0; i--) {
        heapify(v, n, i);
    }
}

void heapSort(Palavra *v, int m) {
    int i;

    buildHeap(v, m);

    printf("build_heap:");
    for (i = 0; i < m; i++) {
        printf(" %s", v[i].texto);
    }
    printf("\n");

    for (i = m - 1; i > 0; i--) {
        trocar(v, 0, i);
        heapify(v, i, 0);
    }
}

int main(void) {
    int n, k;                   
    int freq[ALPHA];           
    Palavra entrada[MAX_N];    
    Palavra validas[MAX_N];     
    int m = 0;                 
    int i, j;

    scanf("%d %d", &n, &k);

    for (i = 0; i < n; i++) {
        scanf("%s", entrada[i].texto);
        entrada[i].peso = 0;
    }

    for (j = 0; j < ALPHA; j++) {
        freq[j] = 0;
    }
    for (i = 0; i < n; i++) {
        for (j = 0; entrada[i].texto[j] != '\0'; j++) {
            freq[(int)(entrada[i].texto[j] - 'a')]++;
        }
    }

    printf("\n");
    
    for (i = 0; i < n; i++) {
        if (!ehValida(entrada[i].texto, freq, k)) {
            printf("A palavra [%s] eh invalida (K=%d)\n",entrada[i].texto, k);
        } else {
            validas[m] = entrada[i];
            validas[m].peso = calcularPeso(entrada[i].texto, freq);
            m++;
        }
    }

    if (m == 0) {
        printf("Erro: Sem palavras validas para ordenacao.\n");
        return 0;
    }

    heapSort(validas, m);

    printf("palavras:");
    for (i = 0; i < m; i++) {
        printf(" %s", validas[i].texto);
    }
    printf("\n");

    return 0;
}