#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "criacao-da-populacao.h"

#define INSTANCES 366 - 8
#define ATRIBUTES 35
#define CLASS_TO_ANALISE 1
#define WEIGHT_LIMIT 0.7

//operador 0 => Igual
//operador 1 => Diferente
//operador 2 => Maior ou Igual
//operador 3 => Menor

void executaBasePopulacao();
void executaBaseIndividuo();
void get_database(int total_data[][ATRIBUTES]);

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
    Se = Tp/(Tp+Fn);
    Sp = Tn/(Tn+Fp);
    populacao.fitness = Se*Sp;
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
      if(gotTheFile) srand(execucao);
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
        int pai2=tornei estocastico();
        crossover(p1,p2,indiceCross);
        */
      }
      ordena(2);
      elitismoP();
      ordena(1);
      //executaBase(); // vai executar a base e calcular o fitness
      imprime();
      geracao++;
    }
    execucao++;
  }

  return 0;
}
