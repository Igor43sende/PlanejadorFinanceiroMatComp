
  TRABALHO PRÁTICO – MATEMÁTICA COMPUTACIONAL
  Simulador de Planejamento Financeiro


Este projeto simula a capitalização e descapitalização de uma meta financeira, utilizando investimentos cadastrados e o método de Newton-Raphson para cálculo da retirada mensal ideal.

REQUISITOS:
-----------
- Windows com o compilador GCC instalado (via MinGW ou WSL).
- PowerShell (ou CMD) para executar os comandos.
- Todos os arquivos devem estar na mesma pasta:
    - plan-fin.c
    - meta.txt
    - capitalizacao.txt
    - descapitalizacao.txt

ARQUIVOS:
---------
1. plan-fin.c .................. Código-fonte em C do simulador.
2. meta-(...).txt .................... Dados da meta financeira.
3. capitalizacao.txt ........... Lista de aplicações para capitalização.
4. descapitalizacao.txt ........ Lista de aplicações para descapitalização.
5. relatorio.txt (gerado) ...... Saída com os resultados simulados.

PASSO A PASSO PARA EXECUTAR:
----------------------------

1. Abra o PowerShell e entre na pasta do projeto:

   Exemplo:
   cd C:\Users\SEU_USUARIO\

2. Compile o código-fonte com o GCC:

   gcc plan-fin.c -o plan-fin.exe

   ➜ Isso irá criar um arquivo chamado: plan-fin.exe

3. Execute o programa com os arquivos de entrada:

   .\plan-fin.exe meta-Igor.txt capitalizacao.txt descapitalizacao.txt relatorio.txt
   .\plan-fin.exe meta-Lucas.txt capitalizacao.txt descapitalizacao.txt relatorio.txt
   .\plan-fin.exe meta-Caua.txt capitalizacao.txt descapitalizacao.txt relatorio.txt
   .\plan-fin.exe meta-Daniel.txt capitalizacao.txt descapitalizacao.txt relatorio.txt
   ➜ O programa irá gerar um arquivo chamado: relatorio.txt

4. Visualize o relatório:

   type relatorio.txt

OBSERVAÇÕES:
------------
- O programa utiliza separadores de linha e espaços conforme o padrão do trabalho.
- Certifique-se de que os arquivos .txt estão no formato correto (sem extensões ocultas .txt.txt).
- Em caso de erro “arquivo não encontrado”, verifique se os nomes estão exatos e se estão na mesma pasta.

SUPORTE:
--------
Dúvidas e manutenção do código: [igorbrito2705@gmail.com]

Boa execução! ✔️
