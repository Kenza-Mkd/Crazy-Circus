#include "cartes.h"
#include "jeu.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static EtatJeu* tableaux_objectifs = NULL;
static int nb_objectifs_total = 0;
static int index_insertion = 0;

static int* cartes_restantes = NULL;
int nb_cartes_restantes = 0;

int factorielle(int n) {
    int f = 1;
    for (int i = 1; i <= n; i++) f *= i;
    return f;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void generer_tous_objectifs(ConfigAnimaux* config) {
    if (config == NULL) return;
    int n = config->nombre_total;

    nb_objectifs_total = factorielle(n) * (n + 1);
    tableaux_objectifs = (EtatJeu*)calloc(nb_objectifs_total, sizeof(EtatJeu));

    int* A = (int*)calloc(n, sizeof(int));
    int* c = (int*)calloc(n, sizeof(int));

    if (tableaux_objectifs == NULL || A == NULL || c == NULL) {
        if (A) free(A);
        if (c) free(c);
        if (tableaux_objectifs) free(tableaux_objectifs);
        return;
    }

    for (int j = 0; j < n; j++) {
        A[j] = j;
    }

    index_insertion = 0;

    enregistrer_configurations_pour_permutation(A, n);

    int i = 0;
    while (i < n) {
        if (c[i] < i) {
            if (i % 2 == 0) swap(&A[0], &A[i]);
            else swap(&A[c[i]], &A[i]);

            enregistrer_configurations_pour_permutation(A, n);
            c[i] += 1;
            i = 0;
        }
        else {
            c[i] = 0;
            i += 1;
        }
    }

    // initialiser la liste des cartes restantes 
    cartes_restantes = (int*)malloc(nb_objectifs_total * sizeof(int));
    if (cartes_restantes != NULL) {
        for (int i = 0; i < nb_objectifs_total; i++) {
            cartes_restantes[i] = i;  // Indice des cartes
        }
        nb_cartes_restantes = nb_objectifs_total;
    }

    free(A);
    free(c);
}

void enregistrer_configurations_pour_permutation(int* A, int n) {
    for (int k = 0; k <= n; k++) {
        init_podium(&(tableaux_objectifs[index_insertion].bleu), n);
        init_podium(&(tableaux_objectifs[index_insertion].rouge), n);

        for (int b = 0; b < k; b++) {
            empiler(&(tableaux_objectifs[index_insertion].bleu), A[b]);
        }

        for (int r = k; r < n; r++) {
            empiler(&(tableaux_objectifs[index_insertion].rouge), A[r]);
        }

        index_insertion++;
    }
}

// piocher SANS remise 
EtatJeu piocher_objectif_aleatoire() {
    if (nb_cartes_restantes <= 0) {
        // Si il n'y a plus de cartes alors on retourne une carte vide (ou erreur)
        EtatJeu vide;
        vide.bleu.sommet = -1;
        vide.rouge.sommet = -1;
        return vide;
    }

    // Choisir un indice aléatoire dans les cartes restantes
    int rand_idx = rand() % nb_cartes_restantes;
    int carte_index = cartes_restantes[rand_idx];

    // Retirer cette carte de la liste
    cartes_restantes[rand_idx] = cartes_restantes[nb_cartes_restantes - 1];
    nb_cartes_restantes--;

    return tableaux_objectifs[carte_index];
}

void liberer_liste_objectifs() {
    if (tableaux_objectifs != NULL) {
        for (int i = 0; i < nb_objectifs_total; i++) {
            if (tableaux_objectifs[i].bleu.animaux_ids != NULL)
                free(tableaux_objectifs[i].bleu.animaux_ids);
            if (tableaux_objectifs[i].rouge.animaux_ids != NULL)
                free(tableaux_objectifs[i].rouge.animaux_ids);
        }
        free(tableaux_objectifs);
    }

    if (cartes_restantes != NULL) {
        free(cartes_restantes);
    }
}
