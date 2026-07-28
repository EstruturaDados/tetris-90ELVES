#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define CAPACIDADE_PILHA 3

// -----------------------------------------------------------------------------
// 1. ESTRUTURAS DE DADOS
// -----------------------------------------------------------------------------

// Struct que representa cada peca do Tetris
typedef struct {
    char nome; // Tipo da peca: 'I', 'O', 'T', 'L'
    int id;    // Identificador unico sequencial
} Peca;

// Struct para a Fila Circular (Pecas Futuras)
typedef struct {
    Peca itens[TAM_FILA];
    int inicio;
    int fim;
    int total;
} FilaCircular;

// Struct para a Pilha (Reserva)
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo; // Indica o indice do elemento no topo (-1 quando vazia)
} PilhaReserva;

// Variavel global para gerar IDs unicos e sequenciais
int contadorIdPeca = 0;

// -----------------------------------------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// -----------------------------------------------------------------------------
void inicializarFila(FilaCircular *fila);
void inicializarPilha(PilhaReserva *pilha);
Peca gerarPeca(void);

// Operacoes da Fila
int enqueue(FilaCircular *fila, Peca peca);
int dequeue(FilaCircular *fila, Peca *pecaRemovida);

// Operacoes da Pilha
int push(PilhaReserva *pilha, Peca peca);
int pop(PilhaReserva *pilha, Peca *pecaRemovida);

// Interface e Exibicao
void exibirEstado(const FilaCircular *fila, const PilhaReserva *pilha);
void limparBuffer(void);

// -----------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL (MENU INTERATIVO)
// -----------------------------------------------------------------------------
int main() {
    // Semente para geracao aleatoria das pecas
    srand((unsigned int)time(NULL));

    FilaCircular filaPecas;
    PilhaReserva pilhaReserva;

    inicializarFila(&filaPecas);
    inicializarPilha(&pilhaReserva);

    // Preenche a fila inicial com 5 pecas geradas automaticamente
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&filaPecas, gerarPeca());
    }

    int opcao;

    do {
        printf("\n==================================================\n");
        printf("         TETRIS STACK - NÍVEL INTERMEDIÁRIO       \n");
        printf("==================================================\n");

        // Exibe a fila e a pilha conforme o formato solicitado
        exibirEstado(&filaPecas, &pilhaReserva);

        printf("\nOpcoes de Acao:\n");
        printf(" 1. Jogar peca (dequeue)\n");
        printf(" 2. Reservar peca (fila -> pilha)\n");
        printf(" 3. Usar peca reservada (pop)\n");
        printf(" 0. Sair\n");
        printf("--------------------------------------------------\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        limparBuffer();

        switch (opcao) {
            case 1: { // JOGAR PEÇA
                Peca jogada;
                if (dequeue(&filaPecas, &jogada)) {
                    printf("\n[AÇAO] Peca [%c %d] foi jogada no tabuleiro!\n", jogada.nome, jogada.id);
                    // Regra: repoe a fila automaticamente
                    enqueue(&filaPecas, gerarPeca());
                }
                break;
            }

            case 2: { // RESERVAR PEÇA
                if (pilhaReserva.topo >= CAPACIDADE_PILHA - 1) {
                    printf("\n[AVISO] A pilha de reserva esta cheia (%d/%d)! Use uma peca primeiro.\n", 
                           CAPACIDADE_PILHA, CAPACIDADE_PILHA);
                } else {
                    Peca tiradaFila;
                    if (dequeue(&filaPecas, &tiradaFila)) {
                        push(&pilhaReserva, tiradaFila);
                        printf("\n[AÇAO] Peca [%c %d] movida da fila para a reserva!\n", 
                               tiradaFila.nome, tiradaFila.id);
                        // Regra: repoe a fila automaticamente
                        enqueue(&filaPecas, gerarPeca());
                    }
                }
                break;
            }

            case 3: { // USAR PEÇA RESERVADA
                Peca usada;
                if (pop(&pilhaReserva, &usada)) {
                    printf("\n[AÇAO] Peca [%c %d] da reserva foi utilizada!\n", usada.nome, usada.id);
                } else {
                    printf("\n[AVISO] A pilha de reserva esta vazia! Nenhuma peca para usar.\n");
                }
                break;
            }

            case 0:
                printf("\nEncerrando o sistema de pecas. Ate a proxima partida!\n");
                break;

            default:
                printf("\n[ERRO] Opcao invalida! Digite 1, 2, 3 ou 0.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}

// -----------------------------------------------------------------------------
// IMPLEMENTAÇÃO DAS ESTRUTURAS
// -----------------------------------------------------------------------------

void inicializarFila(FilaCircular *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->total = 0;
}

void inicializarPilha(PilhaReserva *pilha) {
    pilha->topo = -1; // Topo -1 indica pilha vazia
}

Peca gerarPeca(void) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorIdPeca++;
    return nova;
}

// Insercao na Fila Circular
int enqueue(FilaCircular *fila, Peca peca) {
    if (fila->total >= TAM_FILA) return 0;

    fila->itens[fila->fim] = peca;
    fila->fim = (fila->fim + 1) % TAM_FILA;
    fila->total++;
    return 1;
}

// Remocao da Fila Circular
int dequeue(FilaCircular *fila, Peca *pecaRemovida) {
    if (fila->total == 0) return 0;

    *pecaRemovida = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAM_FILA;
    fila->total--;
    return 1;
}

// Empilhar na Reserva (Push)
int push(PilhaReserva *pilha, Peca peca) {
    if (pilha->topo >= CAPACIDADE_PILHA - 1) return 0;

    pilha->topo++;
    pilha->itens[pilha->topo] = peca;
    return 1;
}

// Desempilhar da Reserva (Pop)
int pop(PilhaReserva *pilha, Peca *pecaRemovida) {
    if (pilha->topo == -1) return 0;

    *pecaRemovida = pilha->itens[pilha->topo];
    pilha->topo--;
    return 1;
}

// Visualizacao formatada do Estado Atual
void exibirEstado(const FilaCircular *fila, const PilhaReserva *pilha) {
    printf("\nEstado atual:\n");

    // Exibe a Fila de Peças
    printf("Fila de pecas    ");
    if (fila->total == 0) {
        printf("(vazia)");
    } else {
        for (int i = 0; i < fila->total; i++) {
            int idx = (fila->inicio + i) % TAM_FILA;
            printf("[%c %d] ", fila->itens[idx].nome, fila->itens[idx].id);
        }
    }
    printf("\n");

    // Exibe a Pilha de Reserva (do Topo para a Base)
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilha->topo == -1) {
        printf("(vazia)");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
        }
    }
    printf("\n");
}

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}