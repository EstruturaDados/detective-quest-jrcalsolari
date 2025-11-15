# Instruções de Uso - Detective Quest

## Compilação

Para compilar o programa, use o Makefile:

```bash
make
```

Ou compile manualmente:

```bash
gcc -Wall -Wextra -std=c11 -o detective_quest algoritmos_avancados.c
```

## Execução

Para executar o programa:

```bash
./detective_quest
```

Ou use o Makefile:

```bash
make run
```

## Como Jogar

### Menu Principal

Ao executar o programa, você verá um menu com três níveis:

1. **🌱 Nível Novato** - Exploração básica da mansão usando árvore binária
2. **🔍 Nível Aventureiro** - Exploração + coleta de pistas usando BST
3. **🧠 Nível Mestre** - Exploração completa com pistas e suspeitos usando tabela hash

### Controles Durante a Exploração

- **`e`** ou **`E`** - Ir para a esquerda
- **`d`** ou **`D`** - Ir para a direita
- **`s`** ou **`S`** - Sair da exploração
- **`p`** ou **`P`** - Ver pistas coletadas (níveis Aventureiro e Mestre)
- **`a`** ou **`A`** - Ver associações pista-suspeito (apenas Nível Mestre)
- **`c`** ou **`C`** - Ver suspeito mais provável (apenas Nível Mestre)

## Estrutura da Mansão

A mansão possui as seguintes salas organizadas em uma árvore binária:

- **Hall de Entrada** (raiz)
  - **Biblioteca** (esquerda)
    - **Cozinha** (esquerda)
    - **Sótão** (direita)
  - **Sala de Estar** (direita)
    - **Quarto Principal** (esquerda)
    - **Banheiro** (direita)
      - **Porão** (esquerda)

## Pistas e Suspeitos (Nível Mestre)

Cada sala contém uma pista que está associada a um suspeito:

- **Livro com páginas arrancadas** → Professor
- **Faca desaparecida** → Chef
- **Copo quebrado** → Mordomo
- **Carta rasgada** → Herdeiro
- **Espelho quebrado** → Herdeiro
- **Baú trancado** → Herdeiro
- **Corda manchada** → Mordomo

## Limpeza

Para remover o executável compilado:

```bash
make clean
```

