#include <string.h> // strlen, strcat
#include <stdio.h>  // fopen, fgets, fclose, printf
#include <assert.h> // assert
#include <stdlib.h> // calloc, realloc, free

// le fichier crazy.cfg doit être dans le même dossier que votre .c 
const char* const FILE_NAME = "crazy.cfg";

// La fonction qui suit illustre la lecture d'un fichier texte bloc par bloc.
// La variable buffer stocke les blocs lus l'un à la suite de l'autre.
//
// La taille des blocs est insuffisante pour pouvoir lire une ligne du 
// fichier en un unique fgets. 
//
// Quelque soit la taille choisie, ce cas de figure peut arriver avec le 
// fichier crazy.cfg car la ligne décrivant les animaux peut être d'une 
// longueur quelconque.

static void byBlock() {
	FILE* f = fopen(FILE_NAME, "r");
	if (f == NULL) {
		printf("fichier non accessible\n");
		return;
	}
	enum { BUFFER_SIZE = 10 };
	char buffer[BUFFER_SIZE];
	char* s = fgets(buffer, BUFFER_SIZE, f);
	while (s != NULL) {
		size_t taille = strlen(buffer);
		assert(taille != 0);
		if (buffer[taille - 1] == '\n') {
			// La fin de ligne a été lue, 
			// Elle peut être facilement retirée du buffer
			buffer[taille - 1] = '\0';
			// Mais la chaîne résultante peut être vide
			if (taille > 1)
				printf("'%s'", buffer);
			printf("\n");
		}
		else {
			// La fin de ligne n'a pas été lue
			printf("'%s'", buffer);
		}
		s = fgets(buffer, BUFFER_SIZE, f);
	}
	printf("\n");
	fclose(f);
}

// Si vous voulez collecter toute une ligne dans une unique variable
// il faut employer l'allocation dynamique (malloc et realloc) 
// associée aux fonctions telle que strcat (concaténation de chaînes).
//
// C'est ce qu'illustre la fonction suivante.

// Quelques réglages spécifiques à Visual...
// 4996 strcat est unsafe, on le sait et on s'en prémuni, ce n'est pas grave.
#pragma warning (disable : 4996)
// 6308 realloc peut retourner NULL s'il n'y a pas assez de mémoire disponible
// 28183 strcat ne veut pas de NULL en premier argument
// on ignore ces deux dernières erreurs potentielles, ce n'est pas bien
#pragma warning (disable : 6308 28183)

static void withConcatenation() {
	FILE* f = fopen(FILE_NAME, "r");
	if (f == NULL) {
		printf("fichier non accessible\n");
		return;
	}
	enum { BUFFER_SIZE = 10 };
	char buffer[BUFFER_SIZE];
	char* line = (char*)calloc(1, 1);
	char* s = fgets(buffer, BUFFER_SIZE, f);
	while (s != NULL) {
		size_t taille = strlen(buffer);
		assert(taille != 0);
		if (buffer[taille - 1] == '\n') {
			// La fin de ligne a été lue, 
			// Elle peut être facilement retirée du buffer
			buffer[taille - 1] = '\0';
			// Mais la chaîne résultante peut être vide
			if (taille > 1) {
				line = (char*)realloc(line, strlen(line) + taille);
				strcat(line, buffer);
			}
			printf("'%s'\n", line);
			line[0] = '\0';
		}
		else {
			// La fin de ligne n'a pas été lue
			line = (char*)realloc(line, strlen(line) + taille + 1);
			strcat(line, buffer);
		}
		s = fgets(buffer, BUFFER_SIZE, f);
	}
	// si le fichier ne termine pas par une ligne vide, on affiche le contenu collecté
	if (strlen(line) != 0)
		printf("'%s'\n", line);
	free(line);
	fclose(f);
}

// Ce programme illustre comment les paramêtres reçus sur la ligne de commande
// peuvent être récupérés un à un.
int main(int argc, const char* argv[]) {
	printf("%d parametre(s) recu(s) sur la ligne de commande\n", argc - 1);
	for (int i = 1; i < argc; ++i)
		printf("%s\n", argv[i]);
	printf("\n");

	byBlock();
	withConcatenation();
}