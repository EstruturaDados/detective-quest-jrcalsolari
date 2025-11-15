#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// ESTRUTURAS DE DADOS
// ============================================================================

// Estrutura para representar uma sala na árvore binária de navegação
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
    char pista[100];  // Pista encontrada nesta sala (usado nos níveis Aventureiro e Mestre)
} Sala;

// Estrutura para representar uma pista na árvore de busca (BST)
typedef struct PistaBST {
    char texto[100];
    struct PistaBST* esquerda;
    struct PistaBST* direita;
} PistaBST;

// Estrutura para lista encadeada de pistas (usada na tabela hash)
typedef struct NoPista {
    char pista[100];
    struct NoPista* proximo;
} NoPista;

// Estrutura para representar um suspeito na tabela hash
typedef struct Suspeito {
    char nome[50];
    NoPista* pistas;  // Lista encadeada de pistas associadas
    int contador;     // Contador de pistas associadas
    struct Suspeito* proximo;  // Para tratamento de colisões
} Suspeito;

// ============================================================================
// NÍVEL NOVATO: ÁRVORE BINÁRIA DE NAVEGAÇÃO
// ============================================================================

// Função para criar uma nova sala
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

// Função para construir a árvore da mansão
Sala* construirMansao() {
    // Criar todas as salas
    Sala* hall = criarSala("Hall de Entrada", "");
    Sala* biblioteca = criarSala("Biblioteca", "Livro com páginas arrancadas");
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida");
    Sala* sala = criarSala("Sala de Estar", "Copo quebrado");
    Sala* quarto = criarSala("Quarto Principal", "Carta rasgada");
    Sala* banheiro = criarSala("Banheiro", "Espelho quebrado");
    Sala* sotao = criarSala("Sótão", "Baú trancado");
    Sala* porao = criarSala("Porão", "Corda manchada");

    // Conectar as salas formando a árvore binária
    // Estrutura hierárquica: Hall -> Biblioteca/Cozinha/Sótão e Sala/Quarto/Banheiro/Porão

    hall->esquerda = biblioteca;
    hall->direita = sala;

    biblioteca->esquerda = cozinha;
    biblioteca->direita = sotao;

    sala->esquerda = quarto;
    sala->direita = banheiro;

    banheiro->esquerda = porao;
    banheiro->direita = NULL;

    return hall;
}

// Função para explorar as salas (Nível Novato)
void explorarSalas(Sala* salaAtual) {
    if (salaAtual == NULL) {
        printf("Você chegou ao fim do caminho!\n");
        return;
    }

    printf("\n=== Você está em: %s ===\n", salaAtual->nome);

    // Verificar se há caminhos disponíveis
    int temEsquerda = (salaAtual->esquerda != NULL);
    int temDireita = (salaAtual->direita != NULL);

    if (!temEsquerda && !temDireita) {
        printf("Este é um cômodo sem saída. Você chegou ao fim do caminho!\n");
        return;
    }

    printf("Para onde deseja ir?\n");
    if (temEsquerda) {
        printf("  [e] Esquerda -> %s\n", salaAtual->esquerda->nome);
    }
    if (temDireita) {
        printf("  [d] Direita -> %s\n", salaAtual->direita->nome);
    }
    printf("  [s] Sair da exploração\n");
    printf("Escolha: ");

    char escolha;
    scanf(" %c", &escolha);

    switch (escolha) {
        case 'e':
        case 'E':
            if (temEsquerda) {
                explorarSalas(salaAtual->esquerda);
            } else {
                printf("Não há caminho à esquerda!\n");
                explorarSalas(salaAtual);
            }
            break;
        case 'd':
        case 'D':
            if (temDireita) {
                explorarSalas(salaAtual->direita);
            } else {
                printf("Não há caminho à direita!\n");
                explorarSalas(salaAtual);
            }
            break;
        case 's':
        case 'S':
            printf("Você saiu da exploração.\n");
            return;
        default:
            printf("Opção inválida! Tente novamente.\n");
            explorarSalas(salaAtual);
            break;
    }
}

// Função para liberar memória da árvore de salas
void liberarSalas(Sala* sala) {
    if (sala != NULL) {
        liberarSalas(sala->esquerda);
        liberarSalas(sala->direita);
        free(sala);
    }
}

