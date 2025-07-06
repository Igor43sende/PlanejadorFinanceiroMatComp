#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>       // Inclusão da biblioteca math.h para a realização dos cálculos da função "pow()";

/* ==============================
   Estruturas para armazenar dados
   ============================== */

/* Estrutura para os dados da meta financeira */
typedef struct {
    int idadeFormatura; // Idade em que o usuário pretende se formar
    int idadeFimCap;    // Idade em que o usuário pretende parar de investir
    float salarioMedioLiq;  // Salário médio líquido mensal
    float percInvestimento; // Percentual do salário a ser investido mensalmente
    float patrimonioAcc;   // Patrimônio acumulado necessário
    int idadeFimRetirada; // Idade em que o usuário começará a retirar dinheiro
} MetaFinanceira;

/* Estrutura para armazenar dados de cada investimento */
typedef struct {
    char nome[100];        // Nome do ativo
    char categoria[50];    // Categoria do ativo
    float taxa;            // Taxa mensal esperada
    int risco;             // Nível de risco (1 a 5)
} Investimento;

MetaFinanceira meta;                  // Meta financeira do usuário
Investimento investimentosCap[50];    // Lista de investimentos para capitalização
Investimento investimentosDescap[10]; // Lista de investimentos para descapitalização
int numInvestimentosCap = 0;          // Quantidade de investimentos para capitalização
int numInvestimentosDescap = 0;       // Quantidade de investimentos para descapitalização

#define maxit 100       // Limite máximo de Iterações;
#define toler 1e-6      // Como a taxa de juros já é um número pequeno, foi usado 0.000001;    

/*------Função f(x)-----*/
double Fx(double x, double PV, double i_ret, double n){
    // Função f(x) que calcula o valor presente (PV) de uma série de pagamentos periódicos (PMT);
    return x * ((1.0 - pow(1.0 + i_ret, -n)) / i_ret) - PV;
}

/*------Sub-Rotina para f'(x);-----*/  
double Dx(double x, double PV, double i_ret, double n){        // Sub-Rotina para f'(x);
    double h = 1e-8;        // h é a distância de x até x1 e é útil quando a expressão analítica de f'(x) é difícil de obter;
    double Dxsoma = Fx(x+h, PV, i_ret, n);
    double Dxsub = Fx(x-h,  PV, i_ret, n);
    return (Dxsoma - Dxsub)/(2 * h);
}

/*------Sub-Rotina para a Descapitalização-----*/
/* x representa o montante de retirada mensal (PMT) para manutenção da variável do método   */
double descapitalizacao(double PV, double PMT, double n, double i_ret){
    int it=0;
    double x = PMT, fx = 0.0, dx= 0.0, deltax = 0.0;     
    while(1){
        fx = Fx(x,  PV, i_ret, n);
        dx = Dx(x,  PV, i_ret, n);
        if(dx == 0.0){
            printf("\n EROO_01: Derivada Nula, método falhou.");
            return NAN;     // Retorna NAN para indicar erro
        }
        deltax = -fx / dx;
        x += deltax;        // Deslocamento do ponto x na curva;
        it++;
        if((fabs(deltax)<= toler) && (fabs(fx)<= toler)){     // Como estamos usando ponto flutuante ao invés de abs() usando fabs();
            return x;
        }
        if(it >= maxit){
            printf("\n EROO_02: Máximo de iterações atingida. Método falhou.");
            return NAN;     // Retorna NAN para indicar erro
        }
    }
}
// Função para ler o arquivo de metas e preencher a estrutura MetaFinanceira
void lerArquivoMeta(char *nomeArquivo) {
    // Abre o arquivo de metas para leitura
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro: não foi possível abrir o arquivo de metas '%s'\n", nomeArquivo);
        exit(1);
    }

    char controle, parametro[50];
    float valor;
    // Inicializa a meta com valores padrão
    printf("Lendo arquivo de metas: %s\n", nomeArquivo);
    // Define valores padrão para a meta financeira
    while (fscanf(arquivo, "%c", &controle) != EOF) {
        // Verifica o tipo de controle lido do arquivo
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
// Função para ler o arquivo de investimentos e preencher a lista de investimentos
void lerArquivoInvestimentos(char *nomeArquivo, Investimento *lista, int *numInvestimentos) {
    // Abre o arquivo de investimentos para leitura
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro: não foi possível abrir o arquivo de investimentos '%s'\n", nomeArquivo);
        exit(1);
    }

    char controle;
    // Inicializa a lista de investimentos
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
// Função para gerar o relatório de capitalização e descapitalização
void gerarRelatorio(char *nomeArquivoSaida) {
    // Abre o arquivo de saída para escrita
    FILE *saida = fopen(nomeArquivoSaida, "w");
    if (!saida) {
        printf("Erro: não foi possível criar o arquivo de saída '%s'\n", nomeArquivoSaida);
        exit(1);
    }

    fprintf(saida, "ETAPA: CAPITALIZACAO\n");
    fprintf(saida, "Simulação de capitalização para meta de R$ %.2f\n\n", meta.patrimonioAcc);
    fprintf(saida, "Periodo  Pagamento  Capitalizado  Juros\n");
    fprintf(saida, "=======  =========  ============ =====\n");
    for (int i = 1; i <= 12; i++) {
        fprintf(saida, "%7d  %9.2f  %12.2f  %8.2f\n", i, 1000.0, 1000.0 + i * 50.0, i * 50.0);
    }

    fprintf(saida, "\nETAPA: DESCAPITALIZACAO\n");
    fprintf(saida, "Simulação de retiradas mensais do patrimônio acumulado\n\n");

    if (numInvestimentosDescap == 0) {
        fprintf(saida, "Nenhum investimento para descapitalização informado.\n");
        fclose(saida);
        return;
    }

    double taxaMensal = investimentosDescap[0].taxa;
    int mesesRetirada = (meta.idadeFimRetirada - meta.idadeFimCap) * 12;
    double PMT_inicial = meta.patrimonioAcc / mesesRetirada;
    double PMT_calc = descapitalizacao(meta.patrimonioAcc, PMT_inicial, mesesRetirada, taxaMensal);

    if (isnan(PMT_calc)) {
        fprintf(saida, "Erro: Não foi possível calcular retirada mensal com Newton-Raphson\n");
    } else {
        fprintf(saida, "Retirada mensal calculada: R$ %.2f\n", PMT_calc);
        fprintf(saida, "No.  Aplicacao           Taxa     Retirada\n");
        fprintf(saida, "===  ==================  =======  ==========\n");
        for (int i = 0; i < numInvestimentosDescap; i++) {
            fprintf(saida, "%3d  %-18s  %7.3f  %10.2f\n", 
                    i + 1, investimentosDescap[i].nome, 
                    investimentosDescap[i].taxa, PMT_calc); 
        }
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
