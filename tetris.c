#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CAPACIDADE_FILA 5

// -----------------------------------------------------------------------------
// 1. ESTRUTURAS DE DADOS
// -----------------------------------------------------------------------------

// Struct que representa cada peca do Tetris
typedef struct {
    char nome; // Tipo da peca: 'I', 'O', 'T', 'L'
    int id;    // Identificador unico sequencial
} Peca;

// Struct para controle da Fila Circular
typedef struct {
    Peca itens[CAPACIDADE_FILA];
    int inicio;     // Indice do primeiro elemento da fila
    int fim;        // Indice da proxima posicao livre
    int total;      // Quantidade atual de elementos na fila
} FilaCircular;

// Variável global para gerar IDs unicos sequenciais
int contadorIdPeca = 0;

// -----------------------------------------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// -----------------------------------------------------------------------------
void inicializarFila(FilaCircular *fila);
Peca gerarPeca(void);
int enqueue(FilaCircular *fila, Peca peca);
int dequeue(FilaCircular *fila, Peca *pecaRemovida);
void exibirFila(const FilaCircular *fila);
void limparBuffer(void);

// -----------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL (MENU INTERATIVO)
// -----------------------------------------------------------------------------
int main() {
    // Inicializa a semente para geracao aleatoria dos tipos de peca
    srand((unsigned int)time(NULL));

    FilaCircular filaPecas;
    inicializarFila(&filaPecas);

    // Preenche a fila inicial com 5 pecas geradas automaticamente
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(&filaPecas, gerarPeca());
    }

    int opcao;

    do {
        printf("\n========================================\n");
        printf("         TETRIS STACK - BYTEBROS        \n");
        printf("========================================\n");
        
        // Exibe o estado atual da fila de pecas
        exibirFila(&filaPecas);

        printf("\nOpcoes de acao:\n");
        printf(" 1. Jogar peca (dequeue)\n");
        printf(" 2. Inserir nova peca (enqueue)\n");
        printf(" 0. Sair\n");
        printf("----------------------------------------\n");
        printf("Escolha um comando: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        limparBuffer();

        switch (opcao) {
            case 1: {
                Peca removida;
                if (dequeue(&filaPecas, &removida)) {
                    printf("\n[AÇAO] Peca [%c %d] foi jogada no tabuleiro!\n", 
                           removida.nome, removida.id);
                } else {
                    printf("\n[AVISO] A fila esta vazia! Nenhuma peca para jogar.\n");
                }
                break;
            }

            case 2: {
                Peca nova = gerarPeca();
                if (enqueue(&filaPecas, nova)) {
                    printf("\n[AÇAO] Nova peca [%c %d] entrou na fila!\n", 
                           nova.nome, nova.id);
                } else {
                    printf("\n[AVISO] A fila esta cheia (%d/%d)! Jogue uma peca primeiro.\n", 
                           filaPecas.total, CAPACIDADE_FILA);
                }
                break;
            }

            case 0:
                printf("\nEncerrando controle da fila. Ate a proxima partida!\n");
                break;

            default:
                printf("\n[ERRO] Opcao invalida! Escolha 1, 2 ou 0.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}

// -----------------------------------------------------------------------------
// IMPLEMENTAÇÃO DA FILA CIRCULAR
// -----------------------------------------------------------------------------

// Inicializa os ponteiros e o contador da fila
void inicializarFila(FilaCircular *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->total = 0;
}

// Gera automaticamente uma nova peca com formato aleatorio e ID sequencial
Peca gerarPeca(void) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca novaPeca;
    
    // Seleciona um dos 4 tipos de forma aleatoria
    novaPeca.nome = tipos[rand() % 4];
    
    // Atribui o proximo ID disponivel
    novaPeca.id = contadorIdPeca++;
    
    return novaPeca;
}

// Insere uma peca no final da fila (Enqueue)
int enqueue(FilaCircular *fila, Peca peca) {
    if (fila->total >= CAPACIDADE_FILA) {
        return 0; // Fila cheia
    }

    fila->itens[fila->fim] = peca;
    
    // Aritmetica circular para o ponteiro do fim
    fila->fim = (fila->fim + 1) % CAPACIDADE_FILA;
    fila->total++;

    return 1; // Sucesso
}

// Remove a peca da frente da fila (Dequeue)
int dequeue(FilaCircular *fila, Peca *pecaRemovida) {
    if (fila->total == 0) {
        return 0; // Fila vazia
    }

    *pecaRemovida = fila->itens[fila->inicio];

    // Aritmetica circular para o ponteiro do inicio
    fila->inicio = (fila->inicio + 1) % CAPACIDADE_FILA;
    fila->total--;

    return 1; // Sucesso
}

// Exibe a fila formatada: Fila de peças: [T 0] [O 1] [L 2] [I 3] [I 4]
void exibirFila(const FilaCircular *fila) {
    printf("\nFila de pecas: ");

    if (fila->total == 0) {
        printf("(vazia)");
    } else {
        // Percorre a fila circular a partir da posicao inicio
        for (int i = 0; i < fila->total; i++) {
            int idx = (fila->inicio + i) % CAPACIDADE_FILA;
            printf("[%c %d] ", fila->itens[idx].nome, fila->itens[idx].id);
        }
    }
    printf("\n");
}

// Auxiliar para limpar o buffer do teclado
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}