// ============================================================================
// NÍVEL AVENTUREIRO: ÁRVORE DE BUSCA (BST) PARA PISTAS
// ============================================================================

// Função para criar um novo nó de pista na BST
PistaBST* criarNoPista(const char* texto) {
    PistaBST* novo = (PistaBST*)malloc(sizeof(PistaBST));
    if (novo == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novo->texto, texto);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// Função para inserir uma pista na BST (ordem alfabética)
PistaBST* inserirPistaBST(PistaBST* raiz, const char* texto) {
    if (raiz == NULL) {
        return criarNoPista(texto);
    }

    int comparacao = strcmp(texto, raiz->texto);
    if (comparacao < 0) {
        raiz->esquerda = inserirPistaBST(raiz->esquerda, texto);
    } else if (comparacao > 0) {
        raiz->direita = inserirPistaBST(raiz->direita, texto);
    }
    // Se já existe, não insere duplicado

    return raiz;
}

// Função para exibir pistas em ordem alfabética (em ordem)
void listarPistasEmOrdem(PistaBST* raiz) {
    if (raiz != NULL) {
        listarPistasEmOrdem(raiz->esquerda);
        printf("  - %s\n", raiz->texto);
        listarPistasEmOrdem(raiz->direita);
    }
}

// Função para buscar uma pista na BST
int buscarPistaBST(PistaBST* raiz, const char* texto) {
    if (raiz == NULL) {
        return 0;  // Não encontrado
    }

    int comparacao = strcmp(texto, raiz->texto);
    if (comparacao == 0) {
        return 1;  // Encontrado
    } else if (comparacao < 0) {
        return buscarPistaBST(raiz->esquerda, texto);
    } else {
        return buscarPistaBST(raiz->direita, texto);
    }
}

// Função para liberar memória da BST de pistas
void liberarBST(PistaBST* raiz) {
    if (raiz != NULL) {
        liberarBST(raiz->esquerda);
        liberarBST(raiz->direita);
        free(raiz);
    }
}

// Função para explorar salas e coletar pistas (Nível Aventureiro)
void explorarSalasComPistas(Sala* salaAtual, PistaBST** raizPistas) {
    if (salaAtual == NULL) {
        printf("Você chegou ao fim do caminho!\n");
        return;
    }

    printf("\n=== Você está em: %s ===\n", salaAtual->nome);

    // Se a sala tem uma pista e ela ainda não foi coletada, adicionar à BST
    if (strlen(salaAtual->pista) > 0) {
        if (!buscarPistaBST(*raizPistas, salaAtual->pista)) {
            *raizPistas = inserirPistaBST(*raizPistas, salaAtual->pista);
            printf("🔍 PISTA ENCONTRADA: %s\n", salaAtual->pista);
        }
    }

    // Verificar se há caminhos disponíveis
    int temEsquerda = (salaAtual->esquerda != NULL);
    int temDireita = (salaAtual->direita != NULL);

    if (!temEsquerda && !temDireita) {
        printf("Este é um cômodo sem saída. Você chegou ao fim do caminho!\n");
        return;
    }

    printf("Para onde deseja ir?\n");
    if (temEsquerda) {
        printf("  [e] Esquerda -> %s\n", salaAtual->esquerda->nome);
    }
    if (temDireita) {
        printf("  [d] Direita -> %s\n", salaAtual->direita->nome);
    }
    printf("  [s] Sair da exploração\n");
    printf("  [p] Ver pistas coletadas\n");
    printf("Escolha: ");

    char escolha;
    scanf(" %c", &escolha);

    switch (escolha) {
        case 'e':
        case 'E':
            if (temEsquerda) {
                explorarSalasComPistas(salaAtual->esquerda, raizPistas);
            } else {
                printf("Não há caminho à esquerda!\n");
                explorarSalasComPistas(salaAtual, raizPistas);
            }
            break;
        case 'd':
        case 'D':
            if (temDireita) {
                explorarSalasComPistas(salaAtual->direita, raizPistas);
            } else {
                printf("Não há caminho à direita!\n");
                explorarSalasComPistas(salaAtual, raizPistas);
            }
            break;
        case 'p':
        case 'P':
            printf("\n=== PISTAS COLETADAS (em ordem alfabética) ===\n");
            if (*raizPistas == NULL) {
                printf("Nenhuma pista coletada ainda.\n");
            } else {
                listarPistasEmOrdem(*raizPistas);
            }
            explorarSalasComPistas(salaAtual, raizPistas);
            break;
        case 's':
        case 'S':
            printf("Você saiu da exploração.\n");
            return;
        default:
            printf("Opção inválida! Tente novamente.\n");
            explorarSalasComPistas(salaAtual, raizPistas);
            break;
    }
}

// ============================================================================
// NÍVEL MESTRE: TABELA HASH PARA SUSPEITOS
// ============================================================================

#define TAMANHO_HASH 7  // Tamanho da tabela hash

// Tabela hash global
Suspeito* tabelaHash[TAMANHO_HASH];

// Função de hash simples baseada na primeira letra do nome
int funcaoHash(const char* nome) {
    if (nome == NULL || strlen(nome) == 0) {
        return 0;
    }
    return (nome[0] - 'A') % TAMANHO_HASH;
}

// Função para inicializar a tabela hash
void inicializarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

// Função para criar um novo suspeito
Suspeito* criarSuspeito(const char* nome) {
    Suspeito* novo = (Suspeito*)malloc(sizeof(Suspeito));
    if (novo == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novo->nome, nome);
    novo->pistas = NULL;
    novo->contador = 0;
    novo->proximo = NULL;
    return novo;
}

// Função para criar um nó de pista na lista encadeada
NoPista* criarNoPistaLista(const char* pista) {
    NoPista* novo = (NoPista*)malloc(sizeof(NoPista));
    if (novo == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(novo->pista, pista);
    novo->proximo = NULL;
    return novo;
}

// Função para inserir uma pista na lista encadeada de um suspeito
void adicionarPistaASuspeito(Suspeito* suspeito, const char* pista) {
    // Verificar se a pista já existe
    NoPista* atual = suspeito->pistas;
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return;  // Pista já existe
        }
        atual = atual->proximo;
    }

    // Adicionar nova pista
    NoPista* novaPista = criarNoPistaLista(pista);
    novaPista->proximo = suspeito->pistas;
    suspeito->pistas = novaPista;
    suspeito->contador++;
}

