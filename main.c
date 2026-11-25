#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FILA 100
#define TAM_SENHA 10

typedef enum {
    CATEGORIA_NORMAL = 0,
    CATEGORIA_PREFERENCIAL = 1,
    CATEGORIA_URGENTE = 2
} Categoria;

typedef struct {
    char senha[TAM_SENHA];
    Categoria categoria;
    char genero; // 'M', 'F', 'O'
    int idade;
    int gestante;
    int lactante;
    int necessidade_especial;
    int obesidade;
} Paciente;

typedef struct {
    Paciente itens[MAX_FILA];
    int inicio;
    int fim;
    int total;
} Fila;

typedef struct {
    int ativo;
    Paciente atual;
} AtendimentoAtual;

/* Prototipos das funcoes */
void inicializar_fila(Fila *fila);
int fila_vazia(const Fila *fila);
int fila_cheia(const Fila *fila);
int enfileirar(Fila *fila, Paciente p);
int desenfileirar(Fila *fila, Paciente *removido);
void limpar_buffer(void);
int ler_inteiro_intervalo(const char *prompt, int min, int max);
char ler_genero(void);
Categoria classificar_paciente(const Paciente *p);
const char *nome_categoria(Categoria cat);
void gerar_codigo_senha(Categoria cat, int numero, char *destino);
void cadastrar_paciente(Fila *fila_normal, Fila *fila_pref, Fila *fila_urg,
                        int *seq_normal, int *seq_pref, int *seq_urg);
int consultar_proxima_senha(const Fila *fila_normal, const Fila *fila_pref,
                            const Fila *fila_urg, Paciente *proxima);
void registrar_chamada(Fila *fila_normal, Fila *fila_pref, Fila *fila_urg,
                       AtendimentoAtual *atendimento);
void finalizar_atendimento(AtendimentoAtual *atendimento);
void exibir_painel(const AtendimentoAtual *atendimento, const Fila *fila_normal,
                   const Fila *fila_pref, const Fila *fila_urg);
void exibir_menu(void);

int main(void) {
    Fila fila_normal, fila_pref, fila_urg;
    AtendimentoAtual atendimento;
    int seq_normal = 0, seq_pref = 0, seq_urg = 0;
    int opcao;

    inicializar_fila(&fila_normal);
    inicializar_fila(&fila_pref);
    inicializar_fila(&fila_urg);
    atendimento.ativo = 0;

    do {
        exibir_menu();
        opcao = ler_inteiro_intervalo("Escolha uma opcao: ", 0, 5);

        switch (opcao) {
            case 1:
                cadastrar_paciente(&fila_normal, &fila_pref, &fila_urg,
                                   &seq_normal, &seq_pref, &seq_urg);
                break;
            case 2: {
                Paciente prox;
                if (consultar_proxima_senha(&fila_normal, &fila_pref, &fila_urg, &prox)) {
                    printf("\nProxima senha a ser chamada: %s (%s)\n",
                           prox.senha, nome_categoria(prox.categoria));
                } else {
                    printf("\nNao ha pacientes aguardando na fila.\n");
                }
                break;
            }
            case 3:
                registrar_chamada(&fila_normal, &fila_pref, &fila_urg, &atendimento);
                break;
            case 4:
                finalizar_atendimento(&atendimento);
                break;
            case 5:
                exibir_painel(&atendimento, &fila_normal, &fila_pref, &fila_urg);
                break;
            case 0:
                printf("\nEncerrando o sistema de senhas...\n");
                break;
            default:
                printf("\nOpcao invalida.\n");
                break;
        }
    } while (opcao != 0);

    return 0;
}

void inicializar_fila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->total = 0;
}

int fila_vazia(const Fila *fila) {
    return fila->total == 0;
}

int fila_cheia(const Fila *fila) {
    return fila->total >= MAX_FILA;
}

int enfileirar(Fila *fila, Paciente p) {
    if (fila_cheia(fila)) {
        return 0;
    }
    fila->itens[fila->fim] = p;
    fila->fim = (fila->fim + 1) % MAX_FILA;
    fila->total++;
    return 1;
}

int desenfileirar(Fila *fila, Paciente *removido) {
    if (fila_vazia(fila)) {
        return 0;
    }
    *removido = fila->itens[fila->inicio];
    fila->inicio = (fila->inicio + 1) % MAX_FILA;
    fila->total--;
    return 1;
}

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* descarta caracteres restantes */
    }
}

