/*Programa para calculo da media I3*/

#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include <ctype.h>
#include <unistd.h>

#define ARQUIVO "conceitos.bin"

typedef struct
{
    int qtd_a;
    int qtd_b;
    int qtd_c;
    int qtd_d;
    int qtd_cancelado;
    int qtd_ff;

    float media;
} i3_t;

typedef enum
{
    VERIFICAR_CONCEITOS,
    VERIFICAR_MEDIA,
    SIMULAR_MEDIA,
    CADASTRAR_CONCEITOS,
    SAIR
} opcoes_e;


int menu();
bool verificaAcesso();
int calculaMedia(i3_t *conceitos);
int cadastrarConceitos(i3_t *conceitos, char c);
int primeiroAcesso(i3_t *conceitos);
int lerArquivo (i3_t *conceitos);
void mostrarConceitos(i3_t *conceitos);
int criarArquivo (i3_t *conceitos);

int main()
{
    i3_t conceitos = {0};
    int index;
    int qtd_cadastro = 0;
    char conceito;

    
    // Seta a linguagem para portugues, logo é possivel adicionar acentos aos printf
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // True se o arquivo existe (já acessou antes)
    if (verificaAcesso()) {
        
        // Se conseguiu ler arquivo, retorna 1 e continua o programa
        if (lerArquivo(&conceitos)) {
            printf("Arquivo lido com sucesso.\n");
            
            // Se não conseguiu ler arquivo, retorna 0 e o programa fecha
        } else {
            return 1;
        }
    
    // Primeiro acesso
    } else {
        if (!primeiroAcesso(&conceitos)) {
            return 1; // Erro ao criar arquivo
        }
    }

    do {
        // Carrega o menu
        index = menu();
        
        // Lê opção do menu e age de acordo
        switch (index) {
            case VERIFICAR_CONCEITOS:
                printf("\n");
                mostrarConceitos(&conceitos);
            break;

            case VERIFICAR_MEDIA:
                printf("\nMedia I3: %f\n\n", conceitos.media);
            break;
            
            case SIMULAR_MEDIA:
                printf("\nWIP\n");
            break;
            
            case CADASTRAR_CONCEITOS:
                printf("Quantos conceitos você gostaria de cadastrar?: ");
                scanf("%d", &qtd_cadastro);
                
                printf("a - A\nb - B\nc - C\nd - D\ne - Cancelado/Trancado\nf - FF.\n");
                for (int i = 0; i < qtd_cadastro; i++) {
                    printf("Qual conceito você gostaria de atualizar?: ");
                    scanf(" %c", &conceito);

                    // Se cadastrarConceito retornar 1 significa que o usuario informou um conceito inexistente, logo será necessário repetir interação
                    if (cadastrarConceitos(&conceitos, conceito)) {
                        i--;
                    }
                }
                calculaMedia(&conceitos);
                criarArquivo(&conceitos);
            break;  

            case SAIR:
                printf("Você escolheu sair do programa, até a próxima!");
            break;
        }

    } while (index != 4);

    return 0;
}

int menu()
{
    int index;

    printf("Menu: \n\n");
    printf("1 - Verificar conceitos.\n");
    printf("2 - Verificar média I3 atual.\n");
    printf("3 - Simular média I3.\n");
    printf("4 - Cadastrar conceitos.\n");
    printf("5 - Sair.\n");

    do {
        printf("Selecione a opção: ");
        scanf("%d", &index);
        if ((index < 1) || (index > 5)) {
            printf("Opção invalida, insira um número válido.\n");
        }
    } while ((index < 1) || (index > 5));

    return index - 1;
}

int lerArquivo (i3_t *conceitos)
{
    FILE *f = NULL;
    f = fopen(ARQUIVO, "rb");

    if (f == NULL) {
        printf("Erro ao abrir arquivo, encerrando programa...");
        return 0;
    }

    if (fread(conceitos, sizeof(*conceitos), 1, f) != 1) {
        printf("Erro ao ler informações do arquivo, encerrando programa...");
        fclose(f);

        return 0;
    }

    fclose(f);
    return 1;
}

