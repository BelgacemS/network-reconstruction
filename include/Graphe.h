#ifndef _GRAPHE_H_
#define _GRAPHE_H_

#include "Reseau.h"
//#include <stdlib.h>
//#include <stdio.h>
//#include "Struct_Liste.h"

typedef struct {
    int u, v; // Numeros des sommets extrémités
} Arete;

typedef struct cellule_arete {
    Arete *a; // pointeur sur l'arete
    struct cellule_arete* suiv;
} Cellule_arete;

typedef struct {
    int num; // Numero du sommet (le meme que dans I_som)
    double x, y;
    Cellule_arete* L_voisin; // Liste chainee des voisins
} Sommet;

typedef struct {
    int e1, e2; // Les deux extremites de la commodite
} Commod;

typedef struct {
    int nbsom;
    Sommet** T_som; // Tableau de pointeurs sur sommets
    int gamma;
    int nbcommod; // Nombre de commodites
    Commod* T_commod; // Tableau des commodites
} Graphe;

Sommet** creerTableauSommet(int taille);
void libererTableauSommet(Sommet** tableau, int taille);
Commod* creerListeCommodites(Reseau *r);
void libererListeCommodites(Commod* T_commod);
void MAJ_Tableau_Sommet(Reseau* r, Sommet** T_som);
void libererSommet(Sommet* sommet);
Graphe* creerGraphe(Reseau* r);
void libererGraphe(Graphe* g);
int plusPetitNbAretes(Graphe * G, int u, int v);
ListeEntier * plusCourteChaine(Graphe * G, int u, int v);
int reorganiseReseau(Reseau * r);
void freeMAT(int ** mat, int taille);
void afficherGraphe(Graphe * G);
#endif
