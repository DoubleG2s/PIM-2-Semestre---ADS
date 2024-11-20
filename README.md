# PIM-2-Semestre---ADS

# Sistema de Gerenciamento de Hortifruti

Este é um programa em C que implementa um sistema de gerenciamento de hortifruti. Ele permite cadastrar, listar, vender e editar produtos, além de salvar e carregar os dados dos produtos de um arquivo.

---

## Funcionalidades

- **Cadastrar Produto**: Adiciona novos produtos ao sistema, verificando se o produto já existe.
- **Listar Produtos**: Exibe uma lista de todos os produtos cadastrados, com detalhes como ID, nome, preço, quantidade em estoque e grupo (Frutas, Verduras, Legumes).
- **Vender Produto**: Realiza vendas de produtos, atualiza o estoque e calcula o troco.
- **Editar Produto**: Permite alterar a quantidade em estoque de um produto existente.
- **Persistência de Dados**: Os dados dos produtos são armazenados em um arquivo (`hortifruti.txt`) para garantir que persistam entre execuções.

---

## Estrutura do Projeto

O programa utiliza uma estrutura para armazenar os dados dos produtos:

```c
typedef struct {
    int id;
    char nome[30];
    float preco;
    int quantidade;
    char grupo[15];
} Produto;
