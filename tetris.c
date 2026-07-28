#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define CAPACIDADE_PILHA 3

// -----------------------------------------------------------------------------
// 1. ESTRUTURAS DE DADOS
// -----------------------------------------------------------------------------

// Struct para representar cada peca do Tetris
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

// Struct para a Pilha Linear (Reserva)
typedef struct {
    Peca itens[CAPACIDADE_PILHA];
    int topo; // Indice do topo (-1 quando vazia)
} PilhaReserva;

// Variavel global para geracao de IDs unicos e sequenciais
int contadorIdPeca = 0;

// -----------------------------------------------------------------------------
// PROTÓTIPOS DAS FUNÇÕES
// -----------------------------------------------------------------------------
void inicializarFila(FilaCircular *fila);
void inicializarPilha(PilhaReserva *pilha);
Peca gerarPeca(void);

// Operacoes Basicas
int enqueue(FilaCircular *fila, Peca peca);
int dequeue(FilaCircular *fila, Peca *pecaRemovida);
int push(PilhaReserva *pilha, Peca peca);
int pop(PilhaReserva *pilha, Peca *pecaRemovida);

// Operacoes Avancadas de Troca
void trocarPecaAtual(FilaCircular *fila, PilhaReserva *pilha);
void trocarMultipla(FilaCircular *fila, PilhaReserva *pilha);

// Interface e Utilitarios
void exibirEstado(const FilaCircular *fila, const PilhaReserva *pilha);
void limparBuffer(void);

// -----------------------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// -----------------------------------------------------------------------------
int main() {
    // Semente para geracao aleatoria dos tipos de peca
    srand((unsigned int)time(NULL));

    FilaCircular filaPecas;
    PilhaReserva pilhaReserva;

    inicializarFila(&filaPecas);
    inicializarPilha(&pilhaReserva);

    // Preenche a fila inicial com 5 pecas
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&filaPecas, gerarPeca());
    }

    int opcao;

    do {
        printf("\n==================================================\n");
        printf("          TETRIS STACK - NÍVEL AVANÇADO           \n");
        printf("==================================================\n");

        // Exibe a fila e a pilha
        exibirEstado(&filaPecas, &pilhaReserva);

        printf("\nOpcoes disponiveis:\n");
        printf(" 1. Jogar peca da frente da fila\n");
        printf(" 2. Enviar peca da fila para a pilha de reserva\n");
        printf(" 3. Usar peca da pilha de reserva\n");
        printf(" 4. Trocar peca da frente da fila com o topo da pilha\n");
        printf(" 5. Trocar os 3 primeiros da fila com as 3 pecas da pilha\n");
        printf(" 0. Sair\n");
        printf("--------------------------------------------------\n");
        printf("Opcao escolhida: ");

        if (scanf("%d", &opcao) != 1) {
            opcao = -1;
        }
        limparBuffer();

        switch (opcao) {
            case 1: { // JOGAR PEÇA
                Peca jogada;
                if (dequeue(&filaPecas, &jogada)) {
                    printf("\nAcao: Peca [%c %d] foi jogada!\n", jogada.nome, jogada.id);
                    // Reposicao automatica da fila
                    enqueue(&filaPecas, gerarPeca());
                }
                break;
            }

            case 2: { // RESERVAR PEÇA
                if (pilhaReserva.topo >= CAPACIDADE_PILHA - 1) {
                    printf("\n[AVISO] A pilha de reserva esta cheia (%d/%d)!\n", 
                           CAPACIDADE_PILHA, CAPACIDADE_PILHA);
                } else {
                    Peca tiradaFila;
                    if (dequeue(&filaPecas, &tiradaFila)) {
                        push(&pilhaReserva, tiradaFila);
                        printf("\nAcao: Peca [%c %d] enviada para a pilha de reserva!\n", 
                               tiradaFila.nome, tiradaFila.id);
                        // Reposicao automatica da fila
                        enqueue(&filaPecas, gerarPeca());
                    }
                }
                break;
            }

            case 3: { // USAR PEÇA RESERVADA
                Peca usada;
                if (pop(&pilhaReserva, &usada)) {
                    printf("\nAcao: Peca [%c %d] da pilha de reserva foi utilizada!\n", 
                           usada.nome, usada.id);
                } else {
                    printf("\n[AVISO] A pilha de reserva esta vazia!\n");
                }
                break;
            }

            case 4: // TROCA SIMPLES
                trocarPecaAtual(&filaPecas, &pilhaReserva);
                break;

            case 5: // TROCA MÚLTIPLA (3 PEÇAS)
                trocarMultipla(&filaPecas, &pilhaReserva);
                break;

            case 0:
                printf("\nEncerrando o gerenciador de pecas. Ate a proxima!\n");
                break;

            default:
                printf("\n[ERRO] Opcao invalida! Escolha um valor de 0 a 5.\n");
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
    pilha->topo = -1;
}

Peca gerarPeca(void) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = contadorIdPeca++;
    return nova;
}

