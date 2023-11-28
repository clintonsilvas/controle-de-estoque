#include <ctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
  int id;
  char descricao[30];
  float preco_venda;
  char unidade[3];
  char fornecedor[40];
  char status;
  int quant_est;
} produto;

int tamanho(FILE *);
void cadastro(FILE *);
void consultar(FILE *arquivoBin);
void consulta_codigo(FILE *arquivoBin);
void consulta_descricao(FILE *arquvoBin);
void gera_Arq_txt(FILE *arquivoBin);
void excluir(FILE *arquivoBin);
void excluir_codigo(FILE *arquivoBin);
void excluir_descricao(FILE *arquivoBin);

int main(void) {
  setlocale(LC_ALL, "Portuguese");
  int op;
  FILE *arquivoBin;
  if ((arquivoBin = fopen("C:\\Prog Estruturada\\Projeto Lab Prog Estr Mod2\\arquivo.dat", "rb+")) == NULL) {

    if ((arquivoBin = fopen("arquivo.dat", "wb+")) == NULL) {
      printf("Erro ao abrir o arquivo");
      system("pause");
    }
  }

  do {
    system("cls");
    printf("\n========= ESTOQUE =========== \n");
    printf("1.Cadastrar produto\n");
    printf("2.Consultar produto\n");
    printf("3.Gerar arquivo\n");
    printf("4.Excluir registro\n");
    printf("5.Sair\n");
    printf("======= Produtos Cadastrados:%d=\n", tamanho(arquivoBin));
    printf("Opcao:");
    scanf("%d", &op);
    switch (op) {
    case 1:
      cadastro(arquivoBin);
      break;
    case 2:
      consultar(arquivoBin);
      break;
    case 3:
      gera_Arq_txt(arquivoBin);
      break;
    case 4:
      excluir(arquivoBin);
    }
  } while (op != 5);
  printf("\n==Programa finalizado com sucesso.==\n");

  return 0;
}

//=======================================================================//
int tamanho(FILE *arquivo) {
  fseek(arquivo, 0, SEEK_END);
  return ftell(arquivo) / sizeof(produto);
}

void cadastro(FILE *arquivoBin) {
  produto produto;
  char confirma;
  produto.status = 'a';
  fflush(stdin);

  printf("Cadastrando produto:\n");
  printf("\nN�mero do registro: %d\n", tamanho(arquivoBin) + 1);
  produto.id = tamanho(arquivoBin) + 1;
  printf("Descri��o:.....:");
  getchar();
  gets(produto.descricao);
  printf("Pre�o de venda:");
  scanf("%f", &produto.preco_venda);
  printf("Unidade.......:");
  getchar();
  gets(produto.unidade);
  printf("Fornecedor....:");
  gets(produto.fornecedor);
  printf("Quantidade....:");
  scanf("%d", &produto.quant_est);
  produto.status = 'a';
  printf("\nConfirma cadastro? (s/n) :");
  fflush(stdin); // limpar buffer
  scanf(" %c", &confirma);
  getchar();
  if (confirma == 's' || confirma == 'S') {
    printf("\ngravando...\n\n");
    fseek(arquivoBin, 0, SEEK_END);
    fwrite(&produto, sizeof(produto), 1, arquivoBin);
    printf("\n==Produto cadastrado com sucesso==\n");
    getchar();
  }
  system("pause");
}

void consultar(FILE *arquivoBin) {
  produto produto;
  char opcao;
  printf("\n==Consulta de produtos cadastrados.==\n");
  do {
    getchar();
    printf("\nDejesa consultar por:");
    printf("\n1.C�digo");
    printf("\n2.Descri��o");
    printf("\n3.Voltar");
    printf("\nOp��o:");
    scanf("%c", &opcao);
    getchar();
    switch (opcao) {
    case '1':
      consulta_codigo(arquivoBin);
      break;
    case '2':
      consulta_descricao(arquivoBin);
      break;
    }
  } while (opcao != '3');
}

