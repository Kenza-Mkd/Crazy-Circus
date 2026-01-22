#define _CRT_SECURE_NO_WARNINGS
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

const char* const FILE_NAME = "crazy.cfg";

/**
 * @brief Lit une ligne complète depuis un fichier, peu importe la taille.
 * 
 * @param f Le fichier ouvert en lecture
 * @return Une chaîne allouée dynamiquement, ou NULL en cas d'erreur
 */
static char* lire_ligne_complete(FILE* f) {
    enum { BUFFER_SIZE = 10 };
    char buffer[BUFFER_SIZE];

    //   INITIALISATION : allouer 1 octet pour '\0'
    char* line = (char*)calloc(1, 1);
    if (line == NULL) {
        fprintf(stderr, "Erreur : calloc dans lire_ligne_complete()\n");
        return NULL;
    }

    while (fgets(buffer, BUFFER_SIZE, f) != NULL) {
        size_t taille_buffer = strlen(buffer);
        size_t ancienne_taille = strlen(line);
        size_t nouvelle_taille = ancienne_taille + taille_buffer + 1;

        //   PATTERN SÉCURISÉ : pointeur temporaire pour realloc
        // Cela évite de perdre line si realloc échoue
        char* temp = (char*)realloc(line, nouvelle_taille);
        if (temp == NULL) {
            fprintf(stderr, "Erreur : realloc dans lire_ligne_complete()\n");
            free(line);  //   Libérer l'ancien pointeur
            return NULL;
        }
        line = temp;  //   Assigner seulement si succès

        strcat(line, buffer);

        // Si on trouve un '\n', on le remplace et on arrête
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
            return line;
        }
    }

    // Si la boucle se termine sans '\n' trouvé
    if (strlen(line) > 0) {
        return line;
    }

    free(line);
    return NULL;
}

/**
 * @brief Lit le fichier crazy.cfg et remplit la structure ConfigAnimaux
 *
 * Le fichier contient les noms des animaux séparés par des espaces
 * sur la première ligne.
 *
 *   VÉRIFICATIONS :
 * - Fichier existe
 * - Au moins 2 animaux
 * - Allocation mémoire réussie
 *
 * @param filename Nom du fichier (généralement "crazy.cfg")
 * @return Pointeur vers ConfigAnimaux, ou NULL en cas d'erreur
 */
ConfigAnimaux* lire_config_animaux(const char* filename) {
    //   Vérification du paramètre
    if (filename == NULL) {
        fprintf(stderr, "Erreur : filename est NULL\n");
        return NULL;
    }

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s\n", filename);
        return NULL;
    }

    //   Allocation de la structure principale
    ConfigAnimaux* config = (ConfigAnimaux*)malloc(sizeof(ConfigAnimaux));
    if (config == NULL) {
        fprintf(stderr, "Erreur : malloc ConfigAnimaux\n");
        fclose(f);
        return NULL;
    }

    config->noms = NULL;
    config->nombre_total = 0;

    // Lire la première ligne (les noms des animaux)
    char* ligne_animaux = lire_ligne_complete(f);
    fclose(f);

    if (ligne_animaux == NULL) {
        fprintf(stderr, "Erreur : impossible de lire la ligne des animaux\n");
        free(config);
        return NULL;
    }

    // Parser les noms avec strtok
    char* token = strtok(ligne_animaux, " ");
    int i = 0;

    while (token != NULL) {
        //   SÉCURITÉ : Agrandir le tableau de pointeurs
        char** temp = (char**)realloc(config->noms, (i + 1) * sizeof(char*));
        if (temp == NULL) {
            fprintf(stderr, "Erreur : realloc dans parsing\n");
            liberer_config_animaux(config);
            free(ligne_animaux);
            return NULL;
        }
        config->noms = temp;

        //   Allouer et copier le nom
        config->noms[i] = (char*)malloc(strlen(token) + 1);
        if (config->noms[i] == NULL) {
            fprintf(stderr, "Erreur : malloc pour nom animal\n");
            liberer_config_animaux(config);
            free(ligne_animaux);
            return NULL;
        }
        strcpy(config->noms[i], token);

        i++;
        token = strtok(NULL, " ");
    }

    config->nombre_total = i;

    //   VALIDATION : au moins 2 animaux
    if (config->nombre_total < 2) {
        fprintf(stderr, "Erreur : Au minimum 2 animaux requis (trouvé : %d)\n",
            config->nombre_total);
        liberer_config_animaux(config);
        free(ligne_animaux);
        return NULL;
    }

    free(ligne_animaux);
    return config;
}

