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

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Escreva o arquivo para abrir, ex: %s <arquivo_labirinto.txt>\n", argv[0]);
    return 0;
  }

  Labirinto lab;
  float chancevitoria = 0.5f;
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

    switch (opcao) {
    case 1: {
      int x, y;
      printf("\nDigite a linha e a coluna onde deseja tentar: ");
      scanf("%d %d", &x, &y);
      getchar();

      if (x >= 0 && x < lab.linhas && y >= 0 && y < lab.colunas) {
        if (lab.labirinto[x][y].conteudo == '%') {
          if (!batalha(&chancevitoria)) {
            return 0; // perdeu o jogo
          } else {
            lab.labirinto[x][y].conteudo = '.'; // inimigo derrotado
          }
        } else {
          printf("Não há inimigo nesta posição.\n");
        }
      } else {
        printf("Posição inválida!\n");
      }
      break;
    }
    case 2:
      printf("\nCalma aí paizão, ainda não dá pra fazer isso.\n");
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

  return 0;
}