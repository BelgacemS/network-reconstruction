#include "Struct_Liste.h"

ListeEntier* creer_ListeEntier(int val) {
    ListeEntier *l = malloc(sizeof(ListeEntier));
    if (l == NULL) {
        printf("Erreur lors de l'allocation de mémoire pour la liste");
        return NULL;
    }
    l->val = val;
    l->suiv = NULL;
    return l;
}

ListeEntier* inserer_en_Tete(ListeEntier* liste, int val) {
    ListeEntier* l = creer_ListeEntier(val);
    l->suiv = liste;
    return l;
}

void liberer_ListeEntier(ListeEntier* liste) {
    while (liste != NULL) {
        ListeEntier* temp = liste->suiv;
        free(liste);
        liste = temp;
    }
}