// Função para buscar ou criar um suspeito na tabela hash
Suspeito* buscarOuCriarSuspeito(const char* nome) {
    int indice = funcaoHash(nome);

    // Buscar na lista encadeada (tratamento de colisões)
    Suspeito* atual = tabelaHash[indice];
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }

    // Se não encontrou, criar novo suspeito
    Suspeito* novo = criarSuspeito(nome);
    novo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novo;
    return novo;
}

// Função para inserir relação pista-suspeito na tabela hash
void inserirNaHash(const char* pista, const char* suspeito) {
    Suspeito* s = buscarOuCriarSuspeito(suspeito);
    adicionarPistaASuspeito(s, pista);
}

// Função para listar todas as associações pista-suspeito
void listarAssociacoes() {
    printf("\n=== ASSOCIAÇÕES PISTA → SUSPEITO ===\n");
    int temAssociacoes = 0;

    for (int i = 0; i < TAMANHO_HASH; i++) {
        Suspeito* atual = tabelaHash[i];
        while (atual != NULL) {
            if (atual->contador > 0) {
                temAssociacoes = 1;
                printf("\n🔍 Suspeito: %s (%d pista(s))\n", atual->nome, atual->contador);
                NoPista* pistaAtual = atual->pistas;
                while (pistaAtual != NULL) {
                    printf("   - %s\n", pistaAtual->pista);
                    pistaAtual = pistaAtual->proximo;
                }
            }
            atual = atual->proximo;
        }
    }

    if (!temAssociacoes) {
        printf("Nenhuma associação registrada ainda.\n");
    }
}

// Função para encontrar o suspeito mais citado
void encontrarSuspeitoMaisProvavel() {
    Suspeito* maisProvavel = NULL;
    int maxContador = 0;

    for (int i = 0; i < TAMANHO_HASH; i++) {
        Suspeito* atual = tabelaHash[i];
        while (atual != NULL) {
            if (atual->contador > maxContador) {
                maxContador = atual->contador;
                maisProvavel = atual;
            }
            atual = atual->proximo;
        }
    }

    if (maisProvavel != NULL && maxContador > 0) {
        printf("\n=== 🎯 SUSPEITO MAIS PROVÁVEL ===\n");
        printf("Nome: %s\n", maisProvavel->nome);
        printf("Pistas associadas: %d\n", maisProvavel->contador);
        printf("Pistas:\n");
        NoPista* pistaAtual = maisProvavel->pistas;
        while (pistaAtual != NULL) {
            printf("  - %s\n", pistaAtual->pista);
            pistaAtual = pistaAtual->proximo;
        }
    } else {
        printf("\nNenhum suspeito com pistas associadas encontrado.\n");
    }
}

