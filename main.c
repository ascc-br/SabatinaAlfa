#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include "sabatina.h"
#include <windows.h>
#include <stdlib.h>
#include <time.h>

const int LARGURA_TELA = 1024;
const int ALTURA_TELA = 768;

ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *seta = NULL;
ALLEGRO_BITMAP *fundo2 = NULL;
ALLEGRO_BITMAP *leaf = NULL;
ALLEGRO_BITMAP *flower = NULL;
ALLEGRO_BITMAP *trunk = NULL;
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_FONT *fonte2 = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;

void manipular_entrada(ALLEGRO_EVENT evento, char *str);

bool inicializar();
bool carregar_arquivos();
void finalizar();

int contaFotoFolhas (char nc[]);
int contaFotoFlor (char nc[]);
int contaFotoTronco (char nc[]);
planta *criaPlantas (char nc[], char nv[], char fam[],char f2[], char f3[], char f4[], planta *head, int picsLeaf, int *erro);
planta *lerDataBase (int *erro, int *qtdCadastradas);
void mostraPlanta(char *nc, int l, int f, int t, int opcao);

typedef struct SABATIAO
{
    planta *n;
    int ls;
    int fs;
    int ts; //leaf sorteada, flower sorteada...
    int ok;
} sabas;

int main()
{
    planta *head, *aux;
    char nv[300], nc[300], f1[300], opcao = '5', subtitle[100];
    int i, j, posicaoSeta = 0, qtdCadastradas = 0, l, f, sorteadas[5], score[5];
    unsigned int  ponteiro;
    bool rodar_allegro = true;
    bool janelaAberta;
    int menu;
    sabas luk[5];

    SetConsoleTitle("Sabatina Alfa - por Alfran Carvalho");

    strcpy(nc, "");
    strcpy(nv, "");
    strcpy(f1, "");
    strcpy(subtitle, "");


    if (!inicializar())
    {
        return -1;
    }

    if (!carregar_arquivos())
    {
        return -1;
    }



    head = lerDataBase(&i,&qtdCadastradas);

    while (opcao != '0')
    {

        system("cls");

        if (rodar_allegro)
        {
            janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
            if (!janela)
            {
                fprintf(stderr, "Falha ao criar a janela.\n");
                return -1;
            }
            else
                janelaAberta = true;

            al_set_window_title(janela, "Sabatina Alfa - por Alfran Carvalho");

            al_register_event_source(fila_eventos, al_get_display_event_source(janela));
            al_register_event_source(fila_eventos, al_get_keyboard_event_source());

            menu = 0;
            posicaoSeta =0;
        }


        while (rodar_allegro)
        {
            sprintf(subtitle,"Total de %i plantas cadastradas!", qtdCadastradas);

            while (!al_is_event_queue_empty(fila_eventos))
            {
                ALLEGRO_EVENT evento;
                al_wait_for_event(fila_eventos, &evento);

                if (menu == 0)
                {
                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN )
                    {
                        switch (evento.keyboard.keycode)
                        {
                        case ALLEGRO_KEY_ENTER:
                            if (posicaoSeta == 0)
                            {
                                if (qtdCadastradas >= 5)
                                {
                                    menu = 2;
                                    for(i = 0; i < 5; i++)
                                    {
                                        sorteadas[i] = -1;
                                        score[i] = 0;
                                    }

                                    for(i=0; i <5; i++)
                                    {
                                        luk[i].n =sorteiaplantas(head,sorteadas,qtdCadastradas,&luk[i].ls,&luk[i].fs,&luk[i].ts);
                                        luk[i].ok = 0;
                                    }

                                    ponteiro = 0; //mecher ponteiro verificando se é >4(vai pra zero) ou <0(vai pra 5)
                                    srand(time(NULL));
                                }
                            }
                            else if (posicaoSeta == 1) //opcao configurações
                            {
                                rodar_allegro = false;
                            }
                            else //opcao sair
                            {
                                rodar_allegro = false;
                                opcao = '0';
                            }

                            break;
                        case ALLEGRO_KEY_UP:
                            if (posicaoSeta == 0)
                                posicaoSeta = 2;
                            else if (posicaoSeta == 1)
                                posicaoSeta = 0;
                            else
                                posicaoSeta = 1;
                            break;
                        case ALLEGRO_KEY_DOWN:
                            if (posicaoSeta == 0)
                                posicaoSeta = 1;
                            else if (posicaoSeta == 1)
                                posicaoSeta = 2;
                            else
                                posicaoSeta = 0;
                            break;
                        }

                    }
                }
                else if (menu == 2) //botões sabatina
                {

                }
                else //botões menu de instruções
                {

                }

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    rodar_allegro = false;
                    opcao = '0';
                }
            }


            if (menu != 2)
            {
                al_draw_bitmap(fundo, 0, 0, 0);
                if (menu == 0)
                {
                    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2,
                                 125, ALLEGRO_ALIGN_CENTRE, subtitle);
                    al_draw_text(fonte2, al_map_rgb(0, 0, 0), LARGURA_TELA / 2,
                                 ALTURA_TELA/2 - al_get_font_ascent(fonte2)*2 - al_get_font_ascent(fonte) - 10, ALLEGRO_ALIGN_CENTRE, "MENU PRINCIPAL:");
                    al_draw_text(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA / 2,
                                 ALTURA_TELA/2 - al_get_font_ascent(fonte2) - al_get_font_ascent(fonte) - 10, ALLEGRO_ALIGN_CENTRE, "Utilize as setas direcionais e Enter:");
                    al_draw_text(fonte2, al_map_rgb(0, 0, 0), LARGURA_TELA / 2,
                                 ALTURA_TELA/2 - al_get_font_ascent(fonte2), ALLEGRO_ALIGN_CENTRE, "Iniciar Sabatina!");
                    al_draw_text(fonte2, al_map_rgb(0, 0, 0), LARGURA_TELA / 2,
                                 ALTURA_TELA/2, ALLEGRO_ALIGN_CENTRE, "Configuracoes");
                    al_draw_text(fonte2, al_map_rgb(0, 0, 0), LARGURA_TELA / 2,
                                 ALTURA_TELA/2 + al_get_font_ascent(fonte2), ALLEGRO_ALIGN_CENTRE, "Sair");

                    switch (posicaoSeta)
                    {
                    case 0 :
                        al_draw_bitmap(seta, 300, ALTURA_TELA/2 - al_get_font_ascent(fonte2), 0);
                        break;

                    case 1 :
                        al_draw_bitmap(seta, 317, ALTURA_TELA/2, 0);
                        break;

                    case 2 :
                        al_draw_bitmap(seta, 425, ALTURA_TELA/2 + al_get_font_ascent(fonte2), 0);
                        break;
                    }
                }
                else   //tela menu de instruções
                {

                }
            } else //tela sabatina
            {

            }

            al_flip_display();
        } //fim do laço do allegro

        if(janelaAberta)
        {
            al_destroy_display(janela);
            janelaAberta = false;
        }

        printf(".dP\"Y8    db    88\"\"Yb    db    888888 88 88b 88    db\n");
        printf("`Ybo.\"   dPYb   88__dP   dPYb     88   88 88Yb88   dPYb\n");
        printf("o.`Y8b  dP__Yb  88\"\"Yb  dP__Yb    88   88 88 Y88  dP__Yb\n");
        printf("8bodP' dP\"\"\"\"Yb 88oodP dP\"\"\"\"Yb   88   88 88  Y8 dP\"\"\"\"Yb\n");

        printf("               db    88     888888   db    \n");
        printf("              dPYb   88     88__    dPYb\n");
        printf("             dP__Yb  88  .o 88\"\"   dP__Yb\n");
        printf("            dP\"\"\"\"Yb 88ood8 88    dP\"\"\"\"Yb\n");
        printf("           Total de %i plantas cadastradas!\n\n", qtdCadastradas);
        printf(">CONFIGURACOES:\n");
        printf("Digite o numero correspondente a opcao desejada:\n\n");
        printf("1.Ver plantas cadastradas\n");
        printf("2.Cadastrar nova planta\n");
        printf("3.Deletar planta ja cadastrada\n");
        printf("4.Voltar ao menu principal (tela inicial)\n");
        printf("0.Fechar o programa (SAIR)\n");

        if(opcao != '0')
            opcao = fgetc(stdin);
        fflush(stdin);

        switch (opcao)
        {
        case '1' : //ver plantas cadastradas
            system("cls");
            printf(">>VER PLANTAS CADASTRADAS:\n");

            if (head == NULL)
            {
                printf("\a\nNÆo ha plantas cadastradas!!!\n\n");
            }
            else
            {

                for(aux = head; aux != NULL ; aux = aux->prox)
                {
                    printf("\n              Nome Cientifico: '%s'\n", aux->nc);
                    printf("                  Nome vulgar: '%s'\n", aux->nv);
                    printf("                      Familia: '%s'\n", aux->f1);
                    if (aux->f2[0] != '\0')
                        printf("                  F.Sinonimia: '%s'\n", aux->f2);
                    if (aux->f3[0] != '\0')
                        printf("                  F.Sinonimia: '%s'\n", aux->f3);
                    if (aux->f4[0] != '\0')
                        printf("                  F.Sinonimia: '%s'\n", aux->f4);
                    printf(" Quantidade de fotos sem flor: %i\n", aux->qtd_foto_folha);
                    printf("  Quantidade de fotos da flor: %i\n", aux->qtd_foto_flor);
                    printf("Quantidade de fotos do tronco: %i\n\n", aux->qtd_foto_tronco);

                }
            }
            printf("Aperte ENTER para voltar ao menu de configuracoes\n");
            opcao = fgetc(stdin);
            fflush(stdin);
            opcao = '6';
            break;
        case '2': //cadastrar plantas
            system("cls");
            printf(">>CADASTRAR PLANTA\n");
            printf("OBS.: SerÆo cadastradas somente plantas com foto, leia o arquivo \"LEIA-ME.txt\"\n");
            printf("OBS2: O programa nÆo reconhece acentos e cedilha. Use somente letras e espaco\n");
            printf("      (hifens e apostrofos sÆo aceitos para nomes vulgares)\n");
            printf("OBS3: O arquivo \"arvoresCadastradas.alf\" sera modificado permanentemente\n");
            printf("\nDigite o nome cientifico da planta, ex. Trichocereus pachanoi:\n");
            gets(nc);
            i = 0;
            while( (corrigeNome(nc) <= 0  || corrigeNome(nc) > 1 ) && i == 0)
            {
                printf("\aNome \"%s\" invalido!!!\n Tente novamente ou tecle '0' para voltar ao menu de configuracoes:\n", nc);
                gets(nc);
                if (nc[0] == '0')
                    i = 1;
            }

            if (i == 0)
            {
                printf("\nDigite o nome vulgar da planta, ex. Jurema-Preta:\n");
                gets(nv);

                while( (corrigeNome(nv) == -1) && i == 0)
                {
                    printf("\aNome \"%s\" invalido!!!\n Tente novamente ou tecle '0' para voltar ao menu de configuracoes:\n", nv);
                    gets(nv);
                    if (f1[0] == '0')
                        i = 1;
                }
            }
            if (i == 0)
            {
                printf("\nDigite o nome da Familia da planta, ex. Cannabaceae:\n");
                gets(f1);

                while( (corrigeNome(f1) < 0 || corrigeNome(f1) > 1) && i == 0)
                {
                    printf("\aFamilia \"%s\" invalida!!!\n Tente novamente ou tecle '0' para voltar ao menu de configuracoes:\n", f1);
                    gets(f1);
                    if (f1[0] == '0')
                        i = 1;
                }
            }

            if (i == 0)
            {
                j = contaFotoFolhas(nc);
                if (j)
                {
                    head = criaPlantas(nc,nv,f1,"","","", head,j, &i);
                    if (i == 1)
                    {
                        printf("\a\nA planta nÆo pode ser cadastrada por que nÆo ha memoria disponivel!!!\n");
                    }
                    else if (i == 0)
                    {
                        i = escreveDataBase(head);
                        if( i != 1)
                        {
                            printf("\nPlanta abaixo cadastrada com sucesso!\n");
                            printf("NC: '%s'\n", nc);
                            printf("NV: '%s'\n", nv);
                            printf("F:  '%s'\n", f1);
                            qtdCadastradas++;
                        }
                        else
                            printf("\a\nErro!!! NÆo foi possivel criar o arquivo 'arvoresCadastradas.txt'!\n");


                    }
                    else
                        printf("\a\nErro!!! Planta ja estava cadastrada!\n");
                }
                else
                    printf("\a\nA planta nÆo pode ser cadastrada por que nÆo ha fotos disponiveis!!!\n");

            }
            i= 0;
            printf("\nAperte ENTER para voltar ao menu de configuracoes\n");
            opcao = fgetc(stdin);
            fflush(stdin);
            opcao = '6';
            break;
        case '0':
            printf("\nFechando programa...\n");
            break;
        case '3':
            system("cls");
            printf(">>DELETAR PLANTA\n\n");
            printf("OBS.: O arquivo \"arvoresCadastradas.alf\" sera modificado permanentemente\n");
            printf("OBS2: Antes de deletar, leia o arquivo \"LEIA-ME.txt\"\n");

            if (head == NULL)
            {
                printf("\aNÆo ha plantas cadastradas!!!\n");
            }
            else
            {
                printf("Digite o nome cientifico da planta, ex. Trichocereus pachanoi:\n");
                gets(nc);
                i = 0;
                while( (corrigeNome(nc) <= 0  || corrigeNome(nc) > 1 ) && i == 0)
                {
                    printf("\aNome \"%s\" invalido!!!\n Tente novamente ou tecle '0' para voltar ao menu de configuracoes:\n", nc);
                    gets(nc);
                    if (nc[0] == '0')
                        i = 1;
                }

                if (i==0)
                {
                    head = deletaPlanta(nc,head,&i);
                }

                if (i == 0)
                {
                    i = escreveDataBase(head);
                    if( i != 1)
                    {
                        printf("\nPlanta abaixo deletada com sucesso!\n");
                        printf("NC: '%s'\n", nc);
                        qtdCadastradas--;
                    }
                    else
                        printf("\a\nErro!!! NÆo foi possivel criar o arquivo 'arvoresCadastradas.txt'!\n");

                }
                else
                    printf("\a\nErro!!! Planta nÆo estava cadastrada!\n");
            }

            printf("\nAperte ENTER para voltar ao menu de configuracoes\n");
            opcao = fgetc(stdin);
            fflush(stdin);
            opcao = '6';
            break;
        case '4':
            rodar_allegro = true;
            break;
        } //fim do switch do menu configurações
    } //fim do laço do menu

    finalizar();
    freePlantas(head);

    return 0;
}

