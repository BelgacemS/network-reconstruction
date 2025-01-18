#include <stdlib.h>
#include <stdio.h>

#include "Struct_File.h"
#include "Struct_Liste.h"
#include "Reseau.h"
#include "Graphe.h"

Sommet** creerTableauSommet(int taille) {
    // Création
    Sommet** tableau = (Sommet**)malloc(taille * sizeof(Sommet*));
    if (tableau == NULL) {
        printf("Erreur d'allocation du tableau de sommets\n");
        return NULL;
    }
    // Initialisation
    for (int i = 0; i < taille; i++) {
        tableau[i] = NULL;
    }
    return tableau;
}

void libererTableauSommet(Sommet** tableau, int taille) {
    // Libération de chaque élément du tableau
    for(int i=0;i<taille;i++){
        libererSommet(tableau[i]);
    }
    

    free(tableau);
}

Commod* creerListeCommodites(Reseau *r) {
    // Allocation dynamique pour le tableau de commodités
    Commod* T_commod = (Commod*)malloc((nbCommodites(r)) * sizeof(Commod));
    if (T_commod == NULL) {
        printf("Erreur lors de l'allocation du tableau de commodités");
        return NULL;
    }

    // Parcours de la liste de commodités du réseau
    CellCommodite* r_commodites = r->commodites;
    int i = 0;
    while (r_commodites != NULL) {
        // Création d'une commodité dans le graphe
        Commod gc;
        gc.e1 = r_commodites->extrA->num;
        gc.e2 = r_commodites->extrB->num;
        // Ajout de la commodité au tableau de commodités du graphe
        T_commod[i] = gc;
        i++;
        r_commodites = r_commodites->suiv;
    }

    return T_commod;
}

void libererListeCommodites(Commod* T_commod) {
    free(T_commod);
}

void MAJ_Tableau_Sommet(Reseau* r, Sommet** T_som) {
    CellNoeud * r_cn = r->noeuds;
    Noeud * r_nd = NULL;
    CellNoeud * r_voisins = NULL;
    int r_numVoisin = 0;

    // Parcours de la liste de noeuds du réseau
    while (r_cn != NULL) {
        r_nd = r_cn->nd;
        r_voisins = r_nd->voisins;

        // Création du sommet
        Sommet * g_sommet = (Sommet *)malloc(sizeof(Sommet));
        
        if (!g_sommet) {
            printf("Erreur lors de l'allocation d'un sommet.\n");
            return;
        }

        // Initialisation des attributs du sommet
        g_sommet->num = r_nd->num;
        g_sommet->x = r_nd->x;
        g_sommet->y = r_nd->y;
        g_sommet->L_voisin = NULL;

        // Parcours des voisins du noeud dans le réseau
        while (r_voisins) {
            r_numVoisin = r_voisins->nd->num;
            Arete * g_areteNouv = NULL;

            // Comparaison des numéros des sommets pour déterminer si on a déjà vu le voisin
            if (r_numVoisin <= r_nd->num) { // Si c'est la première fois qu'on rencontre cet arete, nous allouons l'arete et nous l'initialisons
                g_areteNouv = (Arete *)malloc(sizeof(Arete));
                g_areteNouv->u = r_nd->num;
                g_areteNouv->v = r_numVoisin;
            } else {    // Si ce n'est pas la première fois, nous le trouvons dans la liste des voisins du sommet u
                Cellule_arete * g_cellAreteDejaVu = T_som[r_numVoisin]->L_voisin;
                Arete * g_areteDejaVu = NULL;

                while (g_cellAreteDejaVu != NULL) {
                    g_areteDejaVu = g_cellAreteDejaVu->a;
                    // On cherche l'arete qui a pour numero = le numero du sommet v
                    if (g_areteDejaVu->v == r_nd->num) {
                        g_areteNouv = g_areteDejaVu;
                        break;
                    }
                    g_cellAreteDejaVu = g_cellAreteDejaVu->suiv;
                }
            }

            // Création de la cellule arete et ajout de l'arete dans cette cellule
            Cellule_arete * g_ca = (Cellule_arete *)malloc(sizeof(Cellule_arete));
            g_ca->a = g_areteNouv;
            g_ca->suiv = g_sommet->L_voisin;
            g_sommet->L_voisin = g_ca;

            r_voisins = r_voisins->suiv;
        }

        // Nous ajoutons le sommet dans le tableau de sommets
        T_som[g_sommet->num] = g_sommet;

        r_cn = r_cn->suiv;
    }
}

