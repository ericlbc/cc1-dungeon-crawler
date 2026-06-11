#include <locale.h>  // inclui declarações relacionadas à configuração local
#include <stdio.h>   // necessário para uso da função printf
#include <stdbool.h> // necessário para uso do tipo booleano
#include <string.h>  // necessário para uso de string
#include <stdlib.h>  // necessário para uso das funções srand e rand
#include <time.h>    // necessário para uso da função time

#define SEM_ARMA 0             
#define ESPADA 1
#define ARCO_E_FLECHA 2
#define CAJADO 3
#define RECOMPENSA_ESCADA 1
#define RECOMPENSA_MONSTRO_Z 2

// vetor de char das direções/sentidos do jogador
const char DIRECOES[4] = {'^', '<', 'v', '>'};


// struct de Jogador
typedef struct {
    int qtdVidas;     // quantidade de vidas de Jogador
    char voltadoPara; // direção e sentido de Jogador
    int x;            // coordenada x atual de Jogador (coluna)
    int y;            // coordenada y atual de Jogador (linha)
    int localizacao;  // Vila ou 1o Andar ou 2o Andar ou 3o Andar
    int arma;         // Sem Arma ou Espada ou Arco e Flecha ou Cajado
    int qtdChaves;    // quantidade de chaves
} Jogador;


// struct de Monstro
typedef struct {
    char nome;    // X, Y ou Z
    int xInicial; // coordenada x inicial de Monstro
    int yInicial; // coordenada y inicial de Monstro  
    int x;        // coordenada x atual de Monstro
    int y;        // coordenada y atual de Monstro
} Monstro;


// struct de deslocamento de Monstro
typedef struct {

    int y; // deslocamento na direção y
    int x; // deslocamento na direção x

} DeslocamentoMonstro;


// struc de Fase ()
typedef struct {
    int linhas;                // quantidade de linhas do mapa de Fase
    int colunas;               // quantidade de colunas do mapa de Fase
    char grid[25][26];         // mapa de Fase
    int xPortaSaida;           // coordenada x da porta de saída de Fase
    int yPortaSaida;           // coordenada y da porta de saída de Fase
    int qtdMonstros;           // quantidade de Monstros de Fase
    Monstro monstros[10];      // vetor de Monstros de Fase (até 10)
    int pontosObjetivo;        // pontos adquiridos ao atingir um objetivo em Fase
    int pontosNecessarios;     // pontos necessários para passar de Fase
    int recompensa;            // tipo de recompensa de Fase (RECOMPENSA_ESCADA ou RECOMPENSA_MONSTRO_Z)
    bool seChefeFinalLiberado; // se o Monstro Z foi liberado
} Fase;


// função que mostra a recompensa da fase f se os pontos atingidos são maiores
// ou iguais aos pontos necessários 
void concluirObjetivo(Fase *f) {

    // verifica se os pontos atingidos são menores do que os pontos necessários
    if (f->pontosObjetivo < f->pontosNecessarios) {

        // imprime mensagem da quantidade de pontos atingidos e de pontos que
        // faltam para atingir o objetivo
        wprintf(L"Você tem %d ponto(s); falta(m) %d ponto(s) para cumprir o desafio dessa fase.\n",
                f->pontosObjetivo,
                f->pontosNecessarios - f->pontosObjetivo
        );
        // retorna pois a quantidade de pontos atingidos é inferior à quantidade necessária
        return;
    }

    // verifica se a recompensa da fase f é a escada
    if (f->recompensa == RECOMPENSA_ESCADA) {

        // mostra na fase a escada
        f->grid[f->yPortaSaida][f->xPortaSaida] = 'L';
    }

    // verifica se:
    // 1. a recompensa da fase f é o Monstro Z;
    // 2. o Monstro Z já foi liberado
    else if (f->recompensa == RECOMPENSA_MONSTRO_Z && !f->seChefeFinalLiberado) {

        // incrementa a quantidade de monstros da fase f
        f->qtdMonstros++;                   
        // define o Monstro Z na fase f, que, por definição, é o 3º Andar da Masmorra           
        f->monstros[f->qtdMonstros - 1].nome = 'Z';    
        f->monstros[f->qtdMonstros - 1].xInicial = 12;
        f->monstros[f->qtdMonstros - 1].yInicial = 1;
        f->monstros[f->qtdMonstros - 1].x = 12;
        f->monstros[f->qtdMonstros - 1].y = 1;
        // seta que o Monstro Z foi liberado na fase f
        f->seChefeFinalLiberado = true;
        // imprime que o Monstro Z foi liberado
        wprintf(L"Boss %c liberado!!!\n", f->monstros[f->qtdMonstros - 1].nome);
    }
}


// função para mostrar o mapa da fase f, com seus Monstros, e o jogador j
void mostrarMapa(Fase *f, char locais[][100], Jogador *j) {

    //printf("\033[2J");
    // imprime o nome da localização do jogador j (Vila ou 1o Andar da Masmorra
    // ou 2o Andar da Masmorra ou 3o Andar da Masmorra)
    printf("\n%s:\n\n", locais[j->localizacao]);

    // define o booleano de se desenhou ou não
    bool desenhou;

    // itera sobre as linhas do mapa da fase f
    for (int i = 0; i < f->linhas; i++) {

        // itera sobre as colunas do mapa da fase f
        for (int k = 0; k < f->colunas; k++) {
            
            //inicializa o booleano como false
            desenhou = false;

            // verifica se a linha i e a coluna k correspondem às coordenadas do jogador j
            if (i == j->y && k == j->x) {

                // imprime o símbolo de direção/sentido do jogador j
                printf("%c", j->voltadoPara);
                // seta o booleano como true
                desenhou = true;
            }
            // verifica se a linha i e a coluna k correspondem às coordenadas de algum monstro
            // da fase f
            else {
              
                // itera sobre os monstros da fase f
                for (int m = 0; m < f->qtdMonstros; m++) {

                    // verifica se:
                    // 1. o monstro m não está morto; e
                    // 2. a linha i corresponde à coordenada y do monstro m da fase f; e
                    // 3. a coluna k corresponde à coordenada x do monstro m da fase f
                    if (f->monstros[m].nome != ' ' && i == f->monstros[m].y && k == f->monstros[m].x) {

                        // imprime o símbolo do monstro (X, Y ou Z)
                        printf("%c", f->monstros[m].nome);
                        // seta o booleano como true
                        desenhou = true;
                        // sai do loop de monstros, pois 2 monstros não podem estar no mesmo lugar
                        break;
                    }
                }
            }

            // verifica se não há jogador nem monstro na linha i e na coluna k
            if (!desenhou) {

                // imprime a célula do mapa da fase f
                printf("%c", f->grid[i][k]);
            }
        }
        // muda de linha
        printf("\n");
    }
}


// função de impressão de mensagem ao usuário que morreu
void mensagemMorteJogador(int qtdVidas) {

    // imprime que o jogador morreu
    wprintf(L"Você morreu!\n");

    // verifica se a quantidade de vidas do jogador é igual a zero
    if (qtdVidas == 0) {

        // imprime que o jogo acabou
        wprintf(L"Game over...\n");
    }
    // verifica se a quantidade de vidas do jogador é igual a um
    else if (qtdVidas == 1) {

        // imprime que há uma vida restante
        wprintf(L"Você tem 1 vida restante.\n");
    }
    // verifica se a quantidade de vidas do jogador é maior que um
    else {

        // imprime que há mais de uma vida restante
        wprintf(L"Você tem %d vidas restantes.\n", qtdVidas);
    }
}