// Função para liberar memória da tabela hash
void liberarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        Suspeito* atual = tabelaHash[i];
        while (atual != NULL) {
            Suspeito* proximo = atual->proximo;
            // Liberar lista de pistas
            NoPista* pistaAtual = atual->pistas;
            while (pistaAtual != NULL) {
                NoPista* proxPista = pistaAtual->proximo;
                free(pistaAtual);
                pistaAtual = proxPista;
            }
            free(atual);
            atual = proximo;
        }
    }
}

// Função para explorar salas e relacionar pistas a suspeitos (Nível Mestre)
void explorarSalasComSuspeitos(Sala* salaAtual, PistaBST** raizPistas) {
    if (salaAtual == NULL) {
        printf("Você chegou ao fim do caminho!\n");
        return;
    }

    printf("\n=== Você está em: %s ===\n", salaAtual->nome);

    // Se a sala tem uma pista e ela ainda não foi coletada
    if (strlen(salaAtual->pista) > 0) {
        if (!buscarPistaBST(*raizPistas, salaAtual->pista)) {
            *raizPistas = inserirPistaBST(*raizPistas, salaAtual->pista);
            printf("🔍 PISTA ENCONTRADA: %s\n", salaAtual->pista);

            // Relacionar pista a suspeitos (lógica do jogo)
            // Cada pista está associada a um suspeito específico
            if (strcmp(salaAtual->pista, "Livro com páginas arrancadas") == 0) {
                inserirNaHash(salaAtual->pista, "Professor");
            } else if (strcmp(salaAtual->pista, "Faca desaparecida") == 0) {
                inserirNaHash(salaAtual->pista, "Chef");
            } else if (strcmp(salaAtual->pista, "Copo quebrado") == 0) {
                inserirNaHash(salaAtual->pista, "Mordomo");
            } else if (strcmp(salaAtual->pista, "Carta rasgada") == 0) {
                inserirNaHash(salaAtual->pista, "Herdeiro");
            } else if (strcmp(salaAtual->pista, "Espelho quebrado") == 0) {
                inserirNaHash(salaAtual->pista, "Herdeiro");
            } else if (strcmp(salaAtual->pista, "Baú trancado") == 0) {
                inserirNaHash(salaAtual->pista, "Herdeiro");
            } else if (strcmp(salaAtual->pista, "Corda manchada") == 0) {
                inserirNaHash(salaAtual->pista, "Mordomo");
            }
        }
    }

    // Verificar se há caminhos disponíveis
    int temEsquerda = (salaAtual->esquerda != NULL);
    int temDireita = (salaAtual->direita != NULL);

    if (!temEsquerda && !temDireita) {
        printf("Este é um cômodo sem saída. Você chegou ao fim do caminho!\n");
        return;
    }

    printf("Para onde deseja ir?\n");
    if (temEsquerda) {
        printf("  [e] Esquerda -> %s\n", salaAtual->esquerda->nome);
    }
    if (temDireita) {
        printf("  [d] Direita -> %s\n", salaAtual->direita->nome);
    }
    printf("  [s] Sair da exploração\n");
    printf("  [p] Ver pistas coletadas\n");
    printf("  [a] Ver associações pista-suspeito\n");
    printf("  [c] Ver suspeito mais provável\n");
    printf("Escolha: ");

    char escolha;
    scanf(" %c", &escolha);

    switch (escolha) {
        case 'e':
        case 'E':
            if (temEsquerda) {
                explorarSalasComSuspeitos(salaAtual->esquerda, raizPistas);
            } else {
                printf("Não há caminho à esquerda!\n");
                explorarSalasComSuspeitos(salaAtual, raizPistas);
            }
            break;
        case 'd':
        case 'D':
            if (temDireita) {
                explorarSalasComSuspeitos(salaAtual->direita, raizPistas);
            } else {
                printf("Não há caminho à direita!\n");
                explorarSalasComSuspeitos(salaAtual, raizPistas);
            }
            break;
        case 'p':
        case 'P':
            printf("\n=== PISTAS COLETADAS (em ordem alfabética) ===\n");
            if (*raizPistas == NULL) {
                printf("Nenhuma pista coletada ainda.\n");
            } else {
                listarPistasEmOrdem(*raizPistas);
            }
            explorarSalasComSuspeitos(salaAtual, raizPistas);
            break;
        case 'a':
        case 'A':
            listarAssociacoes();
            explorarSalasComSuspeitos(salaAtual, raizPistas);
            break;
        case 'c':
        case 'C':
            encontrarSuspeitoMaisProvavel();
            explorarSalasComSuspeitos(salaAtual, raizPistas);
            break;
        case 's':
        case 'S':
            printf("Você saiu da exploração.\n");
            return;
        default:
            printf("Opção inválida! Tente novamente.\n");
            explorarSalasComSuspeitos(salaAtual, raizPistas);
            break;
    }
}

