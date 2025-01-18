#ifndef HACHAGE_H
#define HACHAGE_H

#include "Reseau.h"  

typedef struct {
  int nbElement;  // pas nécessaire ici
  int tailleMax; 
  CellNoeud **T;  
} TableHachage;

double fonction_cle(double x, double y);
void test_fonction_cle();
int fonction_hachage(int k, int M);
Noeud* rechercheCreeNoeudHachage(Reseau *R, TableHachage *H, double x, double y);
TableHachage* initTableHachage(int taille);
void freeTableHachage(TableHachage *ht);
Reseau* reconstitueReseauHachage(Chaines *C, int M);

#endif