// função para movimentar o jogador em uma determinada direção/sentido em uma fase
void moverSe(Jogador *j,
             char direcao,
             Fase *f) {

    int dy;    // define o int do deslocamento em y do jogador
    int dx;    // define o int do deslocamento em x do jogador
    int novoY; // define o int da possível nova coordenada y do jogador
    int novoX; // define o int da possível nova coordenada x do jogador

    // verifica se o jogador j se movimentará para cima
    if (direcao == '^') {

        // seta o deslocamento em y como sendo -1
        dy = -1;
        dx = 0;
    }
    // verifica se o jogador j se movimentará para esquerda
    else if (direcao == '<') {

        // seta o deslocamento em x como sendo -1
        dy = 0;
        dx = -1;
    }
    // verifica se o jogador j se movimentará para baixo
    else if (direcao == 'v') {

        // seta o deslocamento em y como sendo 1
        dy = 1;
        dx = 0;
    }
    // verifica se o jogador j se movimentará para direita
    else if (direcao == '>') {

        // seta o deslocamento em x como sendo 1
        dy = 0;
        dx = 1;
    }
    
    // atualiza o sentido/direção de movimento do jogador j
    j->voltadoPara = direcao;

    novoY = j->y + dy; // calcula a possível nova coordenada y do jogador
    novoX = j->x + dx; // calcula a possível nova coordenada x do jogador

    // verifica se a possível nova posição do jogador não está no grid da fase
    if (novoY < 0 || novoY >= f->linhas || novoX < 0 || novoX >= f->colunas) {

        // imprime que o jogador não pode sair do cenário
        wprintf(L"Você não pode sair do cenário.\n");
        // retorna sem movimentar o jogador
        return;
    }
    
    // itera sobre os monstros da fase f
    for (int m = 0; m < f->qtdMonstros; m++) {

        // verifica se:
        // 1. o monstro m da fase f não está morto; e
        // 2. se a posição nova coordenada y do jogador coincide com a do monstro m da fase f
        // 3. se a posição nova coordenada x do jogador coincide com a do monstro m da fase f
        if (f->monstros[m].nome != ' ' && novoY == f->monstros[m].y && novoX == f->monstros[m].x) {

            // o jogador perde uma vida, pois se movimentou para onde havia monstro
            j->qtdVidas--;  
            return;
        }
    }

    // define a variável do tipo char destino que é inicializada com o caractere presente 
    // na possível nova posição do jogador, lembrando que o espaço (' ') também é um caractere
    char destino = f->grid[novoY][novoX];
    
    // verifica se o movimento do jogador é para onde está uma parede
    if (destino == '*') {

        wprintf(L"Você não pode passar devido à parede.\n");        
    }
    // verifica se o movimento do jogador é para onde está um NPC
    else if (destino == 'N') {

        wprintf(L"Você não pode passar por cima do NPC.\n");
    }
    // verifica se o movimento do jogador é para onde está uma porta fechada
    else if (destino == 'D') {

        wprintf(L"A porta está fechada.\n");
    }
    // verifica se o movimento do jogador é para onde está uma caixa
    else if (destino == 'k') {

        wprintf(L"Tem uma caixa a sua frente.\n");
    }
    // verifica se o movimento do jogador é para onde está uma chave
    else if (destino == '@') {

        j->x = novoX;   // atualiza a coordenada x do jogador com o valor de novoX
        j->y = novoY;   // atualiza a coordenada y do jogador com o valor de novoY
        j->qtdChaves++; // incrementa de 1 a quantidade de chaves que o jogador possui

        // imprime a quantidade de chaves que o jogador agora possui
        wprintf(L"Você tem %d chave(s).\n", j->qtdChaves);


        f->grid[novoY][novoX] = ' '; // remove a chave do mapa da fase f
        f->pontosObjetivo++;         // incrementa de 1 a quantidade de pontos obtidos na fase f
        concluirObjetivo(f);         // chama a função que verifica se o objetivo da fase f
                                     // foi atingido

    }
    // verifica se o movimento do jogador é para onde está um espinho
    else if (destino == '#') {

        // o jogador perde uma vida, pois se movimentou para onde havia espinho
        j->qtdVidas--;
    }
    // verifica se o movimento do jogador é para onde está uma escada
    else if (destino == 'L') {

        // muda a fase do jogador
        j->localizacao++;
    }
    // verifica se o movimento do jogador é para onde o movimento é livre
    else {

        j->x = novoX; // atualiza a coordenada x do jogador com o valor de novoX
        j->y = novoY; // atualiza a coordenada y do jogador com o valor de novoY
    }
}


// função das interações do jogador com os elementos da fase
void interagir(Jogador *j, Fase *f) {

    int dy;      // define o int da distância em y entre o jogador e um objeto
    int dx;      // define o int da distância em x entre o jogador e um objeto
    int yObjeto; // define o int da coordenada y do objeto de interação com o jogador
    int xObjeto; // define o int da coordenada x do objeto de interação com o jogador

    // verifica se o jogador j está voltado para cima
    if (j->voltadoPara == '^') {

        // seta a distância entre o jogador e o objeto imediatamente acima dele
        dy = -1;
        dx = 0;
    }
    // verifica se o jogador j está voltado para esquerda
    else if (j->voltadoPara == '<') {

        // seta a distância entre o jogador e o objeto imediatamente à esquerda dele
        dy = 0;
        dx = -1;
    }
    // verifica se o jogador j está voltado para baixo
    else if (j->voltadoPara == 'v') {

        // seta a distância entre o jogador e o objeto imediatamente abaixo dele
        dy = 1;
        dx = 0;
    }
    // verifica se o jogador j está voltado para direita
    else if (j->voltadoPara == '>') {

        // seta a distância entre o jogador e o objeto imediatamente à direita dele
        dy = 0;
        dx = 1;
    }
                    
    yObjeto = j->y + dy; // calcula a coordenada y do objeto de interação com o jogador
    xObjeto = j->x + dx; // calcula a coordenada x do objeto de interação com o jogador
    
    // verifica se a posição do objeto de interação não está no grid da fase
    if (yObjeto < 0 || yObjeto >= f->linhas || xObjeto < 0 || xObjeto >= f->colunas) {

        return;
    }

    // define a variável do tipo char objeto que é inicializada com o caractere representativo 
    // do objeto de interação com o jogador
    char objeto = f->grid[yObjeto][xObjeto];
    
    // verifica se:
    // 1. o objeto de interação é uma porta fechada; e
    // 2. a quantidade de chaves que o jogador possui é maior que zero
    if (objeto == 'D' && j->qtdChaves > 0) {

        // decrementa de 1 a quantidade de chaves do jogador
        j->qtdChaves--;
        // imprime a quantidade de chaves atualizada do jogador
        wprintf(L"Você tem %d chave(s).\n", j->qtdChaves);

        // modifica o símbolo do objeto de interação para uma porta aberta
        f->grid[yObjeto][xObjeto] = '=';
        // incrementa de 1 a quantidade de pontos obtidos na fase f       
        f->pontosObjetivo++;
        // chama a função que verifica se o objetivo da fase f foi atingido
        concluirObjetivo(f);        
    }
    // verifica se o objeto de interação é um NPC (existente apenas na Vila por definição)
    else if (objeto == 'N') {

        // solicita ao jogador que escolha uma arma
        wprintf(L"Escolha uma arma:\n");
        wprintf(L"1. Espada\n");
        wprintf(L"2. Arco e Flecha\n");
        wprintf(L"3. Cajado\n");
        // aloca a opção escolhida ao jogador
        scanf("%d", &j->arma);

        // verifica se a arma escolhida é a espada
        if (j->arma == ESPADA) {

            wprintf(L"Você está armado com a Espada!\n");
        }
        // verifica se a arma escolhida é o arco e flecha
        else if (j->arma == ARCO_E_FLECHA) {

            wprintf(L"Você está armado com o Arco e Flecha!\n");
        }
        // verifica se a arma escolhida é o cajado
        else if (j->arma == CAJADO) {

            wprintf(L"Você está armado com o Cajado!\n");
        }
    }
    // verifica se o objeto de interação é um botão O
    else if (objeto == 'O') {

        if (j->localizacao == 2) {

            // incrementa a quantidade de monstros do 2o Andar da Masmorra
            f->qtdMonstros++;                   
            // cria um Monstro Y 2o no Andar da Masmorra         
            f->monstros[f->qtdMonstros - 1].nome = 'Y';    
            f->monstros[f->qtdMonstros - 1].xInicial = 10;
            f->monstros[f->qtdMonstros - 1].yInicial = 2;
            f->monstros[f->qtdMonstros - 1].x = 10;
            f->monstros[f->qtdMonstros - 1].y = 2;
        }
        else if (j->localizacao == 3) {

            // incrementa a quantidade de monstros do 3o Andar da Masmorra
            f->qtdMonstros++;                   
            // cria um Monstro Y 3o no Andar da Masmorra         
            f->monstros[f->qtdMonstros - 1].nome = 'Y';    
            f->monstros[f->qtdMonstros - 1].xInicial = 20;
            f->monstros[f->qtdMonstros - 1].yInicial = 3;
            f->monstros[f->qtdMonstros - 1].x = 20;
            f->monstros[f->qtdMonstros - 1].y = 3;


        }

        

        wprintf(L"O botão O foi pressionado, liberando um Monstro Y!\n");
    }
}


