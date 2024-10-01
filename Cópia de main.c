#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "sabatina.h"
#include <windows.h>


int contaFotoFolhas (char nc[]) //conta qtos fotos de folha essa planta tem
{
    return 1;
}
int contaFotoFlor (char nc[]) //conta qtos fotos de flor essa planta tem
{
    return 0;
}

int FotoTronco (char nc[]) //ver se tem foto do tronco
{
    return 0;
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
                temp->foto_tronco = FotoTronco(nc);

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

int main()
{
    planta *head, *aux;
    char nv[300], nc[300], f1[300], opcao = '5';
    int i, j, qtdCadastradas = 0;

    SetConsoleTitle("Sabatina Alfa - por Alfran Carvalho");

    head = lerDataBase(&i,&qtdCadastradas);

    while (opcao != '0')
    {
        system("cls");
        printf(".dP\"Y8    db    88\"\"Yb    db    888888 88 88b 88    db\n");
        printf("`Ybo.\"   dPYb   88__dP   dPYb     88   88 88Yb88   dPYb\n");
        printf("o.`Y8b  dP__Yb  88\"\"Yb  dP__Yb    88   88 88 Y88  dP__Yb\n");
        printf("8bodP' dP\"\"\"\"Yb 88oodP dP\"\"\"\"Yb   88   88 88  Y8 dP\"\"\"\"Yb\n");

        printf("               db    88     888888   db    \n");
        printf("              dPYb   88     88__    dPYb\n");
        printf("             dP__Yb  88  .o 88\"\"   dP__Yb\n");
        printf("            dP\"\"\"\"Yb 88ood8 88    dP\"\"\"\"Yb\n");
        printf("           Total de %i plantas cadastradas!\n\n", qtdCadastradas);
        printf("MENU PRINCIPAL:\n");
        printf("Digite o numero correspondente a opcao desejada:\n\n");
        printf("1.Comecar Sabatina!\n");
        printf("2.Configuracoes\n");
        printf("0.Fechar program (SAIR)\n");

        opcao = fgetc(stdin);
        fflush(stdin);

        switch (opcao)
        {
        case '0' :
            printf("\nFechando programa...\n");
            break;

        case '1' : //começar sabatina
            break;

        case '2' : //opcoes
            while (opcao != '0')
            {
                system("cls");
                printf(">CONFIGURACOES:\n");
                printf("Digite o numero correspondente a opcao desejada:\n\n");
                printf("1.Ver plantas cadastradas\n");
                printf("2.Cadastrar nova planta\n");
                printf("3.Deletar planta ja cadastrada\n");
                printf("0.Voltar ao menu principal\n");
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
                            printf("\n             Nome Cientifico: '%s'\n", aux->nc);
                            printf("                 Nome vulgar: '%s'\n", aux->nv);
                            printf("                     Familia: '%s'\n", aux->f1);
                            if (aux->f2[0] != '\0')
                                printf("                 F.Sinonimia: '%s'\n", aux->f2);
                            if (aux->f3[0] != '\0')
                                printf("                 F.Sinonimia: '%s'\n", aux->f3);
                            if (aux->f4[0] != '\0')
                                printf("                 F.Sinonimia: '%s'\n", aux->f4);
                            printf("Quantidade de fotos sem flor: %i\n", aux->qtd_foto_folha);
                            printf(" Quantidade de fotos da flor: %i\n", aux->qtd_foto_flor);
                            printf("          Tem foto do tronco: ");
                            if (aux->foto_tronco)
                                printf("Sim\n\n");
                            else
                                printf("NÆo\n\n");

                        }
                    }
                    printf("Aperte ENTER para voltar ao menu principal\n");
                    opcao = fgetc(stdin);
                    fflush(stdin);
                    opcao = '0';
                    break;
                case '2': //cadastrar plantas
                    system("cls");
                    printf(">>CADASTRAR PLANTA\n");
                    printf("OBS.: SerÆo cadastradas somente plantas com foto, leia o arquivo \"LEIA-ME.txt\"\n");
                    printf("OBS2: Use somente letras e espaço (hifens e apostrofos sÆo aceitos para nomes vulgares)\n");
                    printf("OBS3: O arquivo \"arvoresCadastradas.alf\" sera modificado permanentemente\n");
                    printf("\nDigite o nome cientifico da planta, ex. Trichocereus pachanoi:\n");
                    gets(nc);
                    i = 0;
                    while( (corrigeNome(nc) <= 0  || corrigeNome(nc) > 1 ) && i == 0)
                    {
                        printf("\aNome \"%s\" invalido!!!\n Tente novamente ou tecle '0' para voltar ao menu principal:\n", nc);
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
                            printf("\aNome \"%s\" invalido!!!\n Tente novamente ou tecle '0' para voltar ao menu principal:\n", nv);
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
                            printf("\aFamilia \"%s\" invalida!!!\n Tente novamente ou tecle '0' para voltar ao menu principal:\n", f1);
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
                    printf("\nAperte ENTER para voltar ao menu principal\n");
                    opcao = fgetc(stdin);
                    fflush(stdin);
                    opcao = '0';
                    break;
                case '0':
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
                            printf("\aNome \"%s\" invalido!!!\n Tente novamente ou tecle '0' para voltar ao menu principal:\n", nc);
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

                    printf("\nAperte ENTER para voltar ao menu principal\n");
                    opcao = fgetc(stdin);
                    fflush(stdin);
                    opcao = '0';
                    break;

                }
            } //fim do laço de opções de configuração

            opcao = '6';
            break;



        }
    }

    return 0;
}