int enqueue(FilaCircular *fila, Peca peca) {
    if (fila->total >= TAM_FILA) return 0;

    fila->itens[fila->fim] = peca;
    fila->fim = (fila->fim + 1) % TAM_FILA;
    fila->total++;
    return 1;
}

int dequeue(FilaCircular *fila, Peca *pecaRemovida) {
    if (fila->total == 0) return 0;

    *pecaRemovida = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % TAM_FILA;
    fila->total--;
    return 1;
}

int push(PilhaReserva *pilha, Peca peca) {
    if (pilha->topo >= CAPACIDADE_PILHA - 1) return 0;

    pilha->topo++;
    pilha->itens[pilha->topo] = peca;
    return 1;
}

int pop(PilhaReserva *pilha, Peca *pecaRemovida) {
    if (pilha->topo == -1) return 0;

    *pecaRemovida = pilha->itens[pilha->topo];
    pilha->topo--;
    return 1;
}

// -----------------------------------------------------------------------------
// OPERAÇÕES DE TROCA
// -----------------------------------------------------------------------------

// Opção 4: Troca a peça da frente da fila com o topo da pilha
void trocarPecaAtual(FilaCircular *fila, PilhaReserva *pilha) {
    if (fila->total == 0) {
        printf("\n[AVISO] A fila esta vazia. Nao e possivel realizar a troca.\n");
        return;
    }
    if (pilha->topo == -1) {
        printf("\n[AVISO] A pilha esta vazia. Nao ha peca para trocar.\n");
        return;
    }

    // Troca direta de valores
    Peca temp = fila->itens[fila->inicio];
    fila->itens[fila->inicio] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;

    printf("\nAcao: troca realizada entre a frente da fila e o topo da pilha.\n");
}

// Opção 5: Troca as 3 primeiras peças da fila com as 3 peças da pilha
void trocarMultipla(FilaCircular *fila, PilhaReserva *pilha) {
    // Validação de segurança: Requer pelo menos 3 itens em ambas as estruturas
    if (fila->total < 3) {
        printf("\n[AVISO] A fila precisa ter pelo menos 3 pecas para a troca multipla.\n");
        return;
    }
    if (pilha->topo < 2) { // topo < 2 significa menos de 3 elementos na pilha (indices 0, 1, 2)
        printf("\n[AVISO] A pilha precisa ter exatamente 3 pecas para a troca multipla.\n");
        return;
    }

    // A pilha visualmente vai do topo para a base (indices 2, 1, 0)
    // A fila vai da frente para tras (inicio, inicio+1, inicio+2 com aritmetica circular)
    for (int i = 0; i < 3; i++) {
        int idxFila = (fila->inicio + i) % TAM_FILA;
        int idxPilha = pilha->topo - i; // Troca o topo (i=0) com o 1º da fila, etc.

        Peca temp = fila->itens[idxFila];
        fila->itens[idxFila] = pilha->itens[idxPilha];
        pilha->itens[idxPilha] = temp;
    }

    printf("\nAcao: troca realizada entre os 3 primeiros da fila e os 3 da pilha.\n");
}

// -----------------------------------------------------------------------------
// INTERFACE E EXIBIÇÃO
// -----------------------------------------------------------------------------

void exibirEstado(const FilaCircular *fila, const PilhaReserva *pilha) {
    printf("\nEstado atual:\n");

    // Exibe a Fila
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

    // Exibe a Pilha (Topo -> Base)
    printf("Pilha de reserva (Topo -> base): ");
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