// função para o jogador atacar uma célula da fase
void atingirCelula(Fase *f, Jogador *j, int y, int x) {

    // verifica se a célula a ser atacada não está no grid da fase
    if (y < 0 || y >= f->linhas || x < 0 || x >= f->colunas) {

        return;
    }

    // define a variável do tipo char celula que é inicializada com o caractere da célula 
    // que o jogador deseja atacar
    char celula = f->grid[y][x];

    // verifica se a célula a ser atacada é uma caixa
    if (celula == 'k') {

        f->grid[y][x] = ' '; // remove a caixa do mapa da fase f
        f->pontosObjetivo++; // incrementa de 1 a quantidade de pontos obtidos na fase f
        concluirObjetivo(f); // chama a função que verifica se o objetivo da fase f foi atingido
    }

    // itera sobre os monstros da fase f 
    for (int m = 0; m < f->qtdMonstros; m++) {

        // verifica se:
        // 1. o monstro m não está morto; e
        // 2. a linha i corresponde à coordenada y do monstro m da fase f; e
        // 3. a coluna k corresponde à coordenada x do monstro m da fase f
        if (f->monstros[m].nome != ' ' && f->monstros[m].y == y && f->monstros[m].x == x) {

            if (f->monstros[m].nome  == 'Z') {

                f->monstros[m].nome = 'z'; // deixa o Monstro Z mais fraco
                wprintf(L"O Boss Final foi golpeado, mas segue vivo!\n");
            }
            else {

                // verifica se o monstro é o X
                if (f->monstros[m].nome  == 'X') {

                    f->pontosObjetivo += 100; // incrementa de 100 a quantidade de pontos obtidos na fase f
                    concluirObjetivo(f);      // chama a função que verifica se o objetivo da fase f
                                              // foi atingido                
                }
                // verifica se o monstro é o Y
                else if (f->monstros[m].nome  == 'Y') {

                    f->pontosObjetivo += 1000; // incrementa de 1000 a quantidade de pontos obtidos na fase f
                    concluirObjetivo(f);       // chama a função que verifica se o objetivo da fase f
                                               // foi atingido
                }
                f->monstros[m].nome = ' '; // mata o monstro m da fase f                            
            }
            break;                     // sai do loop de monstros, pois apenas um monstro pode
                                       // se localizar em uma célula
        }
    }
}


// função para o jogador atacar determinada região
void atacarArea(Fase *f, Jogador *j, int yInicial, int yFinal, int xInicial, int xFinal) {

    // itera sobre o range de linhas a atacar
    for (int i = yInicial; i <= yFinal; i++) {

        // itera sobre o range de colunas a atacar
        for (int k = xInicial; k <= xFinal; k++) {

            // chama a função que ataca determinada célula
            atingirCelula(f, j, i, k);
        }
    }
}