bool inicializar()
{


    if (! (al_init()) )
    {

        fprintf(stderr, "Falha ao inicializar a biblioteca Allegro.\n");
        return false;
    }

    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar teclado.\n");
        return false;
    }

    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar allegro_image.\n");
        return false;
    }

    al_init_font_addon();

    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar allegro_ttf.\n");
        return false;
    }

    fila_eventos = al_create_event_queue();

    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        return false;
    }

    return true;
}

bool carregar_arquivos()
{
    fundo = al_load_bitmap("files\\bg.jpg");
    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar \"files\\bg.jpg\".\n");
        return false;
    }

    seta = al_load_bitmap("files\\arrow.jpg");
    if (!seta)
    {
        fprintf(stderr, "Falha ao carregar \"files\\arrow.jpg\".\n");
        return false;
    }

    fonte = al_load_font("files\\engravrl.ttf", 12, 0);
    if (!fonte)
    {
        fprintf(stderr, "Falha ao carregar \"files\\engravrl.ttf\".\n");
        return false;
    }

    fonte2 = al_load_font("files\\engravrl.ttf", 24, 0);
    if (!fonte2)
    {
        fprintf(stderr, "Falha ao carregar \"files\\engravrl.ttf\".\n");
        return false;
    }

    return true;
}

