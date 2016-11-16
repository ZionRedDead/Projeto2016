#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define tanconta 20

FILE * criarArquivo(char nome[])
{
    FILE *arq;

    arq = fopen(nome, "r+b");
    if(arq == NULL)
    {
        arq = fopen(nome, "w+b");
    }
    return arq;
}


typedef struct contaBanco
{
    char conta[tanconta];
    float saldo;
    char nome[100];
    char cpf[12];
    char fone[10];
    int status;
} Tconta;

int busca(FILE *arq, char numCont[]);
void cadastrar(FILE *arq, char numCont[]);
void consultaSaldo(FILE *arq, char numCont[]);
void deposito(FILE *arq, char numCont[]);
void saque(FILE *arq, char numCont[]);
void exibir(FILE *arq);
void remover(FILE *arq, char numCont[]);
void manutencao (FILE * arq);

int main()
{
    FILE *arq;

    int pos = 0, existe = 0;
    char op, numConta[tanconta], resp, resp2, nomeArq[] = "Banco.dat";
    arq = criarArquivo(nomeArq);
    if(arq == NULL)
    {

        printf("Erro ao tentar criar ou abrir Arquivo: %s\n", nomeArq);
    }
    else
    {

        printf("\n\n******** BANCO DINHEIRO CERTO ********\n");
        printf("Cadastro de Clientes: ");

        do
        {
            printf("\n-  Menu  -\n\n");
            printf("1 - Buscar Conta\n2 - Cadastrar Nova Conta\n3 - Consultar Saldo\n4 - Deposito\n5 - Saque\n6 - Exibir\n7 - Remover Conta\n8 - Sair\n");
            printf("Informe sua opcao: ");
            op = getchar();
            fflush(stdin);
            switch(op)
            {
            case '1':
                do
                {
                    printf("\nInforme o numero da conta: ");
                    gets(numConta);
                    fflush(stdin);
                    printf("\nNo Conta: %s\n", numConta);
                    pos = busca(arq, numConta);

                    if(pos == -2)
                    {
                        printf("Erro de Leitura.\n");
                    }
                    else if(pos == -1)
                    {
                        printf("A conta nao existe.\n");
                    }
                    else
                    {
                        printf("A conta se encontra na posicao: %d\n", pos);

                    }
                    printf("Deseja Pesquisar Outra Conta?(S / N)");
                    resp = toupper(getchar());
                    fflush(stdin);

                }
                while(resp == 'S');
                break;

            case '2':
                do
                {
                    printf("Informe os dados solicitados: \n");
                    printf("Numero da conta: ");
                    gets(numConta);
                    fflush(stdin);
                    existe = busca(arq, numConta);
                    if(existe == -2)
                    {
                        printf("Erro de Leitura!\n");
                        // break;
                    }
                    else if(existe == -1)
                    {

                        cadastrar(arq, numConta);


                    }
                    else
                    {
                        printf("Numero de Conta Ja Existe!\n");
                    }
                    printf("Deseja cadastrar outra conta?(S / N)");
                    resp = toupper(getchar());
                    fflush(stdin);
                }
                while(resp == 'S');

                break;

            case '3':
                do
                {
                    printf("\n- CONSULTA SALDO -\n");
                    printf("Informe o Numero da conta: ");
                    gets(numConta);
                    fflush(stdin);
                    consultaSaldo(arq, numConta);
                    printf("Deseja consultar outro Saldo? (S / N)");
                    resp = toupper(getchar());
                    fflush(stdin);
                }
                while(resp == 'S');
                break;

            case '4':
                do
                {
                    printf("\n- DEPOSITO -\n");
                    printf("Informe o numero da conta: ");
                    gets(numConta);
                    fflush(stdin);
                    deposito(arq, numConta);
                    printf("\nDeseja Realizar outro Deposito? (S / N)");
                    resp2 = toupper(getchar());
                    fflush(stdin);
                }
                while(resp2 == 'S');
                break;

            case '5':
                do
                {
                    printf("\n- SAQUE -\n");
                    printf("Informe o numero da conta: ");
                    gets(numConta);
                    fflush(stdin);
                    saque(arq, numConta);
                    printf("\nDeseja Realizar outro Saque? (S / N)");
                    resp2 = toupper(getchar());
                    fflush(stdin);
                }
                while(resp2 == 'S');
                break;

            case '6':
                do
                {

                    printf("\n- EXIBIR -\n");
                    exibir(arq);
                    printf("\nDeseja Exibir Novamente? (S / N)");
                    resp2 = toupper(getchar());fflush(stdin);

                }
                while(resp2 == 'S');
                break;

            case '7':
                do
                {
                    printf("\n- REMOVER -\n");
                    printf("Informe o numero da conta a ser removida: ");
                    gets(numConta);
                    fflush(stdin);
                    remover(arq, numConta);
                    printf("Deseja Remover outra Conta? (S / N)?");
                    resp2 = toupper(getchar());
                    fflush(stdin);
                }
                while(resp2 == 'S');
                break;

            case '8':
                manutencao (arq);
                printf("Volte Sempre!\n");
                break;

            default :
                printf("\nOpcao Invalida! Favor digitar opcao correta.\n");

            }
        }
        while(op != '8');

        //printf("Hello world!\n");
        return 0;
    }
}
int busca(FILE * arq, char numConta[])
{
    int cont = -1, status;
    Tconta c;

    fseek (arq, 0, 0);
    while (1)
    {
        status = fread (&c, sizeof (Tconta), 1, arq);
        if (status != 1)
        {
            if (!feof(arq))
                return -2; // deu erro de leitura
            else
                return -1; // a conta ainda nao cadastrada
        }
        else
        {
            cont++;
            if (c.status == 1 && strcmp (c.conta, numConta) == 0)
                return cont;
        }
    }
}
void cadastrar(FILE *arq, char numCont[])
{
    Tconta c;
    int status;
    fseek(arq, 0, 2);
    strcpy(c.conta, numCont);
    printf("Nome: ");
    gets(c.nome);
    fflush(stdin);
    printf("CPF: ");
    gets(c.cpf);
    fflush(stdin);
    printf("Telefone: ");
    gets(c.fone);
    fflush(stdin);
    printf("Saldo: ");
    scanf("%f", &c.saldo);
    fflush(stdin);
    c.status = 1;
    status = fwrite(&c, sizeof(Tconta), 1, arq);
    if(status != 1)
    {
        printf("Erro de gravacao!\n");
    }
    else
    {
        printf("Conta cadastrada com sucesso!\n");
    }
}
void consultaSaldo(FILE *arq, char numCont[])
{
    Tconta c;
    int saldo = 0, status;

    saldo = busca(arq, numCont);

    if (saldo == -1)
    {
        printf("\nConta nao Cadastrada!\n\n");
    }
    else if(saldo == -2)
    {
        printf("Erro de leitura.\n");
    }
    else
    {
        fseek(arq, saldo * sizeof(Tconta), 0);
        status = fread(&c, sizeof(Tconta), 1, arq);
        if(status != 1)
        {
            printf("Erro de leitura!\n");
        }
        else
        {
            printf("\nA conta consultada foi: %s\n", c.conta);
            printf("\nE o Saldo atual: R$ %.2f\n\n", c.saldo);
        }
    }
}
void deposito(FILE *arq, char numCont[])
{
    Tconta c;
    int deppos = 0, status;
    float valor = 0;

    deppos = busca(arq, numCont);

    if (deppos == -1)
    {
        printf("\nConta Nao Cadastrada!\n\n");
    }
    else if( deppos == -2)
    {
        printf("Erro de leitura!\n");
    }
    else
    {
        fseek(arq, deppos * sizeof(Tconta), 0);
        status = fread(&c, sizeof(Tconta), 1, arq);
        if(status == -1)
        {
            printf("Erro de leitura!\n");
        }
        else
        {
            printf("\nA conta a ser creditada eh %s e o Saldo Atual eh: R$%.2f\n", numCont, c.saldo);
            printf("\nInforme o valor do deposito: ");
            scanf("%f", &valor);
            c.saldo = c.saldo + valor;
            printf("\nO Novo Saldo eh: R$%.2f\n\n", c.saldo);
            fseek(arq, -sizeof(Tconta), 1);
            status = fwrite(&c, sizeof(Tconta), 1, arq);
            if(status != 1)
            {
                printf("Erro de gravacao!\n");
            }
            else
            {
                printf("Deposito realizado com sucesso!\n");
            }
        }
    }
    fflush(stdin);
}
void saque(FILE *arq, char numCont[])
{
    Tconta c;
    int saque = 0, status;
    float valor = 0;

    saque = busca(arq, numCont);

    if(saque == -1)
    {
        printf("\nConta nao Cadastrada!\n\n");
    }
    else if(saque == -2)
    {
        printf("Erro de leitura!\n");
    }
    else
    {
        fseek(arq, saque * sizeof(Tconta), 0);
        status = fread(&c, sizeof(Tconta), 1, arq);
        printf("A conta a ser debitada eh: %s. Saldo Atual: R$%.2f\n", numCont, c.saldo);
        printf("Qual o Valor do Saque: ");
        scanf("%f", &valor);
        if(valor > c.saldo)
        {
            printf("\nVoce Nao Tem Saldo Suficiente!!!!\n\n");
        }
        else
        {
            c.saldo = c.saldo - valor;
            printf("O Novo Saldo eh: R$%.2f\n\n", c.saldo);
            fseek(arq, -sizeof(Tconta), 1);
            status = fwrite(&c, sizeof(Tconta), 1, arq);
            if(status != 1)
            {
                printf("Erro de Gravacao!\n");

            }
            else
            {
                printf("Saque Realizado com Sucesso!\n");
            }
        }
    }
    fflush(stdin);
}
void exibir(FILE *arq)
{
    Tconta c;
    int cont = 0, status;
    fseek(arq, 0, 0);

    while(!feof(arq))
    {
        status = fread(&c, sizeof(Tconta), 1, arq);
        if(status != 1)
        {
            if(!feof(arq))
            {
                printf("Erro de Leitura!\n");
                break;
            }
        }
        else
        {
            if(c.status == 1)
            {
                cont++;
                printf("Cliente no %d\n", cont);
                printf("Conta: no %s\n", c.conta);
                printf("Nome: %s\n", c.nome);
                printf("Telefone: %s\n", c.fone);
                printf("Saldo: R$%.2f\n\n", c.saldo);
            }
        }
    }
}
void remover(FILE *arq, char numCont[])
{
    Tconta c;
    int pos, status;

    pos = busca(arq, numCont);
    if(pos == -1)
    {
        printf("Conta Nao Cadastrada!\n");
    }
    else if(pos == -2)
    {
        printf("Erro de Leitura!\n");
    }
    else
    {
        fseek(arq, pos * sizeof(Tconta), 0);
        status = fread(&c, sizeof(Tconta), 1, arq);
        if(status != 1)
        {
            printf("Erro de Leitura!\n");
        }
        else
        {

            c.status = 0;
            fseek(arq, -sizeof(Tconta), 1);
            status = fwrite(&c, sizeof(Tconta), 1, arq);
            if(status != 1)
            {
                printf("Erro de Gravacao!\n");
            }
            else
            {
                printf("Conta Removida com Sucesso!\n");
            }
        }
    }
}
void manutencao (FILE * arq)
{
    int status;
    Tconta c;
    FILE * aux;

    aux = fopen ("Temporario", "w+b");
    if (aux == NULL)
    {
        printf ("Erro ao executar o processo de manutenção do arquivo \n");
        fclose (arq);
    }
    else
    {
        fseek (arq, 0, 0);
        while (1)
        {
            status = fread (&c, sizeof (Tconta), 1, arq);
            if (status != 1)
            {
                if (!feof(arq))
                {
                    printf ("Erro ao executar o processo de manutenção do arquivo \n");
                    fclose (arq);
                    fclose (aux);
                    remove ("Temporario");
                    return;
                }
                else
                    break;
            }
            else
            {
                if (c.status == 1)
                {
                    status = fwrite (&c, sizeof (Tconta), 1, aux);
                    if (status != 1)
                    {
                        printf ("Erro ao executar o processo de manutenção do arquivo \n");
                        fclose (arq);
                        fclose (aux);
                        remove ("Temporario");
                        return;

                    }
                }
            }
        }
        fclose (arq);
        fclose (aux);
        remove ("Banco.dat");
        rename ("Temporario", "Banco.dat");
    }
}