// função para o jogador atacar
void atacar(Jogador *j, Fase *f) {

    int yInicial; // define o int da coordenada y inicial da região de ataque
    int yFinal;   // define o int da coordenada y final da região de ataque
    int xInicial; // define o int da coordenada x inicial da região de ataque
    int xFinal;   // define o int da coordenada x final da região de ataque
    int frenteY;  // define o int da coordenada y da célula que está à frente do jogador
    int frenteX;  // define o int da coordenada x da célula que está à frente do jogador

    // verifica se o jogador não tem arma
    if (j->arma == SEM_ARMA) {

        wprintf(L"Você não tem arma!\n");
    }
    // verifica se o jogador tem arma
    else {

        // verifica se a arma do jogador é a espada
        if (j->arma == ESPADA) {

            // verifica se o jogador j está voltado para cima
            if (j->voltadoPara == '^') {

                // obtém as coordenadas da célula à frente do jogador
                frenteY = j->y - 1;
                frenteX = j->x;

                // verifica se a célula à frente do jogador contém parede ou porta fechada ou espinho
                if (f->grid[frenteY][frenteX] == '*' ||
                    f->grid[frenteY][frenteX] == 'D' ||
                    f->grid[frenteY][frenteX] == '#') {

                    // impossibilita o ataque do jogador nessas circunstâncias
                    return;
                }
                // obtém a região de ataque do jogador voltado para cima e de posse de espada
                yInicial = j->y - 2;
                yFinal = j->y - 1;
                xInicial = j->x - 1;
                xFinal = j->x + 1;            
            }
            // verifica se o jogador j está voltado para a esquerda
            else if (j->voltadoPara == '<') {

                // obtém as coordenadas da célula à frente do jogador
                frenteY = j->y;
                frenteX = j->x - 1;

                // verifica se a célula à frente do jogador contém parede ou porta fechada ou espinho
                if (f->grid[frenteY][frenteX] == '*' ||
                    f->grid[frenteY][frenteX] == 'D' ||
                    f->grid[frenteY][frenteX] == '#') {

                    // impossibilita o ataque do jogador nessas circunstâncias
                    return;
                }
                // obtém a região de ataque do jogador voltado para esquerda e de posse de espada
                yInicial = j->y - 1;
                yFinal = j->y + 1;
                xInicial = j->x - 2;
                xFinal = j->x - 1;  
            }
            // verifica se o jogador j está voltado para baixo
            else if (j->voltadoPara == 'v') {

                // obtém as coordenadas da célula à frente do jogador
                frenteY = j->y + 1;
                frenteX = j->x;

                // verifica se a célula à frente do jogador contém parede ou porta fechada ou espinho
                if (f->grid[frenteY][frenteX] == '*' ||
                    f->grid[frenteY][frenteX] == 'D' ||
                    f->grid[frenteY][frenteX] == '#') {

                    // impossibilita o ataque do jogador nessas circunstâncias
                    return;
                }
                // obtém a região de ataque do jogador voltado para baixo e de posse de espada
                yInicial = j->y + 1;
                yFinal = j->y + 2;
                xInicial = j->x - 1;
                xFinal = j->x + 1;  
            }
            // verifica se o jogador j está voltado para direita
            else if (j->voltadoPara == '>') {

                // obtém as coordenadas da célula à frente do jogador
                frenteY = j->y;
                frenteX = j->x + 1;

                // verifica se a célula à frente do jogador contém parede ou porta fechada ou espinho
                if (f->grid[frenteY][frenteX] == '*' ||
                    f->grid[frenteY][frenteX] == 'D' ||
                    f->grid[frenteY][frenteX] == '#') {

                    // impossibilita o ataque do jogador nessas circunstâncias
                    return;
                }
                // obtém a região de ataque do jogador voltado para esquerda e de posse de espada
                yInicial = j->y - 1;
                yFinal = j->y + 1;
                xInicial = j->x + 1;
                xFinal = j->x + 2;
            } 
        }
        // verifica se a arma do jogador é o arco e flecha
        else if (j->arma == ARCO_E_FLECHA) {
        
            // verifica se o jogador j está voltado para cima
            if (j->voltadoPara == '^') {

                // obtém as coordenadas da célula à frente do jogador
                frenteY = j->y - 1;
                frenteX = j->x;

                // verifica se a célula à frente do jogador contém parede ou porta fechada ou espinho
                if (f->grid[frenteY][frenteX] == '*' ||
                    f->grid[frenteY][frenteX] == 'D' ||
                    f->grid[frenteY][frenteX] == '#') {

                    // impossibilita o ataque do jogador nessas circunstâncias
                    return;
                }
                // obtém a região de ataque do jogador voltado para cima e de posse de arco e flecha
                yInicial = j->y - 4;
                yFinal = j->y - 1;
                xInicial = j->x;
                xFinal = j->x;
            }
            // verifica se o jogador j está voltado para esquerda
            else if (j->voltadoPara == '<') {

                // obtém as coordenadas da célula à frente do jogador
                frenteY = j->y;
                frenteX = j->x - 1;

                // verifica se a célula à frente do jogador contém parede ou porta fechada ou espinho
                if (f->grid[frenteY][frenteX] == '*' ||
                    f->grid[frenteY][frenteX] == 'D' ||
                    f->grid[frenteY][frenteX] == '#') {

                    // impossibilita o ataque do jogador nessas circunstâncias
                    return;
                }

                // obtém a região de ataque do jogador voltado para esquerda e de posse de arco e flecha
                yInicial = j->y;
                yFinal = j->y;
                xInicial = j->x - 4;
                xFinal = j->x - 1;
            }
            // verifica se o jogador j está voltado para baixo
            else if (j->voltadoPara == 'v') {

                // obtém as coordenadas da célula à frente do jogador
                frenteY = j->y + 1;
                frenteX = j->x;

                // verifica se a célula à frente do jogador contém parede ou porta fechada ou espinho
                if (f->grid[frenteY][frenteX] == '*' ||
                    f->grid[frenteY][frenteX] == 'D' ||
                    f->grid[frenteY][frenteX] == '#') {

                    // impossibilita o ataque do jogador nessas circunstâncias
                    return;
                }
                // obtém a região de ataque do jogador voltado para baixo e de posse de arco e flecha
                yInicial = j->y + 1;
                yFinal = j->y + 4;
                xInicial = j->x;
                xFinal = j->x;
            }
            // verifica se o jogador j está voltado para direita
            else if (j->voltadoPara == '>') {

                // obtém as coordenadas da célula à frente do jogador
                frenteY = j->y;
                frenteX = j->x + 1;

                // verifica se a célula à frente do jogador contém parede ou porta fechada ou espinho
                if (f->grid[frenteY][frenteX] == '*' ||
                    f->grid[frenteY][frenteX] == 'D' ||
                    f->grid[frenteY][frenteX] == '#') {

                    // impossibilita o ataque do jogador nessas circunstâncias
                    return;
                }
                // obtém a região de ataque do jogador voltado para direita e de posse de arco e flecha
                yInicial = j->y;
                yFinal = j->y;
                xInicial = j->x + 1;
                xFinal = j->x + 4;
            }        
        }
        // verifica se a arma do jogador é o cajado
        else if (j->arma == CAJADO) {

            // obtém a região de ataque do jogador voltado de posse de cajado
            yInicial = j->y - 1;
            yFinal = j->y + 1;
            xInicial = j->x - 1;
            xFinal = j->x + 1;        
        }  
        // chama a função que ataca determinada região
        atacarArea(f, j, yInicial, yFinal, xInicial, xFinal);
    }
}


// função para deslocamento do monstro X
DeslocamentoMonstro moverMonstroX() {

    // define e inicializa vetor de DeslocamentoMonstro com 4 elementos
    DeslocamentoMonstro movimentos[4] = {{-1, 0},
                                         {0, -1},
                                         {1, 0},
                                         {0, 1}};
    // retorna aleatoriamente um dos movimentos definidos
    return movimentos[rand() % 4];
}


// função para deslocamento do monstro Y
DeslocamentoMonstro moverMonstroY(Monstro *m, Jogador *j) {

    int dy; // define o int do deslocamento em y do monstro
    int dx; // define o int do deslocamento em x do monstro
    // define o int da direção aleatória de movimento do monstro
    int direcaoAleatoria;
    // define e inicializa o DeslocamentoMonstro deslocamento
    DeslocamentoMonstro deslocamento = {0, 0};

    // calcula a distância em y e em x do jogador ao monstro
    dy = m->y - j->y;
    dx = m->x - j->x;    

    // verifica se:
    // 1.1. o módulo da distância em y é menor que o da distância em x; e
    // 1.2. a distância em y é diferente de zero; ou
    // 2.1. o módulo da distância em y é maior que zero; e
    // 2.2. a distância em x é igual a zero.
    if ((abs(dy) < abs(dx) && dy != 0) || (abs(dy) > 0 && dx == 0)) {

        // verifica se o monstro está abaixo do jogador
        if (dy > 0) {

            // o monstro se "desloca" para cima em uma célula
            dy = -1;
            dx = 0;
        }
        // verifica se o monstro está acima do jogador
        else {

            // o monstro se "desloca" para baixo em uma célula
            dy = 1;
            dx = 0;
        }
    }
    // verifica se:
    // 1.1. o módulo da distância em x é menor que o da distância em y; e
    // 1.2. a distância em x é diferente de zero; ou
    // 2.1. o módulo da distância em x é maior que zero; e
    // 2.2. a distância em y é igual a zero.
    else if ((abs(dx) < abs(dy) && dx != 0) || (abs(dx) > 0 && dy == 0)) {

        // verifica se o monstro está à direita do jogador
        if (dx > 0) {

            // o monstro se "desloca" para esquerda em uma célula
            dy = 0;
            dx = -1;
        }
        // verifica se o monstro está à esquerda do jogador
        else {

            // o monstro se "desloca" para direita em uma célula
            dy = 0;
            dx = 1;
        }
    }
    // verifica se:
    // 1. o módulo da distância em y é igual ao módulo da distância em x; e
    // 2. a distância é diferente de zero em ambas as direções.
    else if (abs(dy) == abs(dx) && dy != 0) {

        // seta aleatoriamente o valor de direcaoAleatoria como 0 (para deslocamento em y)
        // ou 1 (para deslocamento em x)
        direcaoAleatoria = rand() % 2;

        // verifica se o monstro está está abaixo e à direita do jogador
        if (dy > 0 && dx > 0) {

            // verifica se o deslocamento aleatório é em y
            if (direcaoAleatoria == 0) {

                // o monstro se "desloca" para cima em uma célula
                dy = -1;
                dx = 0;
            }
            // verifica se o deslocamento aleatório é em x
            else {

                // o monstro se "desloca" para esquerda em uma célula
                dy = 0;
                dx = -1;
            }
        }
        // verifica se o monstro está está abaixo e à esquerda do jogador
        else if (dy > 0 && dx < 0) {

            // verifica se o deslocamento aleatório é em y
            if (direcaoAleatoria == 0) {

                // o monstro se "desloca" para cima em uma célula
                dy = -1;
                dx = 0;
            }
            // verifica se o deslocamento aleatório é em x
            else {

                // o monstro se "desloca" para direita em uma célula
                dy = 0;
                dx = 1;
            }
        }
        // verifica se o monstro está acima e à direita do jogador
        else if (dy < 0 && dx > 0) {

            // verifica se o deslocamento aleatório é em y
            if (direcaoAleatoria == 0) {

                // o monstro se "desloca" para baixo em uma célula
                dy = 1;
                dx = 0;
            }
            // verifica se o deslocamento aleatório é em x
            else {

                // o monstro se "desloca" para esquerda em uma célula
                dy = 0;
                dx = -1;
            }
        }
        // verifica se o monstro está acima e à esquerda do jogador
        else if (dy < 0 && dx < 0) {

            // verifica se o deslocamento aleatório é em y
            if (direcaoAleatoria == 0) {

                // o monstro se "desloca" para baixo em uma célula
                dy = 1;
                dx = 0;
            }
            // verifica se o deslocamento aleatório é em x
            else {

                // o monstro se "desloca" para direita em uma célula
                dy = 0;
                dx = 1;
            }
        }
    }

    deslocamento.y = dy; // seta que o deslocamento em y corresponde a dy 
    deslocamento.x = dx; // seta que o deslocamento em x corresponde a dx

    return deslocamento;
}


