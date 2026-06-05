# Dungeon Crawler

## Desenvolvedor

* Eric Luis Barroso Cavalcante

---

## História do Jogo

<p align="justify">Em uma vila isolada próxima a uma antiga masmorra, estranhos acontecimentos começaram a assustar os moradores. Criaturas passaram a surgir durante a noite, armadilhas misteriosas apareceram nos corredores subterrâneos e rumores indicam que um poderoso Boss habita as profundezas da masmorra.</p>

<p align="justify">Você é um aventureiro enviado para explorar o local, enfrentar os monstros e descobrir a origem do mal que ameaça a região.</p>

<p align="justify">Sua missão é atravessar os três andares da masmorra, superar os desafios de cada fase e derrotar o Boss Final para restaurar a paz na vila.</p>

---

## Como Jogar

### Objetivo

<p align="justify">Explore a vila e os andares da masmorra, resolva os desafios, utilize armas e elimine monstros até derrotar o Boss Final.</p>

O jogador possui 3 vidas. Ao perder todas elas, o jogo termina.

---

### Controles

| Tecla | Ação                           |
| ----- | ------------------------------ |
| `w`   | Move o jogador para cima       |
| `a`   | Move o jogador para esquerda   |
| `s`   | Move o jogador para baixo      |
| `d`   | Move o jogador para direita    |
| `i`   | Interage com o objeto à frente |
| `o`   | Realiza ataque                 |

---

#### Armas

Durante o jogo, o jogador pode escolher uma das seguintes armas:

##### Espada

Ataca uma área de 3x2 células à frente do jogador.

##### Arco e Flecha

Ataca 4 células consecutivas em linha reta.

##### Cajado

Ataca todas as 8 células adjacentes ao jogador.

---

## Significado dos Símbolos

| Símbolo | Significado                   |
| ------- | ----------------------------- |
| `^`     | Jogador olhando para cima     |
| `<`     | Jogador olhando para esquerda |
| `v`     | Jogador olhando para baixo    |
| `>`     | Jogador olhando para direita  |
| `*`     | Parede                        |
| `#`     | Espinho                       |
| `k`     | Caixa destrutível             |
| `D`     | Porta fechada                 |
| `=`     | Porta aberta                  |
| `@`     | Chave                         |
| `L`     | Escada                        |
| `N`     | NPC da vila                   |
| `O`     | Botão                         |
| `X`     | Monstro Tipo 1                |
| `Y`     | Monstro Tipo 2                |
| `Z`     | Boss Final                    |

---

## Mecânicas do Jogo

* O jogador possui 3 vidas.
* Ao tocar em monstros ou espinhos, perde uma vida.
* Quando o jogador morre, a fase atual é reiniciada.
* O Monstro Tipo 1 (`X`) possui movimentação aleatória.
* O Monstro Tipo 2 (`Y`) persegue o jogador.
* O Boss Final (`Z`) possui comportamento especial e mais agressivo.
* Algumas fases exigem coletar chaves, destruir caixas e abrir portas para avançar.

---

## Estrutura do Jogo

### Vila

Área inicial onde o jogador escolhe sua arma através da interação com o NPC.

### 1º Andar da Masmorra

Tutorial das mecânicas básicas:

* movimentação;
* coleta de chaves;
* abertura de portas;
* destruição de caixas.

### 2º Andar da Masmorra

Introduz:

* espinhos;
* botões;
* monstros do tipo X.

### 3º Andar da Masmorra

Desafio final contendo:

* monstros tipo Y;
* múltiplos obstáculos;
* Boss Final (`Z`).

---

## Tecnologias Utilizadas

* Linguagem C
* Structs
* Vetores
* Ponteiros
* Funções
* Lógica de jogo e IA simples de monstros

---

## Declaração sobre Uso de IA Generativa

A IA foi utilizada principalmente para:

* explicações sobre ponteiros, structs e funções;
* apoio na documentação do projeto, especialmente na elaboração do tutorial do jogo e deste README.

---

## Compilação e Execução

Exemplo utilizando GCC:

```bash
gcc dungeon_crawler.c -o main
./main
```

---

## Considerações Finais

<p align="justify">O Dungeon Crawler foi desenvolvido como projeto acadêmico na disciplina Algoritmo e Codificação de Sistemas com o objetivo de aplicar conceitos fundamentais de programação em C através da criação de um jogo completo em console.</p>

Obrigado por jogar!