void consulta_codigo(FILE *arquivoBin) {
  produto produto;
  int id_consultar;
  printf("\nDigite o c�digo do produto: ");
  scanf("%d", &id_consultar);
  if ((id_consultar <= tamanho(arquivoBin)) && (id_consultar > 0)) {
    fseek(arquivoBin, (id_consultar - 1) * sizeof(produto), SEEK_SET);
    fread(&produto, sizeof(produto), 1, arquivoBin);
    if (produto.status == 'a') {
      printf("\nC�digo:..........: %d\n", produto.id);
      printf("\nDescri��o:.......: %s", produto.descricao);
      printf("\nPre�o de venda:..: %.2f", produto.preco_venda);
      printf("\nUnidade:.........: %s", produto.unidade);
      printf("\nFornecedor:......: %s", produto.fornecedor);
      printf("\nEstique:.........: %d", produto.quant_est);
    } else
      printf("\nProduto inativo.\n");
  } else
    printf("\nProduto n�o encontado;");
}

void consulta_descricao(FILE *arquivoBin) {
  produto produto;
  char produto_consultar[100];
  fseek(arquivoBin, 0, SEEK_SET); // Posiciona o ponteiro no in�cio do arquivo
  printf("\nDigite a descri��o do produto: ");
  fflush(stdin);
  fgets(produto_consultar, sizeof(produto_consultar), stdin);
  produto_consultar[strcspn(produto_consultar, "\n")] =
      '\0'; // Remove o caractere de nova linha
  while (fread(&produto, sizeof(produto), 1, arquivoBin) == 1) {
    if (strncmp(produto.descricao, produto_consultar,
                strlen(produto_consultar)) == 0) {
      if (produto.status == 'a') {
        printf("\nC�digo:..........: %d", produto.id);
        printf("\nDescri��o:.......: %s", produto.descricao);
        printf("\nPre�o de venda:..: %.2f", produto.preco_venda);
        printf("\nUnidade:.........: %s", produto.unidade);
        printf("\nFornecedor:......: %s", produto.fornecedor);
        printf("\nEstoque:.........: %d", produto.quant_est);
      } else {
        printf("\nProduto inativo.\n");
      }
      break; // Produto encontrado, encerra o loop
    }
  }

  if (feof(arquivoBin)) {
    // Chegou ao final do arquivo sem encontrar o produto
    printf("\nProduto n�o encontrado.\n");
  }
}

