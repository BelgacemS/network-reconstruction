#include <stdio.h>
#include <stdlib.h>

#include "Chaine.h"
#include "SVGwriter.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <fichier_instance_à_lire>\n", argv[0]);
        return 1;
    }

    FILE *fichierEntree = fopen(argv[1], "r");
    if (fichierEntree == NULL) {
        perror("Erreur lors de l'ouverture du fichier d'entrée");
        return 1;
    }

    Chaines *chaines = lectureChaines(fichierEntree);
    fclose(fichierEntree);

    if (chaines == NULL) {
        printf("Erreur lors de la lecture des chaines.\n");
        return 1;
    }

    ecrireChaines(chaines, stdout);
    afficheChainesSVG(chaines, "mon_reseau1");
    freeChaines(chaines);

    return 0;
}
