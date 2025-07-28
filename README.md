# 🧮 Planejador Financeiro em C

Este projeto é um **planejador financeiro pessoal** desenvolvido em linguagem C. Ele realiza simulações de **capitalização (acúmulo de patrimônio)** e **descapitalização (retirada mensal)** com base em metas e investimentos definidos pelo usuário.

## 📌 Funcionalidades

- Leitura de dados financeiros e investimentos a partir de arquivos `.txt`;
- Cálculo da **taxa mínima necessária** para atingir o patrimônio desejado usando o **método de Newton-Raphson**;
- Seleção do **melhor investimento disponível** para atingir a meta;
- Simulação da **evolução mensal do patrimônio** durante a fase de investimento;
- Simulação da **retirada mensal possível** com base nos investimentos disponíveis após o período de capitalização;
- Geração de **relatórios detalhados** de capitalização e descapitalização em formato `.txt`.

## 📁 Estrutura Esperada dos Arquivos

Você deve fornecer:

- Um arquivo de **metas** (`meta-Igor.txt`)
- Um arquivo de **investimentos para capitalização** (`capitalizacao.txt`)
- Um arquivo de **investimentos para descapitalização** (`descapitalizacao.txt`)

Todos esses arquivos devem seguir a estrutura esperada pelo código (veja comentários no próprio `.c` ou peça um modelo).

## ⚙️ Como compilar e executar

### 🐧 Linux

1. **Abra o terminal e compile o código:**

```bash
gcc plan-fin.c -o plan-fin -lm
```

> ⚠️ O `-lm` é necessário para linkar a biblioteca matemática (`math.h`).

2. **Execute o programa com os arquivos necessários:**

```bash
./plan-fin meta-Igor.txt capitalizacao.txt descapitalizacao.txt relatorio.txt
```

---

### 🪟 Windows

1. **Compile o programa no terminal (como o CMD ou Git Bash), assumindo que você tenha `gcc` instalado (via MinGW, por exemplo):**

```bash
gcc plan-fin.c -o plan-fin.exe -lm
```

2. **Execute o programa:**

```bash
plan-fin.exe meta-Igor.txt capitalizacao.txt descapitalizacao.txt relatorio.txt
```

> Após a execução, o relatório será aberto automaticamente no **Notepad**.

---

## 📝 Exemplo de Execução

```bash
./plan-fin meta-Igor.txt capitalizacao.txt descapitalizacao.txt relatorio.txt
```

- `meta.txt`: contém os dados da meta financeira (idade de formatura, salário, etc.);
- `invest-cap.txt`: lista de investimentos com taxas e riscos para a fase de capitalização;
- `invest-descap.txt`: lista de investimentos para o período de retirada;
- `relatorio.txt`: arquivo de saída com os resultados da simulação.

---

## 📄 Saída do programa

O arquivo `relatorio.txt` conterá:

- Análise da fase de **acumulação (capitalização)**;
- Melhor investimento encontrado;
- Mês e ano em que a meta foi atingida (se aplicável);
- Simulação de retirada de recursos (descapitalização) para cada ativo disponível.

---

## 🔧 Dependências

- Compilador C compatível com C99 ou superior;
- Biblioteca padrão do C (`stdio.h`, `stdlib.h`, `math.h`, etc).

---

## 📬 Contato

Caso tenha dúvidas ou queira contribuir com melhorias, sinta-se livre para entrar em contato com o autor ou abrir um pull request.