// função para deslocamento do monstro Z
DeslocamentoMonstro moverMonstroZ(Monstro *m, Jogador *j) {

    int dy; // define o int do deslocamento em y do monstro
    int dx; // define o int do deslocamento em x do monstro
    // define o int da direção aleatória de movimento do monstro
    int direcaoAleatoria;
    // define e inicializa o DeslocamentoMonstro deslocamento
    DeslocamentoMonstro deslocamento = {0, 0};

    // calcula a distância em y e em x do jogador ao monstro
    dy = m->y - j->y;
    dx = m->x - j->x;

    // comportamento próprio do monstro Z
    // verifica se:
    // 1. o jogador está com a espada; e
    // 2. o jogador está voltado para cima; e
    // 3. o monstro está uma ou duas células imediatamente acima do jogador 
    if (j->arma == ESPADA && j->voltadoPara == '^' && dx == 0 && dy >= -2 && dy <= -1) {

        // o monstro se "desloca" para abaixo e imediatamente atrás do jogador
        dy = -(dy - 1);
        dx = 0;
    }
    // verifica se:
    // 1. o jogador está com a espada; e
    // 2. o jogador está voltado para esquerda; e
    // 3. o monstro está uma ou duas células imediatamente à esquerda do jogador 
    else if (j->arma == ESPADA && j->voltadoPara == '<' && dy == 0 && dx >= -2 && dx <= -1) {

        // o monstro se "desloca" para direita e imediatamente atrás do jogador
        dy = 0;
        dx = -(dx - 1);
    }
    // verifica se:
    // 1. o jogador está com a espada; e
    // 2. o jogador está voltado para baixo; e
    // 3. o monstro está uma ou duas células imediatamente abaixo do jogador 
    else if (j->arma == ESPADA && j->voltadoPara == 'v' &&  dx == 0 && dy >= 1 && dy <= 2) {

        // o monstro se "desloca" para cima e imediatamente atrás do jogador
        dy = -(dy + 1);
        dx = 0;
    }
    // verifica se:
    // 1. o jogador está com a espada; e
    // 2. o jogador está voltado para direita; e
    // 3. o monstro está uma ou duas células imediatamente à direita do jogador 
    else if (j->arma == ESPADA && j->voltadoPara == '>' && dy == 0 && dx >= 1 && dx <= 2) {

        // o monstro se "desloca" para esquerda e imediatamente atrás do jogador
        dy = 0;
        dx = -(dx + 1);
    }
    // verifica se:
    // 1. o jogador está com o arco e flecha; e
    // 2. o jogador está voltado para cima; e
    // 3. o monstro está três ou quatro células imediatamente acima do jogador 
    else if (j->arma == ARCO_E_FLECHA && j->voltadoPara == '^' && dx == 0 && dy >= -4 && dy <= -3) {

        // o monstro se "desloca" para abaixo e imediatamente atrás do jogador
        dy = -(dy - 1);
        dx = 0;
    }
    // verifica se:
    // 1. o jogador está com o arco e flecha; e
    // 2. o jogador está voltado para esquerda; e
    // 3. o monstro está três ou quatro células imediatamente à esquerda do jogador 
    else if (j->arma == ARCO_E_FLECHA && j->voltadoPara == '<' && dy == 0 && dx >= -4 && dx <= -3) {

        // o monstro se "desloca" para direita e imediatamente atrás do jogador
        dy = 0;
        dx = -(dx - 1);
    }
    // verifica se:
    // 1. o jogador está com o arco e flecha; e
    // 2. o jogador está voltado para baixo; e
    // 3. o monstro está três ou quatro células imediatamente abaixo do jogador 
    else if (j->arma == ARCO_E_FLECHA && j->voltadoPara == 'v' && dx == 0 && dy >= 3 && dy <= 4) {

        // o monstro se "desloca" para cima e imediatamente atrás do jogador
        dy = -(dy + 1);
        dx = 0;
    }
    // verifica se:
    // 1. o jogador está com o arco e flecha; e
    // 2. o jogador está voltado para direita; e
    // 3. o monstro está três ou quatro células imediatamente à direita do jogador 
    else if (j->arma == ARCO_E_FLECHA && j->voltadoPara == '>' && dy == 0 && dx >= 3 && dx <= 4) {

        // o monstro se "desloca" para esquerda e imediatamente atrás do jogador
        dy = 0;
        dx = -(dx + 1);
    }
    // verifica se:
    // 1. o jogador está com o cajado; e
    // 2. o jogador está voltado para cima; e
    // 3. o monstro está uma célula imediatamente acima do jogador 
    else if (j->arma == CAJADO && j->voltadoPara == '^' && dx == 0 && dy == -1) {

        // o monstro se "desloca" para abaixo e imediatamente atrás do jogador
        dy = -(dy - 1);
        dx = 0;
    }
    // verifica se:
    // 1. o jogador está com o cajado; e
    // 2. o jogador está voltado para esquerda; e
    // 3. o monstro está uma célula imediatamente à esquerda do jogador 
    else if (j->arma == CAJADO && j->voltadoPara == '<' && dy == 0 && dx == -1){

        // o monstro se "desloca" para direita e imediatamente atrás do jogador
        dy = 0;
        dy = 0;
        dx = -(dx - 1);
    }
    // verifica se:
    // 1. o jogador está com o cajado; e
    // 2. o jogador está voltado para baixo; e
    // 3. o monstro está uma célula imediatamente abaixo do jogador 
    else if (j->arma == CAJADO && j->voltadoPara == 'v' && dx == 0 && dy == 1) {

        // o monstro se "desloca" para cima e imediatamente atrás do jogador
        dy = -(dy + 1);
        dx = 0;
    }
    // verifica se:
    // 1. o jogador está com o cajado; e
    // 2. o jogador está voltado para direita; e
    // 3. o monstro está uma célula imediatamente à direita do jogador 
    else if (j->arma == CAJADO && j->voltadoPara == '>' && dy == 0 && dx == 1){

        // o monstro se "desloca" para esquerda e imediatamente atrás do jogador
        dy = 0;
        dx = -(dx + 1);
    }
    // o próprio comportamento do monstro Y
    else {

        DeslocamentoMonstro deslocamentoComoY = moverMonstroY(m, j);
        dy = deslocamentoComoY.y;
        dx = deslocamentoComoY.x;
    }    
    deslocamento.y = dy; // seta que o deslocamento em y corresponde a dy 
    deslocamento.x = dx; // seta que o deslocamento em x corresponde a dx 
    return deslocamento;  
}


