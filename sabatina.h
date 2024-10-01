#include <stdlib.h>
#include <stdio.h>


typedef struct plantaEstr
{
    char nc[300]; //nome científico
    char nv[300]; //nome vulgar
    char f1[300]; // familia + subfamilia1
    char f2[300]; // familia + subfamilia2
    char f3[300]; // familia1
    char f4[300]; // familia2


    int qtd_foto_folha;
    int qtd_foto_flor;
    int qtd_foto_tronco;

    struct plantaEstr *prox;
} planta;

int corrigeNome (char nc[]) //corrige os nomes para formato cientifico e retorna qtd de espaço (' ') na string, negativo se n conseguir corrijir
{
    int i,qtdEspaco = 0;

    if ((nc[0] >= 'a') && (nc[0] <= 'z'))
        nc[0] = nc[0] - 'a' + 'A';
    else if (!((nc[0] >= 'A') && (nc[0] <= 'Z')))
        qtdEspaco--;

    if (qtdEspaco >= 0) for(i = 1; nc[i] != '\0'; i++)
        {

            if ((nc[i] >= 'A') && (nc[i] <= 'Z'))
            {
                nc[i] = nc[i] + ('a' - 'A');
            }
            else if (!((nc[i] >= 'a') && (nc[i] <= 'z')))
            {

                if (nc[i] == ' ')
                {
                    if (nc[i+1] == '\0')
                    {
                        nc[i] = '\0';
                        i--;
                        i--;
                    }
                    else
                        qtdEspaco++;
                } else if (nc[i] == '-' || nc[i] == '\'')
                    qtdEspaco = -2;
                else
                    qtdEspaco = -1;
            }
        }

    return qtdEspaco;
}

planta *deletaPlanta (char nc[], planta *head, int *erro)
{
    planta *temp;
    int aux;

    temp = NULL;

    if(head != NULL)
    {
        aux = strcmp(head->nc,nc);
        if (aux == 0)
        {
            temp = head->prox;
            free(head);
        }
        else if (aux < 0)
        {
            head->prox = deletaPlanta(nc,head->prox,erro);
            temp = head;
        }
        else
        {
            erro[0] = 1; //planta n está cadastrada
            temp = head;
        }
    }
    else
        erro[0] = 1; //planta n está cadastrada

    return temp;
}

char *espacoPorUnderline (char nome[])
{
    int i;
    char *returno;

    returno = NULL;
    returno = (char *) malloc(300*sizeof(char));

    for (i = 0; nome[i] != '\0' ; i++)
    {
        returno[i] = nome[i];
        if (nome[i] == ' ')
        {
            if (nome[i+1] == '\0')
            {
                nome[i] = '\0';
                returno[i] = '\0';
                i--;
                i--;
            }
            else
                returno[i] = '_';
        }
        else if (nome[i] == '_')
        {
            if (nome[i+1] == '\0')
            {
                nome[i] = '\0';
                returno[i] = '\0';
                i--;
                i--;
            }
            else
                returno[i] = ' ';
        }
    }

    returno[i] = '\0';

    return returno;
}

int escreveDataBase (planta *head)
{
    int retorno = 0;
    FILE *arqv;
    planta *aux;

    if (head != NULL)
    {
        arqv = fopen("arvoresCadastradas.alf", "w");
        if (arqv != NULL)
        {
            fprintf(arqv,"Leia o arquivo 'LEIA-ME.txt' antes de alterar os dados desse arquivo!\n\n");

            for(aux=head; aux != NULL; aux = aux->prox)
            {
                fprintf(arqv, "nc: %s\n", espacoPorUnderline(aux->nc));
                fprintf(arqv, "nv: %s\n", espacoPorUnderline(aux->nv));
                fprintf(arqv, "f1: %s\n", espacoPorUnderline(aux->f1));
                if (aux->f2[0] != '\0')
                    fprintf(arqv, "f2: %s\n", espacoPorUnderline(aux->f2));
                else
                    fprintf(arqv, "f2: -\n");
                if (aux->f3[0] != '\0')
                    fprintf(arqv, "f3: %s\n", espacoPorUnderline(aux->f3));
                else
                    fprintf(arqv, "f3: -\n");
                if (aux->f4[0] != '\0')
                    fprintf(arqv, "f4: %s\n\n", espacoPorUnderline(aux->f4));
                else
                    fprintf(arqv, "f4: -\n\n");

            }

            fclose(arqv);
        }
        else
            retorno = 1;
    }
    else
        retorno =2;

    return retorno;
}

void freePlantas (planta *head)
{
    if (head != NULL)
    {
        freePlantas(head->prox);
        free(head);
    }
}

planta *sorteiaplantas (planta *head, int *sorteadas, int qtdcadastradas, int *l, int *f, int *t)
{ //chamar essa função somente depois de usar srand() e de preencher um o vetor sorteadas[5] com -1
    planta *resultado;
    int sorte, i, j;

    sorte = rand()%qtdcadastradas;

    for (i= 0; sorteadas[i] != -1; i++)
    {
        if (i==4) return NULL;
    }

    for (j = 0; j < i;)
    {
        if (sorteadas[j] == sorte)
        {
            sorte = ((sorte+1)*sorte)%qtdcadastradas;
            j = 0;
        } else j++;
    }

    sorteadas[i] = sorte;

    resultado = head;
    for(i=0; i < sorte; i++)
    {
        resultado = head->prox;
    }

    *l = rand()%resultado->qtd_foto_folha;
    *f = rand()%resultado->qtd_foto_flor;
    *t = rand()%resultado->qtd_foto_tronco;

    return resultado;
}