int ler_inteiro_intervalo(const char *prompt, int min, int max) {
    int valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &valor) == 1) {
            limpar_buffer();
            if (valor >= min && valor <= max) {
                return valor;
            }
        } else {
            limpar_buffer();
        }
        printf("Entrada invalida. Digite um numero entre %d e %d.\n", min, max);
    }
}

char ler_genero(void) {
    char g;
    while (1) {
        printf("Genero (M - Masculino, F - Feminino, O - Outro/Nao informar): ");
        if (scanf(" %c", &g) == 1) {
            limpar_buffer();
            g = (char)toupper((unsigned char)g);
            if (g == 'M' || g == 'F' || g == 'O') {
                return g;
            }
        } else {
            limpar_buffer();
        }
        printf("Genero invalido. Use M, F ou O.\n");
    }
}

Categoria classificar_paciente(const Paciente *p) {
    if (p->idade > 80) {
        return CATEGORIA_URGENTE;
    }
    if (p->gestante || p->lactante || p->necessidade_especial || p->obesidade) {
        return CATEGORIA_PREFERENCIAL;
    }
    if (p->genero == 'F' && p->idade >= 60) {
        return CATEGORIA_PREFERENCIAL;
    }
    if (p->genero == 'M' && p->idade >= 65) {
        return CATEGORIA_PREFERENCIAL;
    }
    return CATEGORIA_NORMAL;
}

const char *nome_categoria(Categoria cat) {
    switch (cat) {
        case CATEGORIA_URGENTE:
            return "Urgente";
        case CATEGORIA_PREFERENCIAL:
            return "Preferencial";
        case CATEGORIA_NORMAL:
        default:
            return "Normal";
    }
}

void gerar_codigo_senha(Categoria cat, int numero, char *destino) {
    char prefixo = 'N';
    if (cat == CATEGORIA_URGENTE) {
        prefixo = 'U';
    } else if (cat == CATEGORIA_PREFERENCIAL) {
        prefixo = 'P';
    }
    snprintf(destino, TAM_SENHA, "%c%03d", prefixo, numero);
}

void cadastrar_paciente(Fila *fila_normal, Fila *fila_pref, Fila *fila_urg,
                        int *seq_normal, int *seq_pref, int *seq_urg) {
    Paciente p;
    memset(&p, 0, sizeof(Paciente));

    printf("\n--- Cadastro de Paciente ---\n");
    p.genero = ler_genero();
    p.idade = ler_inteiro_intervalo("Idade (0 a 130): ", 0, 130);

    if (p.genero == 'F') {
        p.gestante = ler_inteiro_intervalo("Esta gestante? (1 - Sim, 0 - Nao): ", 0, 1);
    } else {
        p.gestante = 0;
    }

    p.lactante = ler_inteiro_intervalo("E lactante? (1 - Sim, 0 - Nao): ", 0, 1);
    p.necessidade_especial = ler_inteiro_intervalo("Possui necessidade especial? (1 - Sim, 0 - Nao): ", 0, 1);
    p.obesidade = ler_inteiro_intervalo("Possui condicao de obesidade? (1 - Sim, 0 - Nao): ", 0, 1);

    p.categoria = classificar_paciente(&p);

    if (p.categoria == CATEGORIA_URGENTE) {
        if (fila_cheia(fila_urg)) {
            printf("Erro: fila Urgente cheia.\n");
            return;
        }
        (*seq_urg)++;
        gerar_codigo_senha(p.categoria, *seq_urg, p.senha);
        enfileirar(fila_urg, p);
    } else if (p.categoria == CATEGORIA_PREFERENCIAL) {
        if (fila_cheia(fila_pref)) {
            printf("Erro: fila Preferencial cheia.\n");
            return;
        }
        (*seq_pref)++;
        gerar_codigo_senha(p.categoria, *seq_pref, p.senha);
        enfileirar(fila_pref, p);
    } else {
        if (fila_cheia(fila_normal)) {
            printf("Erro: fila Normal cheia.\n");
            return;
        }
        (*seq_normal)++;
        gerar_codigo_senha(p.categoria, *seq_normal, p.senha);
        enfileirar(fila_normal, p);
    }

    printf("\nSenha emitida: %s | Categoria: %s\n", p.senha, nome_categoria(p.categoria));
}