// função para retornar true se o monstro Z está morto
bool seChefeFinalMorto(Fase *f) {

    // verifica se o monstro Z ainda não foi liberado
    if (!f->seChefeFinalLiberado) {

        // retorna que o monstro Z não está morto, pois ainda não foi liberado
        return false;
    }

    // itera sobre os monstros
    for (int i = 0; i < f->qtdMonstros; i++) {

        // verifica se o monstro Z está vivo
        if (f->monstros[i].nome == 'Z' || f->monstros[i].nome == 'z') {

            /// retorna que o monstro Z não está morto, pois ainda está vivo 
            return false;
        }
    }
    // retorna que o monstro Z está morto, pois já foi liberado e não está vivo
    return true;
}


// função para movimentar um monstro
void moverMonstro(Monstro *m, Jogador *j, Fase *f) {

    int novoY; // define o int da possível nova coordenada y do monstro
    int novoX; // define o int da possível nova coordenada x do monstro

    // define e inicializa o DeslocamentoMonstro deslocamento
    DeslocamentoMonstro movimento = {0, 0};
    
    // verifica se se trata do monstro X
    if (m->nome == 'X') {

        // chama a função que movimenta o monstro X
        movimento = moverMonstroX();        
    }
    // verifica se se trata do monstro Y
    else if (m->nome == 'Y') {

        // chama a função que movimenta o monstro Y
        movimento = moverMonstroY(m, j);
    }
    // verifica se se trata do monstro Z
    else if (m->nome == 'Z' || m->nome == 'z') {

        // chama a função que movimenta o monstro Z
        movimento = moverMonstroZ(m, j);
    }

    novoY = m->y + movimento.y; // calcula a possível nova coordenada y do Monstro m
    novoX = m->x + movimento.x; // calcula a possível nova coordenada x do Monstro m
    
    // itera sobre os monstros da fase f
    for (int i = 0; i < f->qtdMonstros; i++) {

        // verifica se o Monstro i da fase f se trata do próprio Monstro m a movimentar
        if (&f->monstros[i] == m) {
            
            // passa para o próximo Monstro da fase f (incrementa i de um)
            continue;
        }

        // verifica se:
        // 1. o Monstro i da fase f não está morto; e
        // 2. a possível nova posição do Monstro m já está ocupada por outro Monstro i da fase f
        if (f->monstros[i].nome != ' ' && novoY == f->monstros[i].y && novoX == f->monstros[i].x) {

            // retorna sem movimentar o Monstro m
            return;
        }
    }

    // verifica se a possível nova posição do Monstro m não está no grid da fase
    if (novoY < 0 || novoY >= f->linhas || novoX < 0 || novoX >= f->colunas) {

        // retorna sem movimentar o Monstro m
        return;
    }

    // define a variável do tipo char destino que é inicializada com o caractere presente 
    // na possível nova posição do Monstro m
    char destino = f->grid[novoY][novoX];    
    
    // verifica se se trata dos monstros X ou Y
    if (m->nome == 'X' || m->nome == 'Y') {

        // verifica se o movimento do Monstro m é para onde está:
        if (destino == '*' || // parede
            destino == 'D' || // porta fechada
            destino == 'L' || // escada
            destino == 'k' || // caixa
            destino == '#') { // espinho

            // retorna sem movimentar o Monstro m
            return;
        }
    }
    // verifica se se trata do monstro Z
    else if (m->nome == 'Z' || m->nome == 'z') {

        // verifica se o movimento do Monstro m é para onde está:
        if (destino == '*' || // parede
            destino == 'D' || // porta fechada
            destino == 'L') { // escada
            
            // retorna sem movimentar o Monstro m
            return;
        }
        // verifica se o movimento do Monstro m é para onde está caixa ou espinho:
        else if (destino == 'k' || destino == '#') {

            f->grid[novoY][novoX] = ' '; // o Monstro m destroi a caixa ou o espinho
            m->x = novoX; // atualiza a coordenada x do Monstro m
            m->y = novoY; // atualiza a coordenada y do Monstro m       
        }
    }    
    // verifica se o Monstro m se colocou na célula do jogador
    if (novoY == j->y && novoX == j->x && m->nome != ' ') {

        // o jogador perde uma vida, pois o Monstro m o alcançou
        j->qtdVidas--;
    }
    // verifica se o Monstro m se movimentou em direção ao vazio
    else {

        m->x = novoX; // atualiza a coordenada x do Monstro m
        m->y = novoY; // atualiza a coordenada y do Monstro m  
    }
}


// função para reiniciar a fase em caso de morte do jogador
void reiniciarFase(Fase fases[], Fase fasesOriginais[], Jogador *j) {

    // define e inicializa o int faseAtual com a fase em que está o jogador
    int faseAtual = j->localizacao;

    // restaura a fase ao seu estado original
    fases[faseAtual] = fasesOriginais[faseAtual];

    // coloca o jogador com a direção/sentido padrão
    j->voltadoPara = '^';

    // coloca o jogador na célula padrão de início de fase
    j->x = fases[faseAtual].colunas / 2;
    j->y = fases[faseAtual].linhas - 1;
}


