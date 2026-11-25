# Sistema de Gerenciamento de Senhas de Atendimento Clínico

Aplicação em linguagem C para registo, classificação por prioridade e controlo de filas de atendimento clínico.

## Objetivo do Sistema
Informatizar o processo de triagem e organização das filas de atendimento numa clínica de especialidades médicas, respeitando a precedência entre categorias (**Urgente**, **Preferencial** e **Normal**) e a ordem de chegada (**FIFO**) dentro de cada categoria.

## Funcionalidades
- **Registo de senhas:** recolhe género, idade, condição de gestante (quando aplicável), lactante, necessidade especial e obesidade, com validação de entradas.
- **Classificação automática e numeração independente:**
  - **Urgente (`U001`, `U002`...):** idade superior a 80 anos.
  - **Preferencial (`P001`, `P002`...):** gestantes, lactantes, pessoas com necessidade especial, obesidade, mulheres com 60 anos ou mais e homens com 65 anos ou mais.
  - **Normal (`N001`, `N002`...):** restantes pacientes sem condições prioritárias registadas.
- **Controlo de atendimento:** consulta da próxima senha, registo de chamada (`Urgente` > `Preferencial` > `Normal`), identificação do atendimento em curso, finalização e atualização das filas sem retorno de senhas já atendidas.
- **Painel de chamada:** apresenta a senha atualmente chamada, categoria, próximas senhas em espera e quantidade aguardando por categoria.

## Procedimentos de Compilação e Execução
Num terminal com compilador C (`gcc`) disponível:

```bash
gcc -Wall -Wextra main.c -o senhas
./senhas# senhas-clinica-c
