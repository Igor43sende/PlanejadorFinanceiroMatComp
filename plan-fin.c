#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // Para cálculos financeiros
#define maxit 100       // Limite máximo de Iterações;
#define toler 1e-6      // Como a taxa de juros já é um número pequeno, foi usado 0.000001;    

/* ==============================
   Estruturas para armazenar dados
   ============================== */

/* Estrutura para os dados da meta financeira */
typedef struct {
    int idadeFormatura;     // Idade em que o usuário pretende se formar
    int idadeFimCap;        // Idade em que o usuário pretende parar de investir
    float salarioMedioLiq;  // Salário médio líquido mensal
    float percInvestimento; // Percentual do salário a ser investido mensalmente
    float patrimonioAcc;    // Patrimônio acumulado necessário
    int idadeFimRetirada; // Idade em que o usuário começará a retirar dinheir
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
/* ==============================
   Funções para descapitalização
   ============================== */
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
double descapitalizacao_newton(double PV, double PMT, double n, double i_ret){
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
        // Visualização das variáveis a cada iteração;
        printf("\n------------------------------------------------------------------------------------------------");
        printf("\n Iteração: %d | x: %.8f | f(x): %.8f | f'(x): %.8f | Detalx: %.8f\n",it,x,fx,dx,deltax);
        printf("-------------------------------------------------------------------------------------------------");
        x += deltax;        // Deslocamento do ponto x na curva;
        it++;
        if((fabs(deltax)<= toler) && (fabs(fx)<= toler)){     // Como estamos usando ponto flutuante ao invés de abs() usando fabs();
            printf("\n Convergência Atingida!");
            return x;
        }
        if(it >= maxit){
            printf("\n EROO_02: Máximo de iterações atingida. Método falhou.");
            return NAN;     // Retorna NAN para indicar erro
        }
    }
}
/* ==============================
   Funções para capitalização
   ============================== */
/* ----- Função f(i) de capitalização ----- */
double f_cap(double i, double PMT, int n, double VF) {
    if (i == 0.0) { // Evita divisão por zero
        return PMT * n - VF;
    }
    return PMT * (pow(1 + i, n) - 1) / i - VF;
}
/* ----- Função derivada de f(i) ----- */
double derivada_f_cap(double i, double PMT, int n, double VF) {
    const double h = 1e-6;
    return (f_cap(i + h, PMT, n, VF) - f_cap(i - h, PMT, n, VF)) / (2 * h);
}
/* ----- Método de Newton-Raphson para calcular taxa mínima ----- */
double capitalizacao_newton(double PMT, int n, double VF) {
    const int MAX_ITER = 100;
    const double TOL = 1e-6;
    double i = 0.005; // Chute inicial: 0.5% ao mês

    for (int iter = 0; iter < MAX_ITER; iter++) {
        double f = f_cap(i, PMT, n, VF);
        double fp = derivada_f_cap(i, PMT, n, VF);

        if (fabs(fp) < 1e-12) {
            break; // Evita divisão por zero
        }
        double i_novo = i - f / fp;

        if (fabs(i_novo - i) < TOL) {
            return i_novo;
        }

        i = i_novo;
    }

    return -1.0; // Retorna taxa negativa para indicar erro
}
/* ----- Função para calcular crescimento do montante ao longo dos meses ----- */
double* calcularCrescimentoMontante(double PMT, int n, double taxa, double metaValor, int *mesMetaAtingida) {
    double* montantes = (double*)malloc((n + 1) * sizeof(double));
    if (montantes == NULL) {
        printf("Erro ao alocar memória para vetor de montantes\n");
        exit(1);
    }

    double montanteAcumulado = 0.0;
    *mesMetaAtingida = -1; // Inicializa como "não atingida"

    for (int i = 0; i <= n; i++) {
        montantes[i] = montanteAcumulado;
        
        if (*mesMetaAtingida == -1 && montanteAcumulado >= metaValor) {
            *mesMetaAtingida = i; // Salva o mês em que a meta foi atingida
        }
        
        montanteAcumulado *= (1 + taxa);
        montanteAcumulado += PMT;
    }

    return montantes;
}