void finalizar()
{
    al_destroy_bitmap(fundo);
    al_destroy_bitmap(fundo2);
    al_destroy_bitmap(seta);
    al_destroy_font(fonte);
    al_destroy_font(fonte2);
    al_destroy_event_queue(fila_eventos);
}

void manipular_entrada(ALLEGRO_EVENT evento, char *str)
{
    if (evento.type == ALLEGRO_EVENT_KEY_CHAR)
    {
        if (strlen(str) <= 299)
        {
            char temp[] = {evento.keyboard.unichar, '\0'};
            if (evento.keyboard.unichar == ' ')
            {
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar >= 'A' &&
                     evento.keyboard.unichar <= 'Z')
            {
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar >= 'a' &&
                     evento.keyboard.unichar <= 'z')
            {
                strcat(str, temp);
            }
            else if (evento.keyboard.unichar == '-' ||
                     evento.keyboard.unichar == ' ' ||
                     evento.keyboard.unichar == '\'')
            {
                strcat(str, temp);
            }

        }

        if (evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && strlen(str) != 0)
        {
            str[strlen(str) - 1] = '\0';
        }
    }
}

int contaFotoFolhas (char nc[]) //conta qtos fotos de folha essa planta tem
{
    int contador;
    char nomeArquivo[500];

    contador = 0;
    sprintf(nomeArquivo, "photos\\%s_L%i.jpg", nc, contador);
    leaf = al_load_bitmap(nomeArquivo);
    while (leaf)
    {
        al_destroy_bitmap(leaf);
        contador++;
        sprintf(nomeArquivo, "photos\\%s_L%i.jpg", nc, contador);
        leaf = al_load_bitmap(nomeArquivo);
    }

    if (leaf)
        al_destroy_bitmap(leaf);

    return 1;
}
int contaFotoFlor (char nc[]) //conta qtos fotos de flor essa planta tem
{
    int contador;
    char nomeArquivo[500];

    contador = 0;
    sprintf(nomeArquivo, "photos\\%s_F%i.jpg", nc, contador);
    flower = al_load_bitmap(nomeArquivo);
    while (flower)
    {
        al_destroy_bitmap(flower);
        contador++;
        sprintf(nomeArquivo, "photos\\%s_F%i.jpg", nc, contador);
        flower = al_load_bitmap(nomeArquivo);
    }

    if (flower)
        al_destroy_bitmap(flower);

    return contador;
}