Graphe* creerGraphe(Reseau* r){

    Sommet** Ts = creerTableauSommet(r->nbNoeuds+1); //+1 numérotation des noeuds débute par 1

    //On crée le graphe
    Graphe *G=(Graphe*)malloc(sizeof(Graphe));

    if(G==NULL){
        printf("Erreur d'allocation du graphe\n");
        libererTableauSommet( Ts, r->nbNoeuds+1);
        return NULL;
    }
    //Initialise G
    G->gamma=r->gamma;
    G->nbcommod=nbCommodites(r);
    G->nbsom=r->nbNoeuds;
    G->T_som=Ts;
    G->T_commod=creerListeCommodites(r);
    MAJ_Tableau_Sommet(r, Ts);

    return G;

}



void libererSommet(Sommet* sommet) {
    if (sommet == NULL) {
        return;
    }

    // Libérer les cellules d'arêtes
    Cellule_arete* caCour = sommet->L_voisin;
    Cellule_arete* caPrec = NULL;

    while (caCour != NULL) {
        if (sommet->num == caCour->a->u) {
            // Si c'est la première fois qu'on rencontre cette arête, on la libère
            free(caCour->a);
        }

        // On passe à la cellule d'arête suivante
        caPrec = caCour;
        caCour = caCour->suiv;
        free(caPrec); // Libérer la cellule
    }

    // Réinitialiser la liste des voisins du sommet
    sommet->L_voisin = NULL;

    // Libérer le sommet lui-même
    free(sommet);
}

void libererGraphe(Graphe* g) {
    // Libération de la mémoire allouée pour le tableau de sommets
    
    libererTableauSommet(g->T_som, g->nbsom+1);

    // Libération de la mémoire allouée pour le tableau de commodités
    free(g->T_commod);

    free(g);
}


int plusPetitNbAretes(Graphe * G, int u, int v){
    //Nous initialisons un tableau visit qui nous donne la longueur de la chaîne de u à visit[i]
    int visit[(G -> nbsom) + 1];
    int i;
    for (i = 0; i < G -> nbsom + 1; i++){
        visit[i] = 0;
    }
    //Nous initialisons la bordure du graphe avec une file
    S_file * F = (S_file *)malloc(sizeof(S_file));
    Init_file(F);
    enfile(F, u);

    while(!estFileVide(F)){
        //A chaque fois on prend un sommet u et on regarde ses sommets adjacents
        int current_vertex = defile(F);

        //Nous regardons les aretes du sommet u
        Cellule_arete * g_voisins = G -> T_som[current_vertex] -> L_voisin;

        while(g_voisins != NULL){
            //Nous choisissons le bon sommet v de l'arete, exemple arete 1-4, si u = 4, nous retournons 1 et sinon, nous retournons 4.
            int adjacent_vertex = 0;
            if(g_voisins -> a -> v == current_vertex){
                adjacent_vertex = g_voisins -> a -> u;
            } else {
                adjacent_vertex = g_voisins -> a -> v;
            }

            //Si nous n'avons jamais parcouru le sommet (visit[adjacent_vertex] == 0) ou s'il est différent du sommet de départ (racine)
            if(visit[adjacent_vertex] == 0 && adjacent_vertex != u){
                //Nous incrémentons de 1 par rapport à son élément précédent dans le tableau visit
                visit[adjacent_vertex] = visit[current_vertex] + 1;
                enfile(F, adjacent_vertex);

                //Si on est arrivé au sommet cible, nous retournons la distance entre u et v, en prenant le plus court chemin entre les deux sommets.
                if(adjacent_vertex == v){
                    while(!estFileVide(F)){
                        defile(F);
                    }
                    free(F);
                    return visit[v];
                }
            }
            g_voisins = g_voisins -> suiv;
        }

    }
    free(F);
    return visit[v];
}



