/*--------------------------------------------------------------------------------------*/
/*              Descapitalização pelo método numérico de Newton-Raphson                 */
/*  Autor: Lucas E. Munhoz Baeta                                                        */
/*  Descrição:                                                                          */
/*  Data de modificação: 02/07/25                                                       */
/*  IFMG campus Formiga                                                                 */
/*--------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>       // Inclusão da biblioteca math.h para a realização dos cálculos da função "pow()";

#define maxit 100       // Limite máximo de Iterações;
#define toler 1e-6      // Como a taxa de juros já é um número pequeno, foi usado 0.000001;    

double Fx(double x, double PV, double i_ret, double n){
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

int main(int argc, char const *argv[])
{
    double PMT = 0.0;     // Valor a ser encontrado a partir de um chute inicial;
    double  PV = 0.0, i_ret = 0.0, n = 0.0;  

    double raiz = 0.0;   
    while(1){
        printf(" Informe o valor do montante (PV_ret) para a descapitalização: ");
        scanf("%lf",&PV);
        printf(" Informe a Taxa de juros (i_ret) para a descapitalização: ");
        scanf("%lf",&i_ret);
        printf(" Informe o número de meses (n_ret) para a retirada: ");
        scanf("%lf",&n);
        printf(" Informe o valor montante (PMT_ret) *inicial* desejado para ser retirado mensalmente: ");
        scanf("%lf",&PMT);
        if((PV <=0 ) || (i_ret <=0 ) || (n <= 0) || (PMT <= 0))
            printf(" ERRO_00: Valores inválidos fornecidos.");
        else break;
    }

    raiz = descapitalizacao(PV, PMT, n, i_ret);      // Chamada de função;

    if (!isnan(raiz)) {     // Se raíz não for NAN (Not A Number);
    printf("\n O montante para retirada (PMT_ret) encontrado foi: %.2f \n", raiz);
    }
    else
        printf("\nNão foi possível encontrar a raiz a partir do chute inicial fornecido.\n");
    return 0;
}