#include <stdio.h>
#include <stdlib.h>

#include "ArbreQuat.h"
#include "Chaine.h"
#include "Reseau.h"


void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax) {

    if (!C || !xmin || !ymin || !xmax || !ymax) return ;

    CellChaine * cc = C -> chaines;
    CellPoint * cp = NULL;

    // On initialise les quatre valeurs */
    *xmin = cc -> points -> x;
    *ymin = cc -> points -> y;
    *xmax = cc -> points -> x;
    *ymax = cc -> points -> y;

    // On parvourt la liste pour touver les mins et les maxs */
    while (cc) {
        cp = cc -> points;
        while (cp) {
            if (cp -> x < (*xmin)) (*xmin) = cp -> x;
            if (cp -> y < (*ymin)) (*ymin) = cp -> y;
            if (cp -> x > (*xmax)) (*xmax) = cp -> x;
            if (cp -> y > (*ymax)) (*ymax) = cp -> y;
            cp = cp -> suiv;
        }
        cc = cc -> suiv;
    }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY) {

    ArbreQuat* nouveau = (ArbreQuat*)malloc(sizeof(ArbreQuat));
    if (nouveau == NULL) {
        printf("Erreur d'allocation.\n");
        return NULL;
    }

    // On initialise le nouveau noeud de l'arbre quaternaire.
    nouveau->xc = xc;
    nouveau->yc = yc;
    nouveau->coteX = coteX;
    nouveau->coteY = coteY;

    nouveau->noeud = NULL; 
    nouveau->no = NULL; 
    nouveau->ne = NULL; 
    nouveau->so = NULL; 
    nouveau->se = NULL; 

    return nouveau;
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent) {
    
    // Premier cas: si le pointeur de l'arbre est NULL, créez un nouvel arbre.
    if (*a == NULL) {
        double newCoteX = parent->coteX / 2.0;
        double newCoteY = parent->coteY / 2.0;
        double newXc = (n->x < parent->xc) ? (parent->xc - newCoteX / 2.0) : (parent->xc + newCoteX / 2.0);
        double newYc = (n->y < parent->yc) ? (parent->yc - newCoteY / 2.0) : (parent->yc + newCoteY / 2.0);

        *a = creerArbreQuat(newXc, newYc, newCoteX, newCoteY);
        (*a)->noeud = n;
        return;
    }

    // Deuxième cas: si l'arbre contient déjà un nœud, transformez-le en nœud interne et réinsérez les nœuds.
    if ((*a)->noeud != NULL) {
        Noeud* ancienNoeud = (*a)->noeud;
        (*a)->noeud = NULL;  // Transformez ce point en nœud interne

        // Insérez d'abord le nouveau nœud, puis l'ancien pour éviter les conflits
        insererNoeudArbre(n, a, *a);
        insererNoeudArbre(ancienNoeud, a, *a);
        return;
    }

    // Troisième cas: l'arbre est un nœud interne. Décidez du quadrant approprié pour le nœud.
    ArbreQuat** quadrant;
    if (n->x < (*a)->xc && n->y < (*a)->yc) {
        quadrant = &(*a)->so; // Sud-ouest
    } else if (n->x >= (*a)->xc && n->y < (*a)->yc) {
        quadrant = &(*a)->se; // Sud-est
    } else if (n->x < (*a)->xc && n->y >= (*a)->yc) {
        quadrant = &(*a)->no; // Nord-ouest
    } else {
        quadrant = &(*a)->ne; // Nord-est
    }

    // Insérez le nœud récursivement dans le quadrant approprié.
    insererNoeudArbre(n, quadrant, *a);
}

