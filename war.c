/* war.c
   Desafio WAR Estruturado - Novato, Aventureiro e Mestre
   Autor: Gerado por ChatGPT (exemplo didático)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QTD_TERRITORIOS 5
#define MAX_NOME 50
#define MAX_COR 20

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
    int conquistado; /* 0 = não, 1 = sim (para estatísticas de conquista) */
} Territorio;

/* --- Utilitários --- */
void limpa_buffer_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void strip_newline(char *s) {
    size_t L = strlen(s);
    if (L > 0 && s[L-1] == '\n') s[L-1] = '\0';
}

/* --- Impressão do mapa --- */
void mostrar_mapa(const Territorio *terr, int n) {
    puts("\n==== MAPA ATUAL ====");
    for (int i = 0; i < n; ++i) {
        printf("%d) Nome: %s | Cor: %s | Tropas: %d | Conquistado? %s\n",
               i+1,
               terr[i].nome,
               terr[i].cor,
               terr[i].tropas,
               terr[i].conquistado ? "Sim" : "Não");
    }
    puts("====================\n");
}

/* ===================== NÍVEL NOVATO ===================== */
/* Usa vetor estático de 5 territórios e entrada com fgets/scanf */
void nivel_novato() {
    puts("\n--- MODO NOVATO (vetor estático) ---");
    Territorio mapa[QTD_TERRITORIOS];

    /* Cadastro */
    for (int i = 0; i < QTD_TERRITORIOS; ++i) {
        printf("\nTerritório %d\n", i+1);
        printf("Nome: ");
        if (!fgets(mapa[i].nome, MAX_NOME, stdin)) {
            mapa[i].nome[0] = '\0';
        }
        strip_newline(mapa[i].nome);

        printf("Cor do Exército: ");
        if (!fgets(mapa[i].cor, MAX_COR, stdin)) {
            mapa[i].cor[0] = '\0';
        }
        strip_newline(mapa[i].cor);

        printf("Número de Tropas (inteiro): ");
        while (scanf("%d", &mapa[i].tropas) != 1) {
            printf("Entrada inválida. Digite um número inteiro: ");
            limpa_buffer_stdin();
        }
        limpa_buffer_stdin(); /* limpar '\n' restante */
        mapa[i].conquistado = 0;
    }

    /* Exibir mapa */
    mostrar_mapa(mapa, QTD_TERRITORIOS);
    puts("Fim do modo Novato.\n");
}

/* ===================== NÍVEL AVENTUREIRO ===================== */
/* Usa calloc para alocar dinamicamente 5 territórios. Simula batalhas simples. */

/* Gere um número de dado entre 1 e 6 */
int rolar_dado() {
    return (rand() % 6) + 1;
}

/* Simula uma rodada de batalha entre atacante (a) e defensor (d).
   Retorna 1 se ocorreu conquista do defensor, 0 caso contrário. */
int simular_batalha(Territorio *a, Territorio *d) {
    if (a == d) {
        puts("Atacante e defensor são o mesmo território. Ação inválida.");
        return 0;
    }
    if (a->tropas <= 0) {
        puts("Território atacante não tem tropas suficientes.");
        return 0;
    }
    if (d->tropas <= 0) {
        puts("Território defensor está sem tropas (já conquistado).");
        return 0;
    }

    int dado_atq = rolar_dado();
    int dado_def = rolar_dado();

    printf("Dado atacante: %d | Dado defensor: %d\n", dado_atq, dado_def);

    /* regra: empate favorece atacante */
    if (dado_atq >= dado_def) {
        /* atacante vence: defensor perde 1 tropa */
        d->tropas -= 1;
        printf("Atacante vence! Território %s perde 1 tropa.\n", d->nome);
        if (d->tropas <= 0) {
            d->tropas = 0;
            d->conquistado = 1;
            printf("Território %s foi conquistado por %s!\n", d->nome, a->nome);
            return 1;
        }
    } else {
        /* defensor vence: atacantes perdem 1 tropa */
        a->tropas -= 1;
        if (a->tropas < 0) a->tropas = 0;
        printf("Defensor vence! Território %s perde 1 tropa.\n", a->nome);
    }
    return 0;
}

