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
./senhas
```
## Funções Implementadas
- `main`: coordena o menu principal, as filas e o estado do atendimento atual.
- `inicializar_fila`, `fila_vazia`, `fila_cheia`, `enfileirar`, `desenfileirar`: gerem as filas circulares (FIFO) de cada categoria.
- `limpar_buffer`, `ler_inteiro_intervalo`, `ler_genero`: validam as entradas do utilizador e evitam leituras inválidas.
- `classificar_paciente`, `nome_categoria`, `gerar_codigo_senha`: classificam o paciente e geram o código sequencial da senha.
- `cadastrar_paciente`: recolhe os dados do paciente, classifica, gera a senha e insere na fila correspondente.
- `consultar_proxima_senha`: consulta qual será a próxima senha a chamar sem a remover da fila.
- `registrar_chamada`: chama o próximo paciente conforme a prioridade e marca o atendimento como ativo.
- `finalizar_atendimento`: encerra o atendimento em curso e liberta a chamada seguinte.
- `exibir_painel`: mostra o painel textual com atendimento atual, próximas senhas e totais por categoria.
- `exibir_menu`: apresenta as opções disponíveis no sistema.

## Exemplos de Utilização

### Exemplo 1: Registo de pacientes em diferentes categorias
```text
1 - Cadastrar paciente e emitir senha
Genero (M - Masculino, F - Feminino, O - Outro/Nao informar): F
Idade (0 a 130): 32
Esta gestante? (1 - Sim, 0 - Nao): 1
E lactante? (1 - Sim, 0 - Nao): 0
Possui necessidade especial? (1 - Sim, 0 - Nao): 0
Possui condicao de obesidade? (1 - Sim, 0 - Nao): 0
Senha emitida: P001 | Categoria: Preferencial

1 - Cadastrar paciente e emitir senha
Genero (M - Masculino, F - Feminino, O - Outro/Nao informar): M
Idade (0 a 130): 25
E lactante? (1 - Sim, 0 - Nao): 0
Possui necessidade especial? (1 - Sim, 0 - Nao): 0
Possui condicao de obesidade? (1 - Sim, 0 - Nao): 0
Senha emitida: N001 | Categoria: Normal

1 - Cadastrar paciente e emitir senha
Genero (M - Masculino, F - Feminino, O - Outro/Nao informar): M
Idade (0 a 130): 85
E lactante? (1 - Sim, 0 - Nao): 0
Possui necessidade especial? (1 - Sim, 0 - Nao): 0
Possui condicao de obesidade? (1 - Sim, 0 - Nao): 0
Senha emitida: U001 | Categoria: Urgente