int contaFotoTronco (char nc[]) //conta quantas fotos de tronco
{
    int contador;
    char nomeArquivo[500];

    contador = 0;
    sprintf(nomeArquivo, "photos\\%s_T%i.jpg", nc, contador);
    trunk = al_load_bitmap(nomeArquivo);
    while (trunk)
    {
        al_destroy_bitmap(trunk);
        contador++;
        sprintf(nomeArquivo, "photos\\%s_T%i.jpg", nc, contador);
        trunk = al_load_bitmap(nomeArquivo);
    }

    if (trunk)
        al_destroy_bitmap(trunk);

    return contador;
}

planta *criaPlantas (char nc[], char nv[], char fam[],char f2[], char f3[], char f4[], planta *head, int picsLeaf, int *erro) //somente criar plantas que tenham ao menos uma foto de folha
{
    //mandar somente head com NULL ou ja allocada
    //se algum malloc der erro, variavel "erro" recebe 1
    planta *temp;
    int aux;

    erro[0] = 0;

    if(head != NULL)
    {
        aux = strcmp(head->nc,nc);

        if (aux < 0 )
        {
            head->prox = criaPlantas(nc,nv,fam,f2,f3,f4,head->prox,picsLeaf,erro);
            temp = head;
        }
        else if (aux > 0)
        {
            temp = (planta*) malloc(sizeof(planta));
            if (temp != NULL)
            {
                strcpy(temp->nc, nc);
                strcpy(temp->nv, nv);
                strcpy(temp->f1, fam);
                strcpy(temp->f2, f2);
                strcpy(temp->f3, f3);
                strcpy(temp->f4, f4);

                temp->qtd_foto_flor = contaFotoFlor(nc);
                temp->qtd_foto_folha = picsLeaf;
                temp->qtd_foto_tronco = contaFotoTronco(nc);

                temp->prox = head;
            }
            else
            {
                erro[0]= 1;
                temp = head;
            }
        }
        else   //caso nc == head->nc
        {
            erro[0] = 2;
            temp = head;
        }

    }
    else   //if head == NULL
    {
        temp = (planta*) malloc(sizeof(planta));
        if (temp != NULL)
        {
            strcpy(temp->nc, nc);
            strcpy(temp->nv, nv);
            strcpy(temp->f1, fam);
            strcpy(temp->f2, f2);
            strcpy(temp->f3, f3);
            strcpy(temp->f4, f4);

            temp->qtd_foto_flor = contaFotoFlor(nc);
            temp->qtd_foto_tronco = contaFotoTronco(nc);
            temp->qtd_foto_folha = picsLeaf;

            temp->prox = NULL;
        }
        else
        {
            erro[0]= 1;
            temp = NULL;
        }
    }

    return temp;
}

