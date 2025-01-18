#include <stdlib.h>

#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y) {
    CellNoeud *current = R->noeuds;
    while (current != NULL) {
        if (current->nd->x == x && current->nd->y == y) {
            return current->nd;
        }
        current = current->suiv;
    }

    Noeud *new_node = (Noeud*)malloc(sizeof(Noeud));
    if (!new_node) return NULL;

    new_node->num = ++R->nbNoeuds;
    new_node->x = x;
    new_node->y = y;
    new_node->voisins = NULL;

    CellNoeud *new_cell = (CellNoeud*)malloc(sizeof(CellNoeud));
    if (!new_cell) {
        free(new_node);
        return NULL;
    }
    
    new_cell->nd = new_node;
    new_cell->suiv = R->noeuds;
    R->noeuds = new_cell;
    
    return new_node;
}

Reseau *creerReseau() {
    Reseau *R = (Reseau*)malloc(sizeof(Reseau));
    if (!R) return NULL;

    R->noeuds = NULL;
    R->commodites = NULL;
    R->nbNoeuds = 0;
    R->gamma = 0;

    return R;
}

int estDejaVoisin(Noeud* noeud, Noeud* voisinPotentiel) {
    CellNoeud* courant = noeud->voisins;
    while (courant != NULL) {
        if (courant->nd == voisinPotentiel) {
            return 1; // ils sont déjà voisins
        }
        courant = courant->suiv;
    }
    return 0; // ils ne sont pas voisins

}

void ajouteVoisinSiNonExistant(Noeud* noeud, Noeud* voisinPotentiel) {
    CellNoeud* voisin = noeud->voisins;
    
    // Parcourt la liste des voisins pour vérifier si le voisin potentiel existe déjà
    while (voisin != NULL && voisin->nd != voisinPotentiel) {
        voisin = voisin->suiv;
    }
    
    // Si le voisin potentiel n'existe pas dans la liste, on l'ajoute
    if (voisin == NULL) {
        CellNoeud* nouveauVoisin = (CellNoeud*)malloc(sizeof(CellNoeud));
        if (nouveauVoisin == NULL) {
            perror("Échec d'allocation pour un nouveau voisin");
            exit(EXIT_FAILURE);
        }
        nouveauVoisin->nd = voisinPotentiel;
        nouveauVoisin->suiv = noeud->voisins;
        noeud->voisins = nouveauVoisin;
    }
}

Reseau* reconstitueReseauListe(Chaines *C) {
    if (!C) return NULL;

    // On initialise le réseau
    Reseau *R = creerReseau();
    R->gamma = C->gamma; 

    // On parcourt les chaînes pour reconstruire le réseau
    for (CellChaine *cc = C->chaines; cc != NULL; cc = cc->suiv) {
        Noeud *noeudPrecedent = NULL;
        for (CellPoint *cp = cc->points; cp != NULL; cp = cp->suiv) {
            Noeud *noeudActuel = rechercheCreeNoeudListe(R, cp->x, cp->y);
            if (noeudPrecedent != NULL) {
                ajouteVoisinSiNonExistant(noeudPrecedent, noeudActuel);
                ajouteVoisinSiNonExistant(noeudActuel, noeudPrecedent);
            }
            noeudPrecedent = noeudActuel;  // màj du noeudPrecedent pour la prochaine itération
        }
    }
    
    return R;
}

void freeReseau(Reseau *R) {
    if (!R) return;

    CellNoeud *currNoeud = R->noeuds, *tmpNoeud;
    while (currNoeud) {
        tmpNoeud = currNoeud;
        currNoeud = currNoeud->suiv;
        
        // On libère la liste des voisins du noeud
        CellNoeud *currVoisin = tmpNoeud->nd->voisins, *tmpVoisin;
        while (currVoisin) {
            tmpVoisin = currVoisin;
            currVoisin = currVoisin->suiv;
            free(tmpVoisin);
        }
        
        free(tmpNoeud->nd); // On libère le noeud
        free(tmpNoeud); // On libère la cellule contenant le noeud
    }

    CellCommodite *currCommod = R->commodites, *tmpCommod;
    while (currCommod) {
        tmpCommod = currCommod;
        currCommod = currCommod->suiv;
        free(tmpCommod);
    }

    free(R);
}

