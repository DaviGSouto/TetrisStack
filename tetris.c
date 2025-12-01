#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// --- Definicao de Constantes ---
#define MAX_FILA 5 
#define MAX_PILHA 3

// --- 1. Estrutura da Peca (Struct) ---
typedef struct {
    char nome;
    int id;   
} Peca;

// --- 2. Variaveis Globais de Controle ---
int proximo_id = 0; // Contador global para gerar IDs unicos sequenciais

// --- 3. Variaveis e Estruturas da FILA CIRCULAR ---
Peca fila[MAX_FILA];
int fila_frente = 0;
int fila_re = 0;    
int fila_contador = 0;

// --- 4. Variaveis e Estruturas da PILHA LINEAR ---
Peca pilha[MAX_PILHA];
int pilha_topo = -1;  

// ----------------------------------------------------------------------
// --- 5. Funcoes Auxiliares Comuns ---
// ----------------------------------------------------------------------

/**
 * @brief Gera uma nova peca aleatoria com um ID unico.
 * @return Peca A nova peca gerada.
 */
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'}; 
    
    int indice = rand() % (sizeof(tipos) / sizeof(tipos[0]));
    novaPeca.nome = tipos[indice];
    
    novaPeca.id = proximo_id++;
    
    return novaPeca;
}

/**
 * @brief Adiciona automaticamente uma nova peca à fila para mantê-la cheia.
 */
void reabastecerFila();

// ----------------------------------------------------------------------
// --- 6. Funcoes da FILA (FIFO) ---
// ----------------------------------------------------------------------

/**
 * @brief Inicializa a fila, preenchendo-a com o maximo de elementos.
 */
void inicializarFila() {
    printf(" Inicializando a fila de pecas com %d elementos iniciais...\n", MAX_FILA);
    // Preenche a fila ate a capacidade maxima
    for (int i = 0; i < MAX_FILA; i++) {
        Peca p = gerarPeca();
        // NUNCA deve falhar na inicializacao
        if (fila_contador < MAX_FILA) {
            fila[fila_re] = p;
            fila_re = (fila_re + 1) % MAX_FILA; 
            fila_contador++;
            printf("   Peca inicial inserida: [%c %d]\n", p.nome, p.id);
        }
    }
    printf(" Inicializacao da fila concluida.\n");
}

/**
 * @brief Insere uma peca no final da fila (enqueue). Assume-se que a fila nunca estara cheia.
 * @param novaPeca A peca a ser inserida.
 */
void enqueue(Peca novaPeca) {
    // Como o requisito e manter a fila sempre cheia, so faremos enqueue quando houver espaco
    if (fila_contador == MAX_FILA) {
        // Isso so deve acontecer se a logica de reabastecimento falhar, mas e um bom "guardrail"
        printf("\n ERRO: Tentativa de Enqueue em fila ja cheia. [%c %d] descartada.\n", novaPeca.nome, novaPeca.id);
        return;
    }

    // Insere a peca na posicao 're'
    fila[fila_re] = novaPeca;
    
    // Atualiza 're' para o proximo local, usando o operador modulo para circularidade
    fila_re = (fila_re + 1) % MAX_FILA; 
    
    // Incrementa o contador de elementos
    fila_contador++;
    
    printf(" **[Fila]** Peca [%c %d] adicionada ao final.\n", novaPeca.nome, novaPeca.id);
}

/**
 * @brief Remove e retorna a peca da frente da fila (dequeue).
 * @param pecaRemovida Ponteiro para armazenar a peca removida.
 * @return bool Retorna true se a remocao foi bem-sucedida, false se a fila estiver vazia.
 */
bool dequeue(Peca *pecaRemovida) {
    if (fila_contador == 0) {
        printf("\n Fila Vazia! Nao ha pecas para remover.\n");
        return false;
    }

    // A peca a ser removida e a que esta na posicao 'frente' (FIFO)
    *pecaRemovida = fila[fila_frente];
    
    // Atualiza 'frente' para o proximo local (circularidade)
    fila_frente = (fila_frente + 1) % MAX_FILA;
    
    // Decrementa o contador de elementos
    fila_contador--;
    
    return true;
}

// ----------------------------------------------------------------------
// --- 7. Funcoes da PILHA (LIFO) ---
// ----------------------------------------------------------------------

/**
 * @brief Insere uma peca no topo da pilha (push).
 * @param novaPeca A peca a ser inserida.
 * @return bool Retorna true se a insercao foi bem-sucedida, false se a pilha estiver cheia.
 */
bool push(Peca novaPeca) {
    if (pilha_topo == MAX_PILHA - 1) {
        printf("\n Pilha Cheia! Nao e possivel reservar a peca [%c %d].\n", novaPeca.nome, novaPeca.id);
        return false;
    }

    // Incrementa o topo e insere a peca
    pilha_topo++;
    pilha[pilha_topo] = novaPeca;
    
    return true;
}

/**
 * @brief Remove e retorna a peca do topo da pilha (pop).
 * @param pecaRemovida Ponteiro para armazenar a peca removida.
 * @return bool Retorna true se a remocao foi bem-sucedida, false se a pilha estiver vazia.
 */
bool pop(Peca *pecaRemovida) {
    if (pilha_topo == -1) {
        printf("\n Pilha Vazia! Nao ha pecas reservadas para usar.\n");
        return false;
    }

    // A peca a ser removida e a que esta no topo (LIFO)
    *pecaRemovida = pilha[pilha_topo];
    
    // Decrementa o topo
    pilha_topo--;
    
    return true;
}

// ----------------------------------------------------------------------
// --- 8. Funcoes de Acao e Logica do Jogo ---
// ----------------------------------------------------------------------

/**
 * @brief Funcao principal para o reabastecimento automatico da fila.
 */
