#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

#include "Chaine.h"
#include "SVGwriter.h"


Chaines* lectureChaines(FILE *fichier) {

    Chaines *chaines = (Chaines*)malloc(sizeof(Chaines));
    if (!fichier || !chaines) {
        return NULL;
    }

    fscanf(fichier, "NbChain: %d\nGamma: %d\n", &chaines->nbChaines, &chaines->gamma);
    chaines->chaines = NULL;

    CellChaine *derniereChaine = NULL;

    for (int i = 0; i < chaines->nbChaines; i++) {
        CellChaine *nouvelleChaine = (CellChaine*)malloc(sizeof(CellChaine));
        if (!nouvelleChaine) {
  
            return NULL;
        }

        // Initialisation des champs de nouvelleChaine
        nouvelleChaine->points = NULL;
        nouvelleChaine->suiv = NULL;
        fscanf(fichier, "%d", &nouvelleChaine->numero);

        if (!chaines->chaines) {
            chaines->chaines = nouvelleChaine;
        } else {
            derniereChaine->suiv = nouvelleChaine;
        }
        derniereChaine = nouvelleChaine;


        int nbPoints; 
        fscanf(fichier, "%d", &nbPoints); 
        
        CellPoint *dernierPoint = NULL;
        for (int j = 0; j < nbPoints; j++) {
            CellPoint *nouveauPoint = (CellPoint*)malloc(sizeof(CellPoint));
            if (!nouveauPoint) {

                return NULL;
            }

            // Initialisation et lecture des coordonnées x, y
            nouveauPoint->suiv = NULL;
            fscanf(fichier, "%lf %lf", &nouveauPoint->x, &nouveauPoint->y);

            if (!nouvelleChaine->points) {
                nouvelleChaine->points = nouveauPoint;
            } else {
                dernierPoint->suiv = nouveauPoint;
            }
            dernierPoint = nouveauPoint;
        }
    }
    return chaines;
}

void ecrireChaines(Chaines *C, FILE *f) {
    if (C == NULL || f == NULL) {
        return; 
    }

    // Écriture de l'entête avec NbChain et Gamma
    fprintf(f, "NbChain: %d\nGamma: %d\n", C->nbChaines, C->gamma);

    // Parcours de chaque CellChaine dans la structure Chaines
    for (CellChaine *chaineCourante = C->chaines; chaineCourante != NULL; chaineCourante = chaineCourante->suiv) {
        // Comptage du nombre de points dans la chaîne courante pour le formatage correct
        int nbPoints = 0;
        for (CellPoint *pointCourant = chaineCourante->points; pointCourant != NULL; pointCourant = pointCourant->suiv) {
            nbPoints++;
        }

        // Écriture du numéro de la chaine et du nombre de points
        fprintf(f, "%d %d ", chaineCourante->numero, nbPoints);

        // Parcours de chaque CellPoint dans la CellChaine courante
        for (CellPoint *pointCourant = chaineCourante->points; pointCourant != NULL; pointCourant = pointCourant->suiv) {
            // Écriture des coordonnées de chaque point
            fprintf(f, "%.2lf %.2lf ", pointCourant->x, pointCourant->y);
        }

        fprintf(f, "\n");
    }
}

void freeChaines(Chaines *C) {
    if (C == NULL) {
        return; 
    }

    // Parcourir chaque CellChaine de la liste
    CellChaine *chaineCourante = C->chaines;
    while (chaineCourante != NULL) {
        CellChaine *chaineASupprimer = chaineCourante; 
        chaineCourante = chaineCourante->suiv;

        // Parcourir et libérer chaque CellPoint de la chaine courante
        CellPoint *pointCourant = chaineASupprimer->points;
        while (pointCourant != NULL) {
            CellPoint *pointASupprimer = pointCourant; 
            pointCourant = pointCourant->suiv; 

            free(pointASupprimer); 
        }

        free(chaineASupprimer); // Libérer la CellChaine après avoir libéré tous ses points
    }

    free(C); // Libérer l'objet Chaines lui-même après avoir libéré toutes ses chaines
}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    // int i; on utilise pas la variable i 
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;  
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    char nom_fichier[256];
    snprintf(nom_fichier, sizeof(nom_fichier), "output/%s", nomInstance);
    SVGinit(&svg, nom_fichier, 500, 500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny)); 
        precx=pcour->x;
        precy=pcour->y;  
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;    
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

