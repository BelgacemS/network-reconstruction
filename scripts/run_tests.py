#!/usr/bin/env python3

import os
import subprocess
import matplotlib.pyplot as plt
import time
from pathlib import Path
import shutil

class TestRunner:
    def __init__(self):
        self.project_root = Path(__file__).parent.parent
        self.bin_dir = self.project_root / "build" / "bin"
        self.data_dir = self.project_root / "tests" / "data"
        self.results_dir = self.project_root / "tests" / "results"
        self.output_dir = self.project_root / "output"
        
        # Vérifier si valgrind est disponible
        self.has_valgrind = shutil.which('valgrind') is not None
        if not self.has_valgrind:
            print("Note: valgrind n'est pas installé, les tests seront exécutés sans valgrind")
        
        # Créer les dossiers s'ils n'existent pas
        self.results_dir.mkdir(parents=True, exist_ok=True)
        self.output_dir.mkdir(parents=True, exist_ok=True)

    def run_command(self, cmd, use_valgrind=True):
        """Exécute une commande avec ou sans valgrind"""
        if use_valgrind and self.has_valgrind:
            full_cmd = ["valgrind"] + cmd
        else:
            full_cmd = cmd
        
        try:
            result = subprocess.run(full_cmd, 
                                 capture_output=True, 
                                 text=True, 
                                 check=True)
            print(f"✓ {' '.join(cmd)}")
            return True
        except subprocess.CalledProcessError as e:
            print(f"✗ Erreur lors de l'exécution de {' '.join(cmd)}")
            print(f"  Sortie d'erreur : {e.stderr}")
            return False

    def run_basic_tests(self):
        """Exécute les tests de base avec les différents fichiers d'entrée"""
        print("\n=== Tests de base ===")
        test_files = ["00014_burma.cha", "05000_USA-road-d-NY.cha", "07397_pla.cha"]
        programs = ["ChaineMain", "main"]

        for prog in programs:
            print(f"\nExécution de {prog}:")
            for test_file in test_files:
                self.run_command([
                    str(self.bin_dir / prog),
                    str(self.data_dir / test_file)
                ])

    def run_reconstitue_tests(self):
        """Exécute les tests de ReconstitueReseau avec différentes options"""
        print("\n=== Tests ReconstitueReseau ===")
        test_files = ["00014_burma.cha", "05000_USA-road-d-NY.cha", "07397_pla.cha"]
        
        for test_file in test_files:
            print(f"\nTests avec {test_file}:")
            for option in range(1, 4):
                self.run_command([
                    str(self.bin_dir / "ReconstitueReseau"),
                    str(self.data_dir / test_file),
                    str(option)
                ])

    def run_performance_test(self):
        """Exécute le test de performance"""
        print("\n=== Test de Performance ===")
        self.run_command([str(self.bin_dir / "PerformanceTest")], use_valgrind=False)

    def plot_performance_results(self):
        """Génère les graphiques de performance avec matplotlib"""
        print("\n=== Génération des graphiques ===")
        
        # Graphique pour les tables de hachage et arbre
        try:
            data = []
            benchmark_file = self.results_dir / "benchmark_hashtree.txt"
            if benchmark_file.exists():
                with open(benchmark_file) as f:
                    for line in f:
                        data.append([float(x) for x in line.strip().split()])
            
                if data:
                    chains = [row[0] for row in data]
                    hash500 = [row[1] for row in data]
                    hash1000 = [row[2] for row in data]
                    hash5000 = [row[3] for row in data]
                    tree = [row[4] for row in data]

                    plt.figure(figsize=(12, 7))
                    plt.plot(chains, hash500, label='Table de Hachage Taille 500', color='blue')
                    plt.plot(chains, hash1000, label='Table de Hachage Taille 1000', color='green')
                    plt.plot(chains, hash5000, label='Table de Hachage Taille 5000', color='orange')
                    plt.plot(chains, tree, label='Arbre', color='red')
                    plt.title('Comparaison des Performances')
                    plt.xlabel('Nombre de Chaînes')
                    plt.ylabel('Temps (secondes)')
                    plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))
                    plt.tight_layout()
                    plt.savefig(self.output_dir / 'output_hashtree.png')
                    print("✓ Graphique hashtree généré")
            else:
                print("! Fichier benchmark_hashtree.txt non trouvé")
            
            # Graphique pour les listes chaînées
            data = []
            benchmark_file = self.results_dir / "benchmark_list.txt"
            if benchmark_file.exists():
                with open(benchmark_file) as f:
                    for line in f:
                        data.append([float(x) for x in line.strip().split()])
            
                if data:
                    chains = [row[0] for row in data]
                    times = [row[1] for row in data]

                    plt.figure(figsize=(12, 7))
                    plt.plot(chains, times, label='Listes Chaînées', color='purple')
                    plt.title('Performance des Listes Chaînées')
                    plt.xlabel('Nombre de Chaînes')
                    plt.ylabel('Temps (secondes)')
                    plt.legend(loc='center left', bbox_to_anchor=(1, 0.5))
                    plt.tight_layout()
                    plt.savefig(self.output_dir / 'output_list.png')
                    print("✓ Graphique listes généré")
            else:
                print("! Fichier benchmark_list.txt non trouvé")
        
        except Exception as e:
            print(f"✗ Erreur lors de la génération des graphiques : {e}")

def main():
    runner = TestRunner()
    
    # Exécuter les tests
    runner.run_basic_tests()
    runner.run_reconstitue_tests()
    runner.run_performance_test()
    
    # Attendre un peu que les fichiers soient écrits
    time.sleep(1)
    
    # Générer les graphiques
    runner.plot_performance_results()

if __name__ == "__main__":
    main()
