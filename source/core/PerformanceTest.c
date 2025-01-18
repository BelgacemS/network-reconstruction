#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Chaine.h"
#include "Reseau.h"
#include "Hachage.h"
#include "ArbreQuat.h"

#define NB_TAILLES 3

int main(void) {
    srand((unsigned)time(NULL)); // pour assurer la variabilité et l'imprévisibilité des résultats produits par rand()


    clock_t debut, fin;
    double duree;
    
    FILE *f_liste = fopen("tests/results/benchmark_list.txt", "w");
    FILE *f_hachage_arbre = fopen("tests/results/benchmark_hashtree.txt", "w");
    
    if (!f_liste || !f_hachage_arbre) {
        fprintf(stderr, "Erreur lors de l'ouverture des fichiers.\n");
        exit(EXIT_FAILURE);
    }
    
    Reseau *r = NULL;
    Chaines *chaines = NULL;
    int nbPointsChaine = 100, xmax = 5000, ymax = 5000;
    int taillesHachage[NB_TAILLES] = {500, 1000,5000};

    for (int nbChaines = 500; nbChaines <= 3000; nbChaines += 500) {
        printf("Traitement pour %d chaines.\n", nbChaines);
        chaines = generationAleatoire(nbChaines, nbPointsChaine, xmax, ymax);
        
        // Liste chaînée
        debut = clock();
        r = reconstitueReseauListe(chaines);
        fin = clock();
        duree = ((double)(fin - debut)) / CLOCKS_PER_SEC;
        fprintf(f_liste, "%d %.6f\n", nbChaines, duree);
        freeReseau(r);

        // Combinaison pour hachage et arbre
        double duree_hachage_500, duree_hachage_1000, duree_hachage_5000, duree_arbre;
        for (int i = 0; i < NB_TAILLES; i++) {
            int taille = taillesHachage[i];
            debut = clock();
            r = reconstitueReseauHachage(chaines, taille);
            fin = clock();
            duree = ((double)(fin - debut)) / CLOCKS_PER_SEC;
            if (taille == 500) {
                duree_hachage_500 = duree;
            } else if (taille == 1000) {
                duree_hachage_1000 = duree;
            } else {
                duree_hachage_5000 = duree;
            }
            freeReseau(r);
        }

        debut = clock();
        r = reconstitueReseauArbre(chaines);
        fin = clock();
        duree_arbre = ((double)(fin - debut)) / CLOCKS_PER_SEC;
        fprintf(f_hachage_arbre, "%d %.6f %.6f %.6f %.6f\n", 
                nbChaines,           // Nombre de chaînes
                duree_hachage_500,    // Temps avec table de hachage taille 500
                duree_hachage_1000,   // Temps avec table de hachage taille 1000
                duree_hachage_5000,   // Temps avec table de hachage taille 5000
                duree_arbre);         // Temps avec arbre
        freeReseau(r);
        
        freeChaines(chaines);
    }
    
    fclose(f_liste);
    fclose(f_hachage_arbre);

    return 0;
}