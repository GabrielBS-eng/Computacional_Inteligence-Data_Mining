#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "criacao-da-populacao.h"

typedef struct gene{
float peso;
int operador,valor;
}gene;

typedef struct individuo
{
    gene fita[35];
    float fitness;
}individuo;

individuo populacao[50];
individuo populacaoFilhos[50];

void criaPopulacao(int tamanho);
void imprime();
void crossover(int p1,int p2,int indiceCross);
void mutacao(int ind);
void ordena(int popCase);
void elitismoP();

void ordena(int popCase)
{
    int i,j,menor;
    individuo aux;
    for(i=0;i<50;i++)
    {
        menor=i;
        for(j=i;j<50;j++)
        {
            if(popCase==1)// ordena a populacao
            {
            if(populacao[j].fitness<populacao[menor].fitness)
                {aux=populacao[j];
                populacao[j]=populacao[menor];
                populacao[menor]=aux;
                menor=j;
                }
            }
            else // ordena a populacao gerada por crossover
            {
            if(populacaoFilhos[j].fitness<populacaoFilhos[menor].fitness)
                {aux=populacaoFilhos[j];
                populacaoFilhos[j]=populacaoFilhos[menor];
                populacaoFilhos[menor]=aux;
                menor=j;
                }
            }
        }
    }
}

void criaPopulacao(int tamanho)
{
    int x=0,i;
    while(x<tamanho)
    {
        for(i=0;i<35;i++)
        {
            //peso de ativa�ao com 3 casas decimais entre 0 e 1.
            populacao[x].fita[i].peso=(rand()%1000)/1000.0;

            if(i!=10)
                populacao[x].fita[i].operador=rand()%4; //campo operador dos genes.
            else
                populacao[x].fita[i].operador=rand()%2; //campo operador do gene historico familiar.

            if(i==10)populacao[x].fita[i].valor=rand()%2; //historico familiar � binario.
            if(i==33)populacao[x].fita[i].valor=rand()%76+1; //gene idade.
            if(i==34)
            {
                populacao[x].fita[i].peso=1;
                populacao[x].fita[i].operador=0;
                populacao[x].fita[i].valor=(rand()%6)+1; //gene que contem o tipo.
            }
        }
        x++;
    }
}

void imprime()
{
    int x,i;
    for(x=0;x<50;x++)
    {
        for(i=0;i<35;i++)
        {
            if(populacao[x].fita[i].peso<0.3) // apenas os pesos menos que 0.3 ativam, se quiser pode ser >0.7 | de qualquer jeito passao 30%
            {
            //printf("- %.3f",populacao[x].fita[i].peso); // se quiser ver o peso
            printf("gene %d",i); //indica o gene.
            if(populacao[x].fita[i].operador==0)
            printf("== %d ",populacao[x].fita[i].operador,populacao[x].fita[i].valor);
            if(populacao[x].fita[i].operador==1)
            printf("!= %d ",populacao[x].fita[i].operador,populacao[x].fita[i].valor);
            if(populacao[x].fita[i].operador==2)
                printf(">= %d ",populacao[x].fita[i].operador,populacao[x].fita[i].valor);
            if(populacao[x].fita[i].operador==3)
            printf("< %d ",populacao[x].fita[i].operador,populacao[x].fita[i].valor);
            printf(" and ");
            }
        }
        printf("\n-----------------------------\n");
    }
}

void crossover(int p1,int p2,int indiceCross)
{
    individuo f1,f2;
    int a,b,i;
    a=rand()%34;+1;
    b=rand()%(34-a);
    for(i=0;i<34;i++)
    {
        if(i<a)
        {
            f1.fita[i]=populacao[p1].fita[i];
            f2.fita[i]=populacao[p2].fita[i];
        }
        else{
        if(i<a+b)
        {
            f2.fita[i]=populacao[p1].fita[i];
            f1.fita[i]=populacao[p2].fita[i];
        }
        else
        {
            f1.fita[i]=populacao[p1].fita[i];
            f2.fita[i]=populacao[p2].fita[i];
        }
        }
    }
    populacaoFilhos[indiceCross]=f1;
    populacaoFilhos[indiceCross+1]=f2;
    mutacao(indiceCross);
    mutacao(indiceCross);
}

void mutacao(int ind)
{
    int a,i;

    for(i=0;i<34;i++)
    {
		//mutacao de 30% em cada campo de todos os individuos gerados
        if(rand()%100<30)
        {
            int k=rand()%2;
            if(k==0)
            populacaoFilhos[ind].fita[i].peso-=(rand()%10000)/10000.0;
            else
                populacaoFilhos[ind].fita[i].peso+=(rand()%10000)/10000.0;
            if(populacaoFilhos[ind].fita[i].peso<0)populacaoFilhos[ind].fita[i].peso=0;
            if(populacaoFilhos[ind].fita[i].peso>1)populacaoFilhos[ind].fita[i].peso=1;
        }
        if(rand()%100<30)
			{
                if(i!=10)populacaoFilhos[ind].fita[i].operador=rand()%4;
                else populacaoFilhos[ind].fita[i].operador=rand()%2;
            }
        if(rand()%100<30)
        {
                if(i==34)
                {
                 populacaoFilhos[ind].fita[i].valor=rand()%76+1;
                }
                else{
            if(i!=10)populacaoFilhos[i].fita[i].valor=rand()%4;
                else populacaoFilhos[i].fita[i].valor=rand()%2;
                }
        }
    }
}

 void elitismoP() // elitismo com apenas o melhor
 {
     int i;
     for(i=1;i<50;i++)
        populacao[i]=populacaoFilhos[i];
 }
