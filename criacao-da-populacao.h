#ifndef CRIACAO_DA_POPULACAO
#define CRIACAO_DA_POPULACAO

typedef struct gene;
typedef struct individuo;

individuo populacao[50];
individuo populacaoFilhos[50];

void criaPopulacao(int tamanho);
void imprime();
void crossover(int p1,int p2,int indiceCross);
void mutacao(int ind);
void ordena(int popCase);
void elitismoP();

#endif
