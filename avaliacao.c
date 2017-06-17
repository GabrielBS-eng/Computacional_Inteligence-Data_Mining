#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "criacao-da-populacao.h"

#define INSTANCES 366 - 8
#define ATRIBUTES 35
#define CLASS_TO_ANALISE 1
#define WEIGHT_LIMIT 0.7

//operador 0 => Igual
//operador 1 => Diferente
//operador 2 => Maior ou Igual
//operador 3 => Menor

typedef struct gene{
float peso;
int operador,valor;
}gene;

typedef struct individuo
{
    gene fita[35];
    float fitness;
}individuo;

struct individuo populacao[50];
struct individuo populacaoFilhos[50];

void executaBasePopulacao();
void executaBaseIndividuo();
void get_database(int total_data[][ATRIBUTES]);

// |-------------------------------------------------------------
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

            populacao[x].fita[i].valor=rand()%4;
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
    int x,i, jump=0;
    for(x=0;x<50;x++)
    {
        for(i=0;i<35;i++)
        {
            if(populacao[x].fita[i].peso >= WEIGHT_LIMIT) // apenas os pesos menos que 0.3 ativam, se quiser pode ser >0.7 | de qualquer jeito passao 30%
            {
              jump = 1;
              // printf("- %.3f",populacao[x].fita[i].peso); // se quiser ver o peso
              printf("gen%d ",i); //indica o gene.
              if(populacao[x].fita[i].operador==0)
                printf("== %d ",populacao[x].fita[i].valor);
              if(populacao[x].fita[i].operador==1)
                printf("!= %d ",populacao[x].fita[i].valor);
              if(populacao[x].fita[i].operador==2)
                printf(">= %d ",populacao[x].fita[i].valor);
              if(populacao[x].fita[i].operador==3)
                printf("< %d ",populacao[x].fita[i].valor);
              //printf(" and ");
            }
            // if(populacao[x].fita[i].peso  WEIGHT_LIMIT)
            // {
            //   printf("\n");
            //   printf("\n-----------------------------\n");
            // }
        }
      if(jump==1)
      {
        printf("\t%f", populacao[x].fitness);
        printf("\n");
      }
      jump=0;
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
//|-------------------------------------------------------------------------------

void executaBasePopulacao(int total_data[][ATRIBUTES])
{
  int Tp = 0, Fp = 0, Tn = 0, Fn = 0;
  float Se, Sp;
  int good = 0, bad = 0;

  for(int i=0; i<50; i++) //Para cada individuo
  {
    for(int p=0; p<INSTANCES; p++) //Comparar com cada registro de caso clínico
    {
      for(int q=0; q<ATRIBUTES-1; q++) //Para cada atributo (sem a classe)
      {
        if(populacao[i].fita[q].peso >= WEIGHT_LIMIT) //O peso é maior que o limite
        {
          switch (populacao[i].fita[q].operador) //análise do operador
          {
            case 0:
              if(total_data[p][q] = populacao[i].fita[q].valor) good++;
              else bad++;
              break;
            case 1:
              if(total_data[p][q] != populacao[i].fita[q].valor) good++;
              else bad++;
              break;
            case 2:
              if(total_data[p][q] >= populacao[i].fita[q].valor) good++;
              else bad++;
              break;
            case 3:
              if(total_data[p][q] < populacao[i].fita[q].valor) good++;
              else bad++;
              break;
            default:
              break;
          }
        }
      }
      if(bad != 0) //Análise caso a regra foi desobedecida em algum ponto
      {
        if(total_data[p][34] != CLASS_TO_ANALISE) Tn++; //Regra desobedecida e classe não bateu (Verdadeiro negativo)
        else Fn++; //Regra desobedecida, mas classe bateu (Falso negativo)
      }
      else //Análise caso a regra deu certo
      {
        if(total_data[p][34] == CLASS_TO_ANALISE) Tp++; //Regra cumprida e classe bateu (Verdadeiro positivo)
        else Fp++; //Regra cumprida, mas classe não bateu (Falso positivo)
      }
      good = 0; //para recomeçar a análise da regra para outro registro
      bad = 0; //para recomeçar a análise da regra para outro registro
    }
    printf("\nTp: %d ",Tp);
    printf("Fp: %d ",Fp);
    printf("Tn: %d ",Tn);
    printf("Fn: %d ",Fn);
    getchar();
    Se = (float) (Tp/(Tp+Fn));
    Sp = (float) (Tn/(Tn+Fp));
    populacao[i].fitness = Se*Sp;
    Tp = 0; //para começar a análise da próxima regra (indivíduo)
    Fp = 0; //para começar a análise da próxima regra (indivíduo)
    Tn = 0; //para começar a análise da próxima regra (indivíduo)
    Fn = 0; //para começar a análise da próxima regra (indivíduo)
  }
}

void executaBaseIndividuo()
{

}

void get_database(int total_data[][ATRIBUTES]) //coleta dos dados do arquivo
{
  char buffer;
  int i, j;
  FILE *fp;
  fp = fopen("dermatology(adjusted).data","r");

  // for(int i=0; i<33; i++)
  // {
  //   fread(&buffer, 1, 1, fp);
  //   total_data[0][i] = atoi(&buffer);
  //   fread(&buffer, 1, 1, fp);
  // }

  for(i=0; i<INSTANCES; i++)
  {
    fscanf(fp,"%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d*c\n*c*c", &total_data[i][0], &total_data[i][1], &total_data[i][2], &total_data[i][3], &total_data[i][4], &total_data[i][5], &total_data[i][6], &total_data[i][7], &total_data[i][8], &total_data[i][9], &total_data[i][10], &total_data[i][11], &total_data[i][12], &total_data[i][13], &total_data[i][14], &total_data[i][15], &total_data[i][16], &total_data[i][17], &total_data[i][18], &total_data[i][19], &total_data[i][20], &total_data[i][21], &total_data[i][22], &total_data[i][23], &total_data[i][24], &total_data[i][25], &total_data[i][26], &total_data[i][27], &total_data[i][28], &total_data[i][29], &total_data[i][30], &total_data[i][31], &total_data[i][32], &total_data[i][33], &total_data[i][34]);
  }
  for(i=0; i<INSTANCES; i++)
  {
    for(j=0; j<ATRIBUTES; j++)
    {
      printf("%d ",total_data[i][j]);
    }
    printf("\n");
  }
}

int main()
{
  clock_t begin = clock();
  clock_t end;
  double time_spent;
  int total_data[INSTANCES][ATRIBUTES];

  int geracao,execucao=0;
  int i,indiceCross;

  //Semente do random, através de arquivo de geração do ubuntu, ou
  unsigned int semente;
  short gotTheFile = 0;
  FILE *fp;
  fp = fopen("/dev/urandom", "r");
  if (fp)
  {
    gotTheFile = 1;
    fread(&semente, sizeof(unsigned int), 1, fp);
    srand(semente);
    fclose(fp);
  }
  else
  {
    srand(execucao);
    execucao++;
  }

  get_database(total_data);

  while(execucao<10)
  {
    printf("\nEXEC%d: \n", execucao);
      if(!gotTheFile) srand(execucao);
      geracao=0;
      criaPopulacao(50);
      executaBasePopulacao(total_data); // vai executar a base e calcular o fitness
      ordena(2);
      imprime();
    while(geracao<50)
    {
      for(indiceCross=0;indiceCross<50;indiceCross+=2)
      {
        /*
        int pai1=torneio estocastico();
        int pai2=torneio estocastico();
        crossover(p1,p2,indiceCross);
        */
      }
      ordena(2);
      // elitismoP();
      ordena(1);
      // executaBasePopulacao(total_data); // vai executar a base e calcular o fitness
      imprime();
      geracao++;
    }
    execucao++;
  }
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nTime: %lf\n",time_spent);
  return 0;
}
