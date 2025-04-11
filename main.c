#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  char conteudo; // '#', '.', '%', '@', '$', etc.
} Ponto;

typedef struct {
  int linhas;
  int colunas;
  Ponto labirinto[20][20];
} Labirinto;

// Função para copiar a estrutura de um struct labirinto para outro
void copiarLabirinto(Labirinto *origem, Labirinto *destino) {
  destino->linhas = origem->linhas;
  destino->colunas = origem->colunas;

  for (int i = 0; i < origem->linhas; i++) {
    for (int j = 0; j < origem->colunas; j++) {
      destino->labirinto[i][j].conteudo = origem->labirinto[i][j].conteudo;
    }
  }
}

// Função de batalha
int batalha(float * chancevitoria) {
  float valoraleatorio = (float)rand() / RAND_MAX;

  printf("Iniciando batalha...\n");
  printf("Chance de vitória: %.0f%%\n", *chancevitoria * 100);
  printf("Sorteio: %.2f\n", valoraleatorio);

  if (valoraleatorio <= *chancevitoria) {
    printf("Ganhou a batalha!\n");

    // Aumentando a chance de vitória
    *chancevitoria += 0.1f;
    if (*chancevitoria > 1.0f) {
      *chancevitoria = 1.0f;
    }

    return 1; 
  } else {
    printf("Perdeu a batalha, game over!\n");
    return 0; 
  }
}

// Carregar o labirinto do arquivo
int imprimirLabirinto(const char *nome_arquivo, Labirinto *lab) {
  FILE *arquivo = fopen(nome_arquivo, "r");
  char ch;

  if (arquivo == NULL) {
    printf("Não foi possível abrir arquivo.\n");
    return 0;
  }

  fscanf(arquivo, "%d %d", &lab->linhas, &lab->colunas);

  for (int i = 0; i < lab->linhas; i++) {
    for (int j = 0; j < lab->colunas; j++) {
      fscanf(arquivo, " %c", &lab->labirinto[i][j].conteudo);
    }
  }

  printf("\nLabirinto carregado:\n\n");

  for (int i = 0; i < lab->linhas; i++) {
    for (int j = 0; j < lab->colunas; j++) {
      printf("%c ", lab->labirinto[i][j].conteudo);
    }
    printf("\n");
  }

  fclose(arquivo);
  return 1;
}

// Salvar o labirinto em um arquivo
void salvarLabirinto(const char *nome_arquivo, Labirinto *lab) {
  char arr_nome_arquivo[100];
  strcpy(arr_nome_arquivo, nome_arquivo);

  FILE *arquivo = fopen(strcat(arr_nome_arquivo, ".txt"), "w");
  if (arquivo == NULL) {
    printf("Não foi possível salvar arquivo.\n");
    return;
  }

  fprintf(arquivo, "%d %d\n", lab->linhas, lab->colunas);
  for (int i = 0; i < lab->linhas; i++) {
    for (int j = 0; j < lab->colunas; j++) {
      fprintf(arquivo, "%c", lab->labirinto[i][j].conteudo);
    }
    fprintf(arquivo, "\n");
  }

  fclose(arquivo);
  printf("Labirinto salvo com sucesso em %s\n", nome_arquivo);
}

void exibirMenu() {
  printf("\nEscolha uma opção:\n\n");
  printf("1. Resolver com uma tentativa\n");
  printf("2. Resolver até conseguir\n");
  printf("3. Salvar\n");
  printf("4. Sair\n");
  printf("> ");
}

void exibirMenuFinal() {
  printf("\nEscolha uma opção:\n\n");
  printf("1. Salvar labirinto acima\n");
  printf("2. Voltar ao menu inicial\n");
  printf("3. Sair\n");
  printf("> ");
}