/* ----- Função para selecionar o melhor investimento ----- */
Investimento selecionarMelhorInvestimento(double taxaCalculada, int *viavel) {
    Investimento melhorInvestimento;
    melhorInvestimento.risco = 6; // Valor maior que o risco máximo permitido
    *viavel = 0; // Assume inviável até encontrar uma aplicação válida

    for (int i = 0; i < numInvestimentosCap; i++) {
        if (investimentosCap[i].taxa >= taxaCalculada) {
            if (investimentosCap[i].risco < melhorInvestimento.risco) {
                melhorInvestimento = investimentosCap[i];
                *viavel = 1;
            }
        }
    }

    return melhorInvestimento;
}
/* ----- Função para ler arquivo de metas ----- */
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
            case '#': while (fgetc(arquivo) != '\n'); break;
            case 'c':
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
            case '\n': case '\r': break;
            default:
                printf("Aviso: marcador desconhecido '%c' no arquivo de metas\n", controle);
                while (fgetc(arquivo) != '\n');
                break;
        }
    }

    // Print para debug da meta carregada:
    printf("\n=== Meta carregada ===\n");
    printf("  IDADEFORMATURA     = %d\n", meta.idadeFormatura);
    printf("  IDADEFIMCAP        = %d\n", meta.idadeFimCap);
    printf("  SALARIOMEDIOLIQ    = %.2f\n", meta.salarioMedioLiq);
    printf("  PERCINVESTIMENTO   = %.2f\n", meta.percInvestimento);
    printf("  PATRIMONIOACC      = %.2f\n", meta.patrimonioAcc);
    printf("  IDADEFIMRETIRADA   = %d\n", meta.idadeFimRetirada);
    
    printf("======================\n\n");

    fclose(arquivo);
}

/* ----- Função para ler arquivo de investimentos ----- */
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
            case '#': while (fgetc(arquivo) != '\n'); break;
            case 'i':
                fscanf(arquivo, "%s %s %f %d", 
                       lista[*numInvestimentos].nome,
                       lista[*numInvestimentos].categoria,
                       &lista[*numInvestimentos].taxa,
                       &lista[*numInvestimentos].risco);
                (*numInvestimentos)++;
                break;
            case 'f': case 'F': break;
            case '\n': case '\r': break;
            default:
                printf("Aviso: marcador desconhecido '%c' no arquivo de investimentos\n", controle);
                while (fgetc(arquivo) != '\n');
                break;
        }
    }
    fclose(arquivo);
}

