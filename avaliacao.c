#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//#include "criacao-da-populacao.h"

#define INSTANCES 366 - 8
#define ATRIBUTES 35
#define CLASS_TO_ANALISE 6
#define WEIGHT_LIMIT 0.7

#define POPULATION 50

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
    gene fita[ATRIBUTES];
    float fitness;
}individuo;

enum {training, testing};

struct individuo melhores[10];
struct individuo populacao[POPULATION];
struct individuo populacaoFilhos[POPULATION];

void executaBasePopulacao(int data[][ATRIBUTES], individuo toExecute[], int orTrainingOrTesting, int populationNumber);
void executaBaseIndividuo();
void get_database(int total_data[][ATRIBUTES]);
void setTrainingAndTestingData(int total_data[][ATRIBUTES], int training_data[][ATRIBUTES], int testing_data[][ATRIBUTES]);

// |-------------------------------------------------------------
void criaPopulacao(int tamanho);
void imprime();
void imprimeIndividuo();
void crossover(int p1,int p2,int indiceCross);
void mutacao(int ind);
void ordena(int popCase);
void elitismoP();

int  roleta()
{
    int i,maior,vetor[POPULATION],sum=0,sorteio;
    vetor[0]=populacao[0].fitness*100; //tratar o problema de acumular float
    for(i=1; i<POPULATION; i++)
    {
      if(populacao[i].fitness*100 > 0)
        vetor[i]=(populacao[i].fitness*100)+vetor[i-1];
      else
        vetor[i]=vetor[i-1]+1;
    } //cria um vetor com a fitness acumulado
    sorteio=rand()%vetor[POPULATION-1];
    for(i=0; i<POPULATION;i++)
      if(vetor[i]>=sorteio){break;}
    return i;
}

int torneio_estocastico(int tour)
{
    int i,torneio[tour],maior;
    for(i=0; i<tour; i++)torneio[i]=roleta();
    for(i=1,maior=torneio[0]; i<tour; i++)if(populacao[maior].fitness<populacao[torneio[i]].fitness)maior=torneio[i];
    return maior;
}

void setTrainingAndTestingData(int total_data[][ATRIBUTES], int training_data[][ATRIBUTES], int testing_data[][ATRIBUTES])
{
  int sorteado[INSTANCES];
  int i,j,numero;
  int instances = INSTANCES;
  int UmTerco= instances/3;
  int DoisTerco=((instances/3)*2);

  int treino[DoisTerco][ATRIBUTES], teste[UmTerco][ATRIBUTES];

  for(i=0;i<INSTANCES;i++) sorteado[i]=0;

  for(i=UmTerco;i<INSTANCES;)//sorteando registros para treinanmento
  {
    numero=rand()%instances;
    if(sorteado[numero]==0)
    {
      sorteado[i]=numero;
      i++;
    }
  }

  for(i=0;i<UmTerco;)//sorteando registros para teste
  {
    numero=rand()%instances;
    if(sorteado[numero]==0)
    {
      sorteado[i]=numero;
      i++;
    }
  }

  for(i=UmTerco;i<INSTANCES;i++)//organizando dados para treino
  {
    for(j=0;j<ATRIBUTES;j++)
    {
      training_data[i-UmTerco][j] = total_data[sorteado[i]][j];
    }
  }

  for(i=0;i<UmTerco;i++)//organizando dados para teste
  {
    for(j=0;j<ATRIBUTES;j++)
    {
      testing_data[i][j] = total_data[sorteado[i]][j];
    }
  }
}

void ordena(int popCase)
{
    int i,j,maior;
    individuo aux;
    for(i=0;i<POPULATION;i++)
    {
        maior=i;
        for(j=i;j<POPULATION;j++)
        {
            if(populacao[j].fitness>populacao[maior].fitness) maior=j;

        }
        aux=populacao[i];
        populacao[i]=populacao[maior];
        populacao[maior]=aux;
    }
}

void criaPopulacao(int tamanho)
{
    int x=0,i;
    while(x<tamanho)
    {
        for(i=0;i<ATRIBUTES;i++)
        {
            //peso de ativacao com 3 casas decimais entre 0 e 1.
            populacao[x].fita[i].peso=(rand()%1000)/1000.0;
            populacao[x].fita[i].valor=rand()%3;
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
                populacao[x].fita[i].valor=CLASS_TO_ANALISE;
                // populacao[x].fita[i].valor=(rand()%6)+1; //gene que contem o tipo.
            }
        }
        x++;
    }
}

void crossover(int p1,int p2,int indiceCross)
{
    individuo f1,f2;
    int a,b,i;
   a=(rand()%ATRIBUTES);
    b=rand()%(ATRIBUTES-a);
    for(i=0;i<ATRIBUTES;i++)
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

    for(i=0;i<ATRIBUTES-1;i++)
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
                if(i==33)
                {
                 populacaoFilhos[ind].fita[i].valor=rand()%76+1;
                }
                else{
            if(i!=10)populacaoFilhos[i].fita[i].valor=rand()%4;
                else populacaoFilhos[i].fita[i].valor=rand()%2;
                }
        }
    }
    populacaoFilhos[ind].fita[34].peso=1;
}

 void elitismoP() // elitismo com apenas o melhor
 {
     int i;
     for(i=1;i<POPULATION;i++)
        populacao[i]=populacaoFilhos[i];
 }
//|-------------------------------------------------------------------------------