void nivel_aventureiro() {
    puts("\n--- MODO AVENTUREIRO (alocação dinâmica) ---");

    /* alocar com calloc */
    Territorio *mapa = calloc(QTD_TERRITORIOS, sizeof(Territorio));
    if (!mapa) {
        perror("calloc");
        return;
    }

    /* Cadastro */
    for (int i = 0; i < QTD_TERRITORIOS; ++i) {
        printf("\nTerritório %d\n", i+1);
        printf("Nome: ");
        if (!fgets(mapa[i].nome, MAX_NOME, stdin)) mapa[i].nome[0] = '\0';
        strip_newline(mapa[i].nome);

        printf("Cor do Exército: ");
        if (!fgets(mapa[i].cor, MAX_COR, stdin)) mapa[i].cor[0] = '\0';
        strip_newline(mapa[i].cor);

        printf("Número de Tropas (inteiro): ");
        while (scanf("%d", &mapa[i].tropas) != 1) {
            printf("Entrada inválida. Digite um número inteiro: ");
            limpa_buffer_stdin();
        }
        limpa_buffer_stdin();
        mapa[i].conquistado = (mapa[i].tropas <= 0) ? 1 : 0;
    }

    /* Laço interativo de batalhas */
    int continuar = 1;
    while (continuar) {
        mostrar_mapa(mapa, QTD_TERRITORIOS);
        printf("Escolha o atacante (1-%d) ou 0 para sair: ", QTD_TERRITORIOS);
        int idx_a;
        while (scanf("%d", &idx_a) != 1) { limpa_buffer_stdin(); printf("Entrada inválida. Digite um número: "); }
        limpa_buffer_stdin();
        if (idx_a == 0) break;
        if (idx_a < 1 || idx_a > QTD_TERRITORIOS) {
            puts("Escolha inválida.");
            continue;
        }

        printf("Escolha o defensor (1-%d): ", QTD_TERRITORIOS);
        int idx_d;
        while (scanf("%d", &idx_d) != 1) { limpa_buffer_stdin(); printf("Entrada inválida. Digite um número: "); }
        limpa_buffer_stdin();
        if (idx_d < 1 || idx_d > QTD_TERRITORIOS) {
            puts("Escolha inválida.");
            continue;
        }

        /* simular batalha */
        simular_batalha(&mapa[idx_a-1], &mapa[idx_d-1]);

        /* perguntar se continua */
        printf("Deseja continuar atacando? (1 = sim, 0 = não): ");
        int r;
        while (scanf("%d", &r) != 1) { limpa_buffer_stdin(); printf("Entrada inválida. Digite 1 ou 0: "); }
        limpa_buffer_stdin();
        if (r == 0) continuar = 0;
    }

    puts("Saindo do modo Aventureiro. Liberando memória.");
    free(mapa);
}

/* ===================== NÍVEL MESTRE ===================== */
/* Modularização total, missões aleatórias */

typedef enum {
    MISS_NENHUMA = 0,
    MISS_DESTRUIR_VERDE = 1,
    MISS_CONQUISTAR_3 = 2
} MissaoTipo;

/* Inicializa automaticamente um conjunto de territórios (exemplo). */
void inicializar_territorios_automatico(Territorio *mapa, int n) {
    const char *nomes[QTD_TERRITORIOS] = {
        "Aldea Azul", "Fortaleza Vermelha", "Planalto Verde", "Ilha Negra", "Deserto Dourado"
    };
    const char *cores[QTD_TERRITORIOS] = {
        "Azul", "Vermelho", "Verde", "Preto", "Dourado"
    };
    int tropas_init[QTD_TERRITORIOS] = {5, 4, 6, 3, 5};

    for (int i = 0; i < n; ++i) {
        strncpy(mapa[i].nome, nomes[i], MAX_NOME-1);
        mapa[i].nome[MAX_NOME-1] = '\0';
        strncpy(mapa[i].cor, cores[i], MAX_COR-1);
        mapa[i].cor[MAX_COR-1] = '\0';
        mapa[i].tropas = tropas_init[i];
        mapa[i].conquistado = 0;
    }
}

/* Atribui uma missão aleatória */
MissaoTipo gerar_missao() {
    int r = rand() % 2; /* 0 ou 1 - duas missões */
    if (r == 0) return MISS_DESTRUIR_VERDE;
    return MISS_CONQUISTAR_3;
}

/* Verifica se a missão foi cumprida */
int verificar_missao(const Territorio *mapa, int n, MissaoTipo missao) {
    if (missao == MISS_NENHUMA) return 0;
    if (missao == MISS_DESTRUIR_VERDE) {
        for (int i = 0; i < n; ++i) {
            /* missão satisfeita se nenhum território com cor "Verde" tiver tropas */
            if (strcasecmp(mapa[i].cor, "Verde") == 0 && mapa[i].tropas > 0) {
                return 0; /* ainda não cumprida */
            }
        }
        return 1; /* cumprida: não há mais tropas verdes */
    } else if (missao == MISS_CONQUISTAR_3) {
        int count = 0;
        for (int i = 0; i < n; ++i) if (mapa[i].conquistado) ++count;
        return (count >= 3);
    }
    return 0;
}

