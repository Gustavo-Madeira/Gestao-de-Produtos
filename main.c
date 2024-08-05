#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int codigo;
    char numeroSerie[50];
    char validade[11]; // Formato: dd/mm/aaaa
} Produto;

typedef struct No {
    Produto produto;
    struct No* esquerda;
    struct No* direita;
} No;

No* criarNo(Produto produto) {
    No* novo = (No*)malloc(sizeof(No));
    novo->produto = produto;
    novo->esquerda = novo->direita = NULL;
    return novo;
}

No* inserirNo(No* raiz, Produto produto) {
    if (raiz == NULL) {
        return criarNo(produto);
    }
    if (strcmp(produto.validade, raiz->produto.validade) < 0) {
        raiz->esquerda = inserirNo(raiz->esquerda, produto);
    } else {
        raiz->direita = inserirNo(raiz->direita, produto);
    }
    return raiz;
}

No* buscarNo(No* raiz, int codigo) {
    if (raiz == NULL || raiz->produto.codigo == codigo) {
        return raiz;
    }
    if (codigo < raiz->produto.codigo) {
        return buscarNo(raiz->esquerda, codigo);
    } else {
        return buscarNo(raiz->direita, codigo);
    }
}

No* excluirNo(No* raiz, int codigo) {
    if (raiz == NULL) {
        return raiz;
    }
    if (codigo < raiz->produto.codigo) {
        raiz->esquerda = excluirNo(raiz->esquerda, codigo);
    } else if (codigo > raiz->produto.codigo) {
        raiz->direita = excluirNo(raiz->direita, codigo);
    } else {
        if (raiz->esquerda == NULL) {
            No* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            No* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }
        No* temp = raiz->direita;
        while (temp && temp->esquerda != NULL) {
            temp = temp->esquerda;
        }
        raiz->produto = temp->produto;
        raiz->direita = excluirNo(raiz->direita, temp->produto.codigo);
    }
    return raiz;
}

int dataVencida(char* dataAtual, char* dataValidade) {
    int diaAtual, mesAtual, anoAtual;
    int diaVal, mesVal, anoVal;

    sscanf(dataAtual, "%d/%d/%d", &diaAtual, &mesAtual, &anoAtual);
    sscanf(dataValidade, "%d/%d/%d", &diaVal, &mesVal, &anoVal);

    if (anoVal < anoAtual) {
        return 1;
    } else if (anoVal == anoAtual) {
        if (mesVal < mesAtual) {
            return 1;
        } else if (mesVal == mesAtual) {
            if (diaVal < diaAtual) {
                return 1;
            }
        }
    }
    return 0;
}

No* excluirProdutosVencidos(No* raiz, char* dataAtual) {
    if (raiz == NULL) {
        return NULL;
    }

    raiz->esquerda = excluirProdutosVencidos(raiz->esquerda, dataAtual);
    raiz->direita = excluirProdutosVencidos(raiz->direita, dataAtual);

    if (dataVencida(dataAtual, raiz->produto.validade)) {
        No* temp;
        if (raiz->esquerda == NULL) {
            temp = raiz->direita;
        } else if (raiz->direita == NULL) {
            temp = raiz->esquerda;
        } else {
            No* current = raiz->direita;
            No* parent = NULL;
            while (current->esquerda != NULL) {
                parent = current;
                current = current->esquerda;
            }
            if (parent != NULL) {
                parent->esquerda = current->direita;
                current->direita = raiz->direita;
            }
            current->esquerda = raiz->esquerda;
            temp = current;
        }
        printf("Produto vencido excluido:\n");
        printf("Codigo: %d, Numero de Serie: %s, Validade: %s\n", raiz->produto.codigo, raiz->produto.numeroSerie, raiz->produto.validade);
        free(raiz);
        return temp;
    }

    return raiz;
}

void alterarProduto(No* raiz, int codigo) {
    No* produto = buscarNo(raiz, codigo);
    if (produto != NULL) {
        printf("Digite a nova validade do produto (dd/mm/aaaa): ");
        scanf(" %10[^\n]", produto->produto.validade);
    } else {
        printf("Produto nao encontrado.\n");
    }
}

void listarProdutos(No* raiz) {
    if (raiz != NULL) {
        listarProdutos(raiz->esquerda);
        printf("Codigo: %d, Numero de Serie: %s, Validade: %s\n", raiz->produto.codigo, raiz->produto.numeroSerie, raiz->produto.validade);
        listarProdutos(raiz->direita);
    }
}

int main() {
    No* raiz = NULL;
    int opcao, codigo;
    Produto produto;
    char dataAtual[11];

    printf("Digite a data atual (dd/mm/aaaa): ");
    scanf(" %10[^\n]", dataAtual);

    do {
        printf("\nMenu:\n");
        printf("1 - Incluir\n");
        printf("2 - Alterar\n");
        printf("3 - Excluir Produtos Vencidos\n");
        printf("4 - Consultar\n");
        printf("5 - Listar\n");
        printf("9 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o codigo do produto: ");
                scanf("%d", &produto.codigo);
                printf("Digite o numero de serie do produto: ");
                scanf(" %49[^\n]", produto.numeroSerie);
                printf("Digite a validade do produto (dd/mm/aaaa): ");
                scanf(" %10[^\n]", produto.validade);
                raiz = inserirNo(raiz, produto);
                break;
            case 2:
                printf("Digite o codigo do produto a ser alterado: ");
                scanf("%d", &codigo);
                alterarProduto(raiz, codigo);
                break;
            case 3:
                raiz = excluirProdutosVencidos(raiz, dataAtual);
                break;
            case 4:
                printf("Digite o codigo do produto a ser consultado: ");
                scanf("%d", &codigo);
                No* produtoEncontrado = buscarNo(raiz, codigo);
                if (produtoEncontrado != NULL) {
                    printf("Codigo: %d, Numero de Serie: %s, Validade: %s\n", produtoEncontrado->produto.codigo, produtoEncontrado->produto.numeroSerie, produtoEncontrado->produto.validade);
                } else {
                    printf("Produto nao encontrado.\n");
                }
                break;
            case 5:
                listarProdutos(raiz);
                break;
            case 9:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 9);

    return 0;
}