// Função que resolve um labirinto utilizando uma matriz como argumento.
int resolverAleatorio(Labirinto *matriz, Labirinto *final) {
  Labirinto aux;
  copiarLabirinto(matriz, &aux);

  // Funções para encontrar a posição inicial do jogador dentro labirinto.
  int jx = -1, jy = -1;
  for (int i = 0; i < aux.linhas; i++) {
    for (int j = 0; j < aux.colunas; j++) {
      if (aux.labirinto[i][j].conteudo == '@') {
        jx = i;
        jy = j;
      }
    }
  }

  int encontrouSaida = 0, perdeuBatalha = 0;
  float chancevitoria = 0.5f;

  // Loop de movimentações
  while (!encontrouSaida && !perdeuBatalha) {
    int movimentos[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    int opcoesValidas[4][2];
    int totalOpcoes = 0;
    
      //Loop que tenta todos os movimentos e guarda dentro de uma matriz a parte todos os possiveis
      for (int i = 0; i < 4; i++) {
        int nx = jx + movimentos[i][0];
        int ny = jy + movimentos[i][1];

        if (nx >= 0 && nx < aux.linhas && ny >= 0 && ny < aux.colunas) {
          char destino = aux.labirinto[nx][ny].conteudo;
          if (destino == '.' || destino == '$' || destino == '%') {
            opcoesValidas[totalOpcoes][0] = nx;
            opcoesValidas[totalOpcoes][1] = ny;
            totalOpcoes++;
          }
        }
      }

    // Caso não há possibilidade de movimentacao, o código imprime que o jogador se perdeu
    if (totalOpcoes == 0) {
      printf("O jogador se perdeu!\n");
      aux.labirinto[jx][jy].conteudo = '?';
      break;
    }

    // Aleatoriamente, o código escolhe uma movimentação e armazena o tipo do charater do destino;
    int escolha = rand() % totalOpcoes;

    int nx = opcoesValidas[escolha][0];
    int ny = opcoesValidas[escolha][1];

    char destino = aux.labirinto[nx][ny].conteudo;

      // Condicionais para determinar as ações do personagem diante a movimentação no labirinto
      if (destino == '%') {
        if (!batalha(&chancevitoria)) {
          aux.labirinto[nx][ny].conteudo = '+';
          perdeuBatalha = 1;
        } else {
          aux.labirinto[nx][ny].conteudo = '!';
          aux.labirinto[jx][jy].conteudo = '*';
          jx = nx;
          jy = ny;
        }
      } else if (destino == '$') {
        aux.labirinto[nx][ny].conteudo = 'V';
        aux.labirinto[jx][jy].conteudo = '*';
        encontrouSaida = 1;
      } else {
        aux.labirinto[nx][ny].conteudo = '*';
        jx = nx;
        jy = ny;
      }

      // Função imprime a matriz do labirinto após a movimentação feita
      for (int i = 0; i < aux.linhas; i++) {
        for (int j = 0; j < aux.colunas; j++) {
          printf("%c ", aux.labirinto[i][j].conteudo);
        }
        printf("\n");
      }
      printf("\n");
    }

    copiarLabirinto(&aux, final);
  // Condicionais para afirmar que, se o jogador saiu do labirinto, a função retorna 1, se o jogador perdeu a rodada, retornar 0, e imprime no terminal o resultado. 
  if (encontrouSaida) {
    printf(">> O jogador encontrou a saída!\n\n");
    return 1;
  } else if (perdeuBatalha) {
    printf(">> O jogador perdeu a luta e morreu.\n\n");
  } else {
    for (int i = 0; i < aux.linhas; i++) {
      for (int j = 0; j < aux.colunas; j++) {
        printf("%c ", aux.labirinto[i][j].conteudo);
      }
      printf("\n");
    }
    printf("\n>> O jogador não conseguiu sair do labirinto.\n\n");
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Escreva o arquivo para abrir, ex: %s <arquivo_labirinto.txt>\n", argv[0]);
    return 0;
  }

  Labirinto lab;
  int opcao;
  char nome_saida[100];
  srand(time(NULL));

  if (!imprimirLabirinto(argv[1], &lab)) {
    return 0;
  }

  while (opcao != 4) {
    exibirMenu();
    scanf("%d", &opcao);
    getchar();
    
    Labirinto final;
    int opcaof = 0;

    switch (opcao) {
    case 1: {
        resolverAleatorio(&lab,&final);
        exibirMenuFinal();
        scanf("%d", &opcaof);
  
        switch (opcaof){
          case 1: {
            printf("\nDigite o nome do arquivo para salvar o labirinto: ");
  
            getchar();
            fgets(nome_saida, sizeof(nome_saida), stdin);
  
            nome_saida[strcspn(nome_saida, "\n")] = 0;
  
            salvarLabirinto(nome_saida, &final);
  
            printf("\n Voltando ao menu inicial... \n");}
          break;
  
          case 2:
            printf("\nVoltando ao menu inicial...\n");
          break;
  
          case 3:
            opcao = 4;
            printf("\nSaindo...\n");
          break;
  
          default:
            printf("Opção inválida.\n");}
          break;
        }
  
    case 2: {
    // Utiliza um loop até que a função resolverAleatório retorne true(1)
    int resultado, tentativa = 1;
    Labirinto final;
    
    do {
      printf("Tentativa %d:\n", tentativa++);
      resultado = resolverAleatorio(&lab, &final);} while (!resultado);

      exibirMenuFinal();
      scanf("%d", &opcaof);

      switch (opcaof){
        case 1: {
          printf("\nDigite o nome do arquivo para salvar o labirinto: ");

          getchar();
          fgets(nome_saida, sizeof(nome_saida), stdin);

          nome_saida[strcspn(nome_saida, "\n")] = 0;

          salvarLabirinto(nome_saida, &final);

          printf("\n Voltando ao menu inicial... \n\n");
          opcao = 0;}
        break;

        case 2:
          printf("\nVoltando ao menu...\n");
        break;

        case 3:
          opcao = 4;
          printf("\nSaindo...\n");
        break;

        default:
          printf("Opção inválida.\n");}
        break;
      }
      break;
    case 3:
      printf("\nDigite o nome do arquivo para salvar o labirinto: ");
      fgets(nome_saida, sizeof(nome_saida), stdin);
      salvarLabirinto(nome_saida, &lab);
      break;
    case 4:
      printf("\nSaindo...\n");
      break;
    default:
      printf("Opção inválida.\n");
    }
  };

  return 0;
}