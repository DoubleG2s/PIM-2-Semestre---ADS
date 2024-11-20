#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int id;
    char nome[30];
    float preco;
    int quantidade;
    int grupo_id; // 0: Frutas, 1: Legumes, 2: Verduras
} Produto;

void cadastrarProduto(Produto **produtos, int *n);
void listarProdutos(Produto *produtos, int n);
void venderProduto(Produto *produtos, int n);
void editarProduto(Produto *produtos, int n);
void salvarDados(Produto *produtos, int n, const char *arquivo);
void carregarDados(Produto **produtos, int *n, const char *arquivo);
int produtoJaExiste(Produto *produtos, int n, char nome[]);
const char *obterNomeGrupo(int grupo_id);

int main() {
    Produto *produtos = NULL;
    int n = 0;
    int opcao;
    const char *arquivo = "hortifruti.txt";

    carregarDados(&produtos, &n, arquivo);

    do {
        printf("\nSistema de Gerenciamento de Hortifruti\n");
        printf("1. Cadastrar produto\n");
        printf("2. Listar produtos\n");
        printf("3. Vender produto\n");
        printf("4. Editar produto\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                cadastrarProduto(&produtos, &n);
                salvarDados(produtos, n, arquivo);
                break;
            case 2:
                listarProdutos(produtos, n);
                break;
            case 3:
                venderProduto(produtos, n);
                salvarDados(produtos, n, arquivo);
                break;
            case 4:
                editarProduto(produtos, n);
                salvarDados(produtos, n, arquivo);
                break;
            case 5:
                printf("Saindo do sistema.\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while(opcao != 5);

    free(produtos);
    return 0;
}

int produtoJaExiste(Produto *produtos, int n, char nome[]) {
    for (int i = 0; i < n; i++) {
        if (strcasecmp(produtos[i].nome, nome) == 0) {
            return 1;
        }
    }
    return 0;
}

const char *obterNomeGrupo(int grupo_id) {
    switch(grupo_id) {
        case 0: return "Frutas";
        case 1: return "Legumes";
        case 2: return "Verduras";
        default: return "Desconhecido";
    }
}

void cadastrarProduto(Produto **produtos, int *n) {
    char nome[30];
    float preco;
    int quantidade, grupo_id;

    printf("Digite o nome do produto: ");
    getchar();
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    if (produtoJaExiste(*produtos, *n, nome)) {
        printf("Erro: Produto '%s' já existe. Use a opção de editar para modificar.\n", nome);
        return;
    }

    printf("Digite o preço do produto: ");
    if (scanf("%f", &preco) != 1 || preco <= 0) {
        printf("Erro: O preço deve ser um número positivo.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Digite a quantidade em estoque: ");
    if (scanf("%d", &quantidade) != 1 || quantidade <= 0) {
        printf("Erro: A quantidade deve ser um número inteiro positivo.\n");
        while (getchar() != '\n');
        return;
    }

    printf("Digite o ID do grupo do produto (0: Frutas, 1: Legumes, 2: Verduras): ");
    if (scanf("%d", &grupo_id) != 1 || grupo_id < 0 || grupo_id > 2) {
        printf("Erro: ID de grupo inválido.\n");
        while (getchar() != '\n');
        return;
    }

    *produtos = realloc(*produtos, (*n + 1) * sizeof(Produto));
    if (*produtos == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    (*produtos)[*n].id = (*n == 0) ? 1 : (*produtos)[*n - 1].id + 1;
    strcpy((*produtos)[*n].nome, nome);
    (*produtos)[*n].preco = preco;
    (*produtos)[*n].quantidade = quantidade;
    (*produtos)[*n].grupo_id = grupo_id;
    (*n)++;

    printf("Produto cadastrado com sucesso!\n");
}

void listarProdutos(Produto *produtos, int n) {
    if (n == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("\nID  | Nome                | Preço    | Quantidade | Grupo\n");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%-3d | %-20s | R$%-7.2f | %-10d | %-10s\n", 
               produtos[i].id, produtos[i].nome, produtos[i].preco, produtos[i].quantidade, obterNomeGrupo(produtos[i].grupo_id));
    }
}

void venderProduto(Produto *produtos, int n) {
    int id, quantidade;
    char maisProdutos;
    float totalVenda = 0.0;
    float valorPago, troco;

    if (n == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    do {
        listarProdutos(produtos, n);
        printf("Digite o ID do produto que deseja vender: ");
        if (scanf("%d", &id) != 1 || id <= 0) {
            printf("Erro: ID inválido.\n");
            while (getchar() != '\n');
            return;
        }

        printf("Digite a quantidade a vender: ");
        if (scanf("%d", &quantidade) != 1 || quantidade <= 0) {
            printf("Erro: A quantidade deve ser um número inteiro positivo.\n");
            while (getchar() != '\n');
            return;
        }

        int produtoEncontrado = 0;
        for (int i = 0; i < n; i++) {
            if (produtos[i].id == id) {
                produtoEncontrado = 1;
                if (produtos[i].quantidade >= quantidade) {
                    float valorProduto = produtos[i].preco * quantidade;
                    totalVenda += valorProduto;
                    produtos[i].quantidade -= quantidade;
                    printf("Produto %s adicionado à venda. Valor: R$%.2f\n", produtos[i].nome, valorProduto);
                } else {
                    printf("Quantidade insuficiente no estoque.\n");
                    return;
                }
                break;
            }
        }

        if (!produtoEncontrado) {
            printf("Produto com ID %d não encontrado.\n", id);
            return;
        }

        do {
            printf("Deseja adicionar mais produtos à venda? (s/n): ");
            getchar();
            scanf("%c", &maisProdutos);
            maisProdutos = tolower(maisProdutos);
            if (maisProdutos != 's' && maisProdutos != 'n') {
                printf("Entrada inválida. Por favor, insira 's' para sim ou 'n' para não.\n");
            }
        } while (maisProdutos != 's' && maisProdutos != 'n');

    } while (maisProdutos == 's');

    printf("Total da venda: R$%.2f\n", totalVenda);
    printf("Digite o valor pago pelo cliente: ");
    if (scanf("%f", &valorPago) != 1 || valorPago < totalVenda) {
        printf("Erro: Valor pago deve ser um número positivo e maior ou igual ao total da venda.\n");
        while (getchar() != '\n');
        return;
    }

    troco = valorPago - totalVenda;
    printf("Troco a ser devolvido: R$%.2f\n", troco);
    printf("Venda finalizada com sucesso!\n");
}

void editarProduto(Produto *produtos, int n) {
    int id, nova_quantidade;

    if (n == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    listarProdutos(produtos, n);
    printf("Digite o ID do produto que deseja editar: ");
    if (scanf("%d", &id) != 1 || id <= 0) {
        printf("Erro: ID inválido.\n");
        while (getchar() != '\n');
        return;
    }

    for (int i = 0; i < n; i++) {
        if (produtos[i].id == id) {
            printf("Quantidade atual: %d\n", produtos[i].quantidade);
            printf("Digite a nova quantidade em estoque: ");
            if (scanf("%d", &nova_quantidade) != 1 || nova_quantidade < 0) {
                printf("Erro: A quantidade deve ser um número inteiro não negativo.\n");
                while (getchar() != '\n');
                return;
            }

            produtos[i].quantidade = nova_quantidade;
            printf("Quantidade atualizada com sucesso!\n");
            return;
        }
    }

    printf("Produto com ID %d não encontrado.\n", id);
}

void salvarDados(Produto *produtos, int n, const char *arquivo) {
    FILE *fp = fopen(arquivo, "w");
    if (fp == NULL) {
        printf("Erro ao salvar os dados.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d|%s|%.2f|%d|%d\n",
                produtos[i].id, produtos[i].nome, produtos[i].preco, produtos[i].quantidade, produtos[i].grupo_id);
    }

    fclose(fp);
    printf("Dados salvos com sucesso!\n");
}

void carregarDados(Produto **produtos, int *n, const char *arquivo) {
    FILE *fp = fopen(arquivo, "r");
    if (fp == NULL) {
        printf("Arquivo não encontrado. Um novo será criado ao salvar dados.\n");
        return;
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), fp)) {
        Produto p;
        sscanf(linha, "%d|%[^|]|%f|%d|%d", &p.id, p.nome, &p.preco, &p.quantidade, &p.grupo_id);
        *produtos = realloc(*produtos, (*n + 1) * sizeof(Produto));
        if (*produtos == NULL) {
            printf("Erro ao alocar memória.\n");
            exit(1);
        }
        (*produtos)[*n] = p;
        (*n)++;
    }

    fclose(fp);
}