/* Função de ataque reutilizável (passagem por referência) */
int mestre_atacar(Territorio *mapa, int n, int idx_a, int idx_d) {
    if (idx_a < 0 || idx_a >= n || idx_d < 0 || idx_d >= n) {
        puts("Índices inválidos.");
        return 0;
    }
    return simular_batalha(&mapa[idx_a], &mapa[idx_d]);
}

/* Menu do Mestre */
void nivel_mestre() {
    puts("\n--- MODO MESTRE (modularizado, missões) ---");
    Territorio *mapa = calloc(QTD_TERRITORIOS, sizeof(Territorio));
    if (!mapa) { perror("calloc"); return; }

    /* Inicialização automática (poupa o cadastro manual) */
    inicializar_territorios_automatico(mapa, QTD_TERRITORIOS);

    /* Gerar missão */
    MissaoTipo missao = gerar_missao();
    printf("Sua missão: ");
    if (missao == MISS_DESTRUIR_VERDE) {
        puts("Destruir o exército Verde (eliminar todas as tropas de territórios com cor 'Verde').");
    } else if (missao == MISS_CONQUISTAR_3) {
        puts("Conquistar 3 territórios (marcados como conquistados quando tropas chegam a 0).");
    }

    int opcao = -1;
    while (1) {
        puts("\n--- MENU MESTRE ---");
        puts("1 - Atacar");
        puts("2 - Verificar Missão");
        puts("0 - Sair do modo Mestre");
        printf("Escolha: ");
        while (scanf("%d", &opcao) != 1) { limpa_buffer_stdin(); printf("Entrada inválida. Digite 0,1 ou 2: "); }
        limpa_buffer_stdin();

        if (opcao == 0) break;
        else if (opcao == 1) {
            mostrar_mapa(mapa, QTD_TERRITORIOS);
            printf("Escolha o atacante (1-%d): ", QTD_TERRITORIOS);
            int a, d;
            while (scanf("%d", &a) != 1) { limpa_buffer_stdin(); printf("Entrada inválida. Digite um número: "); }
            limpa_buffer_stdin();
            if (a < 1 || a > QTD_TERRITORIOS) { puts("Atacante inválido."); continue; }

            printf("Escolha o defensor (1-%d): ", QTD_TERRITORIOS);
            while (scanf("%d", &d) != 1) { limpa_buffer_stdin(); printf("Entrada inválida. Digite um número: "); }
            limpa_buffer_stdin();
            if (d < 1 || d > QTD_TERRITORIOS) { puts("Defensor inválido."); continue; }

            mestre_atacar(mapa, QTD_TERRITORIOS, a-1, d-1);

            /* informar progresso de missão */
            if (verificar_missao(mapa, QTD_TERRITORIOS, missao)) {
                puts("\n*** MISSÃO CUMPRIDA! Parabéns! ***");
                mostrar_mapa(mapa, QTD_TERRITORIOS);
                break;
            }
        }
        else if (opcao == 2) {
            if (verificar_missao(mapa, QTD_TERRITORIOS, missao)) {
                puts("\nMissão já cumprida! Você venceu!");
                mostrar_mapa(mapa, QTD_TERRITORIOS);
                break;
            } else {
                puts("\nMissão ainda não cumprida. Progresso atual:");
                mostrar_mapa(mapa, QTD_TERRITORIOS);
            }
        }
        else {
            puts("Opção inválida.");
        }
    }

    free(mapa);
    puts("Saindo do modo Mestre.\n");
}

/* ===================== PROGRAMA PRINCIPAL ===================== */
int main(void) {
    srand((unsigned) time(NULL));

    puts("Bem-vindo ao Desafio WAR Estruturado - versão C (Novato/Aventureiro/Mestre)");
    int escolha = -1;
    while (1) {
        puts("\nEscolha o modo:");
        puts("1 - Novato (vetor estático, cadastro manual)");
        puts("2 - Aventureiro (alocação dinâmica, batalhas interativas)");
        puts("3 - Mestre (menu, missões, inicialização automática)");
        puts("0 - Sair");
        printf("Sua escolha: ");
        while (scanf("%d", &escolha) != 1) { limpa_buffer_stdin(); printf("Entrada inválida. Digite 0-3: "); }
        limpa_buffer_stdin();

        if (escolha == 0) {
            puts("Saindo. Boa sorte!");
            break;
        } else if (escolha == 1) {
            nivel_novato();
        } else if (escolha == 2) {
            nivel_aventureiro();
        } else if (escolha == 3) {
            nivel_mestre();
        } else {
            puts("Opção inválida.");
        }
    }
    return 0;
}