planta *lerDataBase (int *erro, int *qtdCadastradas)
{
    int pics, unused;
    char nc[300]; //nome científico
    char nv[300]; //nome vulgar
    char f1[300]; // familia + subfamilia1
    char f2[300]; // familia + subfamilia2
    char f3[300]; // familia1
    char f4[300]; // familia2
    planta *head;
    erro[0] = 0;
    FILE *arqv;
    head = NULL;

    arqv = fopen("arvoresCadastradas.alf", "r");
    if (arqv != NULL)
    {
        fscanf(arqv,"Leia o arquivo 'LEIA-ME.txt' antes de alterar os dados desse arquivo!\n\n");

        while (fscanf(arqv, "nc: %s\n", nc) != EOF)
        {
            fscanf(arqv, "nv: %s\n", nv);
            fscanf(arqv, "f1: %s\n", f1);
            fscanf(arqv, "f2: %s\n", f2);
            fscanf(arqv, "f3: %s\n", f3);
            fscanf(arqv, "f4: %s\n\n", f4);


            pics = contaFotoFolhas(nc);

            if (pics)
            {
                strcpy(nc,espacoPorUnderline(nc));
                strcpy(nv,espacoPorUnderline(nv));
                strcpy(f1,espacoPorUnderline(f1));

                if (f2[0] == '-')
                    f2[0] = '\0';
                else
                    strcpy(f2,espacoPorUnderline(f2));
                if (f3[0] == '-')
                    f3[0] = '\0';
                else
                    strcpy(f3,espacoPorUnderline(f3));
                if (f4[0] == '-')
                    f4[0] = '\0';
                else
                    strcpy(f4,espacoPorUnderline(f4));

                head = criaPlantas(nc,nv,f1,f2,f3,f4,head,pics, &unused);
                if (!unused)
                    *qtdCadastradas = (*qtdCadastradas) + 1;
            }

        }
    }
    else
        erro[0] = 1;

    return head;
}

