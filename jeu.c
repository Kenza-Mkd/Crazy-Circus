#include "jeu.h"
#include <stdlib.h>
#include <stdio.h>

void init_podium(Podium* podium, int capacite) {
    podium->animaux_ids = (int*)malloc(capacite * sizeof(int));
    podium->sommet = -1;
    podium->capacite = capacite;
}

void liberer_podium(Podium* podium) {
    if (podium && podium->animaux_ids) {
        free(podium->animaux_ids);
        podium->animaux_ids = NULL;
    }
}

bool est_vide(const Podium* p) {
    if (p == NULL) return true; // NULL = vide
    return p->sommet == -1;
}

bool est_plein(const Podium* p) {
    if (p == NULL) return true;  
    return p->sommet == p->capacite - 1;
}

void empiler(Podium* podium, int animal_id) {
    if (podium->sommet + 1 < podium->capacite) {
        podium->sommet++;
        podium->animaux_ids[podium->sommet] = animal_id;
    }
}

int depiler(Podium* p) {
    // Vérification de NULL
    if (p == NULL || p->animaux_ids == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans depiler()\n");
        return -1;
    }

    if (est_vide(p)) {
        return -1;  // -1 = erreur (pile vide)
    }

    // LOGIQUE CORRIGÉE :
    // 1. Récupérer l'animal à la position sommet
    int animal = p->animaux_ids[p->sommet];

    // 2. Décrémenter sommet (revenir vers -1 si vide)
    p->sommet--;

    return animal;
}

int sommet_podium(const Podium* p) {
    // Vérification de NULL
    if (p == NULL || p->animaux_ids == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans sommet_podium()\n");
        return -1;
    }

    if (est_vide(p)) {
        return -1;
    }
    // Retourner directement l'élément à la position sommet
    // (pas besoin de -1 comme avant)
    return p->animaux_ids[p->sommet];
}
