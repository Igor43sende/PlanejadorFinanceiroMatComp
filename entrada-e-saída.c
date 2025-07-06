#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==============================
   Estruturas para armazenar dados
   ============================== */

/* Estrutura para os dados da meta financeira */
typedef struct {
    int idadeFormatura;
    int idadeFimCap;
    float salarioMedioLiq;
    float percInvestimento;
    float patrimonioAcc;
    int idadeFimRetirada;
} MetaFinanceira;

/* Estrutura para armazenar dados de cada investimento */
typedef struct {
    char nome[100];        // Nome do ativo
    char categoria[50];    // Categoria do ativo
    float taxa;            // Taxa mensal esperada
    int risco;             // Nível de risco (1 a 5)
} Investimento;

/* ==============================
   Variáveis Globais
   ============================== */
MetaFinanceira meta;                  // Meta financeira do usuário
Investimento investimentosCap[50];    // Lista de investimentos para capitalização
Investimento investimentosDescap[10]; // Lista de investimentos para descapitalização
int numInvestimentosCap = 0;          // Quantidade de investimentos para capitalização
int numInvestimentosDescap = 0;       // Quantidade de investimentos para descapitalização

void lerArquivoMeta(char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro: não foi possível abrir o arquivo de metas '%s'\n", nomeArquivo);
        exit(1);
    }

    char controle, parametro[50];
    float valor;

    printf("Lendo arquivo de metas: %s\n", nomeArquivo);
    while (fscanf(arquivo, "%c", &controle) != EOF) {
        switch (controle) {
            case '#': // Comentário, ignora linha
                while (fgetc(arquivo) != '\n');
                break;
            case 'c': // Parâmetro de capitalização
                fscanf(arquivo, "%s %f", parametro, &valor);
                if (strcmp(parametro, "IDADEFORMATURA") == 0)
                    meta.idadeFormatura = (int)valor;
                else if (strcmp(parametro, "IDADEFIMCAP") == 0)
                    meta.idadeFimCap = (int)valor;
                else if (strcmp(parametro, "SALARIOMEDIOLIQ") == 0)
                    meta.salarioMedioLiq = valor;
                else if (strcmp(parametro, "PERCINVESTIMENTO") == 0)
                    meta.percInvestimento = valor;
                else if (strcmp(parametro, "PATRIMONIOACC") == 0)
                    meta.patrimonioAcc = valor;
                break;
            case 'd': // Parâmetro de descapitalização
                fscanf(arquivo, "%s %f", parametro, &valor);
                if (strcmp(parametro, "IDADEFIMRETIRADA") == 0)
                    meta.idadeFimRetirada = (int)valor;
                break;
            case '\n':
            case '\r': // Ignora linhas em branco
                break;
            default:
                printf("Aviso: marcador desconhecido '%c' no arquivo de metas\n", controle);
                while (fgetc(arquivo) != '\n');
                break;
        }
    }
    fclose(arquivo);
}

void lerArquivoInvestimentos(char *nomeArquivo, Investimento *lista, int *numInvestimentos) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro: não foi possível abrir o arquivo de investimentos '%s'\n", nomeArquivo);
        exit(1);
    }

    char controle;

    printf("Lendo arquivo de investimentos: %s\n", nomeArquivo);
    while (fscanf(arquivo, "%c", &controle) != EOF) {
        switch (controle) {
            case '#': // Comentário
                while (fgetc(arquivo) != '\n');
                break;
            case 'i': // Investimento
                fscanf(arquivo, "%s %s %f %d", 
                       lista[*numInvestimentos].nome,
                       lista[*numInvestimentos].categoria,
                       &lista[*numInvestimentos].taxa,
                       &lista[*numInvestimentos].risco);
                (*numInvestimentos)++;
                break;
            case 'f': // Fim do arquivo
            case 'F':
                // Nada a fazer aqui
                break;
            case '\n':
            case '\r':
                break;
            default:
                printf("Aviso: marcador desconhecido '%c' no arquivo de investimentos\n", controle);
                while (fgetc(arquivo) != '\n');
                break;
        }
    }
    fclose(arquivo);
}

void gerarRelatorio(char *nomeArquivoSaida) {
    FILE *saida = fopen(nomeArquivoSaida, "w");
    if (!saida) {
        printf("Erro: não foi possível criar o arquivo de saída '%s'\n", nomeArquivoSaida);
        exit(1);
    }

    fprintf(saida, "ETAPA: CAPITALIZACAO\n");
    fprintf(saida, "Simulação de capitalização para meta de R$ %.2f\n\n", meta.patrimonioAcc);
    fprintf(saida, "Periodo  Pagamento  Capitalizado  Juros\n");
    fprintf(saida, "=======  =========  ============ =====\n");
    for (int i = 1; i <= 12; i++) { // Apenas exemplo com 12 períodos
        fprintf(saida, "%7d  %9.2f  %12.2f  %8.2f\n", i, 1000.0, 1000.0 + i * 50.0, i * 50.0);
    }

    fprintf(saida, "\nETAPA: DESCAPITALIZACAO\n");
    fprintf(saida, "Simulação de retiradas mensais do patrimônio acumulado\n\n");
    fprintf(saida, "No.  Aplicacao           Taxa     Retirada\n");
    fprintf(saida, "===  ==================  =======  ==========\n");
    for (int i = 0; i < numInvestimentosDescap; i++) {
        fprintf(saida, "%3d  %-18s  %7.3f  %10.2f\n", 
                i + 1, investimentosDescap[i].nome, 
                investimentosDescap[i].taxa, 2000.0 + i * 100.0);
    }

    fclose(saida);
    printf("Relatório gerado com sucesso em '%s'\n", nomeArquivoSaida);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <arquivo-meta> <arquivo-capitalizacao> <arquivo-descapitalizacao> <saida-relatorio>\n", argv[0]);
        return 1;
    }
    // Lê arquivos de entrada
    lerArquivoMeta(argv[1]);
    lerArquivoInvestimentos(argv[2], investimentosCap, &numInvestimentosCap);
    lerArquivoInvestimentos(argv[3], investimentosDescap, &numInvestimentosDescap);
    // Gera o relatório de saída
    gerarRelatorio(argv[4]);

    return 0;
}
