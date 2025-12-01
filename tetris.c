#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// --- Definicao de Constantes ---
#define MAX_SIZE 5 // Capacidade maxima da fila de pecas, conforme sugerido.

typedef struct {
    char nome;
    int id;
} Peca;

// --- 2. Variaveis Globais da Fila ---
Peca fila[MAX_SIZE]; 
int frente = 0;      
int re = 0;         
int contador = 0;    
int proximo_id = 0;  

// --- 3. Prototipos das Funcoes ---

void inicializarFila();
Peca gerarPeca();
bool enqueue(Peca novaPeca);
bool dequeue(Peca *pecaRemovida);
void exibirFila();
void exibirMenu();

// --- 4. Implementacao das Funcoes ---

/**
 * @brief Gera uma nova peca aleatoria com um ID unico.
 * @return Peca A nova peca gerada.
 */
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'}; 
    
    // Escolhe aleatoriamente um tipo de peca
    int indice = rand() % (sizeof(tipos) / sizeof(tipos[0]));
    novaPeca.nome = tipos[indice];
    
    // Atribui o ID unico e incrementa o contador global de IDs
    novaPeca.id = proximo_id++;
    
    return novaPeca;
}

/**
 * @brief Inicializa a fila, preenchendo-a com um numero fixo de elementos.
 */
void inicializarFila() {
    printf(" Inicializando a fila de pecas com %d elementos iniciais...\n", MAX_SIZE);
    
    for (int i = 0; i < MAX_SIZE; i++) {
        Peca p = gerarPeca();
        if (enqueue(p)) {
            printf("   Peca inicial inserida: [%c %d]\n", p.nome, p.id);
        }
    }
    printf(" Inicializacao concluida.\n");
}

/**
 * @brief Insere uma peca no final da fila (enqueue).
 * @param novaPeca A peca a ser inserida.
 * @return bool Retorna true se a insercao foi bem-sucedida, false se a fila estiver cheia.
 */
bool enqueue(Peca novaPeca) {
    // Valida se a fila esta cheia
    if (contador == MAX_SIZE) {
        printf("\n Fila Cheia! Nao e possivel inserir a peca [%c %d].\n", novaPeca.nome, novaPeca.id);
        return false;
    }

    // Insere a peca na posicao 're'
    fila[re] = novaPeca;
    
    // Atualiza 're' usando o operador modulo para garantir a circularidade
    re = (re + 1) % MAX_SIZE; 
    
    // Incrementa o contador
    contador++;
    
    printf("\n**Sucesso!** Peca [%c %d] inserida (enqueue).\n", novaPeca.nome, novaPeca.id);
    return true;
}

/**
 * @brief Remove e retorna a peca da frente da fila (dequeue).
 * @param pecaRemovida Ponteiro para armazenar a peca removida.
 * @return bool Retorna true se a remocao foi bem-sucedida, false se a fila estiver vazia.
 */
bool dequeue(Peca *pecaRemovida) {
    if (contador == 0) {
        printf("\n Fila Vazia! Nao ha pecas para jogar/remover (dequeue).\n");
        return false;
    }

    *pecaRemovida = fila[frente];
    
    frente = (frente + 1) % MAX_SIZE;
    
    contador--;
    
    printf("\n➖ **Sucesso!** Peca jogada/removida (dequeue): **[%c %d]**.\n", pecaRemovida->nome, pecaRemovida->id);
    return true;
}

/**
 * @brief Exibe o estado atual da fila, seguindo o exemplo de saida.
 */
void exibirFila() {
    printf("\n--- Estado Atual da Fila de Pecas (Capacidade: %d) ---\n", MAX_SIZE);
    
    if (contador == 0) {
        printf("Fila vazia.\n");
        printf("--- Visao da Fila: [] ---\n");
        return;
    }

    printf("Elementos na Fila: %d\n", contador);
    
    printf("\nCuradoria de TI: Fila de pecas: [");
    
    int i = frente;
    for (int k = 0; k < contador; k++) {
        printf("%c %d", fila[i].nome, fila[i].id);
        i = (i + 1) % MAX_SIZE; // Proximo indice circular
        if (k < contador - 1) {
            printf("] [");
        }
    }
    printf("]\n");
    printf("----------------------------------------------------------------\n");
}

/**
 * @brief Exibe o menu de opcoes para o jogador.
 */
void exibirMenu() {
    printf("\n---  Opcoes de Acao  ---\n");
    printf("| Codigo | Acao                       |\n");
    printf("|--------|----------------------------|\n");
    printf("| **1** | Jogar peca (**dequeue**)   |\n");
    printf("| **2** | Inserir nova peca (**enqueue**) |\n");
    printf("| **0** | Sair                       |\n");
    printf("------------------------------------\n");
}

// --- 5. Funcao Principal (main) ---
int main() {
    srand(time(NULL));

    int escolha;
    
    inicializarFila();
    exibirFila();

    do {
        exibirMenu();
        printf("\nDigite o codigo da acao: ");
        
        if (scanf("%d", &escolha) != 1) {
            while (getchar() != '\n'); 
            escolha = -1; // Garante que caia no 'default'
        }

        switch (escolha) {
            case 1: {
                // Opcao 1: Jogar peca (dequeue) - Remocao da frente
                Peca jogada;
                dequeue(&jogada);
                exibirFila();
                break;
            }
            case 2: {
                // Opcao 2: Inserir nova peca (enqueue) - Insercao no final
                Peca nova = gerarPeca(); // Gera a peca automaticamente
                printf("\n Tentando inserir a peca gerada automaticamente: [%c %d]...", nova.nome, nova.id);
                enqueue(nova);
                exibirFila();
                break;
            }
            case 0:
                // Opcao 0: Sair
                printf("\n Saindo do Tetris Stack Simulator. Finalizando o programa.\n");
                break;
            default:
                printf("\n Opcao invalida. Por favor, digite 1, 2 ou 0.\n");
                exibirFila();
                break;
        }

    } while (escolha != 0);

    return 0;
}