/**
 * @brief Libère toute la mémoire allouée pour ConfigAnimaux
 *
 *   SÉCURITÉ : Vérification NULL avant chaque free
 */
void liberer_config_animaux(ConfigAnimaux* config) {
    if (config == NULL) return;

    if (config->noms != NULL) {
        for (int i = 0; i < config->nombre_total; i++) {
            if (config->noms[i] != NULL) {
                free(config->noms[i]);
            }
        }
        free(config->noms);
    }
    free(config);
}

/**
 * @brief Affiche l'état actuel et l'objectif côte à côte
 */
void afficher_plateau(const EtatJeu* actuel,
    const EtatJeu* objectif,
    const ConfigAnimaux* config)
{
    int h_max = config->nombre_total;

    /* Largeurs minimum par podium */
    int lab = 4; // BLEU
    int lar = 5; // ROUGE
    int lob = 4; // BLEU
    int lor = 5; // ROUGE

    /* Si un animaux est trop grand pour le podium, Ã©largir le podium */
    /* ACTUEL BLEU */
    for (int i = 0; i <= actuel->bleu.sommet; i++) {
        int len = strlen(config->noms[actuel->bleu.animaux_ids[i]]);
        if (len > lab) lab = len;
    }

    /* ACTUEL ROUGE */
    for (int i = 0; i <= actuel->rouge.sommet; i++) {
        int len = strlen(config->noms[actuel->rouge.animaux_ids[i]]);
        if (len > lar) lar = len;
    }

    /* OBJECTIF BLEU */
    for (int i = 0; i <= objectif->bleu.sommet; i++) {
        int len = strlen(config->noms[objectif->bleu.animaux_ids[i]]);
        if (len > lob) lob = len;
    }

    /* OBJECTIF ROUGE */
    for (int i = 0; i <= objectif->rouge.sommet; i++) {
        int len = strlen(config->noms[objectif->rouge.animaux_ids[i]]);
        if (len > lor) lor = len;
    }

    /* Marges */
    lab += 2;
    lar += 2;
    lob += 2;
    lor += 2;

    printf("\n");

    /* AFFICHAGE DES ANIMAUX */
    for (int h = h_max - 1; h >= 0; h--) {

        /* ACTUEL BLEU */
        if (h <= actuel->bleu.sommet)
            printf("%-*s", lab, config->noms[actuel->bleu.animaux_ids[h]]);
        else
            printf("%-*s", lab, "");

        /* ACTUEL ROUGE */
        if (h <= actuel->rouge.sommet)
            printf("%-*s", lar, config->noms[actuel->rouge.animaux_ids[h]]);
        else
            printf("%-*s", lar, "");

        printf("     ");

        /* OBJECTIF BLEU */
        if (h <= objectif->bleu.sommet)
            printf("%-*s", lob, config->noms[objectif->bleu.animaux_ids[h]]);
        else
            printf("%-*s", lob, "");


        /* OBJECTIF ROUGE */
        if (h <= objectif->rouge.sommet)
            printf("%-*s", lor, config->noms[objectif->rouge.animaux_ids[h]]);
        else
            printf("%-*s", lor, "");

        printf("\n");
    }

    /* AFFICHAGE DES PODIUM */
    printf("%-*s%-*s==>  %-*s%-*s\n",
        lab, "----",
        lar, "-----",
        lob, "----",
        lor, "-----");

    printf("%-*s%-*s     %-*s%-*s\n\n",
        lab, "BLEU",
        lar, "ROUGE",
        lob, "BLEU",
        lor, "ROUGE");
}
