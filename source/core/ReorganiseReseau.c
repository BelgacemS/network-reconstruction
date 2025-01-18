#include <stdio.h>
#include <stdlib.h>

#include "Struct_Liste.h"
#include "Struct_File.h"
#include "SVGwriter.h"
#include "Graphe.h"
#include "Chaine.h"
#include "Reseau.h"

int main(int argc, char * argv[]){
    if (argc != 2) {
        printf("Nombre de parametres incorrect\n");
        printf("./ReorganiseReseau [nom de l'instance]\n");
        return 1;
    }

    /* Test l'instance du parametre */
    Reseau * r = NULL;
    FILE * fIn = fopen(argv[1], "r");

    Chaines * c = lectureChaines(fIn);
    printf("lectureChaines Reussi\n");

    r = reconstitueReseauListe(c);
    printf("reconstitueReseauListe Reussi\n");

    /* Test creerGraphe() */
    Graphe * g = creerGraphe(r);
    printf("creerGraphe Reussi\n");

    /* Test plusPetitNbAretes() */
    int i = plusPetitNbAretes(g, 2, 10);
    printf("plusPetitNbAretes (entre 2 et 10) : %d\n",i);

    /* Test reorganiseReseau() */
    int a = reorganiseReseau(r);
    printf("reorganiserReseau Reussi\n");
    if (a) {
        printf("Pour toute arête du graphe, le nombre de chaı̂nes qui passe par cette arête est inférieur à gamma\n\n");
    } else {
        printf("Il existe au moin une arete du graphe, telle que le nombre de chaı̂nes qui passe par cette arête est superieur à gamma\n\n");
    }
    
    freeReseau(r);
    freeChaines(c);
    libererGraphe(g);
    
    fclose(fIn);
    
    

/* Test de l'instance 1 */
FILE *f1 = fopen("fichier_test_1.txt", "r");
printf("Instance 1 : \n");
Chaines *c1 = lectureChaines(f1);
Reseau *r1 = reconstitueReseauListe(c1);
Graphe *g1 = creerGraphe(r1);

int i1 = plusPetitNbAretes(g1, 2, 10);
printf("plusPetitNbAretes (entre 2 et 10) pour l'instance 1 : %d\n", i1);

freeReseau(r1);
freeChaines(c1);
libererGraphe(g1);
fclose(f1);

/* Test de l'instance 2 */
FILE *f2 = fopen("fichier_test_2.txt", "r");
printf("\nInstance 2 : \n");
Chaines *c2 = lectureChaines(f2);
Reseau *r2 = reconstitueReseauListe(c2);
Graphe *g2 = creerGraphe(r2);

int i2 = plusPetitNbAretes(g2, 2, 10);
printf("plusPetitNbAretes (entre 2 et 10) pour l'instance 2 : %d\n", i2);

freeReseau(r2);
freeChaines(c2);
libererGraphe(g2);
fclose(f2);

/* Test de l'instance 3 */
FILE *f3 = fopen("fichier_test_3.txt", "r");
printf("\nInstance 3 : \n");
Chaines *c3 = lectureChaines(f3);
Reseau *r3 = reconstitueReseauListe(c3);
Graphe *g3 = creerGraphe(r3);

int i3 = plusPetitNbAretes(g3, 5, 8);
printf("plusPetitNbAretes (entre 5 et 8) pour l'instance 3 : %d\n", i3);

freeReseau(r3);
freeChaines(c3);
libererGraphe(g3);
fclose(f3);



    return 0;
}