int primeiroAcesso(i3_t *conceitos) 
{
    // Cadastrar primeiros conceitos
    printf("Bem-vindo ao programa, vamos realizar seu primeiro cadastro adicionando seus conceitos.\n");
    for (int i = 0; i < 6; i++) {
        cadastrarConceitos(conceitos, 'a' + i);
    }

    // Calcular I3
    calculaMedia(conceitos);

    // Se conseguiu criar arquivo, ele retorna 1 e avisa o usuario
    if (criarArquivo (conceitos)) {
        printf("\n");
        printf("Arquivo criado com sucesso.\n");
        printf("\n");
        return 1;
    
    // Se não conseguiu criar arquivo, ele retorna 0
    } else {
        return 0;
    }
}

bool verificaAcesso()
{
    if (access(ARQUIVO, F_OK) == 0) {
        return true;
    } else {
        return false;
    }
}

int criarArquivo (i3_t *conceitos) 
{
    FILE *f = NULL;

    f = fopen(ARQUIVO, "w");

    if (f == NULL) {
        printf("Erro ao acessar arquivo, programa encerrando...");
        return 0;
    }

    if (fwrite(conceitos, sizeof(*conceitos), 1, f) != 1) {
        printf("Erro ao salvar no arquivo, programa encerrando...");
        fclose(f);

        return 0;
    }

    fclose(f);

    return 1;
}

int calculaMedia(i3_t *conceitos)
{
    int a = conceitos->qtd_a;
    int b = conceitos->qtd_b;
    int c = conceitos->qtd_c;
    int d = conceitos->qtd_d;
    int cancelado = conceitos->qtd_cancelado;
    int ff = conceitos->qtd_ff;

    float numerador = a + b + c + d + cancelado + ff;
    float denominador = ff + (cancelado / 2.0f) + (d / 3.0f) + (c / 6.0f) + (b / 8.0f) + (a / 10.0f);

    printf("\n");
    printf("Agora com seus conceitos, vamos calcular seu I3 atual.\n");

    if (denominador == 0) {
        printf("Erro ao calcular media.");
        return 0;
    }
    
    conceitos->media = numerador / denominador;

    printf("Seu I3 eh: %f\n", conceitos->media);

    criarArquivo(conceitos);

    return 1;
}

void mostrarConceitos(i3_t *conceitos) 
{
    printf("Conceitos A: %d\n", conceitos->qtd_a);
    printf("Conceitos B: %d\n", conceitos->qtd_b);
    printf("Conceitos C: %d\n", conceitos->qtd_c);
    printf("Conceitos D: %d\n", conceitos->qtd_d);
    printf("Conceitos FF: %d\n", conceitos->qtd_ff);
    printf("Cancelamentos ou trancamentos: %d\n", conceitos->qtd_cancelado);

    return;
}

int cadastrarConceitos(i3_t *conceitos, char c)
{
    int qtd;

    // Dimiunui para minusculo para evitar problemas com switch-case
    c = toupper(c);

    // Verifica se o conceito é um caractere ou se é uma frase (f = ff (falta de frequencia), e = cancelados/trancados )
    if (c == 'A' || c == 'B' || c == 'C' || c == 'D') {
        printf("Quantos conceitos %c você gostaria de adicionar?: ", c);
        scanf("%d", &qtd);
    } else if (c == 'E'){
        printf("Quantos conceitos cancelado/trancados você gostaria de adicionar?: ");
        scanf("%d", &qtd);
    } else if (c == 'F') {
        printf("Quantos conceitos ff você gostaria de adicionar?: ");
        scanf("%d", &qtd);
    } else {
        printf("Esse conceito não existe.\n");
        return 1;
    }

    // Sempre soma a quantidade informada ao conceito
    switch (c) {

        case 'A':
            conceitos->qtd_a += qtd;
        break;

        case 'B':
            conceitos->qtd_b += qtd;
        break;

        case 'C':
            conceitos->qtd_c += qtd;
        break;

        case 'D':
            conceitos->qtd_d += qtd;
        break;

        case 'E':
            conceitos->qtd_cancelado += qtd;
        break;

        case 'F':
            conceitos->qtd_ff += qtd;
        break;
    }

    // Da um retorno sobre os conceitos já existentes
    mostrarConceitos(conceitos);

    return 0;
}
