#ifndef __STRUCT_LISTE_H__
#define __STRUCT_LISTE_H__
#include <stdio.h>
#include <stdlib.h>


typedef struct listeEntier{
	int val;
	struct listeEntier* suiv;
}ListeEntier;

ListeEntier* creer_ListeEntier(int val);
ListeEntier* inserer_en_Tete(ListeEntier * liste , int val);
void liberer_ListeEntier( ListeEntier* liste); 


#endif