void mostraPlanta(char *nc, int l, int f, int t, int opcao)
{
    char arquivo[500];

    if (leaf)
        al_destroy_bitmap(leaf);
    sprintf(arquivo, "photos\\%s_L%i.jpg", nc, l);
    leaf = al_load_bitmap(arquivo);

    al_draw_bitmap(leaf,256,0,0);

    switch(opcao)
    {
    case 1:
        if (flower)
            al_destroy_bitmap(flower);
        strcpy(arquivo,"");
        sprintf(arquivo, "photos\\%s_F%i.jpg", nc, f);
        flower = al_load_bitmap(arquivo);
        al_draw_bitmap(flower,0,128,0);
        break;
    case 2:
        if (trunk)
            al_destroy_bitmap(trunk);
        strcpy(arquivo,"");
        sprintf(arquivo, "photos\\%s_T%i.jpg", nc, t);
        trunk = al_load_bitmap(arquivo);
        al_draw_bitmap(trunk,768,0,0);
        break;
    case 3:
        if (trunk)
            al_destroy_bitmap(trunk);
        strcpy(arquivo,"");
        sprintf(arquivo, "photos\\%s_T%i.jpg", nc, t);
        trunk = al_load_bitmap(arquivo);
        if (flower)
            al_destroy_bitmap(flower);
        strcpy(arquivo,"");
        sprintf(arquivo, "photos\\%s_F%i.jpg", nc, f);
        flower = al_load_bitmap(arquivo);
        al_draw_bitmap(flower,0,128,0);
        break;

    }
}

