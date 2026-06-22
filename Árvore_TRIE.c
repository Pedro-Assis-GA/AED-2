#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ALFABETO_TAMANHO 26 // Apenas letras minúsculas de 'a' a 'z'

// Estrutura do Nó da Árvore Trie
typedef struct NoTrie {
    struct NoTrie *filhos[ALFABETO_TAMANHO];
    bool fim_da_palavra;
} NoTrie;

// Cria um novo nó da Trie
NoTrie* criar_no_trie() {
    NoTrie *novo = (NoTrie*)malloc(sizeof(NoTrie));
    novo->fim_da_palavra = false;
    for (int i = 0; i < ALFABETO_TAMANHO; i++) {
        novo->filhos[i] = NULL;
    }
    return novo;
}

// Insere uma palavra na Trie
void inserir_trie(NoTrie *raiz, const char *palavra) {
    NoTrie *atual = raiz;
    for (int i = 0; palavra[i] != '\0'; i++) {
        int indice = palavra[i] - 'a'; // Mapeia caractere para 0-25
        if (atual->filhos[indice] == NULL) {
            atual->filhos[indice] = criar_no_trie();
        }
        atual = atual->filhos[indice];
    }
    atual->fim_da_palavra = true;
}

// Busca uma palavra exata na Trie
bool buscar_trie(NoTrie *raiz, const char *palavra) {
    NoTrie *atual = raiz;
    for (int i = 0; palavra[i] != '\0'; i++) {
        int indice = palavra[i] - 'a';
        if (atual->filhos[indice] == NULL) {
            return false;
        }
        atual = atual->filhos[indice];
    }
    return (atual != NULL && atual->fim_da_palavra);
}

int main() {
    NoTrie *raiz = criar_no_trie();

    inserir_trie(raiz, "algoritmo");
    inserir_trie(raiz, "estrutura");
    inserir_trie(raiz, "dados");

    printf("Busca por 'algoritmo': %s\n", buscar_trie(raiz, "algoritmo") ? "Encontrado" : "Não Encontrado");
    printf("Busca por 'alvo': %s\n", buscar_trie(raiz, "alvo") ? "Encontrado" : "Não Encontrado");

    return 0;
}
