#include <stdio.h>
#include <stdlib.h>

/* Estrutura para representar um único bloco dentro de uma fita */
typedef struct {
    int *values;
    int count;
    int capacity;
} Block;

/* Estrutura para emular uma fita magnética contendo múltiplos blocos */
typedef struct {
    Block *blocks;
    int block_count;
    int block_capacity;
} Tape;

/* Estrutura para os nós do Heap de Intercalação (Fase 2) */
typedef struct {
    int value;
    int tape_id;
} HeapNode;

/* --- Protótipos das Funções --- */
void min_heapify(int *heap, int size, int i);
void build_min_heap(int *heap, int size);
void intercalation_heapify(HeapNode *heap, int size, int i);
void build_intercalation_heap(HeapNode *heap, int size);
void init_tape(Tape *t);
void add_block_to_tape(Tape *t);
void add_element_to_current_block(Tape *t, int val);
void free_tape_blocks(Tape *t);
void print_tape(Tape *t, int id);

int main() {
    int N, M, K;
    int *heap = NULL;
    Tape *tapes = NULL;
    int i;
    int elements_read;
    int current_block_tape;
    int printed_heap_alterado;
    int active_size;
    int in_start, out_start;
    int is_first_pass;
    int *current_block_idx = NULL;
    int *current_element_idx = NULL;
    int final_tape_idx;

    /* Leitura dos parâmetros iniciais */
    if (scanf("%d %d %d", &N, &M, &K) != 3) {
        return 0;
    }

    /* Alocação do Heap interno para a Fase 1 */
    heap = (int *)malloc(M * sizeof(int));
    for (i = 0; i < M; i++) {
        if (scanf("%d", &heap[i]) != 1) {
            /* Tratamento preventivo de leitura */
        }
    }

    /* Construção do Heap Inicial */
    build_min_heap(heap, M);

    /* 1. [Fase 1 - Heap Inicial] */
    printf("[Fase 1 - Heap Inicial]:");
    for (i = 0; i < M; i++) {
        printf(" %d", heap[i]);
    }
    printf("\n");

    /* Inicialização das 2 * K fitas emuladas */
    tapes = (Tape *)malloc(2 * K * sizeof(Tape));
    for (i = 0; i < 2 * K; i++) {
        init_tape(&tapes[i]);
    }

    /* --- FASE 1: Geração de Blocos Iniciais (Seleção por Substituição) --- */
    active_size = M;
    elements_read = M;
    current_block_tape = 0;
    printed_heap_alterado = 0;

    add_block_to_tape(&tapes[0]);

    while (active_size > 0 || elements_read < N) {
        if (active_size == 0) {
            /* 2. [Fase 1 - Heap Alterado] - Momento exato em que o 1º bloco fecha */
            if (!printed_heap_alterado) {
                printf("[Fase 1 - Heap Alterado]:");
                for (i = 0; i < M; i++) {
                    printf(" %d", heap[i]);
                }
                printf("\n");
                printed_heap_alterado = 1;
            }

            /* Reinicia o heap com os elementos marcados para o próximo bloco */
            active_size = M;
            build_min_heap(heap, M);
            current_block_tape = (current_block_tape + 1) % K;
            add_block_to_tape(&tapes[current_block_tape]);
        }

        /* Retira o menor elemento ativo do topo */
        int root_val = heap[0];
        add_element_to_current_block(&tapes[current_block_tape], root_val);

        if (elements_read < N) {
            int next_val;
            if (scanf("%d", &next_val) == 1) {
                elements_read++;
                if (next_val >= root_val) {
                    /* Pertence ao bloco atual */
                    heap[0] = next_val;
                    min_heapify(heap, active_size, 0);
                } else {
                    /* Marcado para o próximo bloco: vai para o fim da zona ativa */
                    heap[0] = heap[active_size - 1];
                    heap[active_size - 1] = next_val;
                    active_size--;
                    if (active_size > 0) {
                        min_heapify(heap, active_size, 0);
                    }
                }
            }
        } else {
            /* Acabou a entrada, apenas reduz o tamanho do Heap ativo */
            heap[0] = heap[active_size - 1];
            active_size--;
            if (active_size > 0) {
                min_heapify(heap, active_size, 0);
            }
        }
    }

    /* 3. [Fase 1 Fitas Geradas] */
    printf("[Fase 1 Fitas Geradas]:\n");
    for (i = 0; i < K; i++) {
        print_tape(&tapes[i], i);
    }

    /* --- FASE 2: Intercalação Balanceada (K-Way Merge) --- */
    in_start = 0;
    out_start = K;
    is_first_pass = 1;

    current_block_idx = (int *)malloc(2 * K * sizeof(int));
    current_element_idx = (int *)malloc(2 * K * sizeof(int));

    while (1) {
        int output_block_count = 0;

        for (i = 0; i < 2 * K; i++) {
            current_block_idx[i] = 0;
            current_element_idx[i] = 0;
        }

        /* Limpa as fitas de destino antes da passada */
        for (i = 0; i < K; i++) {
            free_tape_blocks(&tapes[out_start + i]);
        }

        while (1) {
            int elements_in_heap = 0;
            HeapNode *intercalation_heap = (HeapNode *)malloc(K * sizeof(HeapNode));

            /* Coleta o primeiro elemento disponível do bloco atual de cada fita de entrada */
            for (i = 0; i < K; i++) {
                int in_tape = in_start + i;
                int b_idx = current_block_idx[in_tape];
                if (b_idx < tapes[in_tape].block_count) {
                    int e_idx = current_element_idx[in_tape];
                    intercalation_heap[elements_in_heap].value = tapes[in_tape].blocks[b_idx].values[e_idx];
                    intercalation_heap[elements_in_heap].tape_id = in_tape;
                    elements_in_heap++;
                }
            }

            if (elements_in_heap == 0) {
                free(intercalation_heap);
                break; /* Todas as fitas e blocos desta passada foram consumidos */
            }

            build_intercalation_heap(intercalation_heap, elements_in_heap);

            /* 4. [Fase 2 Heap de Intercalação] - Apenas no início da primeira intercalação */
            if (is_first_pass && output_block_count == 0) {
                printf("[Fase 2 - Heap Intercalacao]:");
                for (i = 0; i < elements_in_heap; i++) {
                    printf(" %d", intercalation_heap[i].value);
                }
                printf("\n");
            }

            int out_tape_idx = out_start + (output_block_count % K);
            if (tapes[out_tape_idx].block_count <= output_block_count / K) {
                add_block_to_tape(&tapes[out_tape_idx]);
            }

            /* Efetua a fusão (merge) dos blocos atuais */
            while (elements_in_heap > 0) {
                HeapNode min_node = intercalation_heap[0];
                add_element_to_current_block(&tapes[out_tape_idx], min_node.value);

                int t_id = min_node.tape_id;
                current_element_idx[t_id]++;

                int b_idx = current_block_idx[t_id];
                if (current_element_idx[t_id] < tapes[t_id].blocks[b_idx].count) {
                    intercalation_heap[0].value = tapes[t_id].blocks[b_idx].values[current_element_idx[t_id]];
                    intercalation_heap[0].tape_id = t_id;
                    intercalation_heapify(intercalation_heap, elements_in_heap, 0);
                } else {
                    /* Fim do bloco atual desta fita específica */
                    current_block_idx[t_id]++;
                    current_element_idx[t_id] = 0;

                    intercalation_heap[0] = intercalation_heap[elements_in_heap - 1];
                    elements_in_heap--;
                    if (elements_in_heap > 0) {
                        intercalation_heapify(intercalation_heap, elements_in_heap, 0);
                    }
                }
            }

            output_block_count++;
            free(intercalation_heap);
        }

        /* 5. [Fase 2 Fitas após 1ª Passada] */
        if (is_first_pass) {
            printf("[Fase 2 Fitas apos 1a Passada]:\n");
            for (i = 0; i < K; i++) {
                print_tape(&tapes[out_start + i], out_start + i);
            }
            is_first_pass = 0;
        }

        /* Se sobrou apenas 1 bloco resultante global, o processo acabou */
        if (output_block_count <= 1) {
            final_tape_idx = -1;
            for (i = 0; i < K; i++) {
                if (tapes[out_start + i].block_count == 1) {
                    final_tape_idx = out_start + i;
                    break;
                }
            }
            if (final_tape_idx == -1) {
                for (i = 0; i < K; i++) {
                    if (tapes[in_start + i].block_count == 1) {
                        final_tape_idx = in_start + i;
                        break;
                    }
                }
            }
            break;
        }

        /* Inverte os papéis: quem era saída vira entrada e vice-versa */
        int temp = in_start;
        in_start = out_start;
        out_start = temp;
    }

    /* 6. [Resultado Final] */
    printf("[Resultado Final]:");
    if (final_tape_idx != -1 && tapes[final_tape_idx].block_count > 0) {
        for (i = 0; i < tapes[final_tape_idx].blocks[0].count; i++) {
            printf(" %d", tapes[final_tape_idx].blocks[0].values[i]);
        }
    }
    printf("\n");

    /* Liberação de Memória Limpa */
    for (i = 0; i < 2 * K; i++) {
        free_tape_blocks(&tapes[i]);
    }
    free(tapes);
    free(heap);
    free(current_block_idx);
    free(current_element_idx);

    return 0;
}