ListeEntier * plusCourteChaine(Graphe * G, int u, int v){
    // On commence par créer une liste d'entiers qui contiendra la chaîne la plus courte
    ListeEntier * res = NULL;

    // On va garder une trace des sommets que l'on a visités pendant le parcours en largeur
    int visite[(G -> nbsom) + 1];
    // On va aussi garder une trace du prédécesseur de chaque sommet pour reconstruire la chaîne
    int precedent[(G -> nbsom) + 1];
    int i;
    // Initialisation des tableaux
    for (i = 0; i < (G -> nbsom) + 1; i++){
        visite[i] = 0;
        precedent[i] = -1;
    }
    
    // On initialise la file pour le parcours en largeur
    S_file * F = (S_file *)malloc(sizeof(S_file));
    Init_file(F);
    // On ajoute le sommet de départ à la file
    enfile(F,u);

    // On commence le parcours en largeur
    while(!estFileVide(F)){
        // On récupère le sommet courant dans la file
        int sommet_courant = defile(F);
        // On parcourt les arêtes du sommet courant
        Cellule_arete * g_voisins = G -> T_som[sommet_courant] -> L_voisin;

        while(g_voisins != NULL){
            // On récupère le sommet adjacent à l'arête courante
            int sommet_adjacent = (g_voisins -> a -> v == sommet_courant) ? g_voisins -> a -> u : g_voisins -> a -> v;
            
            // Si le sommet adjacent n'a pas été visité et n'est pas le sommet de départ
            if(visite[sommet_adjacent] == 0 && sommet_adjacent != u){
                // On marque le sommet comme visité et on enregistre son prédécesseur
                visite[sommet_adjacent] = visite[sommet_courant] + 1;
                enfile(F,sommet_adjacent);

                if(precedent[sommet_adjacent] == -1){
                    precedent[sommet_adjacent] = sommet_courant;
                }
                
                // Si on atteint le sommet cible, on reconstruit la chaîne
                if(sommet_adjacent == v){
                    int temp = v;
                    // On parcourt le tableau des prédécesseurs pour reconstruire la chaîne
                    while(precedent[temp] != -1){
                        inserer_en_Tete(res, temp);
                        temp = precedent[temp];
                    }
                    inserer_en_Tete(res,temp);

                    // On vide la file avant de terminer la fonction
                    while (!estFileVide(F)) {
                        defile(F);
                    }
                    free(F);

                    return res;
                }
            }
            g_voisins = g_voisins -> suiv;
        }
    }

    // On libère la mémoire et on retourne la liste de la chaîne la plus courte
    free(F);
    return res;
}

int reorganiseReseau(Reseau * r){
    Graphe * g = creerGraphe(r);

    // Allocation et initialisation de la matrice
    int ** matArete = (int **)malloc(((g -> nbsom) + 1) * sizeof(int*));
    if(matArete == NULL){    
        libererGraphe(g);
        return -1;
    }

    for (int i = 0; i < (g -> nbsom) + 1; i++){
        matArete[i] = (int *)malloc(((g -> nbsom) + 1) * sizeof(int));
        if(matArete[i] == NULL){
            for(int j = 0; j < i; i++){
                free(matArete[j]);
            }
            libererGraphe(g);
            free(matArete);
            return -1;
        }
    }

    for(int i = 0; i < (g -> nbsom) + 1; i++){
        for(int j = 0; j < (g -> nbsom) + 1; j++){
            matArete[i][j] = 0;
        }
    }

    // Parcours des commodités
    Commod * tabCommod = g -> T_commod;
    Commod k;
    for(int l = 0; l < g -> nbcommod; l++){
        k = tabCommod[l];
        ListeEntier * LEtemp = plusCourteChaine(g, k.e1, k.e2);
        ListeEntier * parcours = LEtemp;

        while(parcours->suiv != NULL){
            matArete[parcours->val][parcours->suiv->val]++;
            parcours = parcours->suiv;
        }
        liberer_ListeEntier(LEtemp);
        free(LEtemp);
    }

    // Vérification du nombre de chaînes pour chaque arête
    int aretes = 0;
    for (int m = 0; m < (g -> nbsom) + 1; m++){
        for(int n = m; n < (g -> nbsom) + 1; n++){
            aretes = matArete[m][n] + matArete[n][m];
            if(aretes > g -> gamma){
                freeMAT(matArete, (g -> nbsom) + 1);
                libererGraphe(g);
                return 0;
            }
        }
    }

    freeMAT(matArete, (g -> nbsom) + 1);
    libererGraphe(g);
    return 1;
}


void freeMAT(int ** mat, int taille){
    if (mat == NULL) {
        return;
    }
    for (int i = 0; i < taille; i++) {
        if(mat[i]) free(mat[i]);
    }
    free(mat);
}