int consultar_proxima_senha(const Fila *fila_normal, const Fila *fila_pref,
                            const Fila *fila_urg, Paciente *proxima) {
    if (!fila_vazia(fila_urg)) {
        *proxima = fila_urg->itens[fila_urg->inicio];
        return 1;
    }
    if (!fila_vazia(fila_pref)) {
        *proxima = fila_pref->itens[fila_pref->inicio];
        return 1;
    }
    if (!fila_vazia(fila_normal)) {
        *proxima = fila_normal->itens[fila_normal->inicio];
        return 1;
    }
    return 0;
}

void registrar_chamada(Fila *fila_normal, Fila *fila_pref, Fila *fila_urg,
                       AtendimentoAtual *atendimento) {
    Paciente chamado;
    int encontrou = 0;

    if (atendimento->ativo) {
        printf("\nJa existe uma senha em atendimento (%s). Finalize antes de chamar a proxima.\n",
               atendimento->atual.senha);
        return;
    }

    if (!fila_vazia(fila_urg)) {
        encontrou = desenfileirar(fila_urg, &chamado);
    } else if (!fila_vazia(fila_pref)) {
        encontrou = desenfileirar(fila_pref, &chamado);
    } else if (!fila_vazia(fila_normal)) {
        encontrou = desenfileirar(fila_normal, &chamado);
    }

    if (!encontrou) {
        printf("\nNao ha pacientes na fila para chamar.\n");
        return;
    }

    atendimento->ativo = 1;
    atendimento->atual = chamado;
    printf("\nChamando senha: %s (%s)\n", chamado.senha, nome_categoria(chamado.categoria));
}

void finalizar_atendimento(AtendimentoAtual *atendimento) {
    if (!atendimento->ativo) {
        printf("\nNao ha nenhum atendimento em andamento no momento.\n");
        return;
    }
    printf("\nAtendimento da senha %s (%s) finalizado.\n",
           atendimento->atual.senha, nome_categoria(atendimento->atual.categoria));
    atendimento->ativo = 0;
}

void exibir_painel(const AtendimentoAtual *atendimento, const Fila *fila_normal,
                   const Fila *fila_pref, const Fila *fila_urg) {
    int i, idx, exibidos = 0;

    printf("\n================ PAINEL DE CHAMADA ================\n");
    if (atendimento->ativo) {
        printf("Senha em atendimento: %s\n", atendimento->atual.senha);
        printf("Categoria: %s\n", nome_categoria(atendimento->atual.categoria));
    } else {
        printf("Senha em atendimento: Nenhuma\n");
        printf("Categoria: -\n");
    }

    printf("\nProximas senhas aguardando:\n");
    if (!fila_vazia(fila_urg)) {
        for (i = 0; i < fila_urg->total && exibidos < 5; i++) {
            idx = (fila_urg->inicio + i) % MAX_FILA;
            printf("  - %s (Urgente)\n", fila_urg->itens[idx].senha);
            exibidos++;
        }
    }
    if (!fila_vazia(fila_pref)) {
        for (i = 0; i < fila_pref->total && exibidos < 5; i++) {
            idx = (fila_pref->inicio + i) % MAX_FILA;
            printf("  - %s (Preferencial)\n", fila_pref->itens[idx].senha);
            exibidos++;
        }
    }
    if (!fila_vazia(fila_normal)) {
        for (i = 0; i < fila_normal->total && exibidos < 5; i++) {
            idx = (fila_normal->inicio + i) % MAX_FILA;
            printf("  - %s (Normal)\n", fila_normal->itens[idx].senha);
            exibidos++;
        }
    }
    if (exibidos == 0) {
        printf("  (Nenhuma senha na fila)\n");
    }

    printf("\nAguardando por categoria:\n");
    printf("  Urgente: %d | Preferencial: %d | Normal: %d\n",
           fila_urg->total, fila_pref->total, fila_normal->total);
    printf("===================================================\n");
}

void exibir_menu(void) {
    printf("\n--- Sistema de Senhas Clinicas ---\n");
    printf("1 - Cadastrar paciente e emitir senha\n");
    printf("2 - Consultar proxima senha\n");
    printf("3 - Registrar chamada do paciente\n");
    printf("4 - Finalizar atendimento atual\n");
    printf("5 - Exibir painel de chamada\n");
    printf("0 - Sair\n");
}
