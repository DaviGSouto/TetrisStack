#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_FILA 5   
#define MAX_PILHA 3  

typedef struct {
    char nome; 
    int id;    
} Peca;

int proximo_id = 0; 

Peca fila[MAX_FILA];     
int fila_frente = 0;     
int fila_re = 0;         
int fila_contador = 0;   

Peca pilha[MAX_PILHA];   
int pilha_topo = -1;     

Peca gerarPeca();
void reabastecerFila();
void inicializarFila();
bool enqueue(Peca novaPeca);
bool dequeue(Peca *pecaRemovida);
bool push(Peca novaPeca);
bool pop(Peca *pecaRemovida);
void acao_jogar_peca();
void acao_reservar_peca();
void acao_usar_reserva();
void acao_troca_simples();
void acao_troca_multipla();
void exibirEstado();
void exibirMenu();



Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'}; 
    
    int indice = rand() % (sizeof(tipos) / sizeof(tipos[0]));
    novaPeca.nome = tipos[indice];
    novaPeca.id = proximo_id++;
    
    return novaPeca;
}

void reabastecerFila() {
    if (fila_contador < MAX_FILA) {
        Peca p_nova = gerarPeca();
        
        fila[fila_re] = p_nova;
        
        fila_re = (fila_re + 1) % MAX_FILA; 
        
        fila_contador++;
        
        printf(" **[Fila]** Peca [%c %d] adicionada para reabastecer.\n", p_nova.nome, p_nova.id);
    } else {
        printf(" [Fila] Fila ja esta cheia. Nao e necessario reabastecer.\n");
    }
}

void inicializarFila() {
    printf(" Inicializando a fila de pecas com %d elementos iniciais...\n", MAX_FILA);
    for (int i = 0; i < MAX_FILA; i++) {
        Peca p = gerarPeca();
        
        fila[fila_re] = p;
        fila_re = (fila_re + 1) % MAX_FILA; 
        fila_contador++;
        printf(" Peca inicial inserida: [%c %d]\n", p.nome, p.id);
    }
    printf(" Inicializacao da fila concluida.\n");
}

bool dequeue(Peca *pecaRemovida) {
    if (fila_contador == 0) {
        printf("\n Fila Vazia! Nao ha pecas para remover.\n");
        return false;
    }

    *pecaRemovida = fila[fila_frente];
    fila_frente = (fila_frente + 1) % MAX_FILA;
    fila_contador--;
    
    return true;
}

bool push(Peca novaPeca) {
    if (pilha_topo == MAX_PILHA - 1) {
        return false;
    }
    pilha_topo++;
    pilha[pilha_topo] = novaPeca;
    return true;
}

bool pop(Peca *pecaRemovida) {
    if (pilha_topo == -1) {
        return false;
    }
    *pecaRemovida = pilha[pilha_topo];
    pilha_topo--;
    return true;
}

void acao_jogar_peca() {
    printf("\n--- ACAO 1: Jogar Peca (Dequeue) ---\n");
    Peca jogada;
    
    if (dequeue(&jogada)) {
        printf(" **[Jogo]** Peca jogada da fila: **[%c %d]**.\n", jogada.nome, jogada.id);
        reabastecerFila();
    }
}

void acao_reservar_peca() {
    printf("\n--- ACAO 2: Reservar Peca (Fila -> Pilha) ---\n");
    Peca peca_fila;

    if (!dequeue(&peca_fila)) {
        return; 
    }

    if (push(peca_fila)) {
        printf(" **[Reserva]** Peca [%c %d] movida da fila para a reserva.\n", peca_fila.nome, peca_fila.id);
        reabastecerFila();
    } else {
        printf(" Peca [%c %d] foi removida da fila, mas nao pode ser reservada (Pilha Cheia) e foi descartada.\n", peca_fila.nome, peca_fila.id);
        reabastecerFila(); 
    }
}

void acao_usar_reserva() {
    printf("\n--- ACAO 3: Usar Peca Reservada (Pop) ---\n");
    Peca usada;
    
    if (pop(&usada)) {
        printf(" **[Jogo]** Peca usada da reserva: **[%c %d]**.\n", usada.nome, usada.id);
        reabastecerFila();
    }
}