/* --- Implementação das Funções Auxiliares de Heap e Fita --- */

void min_heapify(int *heap, int size, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left] < heap[smallest]) smallest = left;
    if (right < size && heap[right] < heap[smallest]) smallest = right;

    if (smallest != i) {
        int temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;
        min_heapify(heap, size, smallest);
    }
}

void build_min_heap(int *heap, int size) {
    int i;
    for (i = size / 2 - 1; i >= 0; i--) {
        min_heapify(heap, size, i);
    }
}

void intercalation_heapify(HeapNode *heap, int size, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && heap[left].value < heap[smallest].value) smallest = left;
    if (right < size && heap[right].value < heap[smallest].value) smallest = right;

    if (smallest != i) {
        HeapNode temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;
        intercalation_heapify(heap, size, smallest);
    }
}

void build_intercalation_heap(HeapNode *heap, int size) {
    int i;
    for (i = size / 2 - 1; i >= 0; i--) {
        intercalation_heapify(heap, size, i);
    }
}

void init_tape(Tape *t) {
    t->blocks = NULL;
    t->block_count = 0;
    t->block_capacity = 0;
}

void add_block_to_tape(Tape *t) {
    if (t->block_count >= t->block_capacity) {
        t->block_capacity = t->block_capacity == 0 ? 4 : t->block_capacity * 2;
        t->blocks = (Block *)realloc(t->blocks, t->block_capacity * sizeof(Block));
    }
    t->blocks[t->block_count].values = NULL;
    t->blocks[t->block_count].count = 0;
    t->blocks[t->block_count].capacity = 0;
    t->block_count++;
}

void add_element_to_current_block(Tape *t, int val) {
    int idx = t->block_count - 1;
    if (t->blocks[idx].count >= t->blocks[idx].capacity) {
        t->blocks[idx].capacity = t->blocks[idx].capacity == 0 ? 8 : t->blocks[idx].capacity * 2;
        t->blocks[idx].values = (int *)realloc(t->blocks[idx].values, t->blocks[idx].capacity * sizeof(int));
    }
    t->blocks[idx].values[t->blocks[idx].count] = val;
    t->blocks[idx].count++;
}

void free_tape_blocks(Tape *t) {
    int i;
    for (i = 0; i < t->block_count; i++) {
        if (t->blocks[i].values) {
            free(t->blocks[i].values);
        }
    }
    if (t->blocks) {
        free(t->blocks);
    }
    init_tape(t);
}

void print_tape(Tape *t, int id) {
    int i, j;
    printf("Fita %d:", id);
    for (i = 0; i < t->block_count; i++) {
        printf(" [");
        for (j = 0; j < t->blocks[i].count; j++) {
            printf("%d", t->blocks[i].values[j]);
            if (j < t->blocks[i].count - 1) printf(" ");
        }
        printf("]");
    }
    printf("\n");
}