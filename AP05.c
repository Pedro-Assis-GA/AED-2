#include <stdio.h>
#include <stdlib.h>

// Estrutura do Nó
typedef struct No {
    int valor;
    int peso;
    struct No *esq, *dir;
} No;

// Função para obter o peso de um nó com segurança para NULL [cite: 115]
int getPeso(No *n) {
    if (n == NULL) return 0;
    return n->peso;
}

// Atualiza o peso de um nó baseando-se nos filhos [cite: 114]
void atualizarPeso(No *n) {
    if (n != NULL) {
        n->peso = 1 + getPeso(n->esq) + getPeso(n->dir);
    }
}

// Cria um novo nó folha [cite: 116]
No* criarNo(int valor) {
    No *novo = (No*)malloc(sizeof(No));
    novo->valor = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->peso = 1;
    return novo;
}

// Rotação Simples à Direita (LL)
No* rotacaoLL(No *y) {
    No *x = y->esq;
    No *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    atualizarPeso(y);
    atualizarPeso(x);

    return x;
}

// Rotação Simples à Esquerda (RR)
No* rotacaoRR(No *x) {
    No *y = x->dir;
    No *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    atualizarPeso(x);
    atualizarPeso(y);

    return y;
}

// Rotação Dupla à Direita (LR) [cite: 135]
No* rotacaoLR(No *n) {
    n->esq = rotacaoRR(n->esq);
    return rotacaoLL(n);
}

// Rotação Dupla à Esquerda (RL) [cite: 136]
No* rotacaoRL(No *n) {
    n->dir = rotacaoLL(n->dir);
    return rotacaoRR(n);
}

// Verifica e aplica o balanceamento por peso [cite: 125, 126, 128, 130]
No* balancear(No *n) {
    if (n == NULL) return NULL;

    atualizarPeso(n);

    int pEsq = getPeso(n->esq);
    int pDir = getPeso(n->dir);

    // Pendente à esquerda [cite: 133]
    if ((pEsq + 1) > 2 * (pDir + 1)) {
        int pEsqEsq = getPeso(n->esq->esq);
        int pEsqDir = getPeso(n->esq->dir);
        
        if (pEsqEsq >= pEsqDir) {
            return rotacaoLL(n); // [cite: 134]
        } else {
            return rotacaoLR(n); // [cite: 135]
        }
    }

    // Pendente à direita [cite: 136]
    if ((pDir + 1) > 2 * (pEsq + 1)) {
        int pDirDir = getPeso(n->dir->dir);
        int pDirEsq = getPeso(n->dir->esq);

        if (pDirDir >= pDirEsq) {
            return rotacaoRR(n);
        } else {
            return rotacaoRL(n);
        }
    }

    return n;
}

// Inserção na Árvore
No* inserir(No *raiz, int valor) {
    if (raiz == NULL) return criarNo(valor);

    if (valor < raiz->valor) {
        raiz->esq = inserir(raiz->esq, valor);
    } else if (valor > raiz->valor) {
        raiz->dir = inserir(raiz->dir, valor);
    } else {
        return raiz; // Ignora duplicatas
    }

    return balancear(raiz);
}

// Remoção na Árvore promovendo o predecessor [cite: 138, 139]
No* remover(No *raiz, int valor) {
    if (raiz == NULL) return NULL;

    if (valor < raiz->valor) {
        raiz->esq = remover(raiz->esq, valor);
    } else if (valor > raiz->valor) {
        raiz->dir = remover(raiz->dir, valor);
    } else {
        // Nó com apenas um filho ou folha
        if (raiz->esq == NULL || raiz->dir == NULL) {
            No *temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp; 
            }
            free(temp);
        } else {
            // Nó com dois filhos: pega o maior da subárvore esquerda [cite: 138]
            No *temp = raiz->esq;
            while (temp->dir != NULL) {
                temp = temp->dir;
            }
            raiz->valor = temp->valor;
            raiz->esq = remover(raiz->esq, temp->valor);
        }
    }

    if (raiz == NULL) return NULL;
    return balancear(raiz);
}

// Busca in-order e imprime os elementos dentro da faixa X e Y [cite: 146, 153]
void imprimirFaixa(No *raiz, int X, int Y, int *primeiro, int *encontrou) {
    if (raiz == NULL) return;

    if (raiz->valor > X) {
        imprimirFaixa(raiz->esq, X, Y, primeiro, encontrou);
    }

    if (raiz->valor >= X && raiz->valor <= Y) {
        if (!(*primeiro)) {
            printf(", ");
        }
        printf("%d", raiz->valor);
        *primeiro = 0;
        *encontrou = 1;
    }

    if (raiz->valor < Y) {
        imprimirFaixa(raiz->dir, X, Y, primeiro, encontrou);
    }
}

// Limpa a memória
void liberarArvore(No *raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
    }
}

int main() {
    No *raiz = NULL;
    int valor;

    // Leitura das Inserções (Linha 1) [cite: 144]
    while (scanf("%d", &valor) == 1 && valor != -1) {
        raiz = inserir(raiz, valor);
    }

    // Saída das Inserções [cite: 149]
    if (raiz != NULL) {
        printf("%d,%d,%d\n", getPeso(raiz), getPeso(raiz->esq), getPeso(raiz->dir));
    } else {
        printf("ARVORE VAZIA\n");
    }

    // Leitura das Remoções (Linha 2) [cite: 145]
    while (scanf("%d", &valor) == 1 && valor != -1) {
        raiz = remover(raiz, valor);
    }

    // Saída das Remoções [cite: 150, 151, 152]
    if (raiz != NULL) {
        printf("%d,%d,%d\n", getPeso(raiz), getPeso(raiz->esq), getPeso(raiz->dir));
    } else {
        printf("ARVORE VAZIA\n");
    }

    // Leitura da Faixa (Linha 3) e Impressão [cite: 146, 153, 154, 155]
    int X, Y;
    if (scanf("%d %d", &X, &Y) == 2) {
        int primeiro = 1;
        int encontrou = 0;
        
        imprimirFaixa(raiz, X, Y, &primeiro, &encontrou);
        
        if (!encontrou) {
            printf("NADA A EXIBIR"); // [cite: 155]
        }
        printf("\n");
    }

    liberarArvore(raiz);
    return 0;
}