void reabastecerFila() {
    if (fila_contador < MAX_FILA) {
        Peca p_nova = gerarPeca();
        enqueue(p_nova);
    }
}

/**
 * @brief Acao 1: Remove a peca da frente da fila e joga.
 */
void acao_jogar_peca() {
    printf("\n--- ACAO: Jogar Peca (Dequeue) ---\n");
    Peca jogada;
    
    if (dequeue(&jogada)) {
        printf(" **[Jogo]** Peca jogada da fila: **[%c %d]**.\n", jogada.nome, jogada.id);
        // Regra do jogo: reabastecer a fila
        reabastecerFila();
    }
}

/**
 * @brief Acao 2: Move a peca da frente da fila para o topo da pilha (Reservar).
 */
void acao_reservar_peca() {
    printf("\n--- ACAO: Reservar Peca ---\n");
    Peca peca_fila;

    // 1. Tenta remover da frente da fila
    if (!dequeue(&peca_fila)) {
        // Mensagem de fila vazia ja foi dada no dequeue
        return;
    }

    // 2. Tenta inserir no topo da pilha
    if (push(peca_fila)) {
        printf(" **[Reserva]** Peca [%c %d] movida da fila para a reserva.\n", peca_fila.nome, peca_fila.id);
        // Regra do jogo: reabastecer a fila, pois um slot foi liberado
        reabastecerFila();
    } else {
        // Se a pilha estiver cheia, a peca que saiu da fila foi perdida (regra do desafio)
        printf(" Peca [%c %d] foi removida da fila, mas nao pode ser reservada (Pilha Cheia) e foi descartada.\n", peca_fila.nome, peca_fila.id);
        // A fila ja esta com um elemento a menos, entao reabastecemos
        reabastecerFila(); 
    }
}

/**
 * @brief Acao 3: Remove a peca do topo da pilha e joga (Usar Reserva).
 */
void acao_usar_reserva() {
    printf("\n--- ACAO: Usar Peca Reservada (Pop) ---\n");
    Peca usada;
    
    if (pop(&usada)) {
        printf(" **[Jogo]** Peca usada da reserva: **[%c %d]**.\n", usada.nome, usada.id);
        reabastecerFila();
    }
}


// ----------------------------------------------------------------------
// --- 9. Funcoes de Exibicao (Usabilidade) ---
// ----------------------------------------------------------------------

/**
 * @brief Exibe o estado atual da fila e da pilha.
 */
void exibirEstado() {
    printf("\n==========================================================\n");
    printf("--- ESTADO ATUAL DO JOGO ---\n");

    // Exibe a Fila
    printf("\n **FILA DE PECAS FUTURAS** (FIFO, Capacidade: %d, Elementos: %d)\n", MAX_FILA, fila_contador);
    if (fila_contador == 0) {
        printf("   Fila vazia. [ ]\n");
    } else {
        printf("   Fila (Frente -> Re): [");
        int i = fila_frente;
        for (int k = 0; k < fila_contador; k++) {
            printf("%c %d", fila[i].nome, fila[i].id);
            i = (i + 1) % MAX_FILA;
            if (k < fila_contador - 1) {
                printf("] [");
            }
        }
        printf("]\n");
    }
    

    // Exibe a Pilha
    printf("\n **PILHA DE RESERVA** (LIFO, Capacidade: %d, Topo: %d)\n", MAX_PILHA, pilha_topo);
    if (pilha_topo == -1) {
        printf("   Pilha vazia. (Topo -> Base): [ ]\n");
    } else {
        printf("   Pilha (Topo -> Base): [");
        for (int k = pilha_topo; k >= 0; k--) {
            printf("%c %d", pilha[k].nome, pilha[k].id);
            if (k > 0) {
                printf("] [");
            }
        }
        printf("]\n");
    }
    

    printf("\nCuradoria de TI: Visualizacao atual da fila de pecas e da pilha de reserva.\n");
    printf("==========================================================\n");
}

/**
 * @brief Exibe o menu de opcoes para o jogador.
 */
void exibirMenu() {
    printf("\n--- Opcoes de Acao ---\n");
    printf("| Codigo | Acao                       |\n");
    printf("|--------|----------------------------|\n");
    printf("| **1** | Jogar peca |\n");
    printf("| **2** | Reservar peca |\n");
    printf("| **3** | Usar peca reservada |\n");
    printf("| **0** | Sair |\n");
    printf("------------------------------------\n");
}


// ----------------------------------------------------------------------
// --- 10. Funcao Principal (main) ---
// ----------------------------------------------------------------------

int main() {
    // Inicializa o gerador de numeros aleatorios
    srand(time(NULL));

    int escolha;
    
    // Inicializa a Fila e a Pilha
    inicializarFila();
    // A pilha inicia vazia, nao precisa de inicializacao explicita alem de pilha_topo = -1.
    
    exibirEstado();

    do {
        exibirMenu();
        printf("\nDigite o codigo da acao: ");
        
        // Leitura da escolha do usuario
        if (scanf("%d", &escolha) != 1) {
            // Limpa o buffer em caso de entrada invalida (texto)
            while (getchar() != '\n'); 
            escolha = -1; 
        }

        switch (escolha) {
            case 1:
                acao_jogar_peca();
                break;
            case 2:
                acao_reservar_peca();
                break;
            case 3:
                acao_usar_reserva();
                break;
            case 0:
                printf("\n Saindo do Tetris Stack Simulator. Ate mais!\n");
                break;
            default:
                printf("\n Opcao invalida. Por favor, digite 1, 2, 3 ou 0.\n");
                break;
        }

        if (escolha != 0) {
            exibirEstado();
        }

    } while (escolha != 0);

    return 0;
}