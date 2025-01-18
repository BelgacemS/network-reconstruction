#include <stdio.h>
#include <stdlib.h>

#include "Reseau.h"
#include "Chaine.h"
#include "SVGwriter.h"
#include "Hachage.h"
#include "ArbreQuat.h"

#define M 100

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <fichier.cha> <methode>\n", argv[0]);
        printf("Methode: 1 pour liste chaînée, 2 pour table de hachage, 3 pour arbre.\n");
        return 1;
    }

    int methode = atoi(argv[2]); // Convertit l'argument de méthode en entier.


    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    Chaines *C = lectureChaines(f); 
    fclose(f);

    if (C == NULL) {
        fprintf(stderr, "Erreur lors de la lecture des chaines\n");
        return 1;
    }

    Reseau *R;
    switch (methode) {
        case 1:
            R = reconstitueReseauListe(C);
            break;
        case 2:
            R = reconstitueReseauHachage(C, M);
            break;
        case 3:
            R = reconstitueReseauArbre(C);
            break;
        default:
            fprintf(stderr, "Méthode non reconnue\n");
            freeChaines(C); 
            return 1;
    }

    if (R == NULL) { 
        fprintf(stderr, "Erreur lors de la reconstruction du reseau\n");
        freeChaines(C);
        return 1;
    }

    afficheReseauSVG(R, "mon_reseau2");

    freeReseau(R);  
    freeChaines(C); 

    return 0;
}