void acao_troca_simples() {
    printf("\n--- ACAO 4: Trocar Peca Atual (Frente Fila <-> Topo Pilha) ---\n");
    
    if (fila_contador == 0) {
        printf(" Troca Impossivel: Fila vazia.\n");
        return;
    }
    if (pilha_topo == -1) {
        printf(" Troca Impossivel: Pilha vazia.\n");
        return;
    }

    Peca temp = fila[fila_frente];
    
    fila[fila_frente] = pilha[pilha_topo]; 
    pilha[pilha_topo] = temp;              
    
    printf(" **[Troca Simples]** Realizada entre: \n");
    printf("   - Fila (de %c %d) -> (para %c %d)\n", temp.nome, temp.id, fila[fila_frente].nome, fila[fila_frente].id);
    printf("   - Pilha (de %c %d) -> (para %c %d)\n", pilha[pilha_topo].nome, pilha[pilha_topo].id, temp.nome, temp.id);
    
}

void acao_troca_multipla() {
    printf("\n--- ACAO 5: Troca Multipla (3 Fila <-> 3 Pilha) ---\n");
    
    if (fila_contador < MAX_PILHA) {
        printf(" Troca Multipla Impossivel: Fila tem menos de %d pecas (%d).\n", MAX_PILHA, fila_contador);
        return;
    }
    if (pilha_topo < MAX_PILHA - 1) {
        printf(" Troca Multipla Impossivel: Pilha tem menos de %d pecas (%d).\n", MAX_PILHA, pilha_topo + 1);
        return;
    }

    Peca temp;
    int fila_idx;
    
    printf("🔄 **[Troca Multipla]** Trocando %d pecas...\n", MAX_PILHA);

    for (int i = 0; i < MAX_PILHA; i++) {
        fila_idx = (fila_frente + i) % MAX_FILA; 
        
        int pilha_idx = pilha_topo - i; 

        temp = fila[fila_idx];
        fila[fila_idx] = pilha[pilha_idx];
        pilha[pilha_idx] = temp;
    }
    
    printf(" Troca multipla concluida. Os 3 primeiros da fila e os 3 da pilha foram invertidos.\n");
    
}

void exibirEstado() {
    printf("\n==========================================================\n");
    printf("--- ESTADO ATUAL DO JOGO (Nivel Mestre) ---\n");

    printf("\n **FILA DE PECAS FUTURAS** (FIFO, Capacidade: %d, Elementos: %d)\n", MAX_FILA, fila_contador);
    if (fila_contador == 0) {
        printf("  Fila de pecas: [ ]\n");
    } else {
        printf("  Fila de pecas: [");
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

    printf("\n **PILHA DE RESERVA** (LIFO, Capacidade: %d, Elementos: %d)\n", MAX_PILHA, pilha_topo + 1);
    if (pilha_topo == -1) {
        printf(" Pilha de reserva: (Topo -> Base): [ ]\n");
    } else {
        printf(" Pilha de reserva: (Topo -> Base): [");
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

void exibirMenu() {
    printf("\n--- Opcoes de Acao (Nivel Mestre) ---\n");
    printf("| Codigo | Acao                       |\n");
    printf("|--------|----------------------------|\n");
    printf("| **1** | Jogar peca da frente da fila|\n");
    printf("| **2** | Reservar peca (Fila -> Pilha)|\n");
    printf("| **3** | Usar peca da pilha de reserva|\n");
    printf("| **4** | **Trocar peca atual** (Simples) |\n");
    printf("| **5** | **Troca Multipla** (3 Fila <-> 3 Pilha) |\n");
    printf("| **0** | Sair                       |\n");
    printf("------------------------------------\n");
}

int main() {
    srand(time(NULL));

    int escolha;
    
    inicializarFila();
    exibirEstado();

    do {
        exibirMenu();
        printf("\nDigite o codigo da acao: ");
        
        if (scanf("%d", &escolha) != 1) {
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
            case 4:
                acao_troca_simples();
                break;
            case 5:
                acao_troca_multipla();
                break;
            case 0:
                printf("\n Desafio concluido! Saindo do simulador.\n");
                break;
            default:
                printf("\n Opcao invalida. Digite 1, 2, 3, 4, 5 ou 0.\n");
                break;
        }

        if (escolha != 0) {
            exibirEstado();
        }

    } while (escolha != 0);

    return 0;
}