int nbCommodites(Reseau *R) {
    int compteur = 0;
    CellCommodite *commoditeActuelle = R->commodites;
    while (commoditeActuelle) {
        compteur++;
        commoditeActuelle = commoditeActuelle->suiv;
    }
    return compteur;
}

int nbLiaisons(Reseau *R) {
    int compteur = 0;
    CellNoeud *noeudActuel = R->noeuds;
    while (noeudActuel) {
        CellNoeud *voisinActuel = noeudActuel->nd->voisins;
        while (voisinActuel) {
            compteur++;
            voisinActuel = voisinActuel->suiv;
        }
        noeudActuel = noeudActuel->suiv;
    }
    // Chaque liaison est comptée deux fois (une fois par chaque noeud), donc on divise par 2
    return compteur / 2;
}

void ecrireReseau(Reseau *R, FILE *f) {
    if (R == NULL || f == NULL) {
        return;
    }

    fprintf(f, "NbNoeuds: %d\n", R->nbNoeuds);
    fprintf(f, "NbLiaisons: %d\n", nbLiaisons(R));
    fprintf(f, "NbCommodites: %d\n", nbCommodites(R));
    fprintf(f, "Gamma: %d\n", R->gamma);

    // Écriture des noeuds
    CellNoeud *parcoursNoeud = R->noeuds;
    while (parcoursNoeud != NULL) {
        fprintf(f, "v %d %.6f %.6f\n", parcoursNoeud->nd->num, parcoursNoeud->nd->x, parcoursNoeud->nd->y);
        parcoursNoeud = parcoursNoeud->suiv;
    }

    // Écriture des liaisons, en verifiant de ne pas écrire deux fois la même liaison
    CellNoeud *parcoursLiaison;
    for (parcoursNoeud = R->noeuds; parcoursNoeud != NULL; parcoursNoeud = parcoursNoeud->suiv) {
        for (parcoursLiaison = parcoursNoeud->nd->voisins; parcoursLiaison != NULL; parcoursLiaison = parcoursLiaison->suiv) {
            if (parcoursNoeud->nd->num < parcoursLiaison->nd->num) { // Éviter les doublons
                fprintf(f, "l %d %d\n", parcoursNoeud->nd->num, parcoursLiaison->nd->num);
            }
        }
    }

    // Écriture des commodités
    CellCommodite *parcoursCommodite = R->commodites;
    while (parcoursCommodite != NULL) {
        fprintf(f, "k %d %d\n", parcoursCommodite->extrA->num, parcoursCommodite->extrB->num);
        parcoursCommodite = parcoursCommodite->suiv;
    }
}

void afficheReseauSVG(Reseau *R, char* nomInstance){
    CellNoeud *courN,*courv;
    SVGwriter svg;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    char nom_fichier[256];

    courN=R->noeuds;
    while (courN!=NULL){
        if (maxx<courN->nd->x) maxx=courN->nd->x;
        if (maxy<courN->nd->y) maxy=courN->nd->y;
        if (minx>courN->nd->x) minx=courN->nd->x;
        if (miny>courN->nd->y) miny=courN->nd->y;
        courN=courN->suiv;
    }
    snprintf(nom_fichier, sizeof(nom_fichier), "output/%s", nomInstance);
    SVGinit(&svg, nom_fichier, 500, 500);
    courN=R->noeuds;
    while (courN!=NULL){
        SVGpoint(&svg,500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
        courv=courN->nd->voisins;
        while (courv!=NULL){
            if (courv->nd->num<courN->nd->num)
                SVGline(&svg,500*(courv->nd->x-minx)/(maxx-minx),500*(courv->nd->y-miny)/(maxy-miny),500*(courN->nd->x-minx)/(maxx-minx),500*(courN->nd->y-miny)/(maxy-miny));
            courv=courv->suiv;
        }
        courN=courN->suiv;
    }
    SVGfinalize(&svg);
}