// função main
int main() {

    setlocale(LC_ALL, ""); // configura para usar as configurações regionais (locale) 
                           // do sistema operacional (no caso: pt-br).

    // define um seed diferente toda vez que o programa é executado
    srand(time(NULL));

    // definição da variável int das opções do programa (1- Jogar; 2- Tutorial; 3- Sair)
    int opcaoInicial;
    // definição da variável char do tipo de ação do jogador (mover, interagir ou atacar)
    char tipoAcao;    
    // definição da variável jogador do tipo Jogador
    Jogador jogador; 
    
    // definição da variável vila do tipo Fase
    Fase vila = {
        10,            // seta a quantidade de linhas do mapa da Vila
        10,            // seta a quantidade de colunas do mapa da Vila
        {"****L*****", // seta o mapa da Vila
         "**** *****",
         "*         ",
         "***       ",
         "N =       ",
         "***       ",
         "          ",
         "          ",
         "          ",
         "*        *"}
    };
    // seta a quantidade de monstros da Vila
    vila.qtdMonstros = 0;
    // seta a quantidade de pontos obtidos pelo jogador na Vila      
    vila.pontosObjetivo = 0;
    // seta a quantidade de pontos necessários ao jogador para sair da Vila    
    vila.pontosNecessarios = 0; 

    // definição da variável masmorra1 do tipo Fase
    Fase masmorra1 = {
        10,            // seta quantidade de linhas do mapa do 1o Andar da Masmorra
        10,            // seta quantidade de colunas do mapa do 1o Andar da Masmorra
        {"**********", // seta o mapa do 1o Andar da Masmorra
         "*        *",
         "         *",
         "****D*****",
         "*       k@",
         "*      ***",
         "*        *",
         "*        *",
         "*        *",
         "*        *"}
    };
    // seta a coordenada x da porta de saída do 1o Andar da Masmorra
    masmorra1.xPortaSaida = 1;
    // seta a coordenada y da porta de saída do 1o Andar da Masmorra
    masmorra1.yPortaSaida = 0;
    // seta a quantidade de monstros do 1o Andar da Masmorra
    masmorra1.qtdMonstros = 0;
    // seta a quantidade de pontos obtidos pelo jogador no 1o Andar da Masmorra
    masmorra1.pontosObjetivo = 0;
    // seta a quantidade de pontos necessários ao jogador para sair do 1o Andar da Masmorra
    masmorra1.pontosNecessarios = 3;
    // seta o tipo de recompensa no caso de pontosObjetivo >= pontosNecessarios
    masmorra1.recompensa = RECOMPENSA_ESCADA;

    // definição da variável masmorra2 do tipo Fase
    Fase masmorra2 = {
        15,                 // seta quantidade de linhas do mapa do 2o Andar da Masmorra
        15,                 // seta quantidade de colunas do mapa do 2o Andar da Masmorra
        {"***************", // seta o mapa do 2o Andar da Masmorra
         "*             *",
         "#       **    *",
         "*D*           *",
         "*  ##        **",
         "*    ##        ",
         "#      ##    **",
         "#        ##   *",
         "**  **     ## #",
         "@k           ##",
         "************k**",
         "*            O#",
         "*######D######*",
         "*            @#",
         "*             #"}
    };
    // seta a coordenada x da porta de saída do 2o Andar da Masmorra
    masmorra2.xPortaSaida = 5;
    // seta a coordenada y da porta de saída do 2o Andar da Masmorra
    masmorra2.yPortaSaida = 0;
    // seta a quantidade de monstros do 2o Andar da Masmorra
    masmorra2.qtdMonstros = 2;
    // seta a quantidade de pontos necessários ao jogador para sair do 2o Andar da Masmorra
    masmorra2.pontosNecessarios = 1206;
    // seta o tipo de recompensa no caso de pontosObjetivo >= pontosNecessarios
    masmorra2.recompensa = RECOMPENSA_ESCADA;
    
    // definição da variável masmorra3 do tipo Fase
    Fase masmorra3 = {
        25,                           // seta a quantidade de linhas do mapa do 3o Andar da Masmorra
        25,                           // seta a quantidade de colunas do mapa do 3o Andar da Masmorra
        {"*************************", // seta o mapa do 3o Andar da Masmorra
         "*                       *",
         "#                       *",
         "#                       *",
         "*                       *",
         "*                       *",
         "#**********O******##D###*",
         "#         *k*           *",
         "*                      @#",
         "*         ###           #",
         "*         ###           #",
         "*                       #",
         "*         ***           #",
         "***########@####D#******#",
         "***#D             *******",
         "*kkkk*************      #",
         "*    *************      #",
         "*    *                  #",
         "*       # #             #",
         "*       #@####          #",
         "*       #               #",
         "*       #####*          #",
         "*       ******          #",
         "*                       #",
         "*                       #"}
    };
    // seta a quantidade de monstros do 3o Andar da Masmorra
    masmorra3.qtdMonstros = 3;
    // seta a quantidade de pontos necessários ao jogador para sair do 3o Andar da Masmorra
    masmorra3.pontosNecessarios = 2209;
    // seta o tipo de recompensa no caso de pontosObjetivo >= pontosNecessarios
    masmorra3.recompensa = RECOMPENSA_MONSTRO_Z;
    // seta o booleano para indicar que o monstro Z não foi liberado
    masmorra3.seChefeFinalLiberado = false;

    // definição de vetor de Fase com quatro elementos (Vila, e 1o, 2o e 3o Andares da Masmorra)
    Fase fases[4];
    // definição de vetor de Fase com quatro elementos (Vila, e 1o, 2o e 3o Andares da Masmorra)          
    Fase fasesOriginais[4];    
    // vetor de string do nome das fases com quatro elementos
    char localizacoes[4][100] = {
        "Vila",
        "1o Andar da Masmorra",
        "2o Andar da Masmorra",
        "3o Andar da Masmorra"
    };

    // enquanto a opção for jogar ou tutorial ou sair
    do {

        wprintf(L"1. Jogar\n");
        wprintf(L"2. Tutorial\n");
        wprintf(L"3. Sair\n");

        // lê a opção digitada pelo usuário
        scanf("%d", &opcaoInicial);

        // verifica se a opção é jogar
        if (opcaoInicial == 1) {

            jogador.qtdVidas = 3;         // seta a quantidade de vidas
            jogador.voltadoPara = '^';    // seta a direção/sentido do jogador
            jogador.x = vila.colunas / 2; // seta a coordenada x inicial do jogador na Vila 
            jogador.y = vila.linhas - 1;  // seta a coordenada y inicial do jogador na Vila   
            jogador.localizacao = 0;      // seta que a localização do jogador é a Vila
            jogador.arma = 0;             // seta que o jogador não tem arma
            jogador.qtdChaves = 0;        // seta que o jogador não tem chave

            int vidasAnteriores;     // define o int de quantidade de vidas que o jogador possuía
            int localizacaoAnterior; // define o int de localização anterior do jogador

            // seta um Monstro X para o 2o Andar da Masmorra
            masmorra2.monstros[0] = (Monstro){'X', 7, 1, 7, 1};
            // seta outro Monstro X para o 2o Andar da Masmorra
            masmorra2.monstros[1] = (Monstro){'X', 3, 5, 3, 5};
            // seta a quantidade de pontos obtidos pelo jogador no 2o Andar da Masmorra
            masmorra2.pontosObjetivo = 0;

            // seta um Monstro X para o 3o Andar da Masmorra
            masmorra3.monstros[0] = (Monstro){'X', 9, 20, 9, 20};
            // seta outro Monstro X para o 3o Andar da Masmorra
            masmorra3.monstros[1] = (Monstro){'X', 18, 9, 18, 9};
            // seta um Monstro Y para o 3o Andar da Masmorra
            masmorra3.monstros[2] = (Monstro){'Y', 3, 7, 3, 7};
            // seta a quantidade de pontos obtidos pelo jogador no 3o Andar da Masmorra
            masmorra3.pontosObjetivo = 0;
                        
            // seta que o 1o elemento do vetor de Fase é a Fase vila
            fases[0] = vila;
            // seta que o 2o elemento do vetor de Fase é a Fase masmorra1     
            fases[1] = masmorra1;
            // seta que o 3o elemento do vetor de Fase é a Fase masmorra2
            fases[2] = masmorra2;
            // seta que o 4o elemento do vetor de Fase é a Fase masmorra3
            fases[3] = masmorra3;
            
            // define e inicializa o int da quantidade de fases
            int qtdFases = sizeof(fases) / sizeof(fases[0]);

            // similar ao vetor fases, mas não é modificado
            fasesOriginais[0] = vila;
            fasesOriginais[1] = masmorra1;
            fasesOriginais[2] = masmorra2;
            fasesOriginais[3] = masmorra3;

            // chama a função para mostrar o mapa da fase em que o jogador está; no caso a Vila
            mostrarMapa(&fases[jogador.localizacao], localizacoes, &jogador);            

            // enquanto:
            // 1. a quantidade de vidas do jogador é maior que zero; e
            // 2. o Monstro Z não está morto
            do {

                // guarda a quantidade de vidas do jogador
                vidasAnteriores = jogador.qtdVidas;

                wprintf(L"Qual ação deseja realizar?\n");
                // lê o tipo de ação a realizar (mover-se, interagir ou atacar)
                scanf(" %c", &tipoAcao);

                // verifica se a ação desejada é de movimento
                if (tipoAcao == 'w' || tipoAcao == 'a' || tipoAcao == 's' || tipoAcao == 'd') {

                    // guarda localização do jogador
                    localizacaoAnterior = jogador.localizacao;

                    // verifica se o movimento desejado é para cima
                    if (tipoAcao == 'w') {

                        // chama a função para movê-lo para cima na fase em que se encontra
                        moverSe(&jogador, '^', &fases[jogador.localizacao]);
                    }
                    // verifica se o movimento desejado é para esquerda
                    else if (tipoAcao == 'a') {
                        
                        // chama a função para movê-lo para esquerda na fase em que se encontra
                        moverSe(&jogador, '<', &fases[jogador.localizacao]);
                    }
                    // verifica se o movimento desejado é para baixo
                    else if (tipoAcao == 's') {
                        
                        // chama a função para movê-lo para baixo na fase em que se encontra
                        moverSe(&jogador, 'v', &fases[jogador.localizacao]);
                    }                
                    // verifica se o movimento desejado é para direita
                    else if (tipoAcao == 'd') {
                        
                        // chama a função para movê-lo para direita na fase em que se encontra
                        moverSe(&jogador, '>', &fases[jogador.localizacao]);
                    }

                    // verifica se o jogador mudou de fase
                    if (jogador.localizacao != localizacaoAnterior) {

                        // atualiza a posição do jogador na nova fase
                        jogador.x = fases[jogador.localizacao].colunas / 2;
                        jogador.y = fases[jogador.localizacao].linhas - 1;
                    }                                        
                }
                // verifica se a ação desejada é de interação
                else if (tipoAcao == 'i') {

                    // chama a função para fazer o jogador interagir na fase em que se encontra
                    interagir(&jogador, &fases[jogador.localizacao]);                    
                }
                // verifica se a ação desejada é de ataque
                else if (tipoAcao == 'o') {

                    // chama a função para fazer o jogador atacar na fase em que se encontra
                    atacar(&jogador, &fases[jogador.localizacao]);                        
                }

                // itera sobre os monstros da fase em que o jogador se encontra
                for (int i = 0; i < fases[jogador.localizacao].qtdMonstros; i++) {

                    // chama a função para movimentar os monstros da fase em que o jogador se encontra
                    moverMonstro(&fases[jogador.localizacao].monstros[i],
                                 &jogador,
                                 &fases[jogador.localizacao]);
                }

                // verifica se:
                // 1. o jogador morreu; e
                // 2. a quantidade de vidas do jogador é maior que zero
                if (jogador.qtdVidas < vidasAnteriores && jogador.qtdVidas > 0) {

                    // chama a função para informar ao usuário que o jogador morreu
                    mensagemMorteJogador(jogador.qtdVidas);
                    // chama a função para setar fases com os dados de fasesOriginais e
                    // o jogador na posição e direção/sentido padrões
                    reiniciarFase(fases, fasesOriginais, &jogador);
                }

                // chama a função para mostrar o mapa da fase em que o jogador está
                mostrarMapa(&fases[jogador.localizacao], localizacoes, &jogador);

            } while (jogador.qtdVidas > 0 && !seChefeFinalMorto(&fases[qtdFases - 1]));
            
            // verifica se a quantidade de vidas do jogador é maior que zero
            if (jogador.qtdVidas > 0) {
                
                // tela de vitória
                wprintf(L"\n");
                wprintf(L"=====================================================\n");
                wprintf(L"                    Vitória!\n");
                wprintf(L"=====================================================\n\n");
                wprintf(L"Após uma longa jornada pelos corredores escuros\n");
                wprintf(L"da antiga masmorra, o Boss Final finalmente foi\n");
                wprintf(L"derrotado.\n\n");               
                wprintf(L"A energia sombria que dominava a masmorra desapareceu.\n\n");
                wprintf(L"Os moradores da vila celebraram o retorno da paz,\n");
                wprintf(L"agradecendo ao aventureiro que teve coragem de\n");
                wprintf(L"enfrentar os perigos da masmorra.\n\n");
                wprintf(L"=====================================================\n");
                wprintf(L"             Você concluiu o jogo!\n");
                wprintf(L"=====================================================\n\n");
                
                // sai do do-while                
                break;
            }            
        }
        // verifica se a opção é mostrar o tutorial
        else if (opcaoInicial == 2) {

            wprintf(L"\n=====================================================\n");
            wprintf(L"                 Dungeon Crawler\n");
            wprintf(L"=====================================================\n\n");
            wprintf(L"História\n\n");
            wprintf(L"Você é um aventureiro que chegou a uma vila\n");
            wprintf(L"misteriosa próxima de uma antiga masmorra.\n");
            wprintf(L"Seu objetivo é atravessar os 3 andares da\n");
            wprintf(L"masmorra e derrotar o Boss Final.\n\n");
            wprintf(L"Durante a jornada você enfrentará monstros,\n");
            wprintf(L"armadilhas e diversos desafios.\n\n");
            wprintf(L"=====================================================\n");
            wprintf(L"                   Controles\n");
            wprintf(L"=====================================================\n\n");
            wprintf(L"w -> mover para cima\n");
            wprintf(L"a -> mover para esquerda\n");
            wprintf(L"s -> mover para baixo\n");
            wprintf(L"d -> mover para direita\n");
            wprintf(L"i -> interagir\n");
            wprintf(L"o -> atacar\n\n");
            wprintf(L"=====================================================\n");
            wprintf(L"             Símbolos do Jogo\n");
            wprintf(L"=====================================================\n\n");
            wprintf(L"^  Jogador olhando para cima\n");
            wprintf(L"<  Jogador olhando para esquerda\n");
            wprintf(L"v  Jogador olhando para baixo\n");
            wprintf(L">  Jogador olhando para direita\n\n");
            wprintf(L"*  Parede\n");
            wprintf(L"#  Espinho\n");
            wprintf(L"k  Caixa destrutível\n");
            wprintf(L"D  Porta fechada\n");
            wprintf(L"=  Porta aberta\n");
            wprintf(L"@  Chave\n");
            wprintf(L"L  Escada\n");
            wprintf(L"N  NPC da vila\n\n");
            wprintf(L"X  Monstro Tipo 1\n");
            wprintf(L"Y  Monstro Tipo 2\n");
            wprintf(L"Z  Boss Final\n\n");
            wprintf(L"=====================================================\n");
            wprintf(L"                    Mecânicas\n");
            wprintf(L"=====================================================\n\n");
            wprintf(L"- Você possui 3 vidas.\n");
            wprintf(L"- Ao tocar em um monstro ou espinho,\n");
            wprintf(L"  perde 1 vida.\n");
            wprintf(L"- Quando perde uma vida, a fase reinicia.\n");
            wprintf(L"- Ao perder todas as vidas, ocorre Game Over.\n\n");
            wprintf(L"=====================================================\n");
            wprintf(L"                     Armas\n");
            wprintf(L"=====================================================\n\n");
            wprintf(L"Espada\n");
            wprintf(L"- Ataca uma área de 3x2 à frente.\n\n");
            wprintf(L"Arco e Flecha\n");
            wprintf(L"- Ataca 4 células em linha reta.\n\n");
            wprintf(L"Cajado\n");
            wprintf(L"- Ataca todas as 8 células adjacentes.\n\n");
            wprintf(L"=====================================================\n");
            wprintf(L"                   Monstros\n");
            wprintf(L"=====================================================\n\n");
            wprintf(L"X -> movimentação aleatória.\n");
            wprintf(L"Y -> persegue o jogador.\n");
            wprintf(L"Z -> Boss Final com comportamento especial.\n\n");
            wprintf(L"=====================================================\n");            
            wprintf(L"                    Objetivo\n");
            wprintf(L"=====================================================\n\n");
            wprintf(L"Explore a masmorra, resolva os desafios,\n");
            wprintf(L"derrote os monstros e elimine o Boss Final\n");
            wprintf(L"para concluir a aventura.\n\n");
        }
        // verifica se a opção é sair
        else if (opcaoInicial == 3) {

            wprintf(L"\n=====================================================\n");            
            wprintf(L"                     Créditos\n");
            wprintf(L"=====================================================\n\n");
            wprintf(L"                Dungeon Crawler\n\n");
            wprintf(L"Projeto desenvolvido na linguagem C\n");
            wprintf(L"como trabalho da disciplina Algoritmo e Codificação de Sistemas.\n\n");
            wprintf(L"=====================================================\n");
            wprintf(L"                 Desenvolvedor\n");
            wprintf(L"=====================================================\n\n");
            wprintf(L"Eric Luis Barroso Cavalcante\n\n");
            wprintf(L"=====================================================\n");
            wprintf(L"              Tecnologias Utilizadas\n");
            wprintf(L"=====================================================\n\n");
            wprintf(L"- Linguagem C\n");
            wprintf(L"- Structs\n");
            wprintf(L"- Vetores\n");
            wprintf(L"- Ponteiros\n");
            wprintf(L"- Funções\n");
            wprintf(L"- Lógica de jogo e IA simples de monstros\n\n");
            
            // sai do do-while
            break;
        }

    } while (opcaoInicial == 1 || opcaoInicial == 2 || opcaoInicial == 3);    

return 0;
}