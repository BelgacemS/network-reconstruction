#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Reseau.h"
#include "Chaine.h"
#include "SVGwriter.h"
#include "Hachage.h"
#include "ArbreQuat.h"

int main(int argc, char* argv[]){
    if (argc != 2) {
        printf("Veuillez entrer un nom de fichier existant .cha\n");
        return 1;
    }

    // Lecture du fichier des chaînes.
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

    //Création du fichier résultant
    FILE *resultat = fopen("tests/results/results.txt", "w");
    if (resultat == NULL) {
        perror("Erreur lors de la création du fichier tests/results/results.txt");
        return 1;
    }

    clock_t debut, fin;
    double duree;
    Reseau *R;

    // Cas de la liste chaînée
    fprintf(resultat, "Structure : Liste chainee\n");

    debut = clock();
    R = reconstitueReseauListe(C);
    fin = clock();

    duree = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    fprintf(resultat, "Temps d'execution : %f secondes\n", duree);
    freeReseau(R);
    
    // Cas de la table de Hachage
    fprintf(resultat, "\nStructure : Table de hachage\n");

    for(int i=500;i<=5000; i+=500){

        debut = clock();
        R = reconstitueReseauHachage( C, i);
        fin = clock();

        duree = ((double)(fin - debut)) / CLOCKS_PER_SEC;
        fprintf(resultat, "Taille %d Temps d'execution : %f secondes\n", i,duree);
        freeReseau(R);

    }

    // Cas de l'arbre quaternaire'
    fprintf(resultat, "\nStructure : Arbre quaternaire\n");

    debut = clock();
    R = reconstitueReseauArbre(C);
    fin = clock();

    duree = ((double)(fin - debut)) / CLOCKS_PER_SEC;
    fprintf(resultat, "Temps d'execution : %f secondes\n", duree);
    freeReseau(R);
    

    fclose(resultat);

    freeChaines(C);

    return 0;
}