double longueurChaine(CellChaine *c) {
    if (c == NULL || c->points == NULL) {
        return 0.0;
    }

    double longueur = 0.0;
    CellPoint *pointActuel = c->points;
    CellPoint *pointSuivant = pointActuel->suiv;

    while (pointSuivant != NULL) {
        double dx = pointSuivant->x - pointActuel->x;
        double dy = pointSuivant->y - pointActuel->y;
        longueur += sqrt(dx * dx + dy * dy);

        pointActuel = pointSuivant;
        pointSuivant = pointSuivant->suiv;
    }

    return longueur;
}

double longueurTotale(Chaines *C) {
    if (C == NULL || C->chaines == NULL) {
        return 0.0;
    }

    double longueurTotale = 0.0;
    CellChaine *chaineCourante = C->chaines;

    while (chaineCourante != NULL) {
        longueurTotale += longueurChaine(chaineCourante);
        chaineCourante = chaineCourante->suiv;
    }

    return longueurTotale;
}

int comptePointsTotal(Chaines *C) {
    if (C == NULL) {
        return 0; 
    }

    int totalPoints = 0;
    CellChaine *chaineCourante = C->chaines;

    while (chaineCourante != NULL) {
        CellPoint *pointCourant = chaineCourante->points;
        while (pointCourant != NULL) {
            totalPoints++; // Incrémente le compteur pour chaque point trouvé
            pointCourant = pointCourant->suiv;
        }
        chaineCourante = chaineCourante->suiv;
    }

    return totalPoints;
}

void ajouterPoint(CellChaine *chaine, double x, double y) {
    
    CellPoint *new = (CellPoint*)malloc(sizeof(CellPoint));
    if (new == NULL) {
        fprintf(stderr, "Erreur d'allocation pour le nouveau point\n");
        exit(EXIT_FAILURE);
    }

    new->x = x;
    new->y = y;
    
    // Ajout du point au début
    new->suiv = chaine->points;
    chaine->points = new;
}

CellChaine* creerCellChaine(int numero) {
    CellChaine *nc = (CellChaine*)malloc(sizeof(CellChaine));
    if (nc == NULL) {
        fprintf(stderr, "Erreur d'allocation pour la nouvelle cellule de chaîne\n");
        exit(EXIT_FAILURE);
    }
    nc->numero = numero;
    nc->points = NULL;
    nc->suiv = NULL;
    return nc;
}

void ajouterCellChaineDebut(Chaines *C, CellChaine *cell) {
    // Vérifier si la nouvelle cellule de chaîne est valide
    if (cell == NULL) {
        fprintf(stderr, "La cellule de chaîne à ajouter est nulle\n");
        return;
    }

    //Ajout de la cellule au début
    cell->suiv = C->chaines;
    C->chaines = cell;

    // MAJ 
    C->nbChaines++;
}

Chaines* generationAleatoire(int nbChaines,int nbPointsChaine,int xmax, int ymax){
    Chaines *C = (Chaines*)malloc(sizeof(Chaines));
    C->gamma=5; //importe peu
    C->nbChaines=0;

    for(int i=0; i<nbChaines;i++){
        //Création de la cellule et ajout dans C
        CellChaine *cell=creerCellChaine(i);
        ajouterCellChaineDebut(C, cell);

        for(int j=0; j<nbPointsChaine;j++){
            //ajout de point généré aléatoirement 
            double x= ((double)rand() / RAND_MAX) * xmax;
            double y= ((double)rand() / RAND_MAX) * xmax;
            ajouterPoint( cell, x, y);
        }

    }

    return C;
}