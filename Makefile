CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

# Chemins des dossiers
SRC_CORE = source/core
SRC_DS = source/data_structures
SRC_UTILS = source/utilities
INC = include
BUILD = build
OBJ = $(BUILD)/obj
BIN = $(BUILD)/bin

# Création des dossiers build si nécessaire
$(shell mkdir -p $(OBJ)/core $(OBJ)/data_structures $(OBJ)/utilities $(BIN))

# Inclure les chemins des headers
INCLUDES = -I$(INC)

# Liste des exécutables
EXECUTABLES = ChaineMain ReconstitueReseau main PerformanceTest ReorganiseReseau

all: $(addprefix $(BIN)/, $(EXECUTABLES))
	@echo "Les exécutables sont disponibles dans $(BIN)/"
	@echo "Exemple d'utilisation : $(BIN)/main tests/data/00014_burma.cha"

# Lien des exécutables
$(BIN)/ChaineMain: $(OBJ)/core/ChaineMain.o $(OBJ)/core/Chaine.o $(OBJ)/utilities/SVGwriter.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN)/ReconstitueReseau: $(OBJ)/core/ReconstitueReseau.o $(OBJ)/core/Chaine.o $(OBJ)/utilities/SVGwriter.o $(OBJ)/core/Reseau.o $(OBJ)/data_structures/Hachage.o $(OBJ)/data_structures/ArbreQuat.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN)/main: $(OBJ)/core/main.o $(OBJ)/core/Chaine.o $(OBJ)/utilities/SVGwriter.o $(OBJ)/core/Reseau.o $(OBJ)/data_structures/Hachage.o $(OBJ)/data_structures/ArbreQuat.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN)/PerformanceTest: $(OBJ)/core/PerformanceTest.o $(OBJ)/core/Chaine.o $(OBJ)/utilities/SVGwriter.o $(OBJ)/core/Reseau.o $(OBJ)/data_structures/Hachage.o $(OBJ)/data_structures/ArbreQuat.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BIN)/ReorganiseReseau: $(OBJ)/core/ReorganiseReseau.o $(OBJ)/core/Reseau.o $(OBJ)/core/Graphe.o $(OBJ)/utilities/SVGwriter.o $(OBJ)/core/Chaine.o $(OBJ)/data_structures/Struct_File.o $(OBJ)/data_structures/Struct_Liste.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Règles de compilation des fichiers objets
$(OBJ)/core/%.o: $(SRC_CORE)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ)/data_structures/%.o: $(SRC_DS)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ)/utilities/%.o: $(SRC_UTILS)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Règle pour exécuter les tests
test: all
	@echo "\nExécution des tests..."
	@./scripts/run_tests.py

clean:
	rm -rf $(BUILD)/*
	rm -f $(SRC_CORE)/*.o $(SRC_DS)/*.o $(SRC_UTILS)/*.o
	rm -f output/*.png output/*.svg output/*.html
	rm -f tests/results/benchmark_*.txt tests/results/results.txt
	rm -f mon_reseau*.svg mon_reseau*.html
	# Suppression des exécutables qui pourraient être à la racine
	rm -f ChaineMain ReconstitueReseau main PerformanceTest ReorganiseReseau
	rm -f ./*.o
	# Suppression des fichiers qui pourraient être à la racine par erreur
	rm -f ./benchmark_*.txt ./results.txt

.PHONY: all clean test