void gera_Arq_txt(FILE *arquivoBin) {
  char nome_arquivo[20];
  printf("Nome do arquivo texto:");
  getchar();
  gets(nome_arquivo);
  strcat(nome_arquivo, ".txt");
  FILE *arqtxt = fopen(nome_arquivo, "w");
  if (!arqtxt) {
    printf("Nao foi possivel criar esse arquivo!\n");
    system("pause");
  }
  fprintf(
      arqtxt,
      "Codigo |  Descri��o                         | Preco de Venda | Un |  "
      "Fornecedor       | Estoque | Status \n");
  fprintf(arqtxt, "============================================================"
                  "==================================================\n");
  int nr;
  produto produto;
  for (nr = 0; nr < tamanho(arquivoBin); nr++) {
    fseek(arquivoBin, nr * sizeof(produto), SEEK_SET);
    fread(&produto, sizeof(produto), 1, arquivoBin);
    fprintf(arqtxt, "%-8d%-40s%-12.3f%4s%19s%9d%6c\n", produto.id,
            produto.descricao, produto.preco_venda, produto.unidade,
            produto.fornecedor, produto.quant_est, produto.status);
  }
  fprintf(arqtxt, "============================================================"
                  "==================================================\n");
  fclose(arqtxt);
  printf("Arquivo criado com sucesso!\n");
  getchar();
  system("pause");
}
void excluir(FILE *arquivoBin) {
  produto produto;
  char opcao;
  printf("\n==Exclus�o de produtos cadastrados.==\n");
  do {
    getchar();
    printf("\nDejesa exluir por:");
    printf("\n1.C�digo");
    printf("\n2.Descri��o");
    printf("\n3.Voltar");
    printf("\nOp��o:");
    scanf("%c", &opcao);
    getchar();
    switch (opcao) {
    case '1':
      excluir_codigo(arquivoBin);
      break;
    case '2':
      excluir_descricao(arquivoBin);
      break;
    }
  } while (opcao != '3');
}
void excluir_codigo(FILE *arquivoBin) {
  produto produto;
  char confirma;
  int codigo_excluir;
  printf("\nInforme o codigo do registro para excluir: ");
  scanf("%d", &codigo_excluir);
  getchar();
  if ((codigo_excluir <= tamanho(arquivoBin)) && (codigo_excluir > 0)) {
    fseek(arquivoBin, (codigo_excluir - 1) * sizeof(produto), SEEK_SET);
    fread(&produto, sizeof(produto), 1, arquivoBin);
    if (produto.status == 'a') {
      printf("\nDescri��o:.......:%s", produto.descricao);
      printf("\nPre�o de venda:..:%.2f", produto.preco_venda);
      printf("\nUnidade:.........:%s", produto.unidade);
      printf("\nFornecedor:......:%s", produto.fornecedor);
      printf("\nEstoque:.........:%d", produto.quant_est);
    }
    printf("\nConfirma a exclusao: <s/n>\n");
    scanf("%c", &confirma);

    if (toupper(confirma) == 'S') {
      printf("\n==excluindo...==\n\n");
      fseek(arquivoBin, (codigo_excluir - 1) * sizeof(produto), SEEK_SET);
      produto.status = 'i';
      fwrite(&produto, sizeof(produto), 1, arquivoBin);
      printf("\n===Produto excluido com sucesso===\n");
    } else
      printf("Registro j� inativo! \n");
  } else
    printf("\nNumero de registro invalido!\n");
  
}

void excluir_descricao(FILE *arquivoBin) {
  produto produto;
  char confirma;
  char descricao_excluir[100];

  printf("\nInforme a descricao do registro para excluir: ");
  fgets(descricao_excluir, sizeof(descricao_excluir), stdin);
  descricao_excluir[strcspn(descricao_excluir, "\n")] =
      '\0';                       // Remove o caractere de nova linha
  fseek(arquivoBin, 0, SEEK_SET); // Posiciona o ponteiro no in�cio do arquivo

  while (fread(&produto, sizeof(produto), 1, arquivoBin) == 1) {
    if (strncmp(produto.descricao, descricao_excluir,
                strlen(descricao_excluir)) ==0) 
                { // compara string digitada com
      if (produto.status == 'a') {
        printf("\nDescri��o:.......:%s", produto.descricao);
        printf("\nPre�o de venda:..:%f", produto.preco_venda);
        printf("\nUnidade:.........:%s", produto.unidade);
        printf("\nFornecedor:......:%s", produto.fornecedor);
        printf("\nEstoque:.........:%d", produto.quant_est);

        printf("\nConfirma a exclus�o: <s/n>\n");
        scanf(" %c", &confirma);
        if (toupper(confirma) == 'S') {
          printf("\n== Excluindo... ==\n\n");
          fseek(arquivoBin, -sizeof(produto), SEEK_CUR);
          produto.status = 'i';
          fwrite(&produto, sizeof(produto), 1, arquivoBin);
          printf("\n=== Produto exclu�do com sucesso ===\n");
        } else {
          printf("\nProduto n�o exclu�do.\n");
        }
      } else {
        printf("\nProduto inativo.\n");
      }
      break; // Produto encontrado, encerra o loop
    }
    if (feof(arquivoBin)) {
      // Chegou ao final do arquivo sem encontrar o produto
      printf("\nProduto n�o encontrado.\n");
    }
  }
}