void imprimeIndividuo(individuo qualquer)
{
    int i;
    if(qualquer.fitness>0)
    {
      for(i=0;i<ATRIBUTES;i++)
      {
          if(qualquer.fita[i].peso >= WEIGHT_LIMIT) // apenas os pesos menos que 0.3 ativam, se quiser pode ser >0.7 | de qualquer jeito passao 30%
          {
          // printf("- %.3f",populacao[x].fita[i].peso); // se quiser ver o peso
          printf("g%d ",i); //indica o gene.
          if(qualquer.fita[i].operador==0)
              printf("== %d ",qualquer.fita[i].valor);
          if(qualquer.fita[i].operador==1)
              printf("!= %d ",qualquer.fita[i].valor);
          if(qualquer.fita[i].operador==2)
              printf(">= %d ",qualquer.fita[i].valor);
          if(qualquer.fita[i].operador==3)
              printf("< %d ",qualquer.fita[i].valor);
          }
      }
      printf("<%f>\n", qualquer.fitness);
    }
}


void executaBasePopulacao(int data[][ATRIBUTES], individuo toExecute[], int orTrainingOrTesting, int populationNumber)
{
  int Tp = 0, Fp = 0, Tn = 0, Fn = 0;
  float Se, Sp;
  int instances = INSTANCES;
  int UmTerco= instances/3;
  int triggered,p,q,i;
  int data_limit;
  if(orTrainingOrTesting == training) data_limit = instances - UmTerco;
  if(orTrainingOrTesting == testing) data_limit = UmTerco;

  for(i=0; i<populationNumber; i++) //Para cada individuo
  {
    for(p=0; p<data_limit; p++) //Comparar com cada registro de caso clínico
    {
      triggered = 1;
      for(q=0; (triggered)&&(q<ATRIBUTES-1); q++) //Para cada atributo (sem a classe)
      {
        if(toExecute[i].fita[q].peso > WEIGHT_LIMIT) //O peso é maior que o limite
        {
          switch (toExecute[i].fita[q].operador) //análise do operador
          {
            case 0:
              if(!(data[p][q] == toExecute[i].fita[q].valor)) triggered=0;
              break;
            case 1:
              if(!(data[p][q] != toExecute[i].fita[q].valor)) triggered=0;
              break;
            case 2:
              if(!(data[p][q] >= toExecute[i].fita[q].valor)) triggered=0;
              break;
            case 3:
              if(!(data[p][q] < toExecute[i].fita[q].valor)) triggered=0;
              break;
            default:
              break;
          }
        }
      }


      if(triggered) //Análise caso a regra deu certo
      {
        if(data[p][34] == CLASS_TO_ANALISE) Tp++; //Regra cumprida e  classe bateu (Verdadeiro positivo)
        else Fp++; //Regra cumprida, mas classe não bateu (Falso positivo)
      }
      else //Análise caso a regra foi desobedecida em algum ponto
      {
        if(data[p][34] != CLASS_TO_ANALISE) Tn++; //Regra desobedecida e classe não bateu (Verdadeiro negativo)
        else Fn++; //Regra desobedecida, mas classe bateu (Falso negativo)
      }
    }
    Se = (Tp/(Tp+Fn*1.0));
    Sp = (Tn/(Tn+Fp*1.0));
    toExecute[i].fitness = Se*Sp;
    Tp = 0; //para começar a análise da próxima regra (indivíduo)
    Fp = 0; //para começar a análise da próxima regra (indivíduo)
    Tn = 0; //para começar a análise da próxima regra (indivíduo)
    Fn = 0; //para começar a análise da próxima regra (indivíduo)
  }
}

// void executaBaseIndividuo()
// {
// }

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
  int instances = INSTANCES;
  int UmTerco= instances/3;
  int total_data[INSTANCES][ATRIBUTES];
  int training_data[instances - UmTerco][ATRIBUTES];
  int testing_data[UmTerco][ATRIBUTES];

  int geracao,execucao=0;
  int i,indiceCross;

  //Semente do random, através de arquivo de geração do ubuntu, ou com semente fixa, atualizada a cada execucao
  unsigned int semente;
  short gotTheFile = 0;
  FILE *fp;
  fp = fopen("/dev/urandom", "r");
  if (fp)
  {
    gotTheFile = 1;
    fread(&semente, sizeof(unsigned int), 1, fp);
    // srand(semente);
    fclose(fp);
  }
  else
  {
    srand(execucao);
    execucao++;
  }

  get_database(total_data);
  setTrainingAndTestingData(total_data, training_data, testing_data);

  //Início das execuções
  while(execucao<10)
  {
   // printf("\nEXEC%d: \n", execucao);
      if(!gotTheFile) srand(execucao);
      geracao=0;
      criaPopulacao(POPULATION);

// ######################################TRAINING#######################################################
      executaBasePopulacao(training_data, populacao, training, POPULATION); // vai executar a base e calcular o fitness
      ordena(1);
    while(geracao<POPULATION)
    {
      for(indiceCross=0;indiceCross<POPULATION;indiceCross+=2)
      {
        int pai1=torneio_estocastico(3);
        int pai2=torneio_estocastico(3);
        crossover(pai1,pai2,indiceCross);
      }
      elitismoP();
      executaBasePopulacao(training_data, populacao, training, POPULATION);
      ordena(1);
    // for(i=0;i<50;i++){imprimeIndividuo(populacao[i]);}
      geracao++;

    }
    // for(i=0;i<50;i++){imprimeIndividuo(populacao[i]);}
    melhores[execucao]=populacao[0];// parte 4 do trabalho
    execucao++;
  }

  printf("melhores:\n");
  for(i=0;i<10;i++)imprimeIndividuo(melhores[i]);

// #####################################TESTING#########################################################
  executaBasePopulacao(testing_data, melhores, testing, 10);
  printf("melhores após o training:\n");
  for(i=0;i<10;i++)imprimeIndividuo(melhores[i]);
// #####################################################################################################
  end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nTime: %lf\n",time_spent);
  return 0;
}