// ============================================================================
// MENU PRINCIPAL
// ============================================================================

void menuPrincipal() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║          🕵️  DETECTIVE QUEST - ENIGMA STUDIOS 🕵️          ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("Escolha o nível de dificuldade:\n");
    printf("  [1] 🌱 Nível Novato - Mapa da Mansão (Árvore Binária)\n");
    printf("  [2] 🔍 Nível Aventureiro - Pistas (Árvore de Busca)\n");
    printf("  [3] 🧠 Nível Mestre - Suspeitos (Tabela Hash)\n");
    printf("  [0] Sair\n");
    printf("\nEscolha: ");
}

int main() {
    int opcao;

    do {
        menuPrincipal();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                // Nível Novato: Apenas navegação
                printf("\n=== 🌱 NÍVEL NOVATO: MAPA DA MANSÃO ===\n");
                printf("Explore a mansão usando 'e' (esquerda), 'd' (direita) ou 's' (sair)\n\n");
                Sala* raizMansao = construirMansao();
                explorarSalas(raizMansao);
                liberarSalas(raizMansao);
                break;
            }

            case 2: {
                // Nível Aventureiro: Navegação + coleta de pistas
                printf("\n=== 🔍 NÍVEL AVENTUREIRO: COLETA DE PISTAS ===\n");
                printf("Explore a mansão e colete pistas!\n");
                printf("Use 'p' para ver suas pistas coletadas.\n\n");
                Sala* raizMansao = construirMansao();
                PistaBST* raizPistas = NULL;
                explorarSalasComPistas(raizMansao, &raizPistas);
                
                printf("\n=== RESUMO FINAL ===\n");
                printf("Pistas coletadas:\n");
                if (raizPistas == NULL) {
                    printf("Nenhuma pista coletada.\n");
                } else {
                    listarPistasEmOrdem(raizPistas);
                }
                
                liberarSalas(raizMansao);
                liberarBST(raizPistas);
                break;
            }

            case 3: {
                // Nível Mestre: Navegação + pistas + suspeitos
                printf("\n=== 🧠 NÍVEL MESTRE: INVESTIGAÇÃO COMPLETA ===\n");
                printf("Explore a mansão, colete pistas e descubra o culpado!\n");
                printf("Use 'p' para ver pistas, 'a' para associações e 'c' para o suspeito mais provável.\n\n");
                
                inicializarHash();
                Sala* raizMansao = construirMansao();
                PistaBST* raizPistas = NULL;
                explorarSalasComSuspeitos(raizMansao, &raizPistas);
                
                printf("\n=== ANÁLISE FINAL ===\n");
                printf("\nPistas coletadas:\n");
                if (raizPistas == NULL) {
                    printf("Nenhuma pista coletada.\n");
                } else {
                    listarPistasEmOrdem(raizPistas);
                }
                
                listarAssociacoes();
                encontrarSuspeitoMaisProvavel();
                
                liberarSalas(raizMansao);
                liberarBST(raizPistas);
                liberarHash();
                break;
            }

            case 0:
                printf("\nObrigado por jogar Detective Quest!\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }

        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
            getchar();
        }

    } while (opcao != 0);

    return 0;
}
