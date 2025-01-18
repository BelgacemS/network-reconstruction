#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"

double fonction_cle(double x, double y) {
    return y + (x + y) * (x + y + 1) / 2.0; 
} // Nous apercevons que les valeurs sont toutes différentes, fonction injective, donc on peut utiliser cette fonction pour la table de hachage

int fonction_hachage(int k, int M) {
    double A = (sqrt(5) - 1) / 2;
    return (int)(M * (k * A - (int)(k * A))); // cast car doit retourtner un int qui correspond à l'indice
}

Noeud* rechercheCreeNoeudHachage(Reseau *R, TableHachage *H, double x, double y) {

    // On calcule l'index avec la fonction de hachage
    int index = fonction_hachage(fonction_cle(x, y), H->tailleMax);
    
    // On recherche dans la liste à cet index
    CellNoeud *cellule = H->T[index];
    while (cellule != NULL) {
        if (cellule->nd->x == x && cellule->nd->y == y) {
            // Nœud trouvé, on le retourner
            return cellule->nd;
        }
        cellule = cellule->suiv;
    }

    // Noeud non trouvé, on en crée un nouveau et l'ajoute à la table de hachage
    Noeud *nouveauNoeud = (Noeud*)malloc(sizeof(Noeud));
    if (!nouveauNoeud) {
        fprintf(stderr, "Erreur d'allocation mémoire pour un nouveau noeud\n");
        exit(EXIT_FAILURE); // ou return NULL;
    }
    nouveauNoeud->num = ++R->nbNoeuds; // on attribue un nouveau numéro de noeud
    nouveauNoeud->x = x;
    nouveauNoeud->y = y;
    nouveauNoeud->voisins = NULL; // on initialise la liste des voisins

    // On ajoute le nouveau noeud à la tête de la liste de la table de hachage
    CellNoeud *nouvelleCellule = (CellNoeud*)malloc(sizeof(CellNoeud));
    
    nouvelleCellule->nd = nouveauNoeud;
    nouvelleCellule->suiv = H->T[index];
    H->T[index] = nouvelleCellule;

    // On ajouter également le nouveau noeud à la liste des noeuds du réseau
    CellNoeud *nouvelleCelluleReseau = (CellNoeud*)malloc(sizeof(CellNoeud));

    nouvelleCelluleReseau->nd = nouveauNoeud;
    nouvelleCelluleReseau->suiv = R->noeuds;
    R->noeuds = nouvelleCelluleReseau;

    return nouveauNoeud;
}

TableHachage* initTableHachage(int taille) {
    TableHachage *ht = malloc(sizeof(TableHachage));
    if (ht == NULL) {
        return NULL;
    }

    ht->tailleMax = taille;
    ht->nbElement = 0; // au début, il n'y a aucun élément dans la table
    ht->T = malloc(sizeof(CellNoeud*) * taille);

    if (ht->T == NULL) {
        free(ht); // Libération de la table de hachage si l'allocation du tableau échoue
        return NULL;
    }

    // Initialisation de tous les pointeurs à NULL
    for (int i = 0; i < taille; ++i) {
        ht->T[i] = NULL;
    }
    
    return ht;
}

void freeTableHachage(TableHachage *ht) {
    if (ht == NULL) {
        return; // Si la table de hachage est déjà NULL, il n'y a rien à libérer
    }

    // Libération de chaque liste chaînée dans la table de hachage
    for (int i = 0; i < ht->tailleMax; ++i) {
        CellNoeud *cell = ht->T[i];
        while (cell) {
            CellNoeud *tmp = cell;
            cell = cell->suiv;
            free(tmp); // Libération de la cellule mais pas du noeud, car il appartient au réseau
        }
    }

    free(ht->T); // Libération du tableau de pointeurs
    free(ht);    // Libération de la structure de la table de hachage elle-même
}

Reseau* reconstitueReseauHachage(Chaines *C, int M){
    if(!C) return NULL;

    // On Declare et initialise la table hachage et le reseau 
    TableHachage * H = (TableHachage *)malloc(sizeof(TableHachage));
    H->nbElement = 0;
    H->tailleMax = M;
    H->T = (CellNoeud**)malloc(M * sizeof(CellNoeud*));

    for (int i = 0 ; i < M ; i++) (H->T[i]) = NULL;
    
    Reseau * r = creerReseau();
    r->gamma = C->gamma;

    
    CellChaine * cc = C -> chaines;
    CellPoint * cp = NULL;

    while(cc){
        // On traite d'abord le cas des extrimites de chaines (#les commodités) 
        CellCommodite * k = (CellCommodite*)malloc(sizeof(CellCommodite));
        cp = cc -> points;
        Noeud * noeudAjouter = rechercheCreeNoeudHachage(r, H, cp -> x, cp -> y);
        Noeud * noeudPrec = NULL;
        //commodite extrA
        k -> extrA = noeudAjouter;
        cp = cp -> suiv;
        
        while(cp){
            /* On gardons à chaque fois, le dernier noeudAjouter pour déterminer les voisins. */
            noeudPrec = noeudAjouter;
            noeudAjouter = rechercheCreeNoeudHachage(r, H, cp -> x, cp -> y);
            
            ajouteVoisinSiNonExistant(noeudPrec, noeudAjouter);
            ajouteVoisinSiNonExistant(noeudAjouter, noeudPrec);
            
            cp = cp -> suiv;
        }
        //commodite extrB
        k -> extrB = noeudAjouter;
        k -> suiv = r -> commodites;
        r -> commodites = k;
        
        cc = cc -> suiv;
    }
    
    freeTableHachage(H);
    return r;
}