/* ----- Função para gerar o relatório estilo PDF ----- */
void gerarRelatorioCap(FILE *saida, double *montantes, int n, double PMT,double taxa, char *nomeAtivo, char *categoria,int viavel, double taxaCalculada, int mesMetaAtingida) {

    if (!saida) {
        printf("Erro: arquivo de saída inválido na geração do relatório de capitalização.\n");
        exit(1);
    }

    fprintf(saida, "============================================================\n");
    fprintf(saida, "RELATÓRIO DE CAPITALIZAÇÃO\n");
    fprintf(saida, "============================================================\n\n");

    if (!viavel) {
        fprintf(saida, "Não existe aplicação de capitalização capaz de obter o valor de R$ %.2f com a taxa %.5f.\n",
                meta.patrimonioAcc, taxaCalculada);
        fclose(saida);
        printf("Planejamento inviável: nenhuma aplicação atende à taxa mínima.\n");
        return;
    }

    if (mesMetaAtingida >= 0) {
        int anos = mesMetaAtingida / 12;
        int meses = mesMetaAtingida % 12;
        fprintf(saida, "A meta de R$ %.2f foi atingida em %d anos e %d meses.\n\n",
                meta.patrimonioAcc, anos, meses);
    } else {
        fprintf(saida, "A meta de R$ %.2f não foi atingida no período definido.\n\n",
                meta.patrimonioAcc);
    }
    
    fprintf(saida, "Simulação considerando pagamentos mensais de R$ %.2f\n", PMT);
    fprintf(saida, "Idade inicial: %d anos\n", meta.idadeFormatura);
    fprintf(saida, "Idade final da capitalização: %d anos\n", meta.idadeFimCap);
    fprintf(saida, "Investimento escolhido: %s (%s)\n", nomeAtivo, categoria);
    fprintf(saida, "Taxa mensal aplicada: %.5f\n", taxa);
    fprintf(saida, "Patrimônio final acumulado: R$ %.2f\n\n", montantes[n]);

    fprintf(saida, "Periodo  Pagamento Acumulado  Montante Capitalizado  Juros Acumulados\n");
    fprintf(saida, "=======  ====================  ======================  ================\n");

    double pagamentoAcumulado = 0.0;
    for (int i = 1; i <= n; i++) {
        pagamentoAcumulado += PMT;
        double juros = montantes[i] - pagamentoAcumulado;
        fprintf(saida, "%7d  %19.2f  %21.2f  %16.2f\n", i, pagamentoAcumulado, montantes[i], juros);
    }

    fprintf(saida, "\n============================================================\n");
    fprintf(saida, "Fim do relatório de capitalização\n");
    fprintf(saida, "============================================================\n");

}
/* ----- Função para gerar o relatório de descapitalização ----- */
void gerarRelatorioDescap(FILE *saida, double patrimonio, int nMeses,char nomes[][50], char categorias[][30], double taxas[], double retiradas[], int qtdAplicacoes) {
    if (!saida) {
        printf("Erro: arquivo de saída inválido para a seção de descapitalização.\n");
        exit(1);
    }

    fprintf(saida, "\n\n============================================================\n");
    fprintf(saida, "RELATÓRIO DE DESCAPITALIZAÇÃO\n");
    fprintf(saida, "============================================================\n\n");

    fprintf(saida, "ETAPA: DESCAPITALIZAÇÃO\n\n");

    fprintf(saida, "O patrimônio acumulado de R$ %.2f será consumido ao longo de %d meses.\n", patrimonio, nMeses);
    fprintf(saida, "As retiradas mensais possíveis são:\n\n");

    fprintf(saida, "No.  Aplicacao                    Categoria            Taxa     Retirada\n");
    fprintf(saida, "==== ==========================  ====================  =======  ==========\n");

    for (int i = 0; i < qtdAplicacoes; i++) {
        fprintf(saida, "%-4d %-25s  %-20s  %7.4f  R$ %9.2f\n",
                i + 1, nomes[i], categorias[i], taxas[i], retiradas[i]);
    }

    fprintf(saida, "\n============================================================\n");
    fprintf(saida, "Fim do relatório de descapitalização\n");
    fprintf(saida, "============================================================\n");
}
/* ----- Função principal ----- */
int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <arquivo-meta> <arquivo-capitalizacao> <arquivo-descapitalizacao> <saida-relatorio>\n", argv[0]);
        return 1;
    }

    // 1. Leitura dos dados de entrada
    lerArquivoMeta(argv[1]);
    lerArquivoInvestimentos(argv[2], investimentosCap, &numInvestimentosCap);

    // 2. Parâmetros da capitalização
    double PMT = meta.salarioMedioLiq * meta.percInvestimento;
    int n = (meta.idadeFimCap - meta.idadeFormatura) * 12;
    double taxaCalculada = capitalizacao_newton(PMT, n, meta.patrimonioAcc);

    int viavel = 0;
    Investimento investimentoSelecionado = selecionarMelhorInvestimento(taxaCalculada, &viavel);

    double* montantes = NULL;
    int mesMetaAtingida = -1;

    if (viavel) {
        montantes = calcularCrescimentoMontante(PMT, n, investimentoSelecionado.taxa, meta.patrimonioAcc, &mesMetaAtingida);
    }

    // 3. Abre arquivo de saída uma única vez
    FILE *saida = fopen(argv[4], "w");
    if (!saida) {
        printf("Erro: não foi possível criar o arquivo de saída '%s'\n", argv[4]);
        return 1;
    }

    // Passa ponteiro para gerarRelatorioCap
    gerarRelatorioCap(saida, montantes, n, PMT, investimentoSelecionado.taxa,
                    investimentoSelecionado.nome, investimentoSelecionado.categoria,
                    viavel, taxaCalculada, mesMetaAtingida);

    // 5. Se viável, passa para a descapitalização
    if (viavel) {
        // Leitura das aplicações de descapitalização (via argv[3])
        lerArquivoInvestimentos(argv[3], investimentosDescap, &numInvestimentosDescap);

        // Patrimônio final obtido com a taxa do investimento escolhido
        double patrimonioFinal = montantes[n];
        int nDescap = (meta.idadeFimRetirada - meta.idadeFimCap) * 12;

        // Arrays para relatório
        // Arrays para armazenar dados das aplicações de descapitalização
        char nomes[10][50], categorias[10][30]; // Matrizes para nomes e categorias dos investimentos
        double taxas[10], retiradas[10];        // Vetores para taxas e valores de retirada mensal
        int qtdValidos = 0;                     // Contador de investimentos válidos

        // Para cada investimento de descapitalização, calcula a retirada mensal possível
        for (int i = 0; i < numInvestimentosDescap; i++) {
            double taxa = investimentosDescap[i].taxa; // Obtém a taxa do investimento atual
            double chuteInicial = patrimonioFinal / nDescap; // Chute inicial para a retirada mensal
            double retirada = descapitalizacao_newton(patrimonioFinal, chuteInicial, (double)nDescap, taxa); // Calcula a retirada mensal possível usando Newton-Raphson

            // Armazena os dados para o relatório
            strcpy(nomes[qtdValidos], investimentosDescap[i].nome);         // Copia o nome do investimento
            strcpy(categorias[qtdValidos], investimentosDescap[i].categoria); // Copia a categoria do investimento
            taxas[qtdValidos] = taxa;                                       // Armazena a taxa
            retiradas[qtdValidos] = retirada;                               // Armazena o valor da retirada mensal
            qtdValidos++;                                                   // Incrementa o contador de investimentos válidos
        }

        // 6. Gera relatório da descapitalização
        gerarRelatorioDescap(saida, patrimonioFinal, nDescap,
                             nomes, categorias, taxas, retiradas, qtdValidos);
    }

    // 7. Libera memória e finaliza
    fclose(saida);
    if (montantes != NULL) {
        free(montantes);
    }

    return 0;
}