Noeud* creerEtInsererNoeud(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y) {
    Noeud * n = (Noeud *)malloc(sizeof(Noeud));
    if (!n) {
        // Gestion de l'échec de l'allocation de mémoire.
        return NULL;
    }
    n->num = R->nbNoeuds + 1;
    n->x = x;
    n->y = y;
    n->voisins = NULL;

    CellNoeud * cn = (CellNoeud *)malloc(sizeof(CellNoeud));
    if (!cn) {
        // Gestion de l'échec de l'allocation de mémoire.
        free(n);
        return NULL;
    }
    cn->nd = n;
    cn->suiv = R->noeuds;
    R->noeuds = cn;
    R->nbNoeuds++;

    insererNoeudArbre(n, &(*a), parent);
    
    return n;
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y) {
    if (*a == NULL) { // Arbre vide
        return creerEtInsererNoeud(R, a, parent, x, y);
    }

    // Si on est sur une feuille de l'arbre
    if ((*a)->noeud != NULL) {
        // Si le noeud correspond à celui de la feuille
        if ((*a)->noeud->x == x && (*a)->noeud->y == y) {
            return (*a)->noeud;
        } else {
            return creerEtInsererNoeud(R, a, parent, x, y);
        }
    }

    // Choix du quadrant
    if (x < (*a)->xc && y < (*a)->yc) {
        return rechercheCreeNoeudArbre(R, &((*a)->so), *a, x, y);
    } else if (x >= (*a)->xc && y < (*a)->yc) {
        return rechercheCreeNoeudArbre(R, &((*a)->se), *a, x, y);
    } else if (x < (*a)->xc && y >= (*a)->yc) {
        return rechercheCreeNoeudArbre(R, &((*a)->no), *a, x, y);
    } else {
        return rechercheCreeNoeudArbre(R, &((*a)->ne), *a, x, y);
    }
}

void freeArbreQuat(ArbreQuat* a) {
    if (a == NULL) {
        return;
    }

    freeArbreQuat(a->no);
    freeArbreQuat(a->ne);
    freeArbreQuat(a->so);
    freeArbreQuat(a->se);

    free(a);
}

Reseau* reconstitueReseauArbre(Chaines* C) {
    if (!C) return NULL;

    /* Creer le reseau */
    Reseau * r = (Reseau *)malloc(sizeof(Reseau));
    r -> gamma = C -> gamma;
    r -> nbNoeuds = 0;
    r -> noeuds = NULL;
    r -> commodites = NULL;
    Noeud * nAjoute = NULL;
    Noeud * nPrec = NULL;

    CellChaine * cc = C -> chaines;
    CellPoint * cp = NULL;

    /* Creer la racine de l'arbre */
    double xmin, ymin, xmax, ymax;
    chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
    ArbreQuat * a = creerArbreQuat((xmin + xmax) * 1.0 / 2, (ymin + ymax) * 1.0 / 2, xmax - xmin, ymax - ymin);

    /* Parcourir les chaines */
    while (cc) {

        /* Nous traitons d'abord le cas des extrimites de chaines (commodite) */
        CellCommodite * k = (CellCommodite*)malloc(sizeof(CellCommodite));
        cp = cc -> points;

        if ((cp -> x) < (a -> xc) && (cp -> y) < (a -> yc)) {
            nAjoute = rechercheCreeNoeudArbre(r, &(a -> so), a, cp -> x, cp -> y);
        } else if ((cp -> x) >= (a -> xc) && (cp -> y) < (a -> yc)) {
            nAjoute = rechercheCreeNoeudArbre(r, &(a -> se), a, cp -> x, cp -> y);
        } else if ((cp -> x) < (a -> xc) && (cp -> y) >= (a -> yc)) {
            nAjoute = rechercheCreeNoeudArbre(r, &(a -> no), a, cp -> x, cp -> y);
        } else {
            nAjoute = rechercheCreeNoeudArbre(r, &(a -> ne), a, cp -> x, cp -> y);
        }

        cp = cc -> points;
        //commodite extrA
        k -> extrA = nAjoute;
        cp = cp -> suiv;

        while (cp) {
            /* Ajouter ce noeud dans la liste */
            /* Nous gardons à chaque fois, le dernier noeudAjouter pour déterminer les voisins. */
            nPrec = nAjoute;

            if (cp -> x < a -> xc && cp -> y < a -> yc) {
                nAjoute = rechercheCreeNoeudArbre(r, &(a -> so), a, cp -> x, cp -> y);
            } else if (cp -> x >= a -> xc && cp -> y < a -> yc) {
                nAjoute = rechercheCreeNoeudArbre(r, &(a -> se), a, cp -> x, cp -> y);
            } else if (cp -> x < a -> xc && cp -> y >= a -> yc) {
                nAjoute = rechercheCreeNoeudArbre(r, &(a -> no), a, cp -> x, cp -> y);
            } else {
                nAjoute = rechercheCreeNoeudArbre(r, &(a -> ne), a, cp -> x, cp -> y);
            }


            /* Ajouter les voisins */
            ajouteVoisinSiNonExistant(nPrec, nAjoute);
            ajouteVoisinSiNonExistant(nAjoute, nPrec);
            cp = cp -> suiv;

        }

        //commodite extrB
        k -> extrB = nAjoute;
        k -> suiv = r -> commodites;
        r -> commodites = k;

        cc = cc -> suiv;
    }

    freeArbreQuat(a);
    return r;
}
