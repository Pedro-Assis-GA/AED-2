#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_TABELA 10

// Estrutura do Nó da Lista Encadeada
typedef struct No {
    int chave;
    int valor;
    struct No *proximo;
} No;

// Estrutura da Tabela Hash
typedef struct {
    No *listas[TAMANHO_TABELA];
} TabelaHash;

// Função Hash simples (Módulo)
int funcao_hash(int chave) {
    return chave % TAMANHO_TABELA;
}

// Inicializa a tabela preenchendo as posições com NULL
void inicializar_tabela(TabelaHash *tabela) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        tabela->listas[i] = NULL;
    }
}

// Insere um elemento na Tabela Hash
void inserir_hash(TabelaHash *tabela, int chave, int valor) {
    int indice = funcao_hash(chave);
    
    No *novo = (No*)malloc(sizeof(No));
    novo->chave = chave;
    novo->valor = valor;
    novo->proximo = tabela->listas[indice]; // Insere no início da lista (O(1))
    
    tabela->listas[indice] = novo;
}

// Busca um elemento na Tabela Hash
int buscar_hash(TabelaHash *tabela, int chave) {
    int indice = funcao_hash(chave);
    No *atual = tabela->listas[indice];
    
    while (atual != NULL) {
        if (atual->chave == chave) {
            return atual->valor; // Chave encontrada
        }
        atual = atual->proximo;
    }
    return -1; // Chave não encontrada
}

// Exibe a tabela para fins de debug
void imprimir_tabela(TabelaHash *tabela) {
    for (int i = 0; i < TAMANHO_TABELA; i++) {
        printf("Posição [%d]: ", i);
        No *atual = tabela->listas[i];
        while (atual != NULL) {
            printf("(%d -> %d) ", atual->chave, atual->valor);
            atual = atual->proximo;
        }
        printf("\n");
    }
}

int main() {
    TabelaHash minha_tabela;
    inicializar_tabela(&minha_tabela);

    inserir_hash(&minha_tabela, 15, 100);
    inserir_hash(&minha_tabela, 25, 200); // Gera colisão proposital com o 15
    inserir_hash(&minha_tabela, 32, 300);

    imprimir_tabela(&minha_tabela);

    int busca = buscar_hash(&minha_tabela, 25);
    if (busca != -1) {
        printf("\nValor encontrado para a chave 25: %d\n", busca);
    } else {
        printf("\nChave não encontrada.\n");
    }

    return 0;
}
