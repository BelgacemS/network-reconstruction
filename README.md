# Projet LU2IN006 - Reconstitution de Réseaux

Ce projet implémente différentes méthodes pour reconstituer un réseau à partir d'un ensemble de chaînes de points. Il propose trois approches différentes : liste chaînée, table de hachage et arbre quaternaire.

## Structure du Projet

```
projet/
├── source/
│   ├── core/              # Fonctionnalités principales
│   ├── data_structures/   # Structures de données
│   └── utilities/         # Fonctions utilitaires
├── include/               # Fichiers d'en-tête
├── build/                 # Fichiers compilés
│   ├── bin/              # Exécutables
│   └── obj/              # Fichiers objets
├── tests/
│   ├── data/             # Fichiers de test
│   └── results/          # Résultats des tests
├── output/               # Fichiers générés (SVG, HTML)
├── docs/                 # Documentation
└── scripts/              # Scripts d'automatisation
```

## Compilation

Pour compiler le projet :
```bash
make clean  # Nettoie les fichiers précédents
make        # Compile tous les exécutables
```

Les exécutables seront générés dans le dossier `build/bin/`.

## Exécutables

1. **ChaineMain** : Visualise les chaînes d'entrée
   ```bash
   ./build/bin/ChaineMain tests/data/fichier.cha
   ```

2. **ReconstitueReseau** : Reconstitue le réseau selon différentes méthodes
   ```bash
   ./build/bin/ReconstitueReseau tests/data/fichier.cha methode
   ```
   où `methode` est :
   - 1 : Liste chaînée
   - 2 : Table de hachage
   - 3 : Arbre quaternaire

3. **PerformanceTest** : Compare les performances des différentes méthodes

## Tests et Visualisations

Pour exécuter tous les tests et générer les visualisations :
```bash
make test
```

Cette commande va :
1. Compiler tous les programmes
2. Exécuter les tests sur différents fichiers d'entrée
3. Générer des graphiques de performance dans `output/`

### Fichiers Générés

- `mon_reseau1.html` : Visualisation des chaînes d'entrée
- `mon_reseau2.html` : Visualisation du réseau reconstitué
- `output_hashtree.png` : Comparaison des performances (hachage vs arbre)
- `output_list.png` : Performance des listes chaînées

> **Note** : Des exemples de visualisations sont directement disponibles dans le dépôt GitHub :
> - Visualisations dans le dossier `output/`
> - Résultats des tests et benchmarks dans `tests/results/`
>   - `benchmark_*.txt` : Données de performance brutes
>   - `results.txt` : Analyse détaillée
>
> Vous pouvez consulter ces fichiers pour voir des exemples de résultats sans avoir à exécuter le code.

## Structure des Fichiers d'Entrée

Les fichiers `.cha` doivent suivre le format :
```
NbChaines
NbPoints
NumChaine x1 y1 x2 y2 ... xn yn
...
```

## Dépendances

- GCC (compilation)
- Python 3 (tests et visualisations)
- Matplotlib (génération des graphiques)
- Navigateur web (visualisation des résultats)

## Nettoyage

Pour nettoyer tous les fichiers générés :
```bash
make clean
```

Cette commande supprime :
- Les fichiers objets et exécutables
- Les fichiers de résultats
- Les visualisations générées
