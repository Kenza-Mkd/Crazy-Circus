#define _CRT_SECURE_NO_WARNINGS
#include "commandes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int estCommandeValide(const char* cmd, ConfigCommandes* config) {
    static const char* CommandesBase[] = { "KI", "LO", "SO", "NI", "MA" };

    for (int i = 0; i < 5; i++) {
        if (strcmp(cmd, CommandesBase[i]) == 0) {
            if (config->nb_commandes >= 5) {
                fprintf(stderr, "Erreur : Trop de commandes (max 5)\n");
                return 0;
            }
            config->commandes[config->nb_commandes] = (Commande)i;
            config->nb_commandes++;
            return 1;
        }
    }
    return 0;
}

int lire_config_commandes(const char* filename, ConfigCommandes* config) {
    if (filename == NULL || config == NULL) {
        fprintf(stderr, "Erreur : paramètres NULL dans lire_config_commandes\n");
        return 1;
    }

    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s\n", filename);
        return 1;
    }

    char buffer[256];

    // Passer la première ligne (les animaux)
    if (fgets(buffer, sizeof(buffer), f) == NULL) {
        fprintf(stderr, "Erreur : fichier vide ou illisible\n");
        fclose(f);
        return 1;
    }

    config->nb_commandes = 0;
    char commande[8];
    int nbCommandes = 0;

    // Lire la deuxième ligne (les commandes)
    while (fscanf(f, "%7s", commande) == 1) {
        if (!estCommandeValide(commande, config)) {
            fprintf(stderr, "Erreur : Commande inconnue : %s\n", commande);
            fclose(f);
            return 1;
        }
        nbCommandes++;

        // Arrêter à la fin de la ligne
        int c = fgetc(f);
        if (c == '\n' || c == EOF)
            break;
    }

    fclose(f);

    // Vérifier qu'il y a au moins 3 commandes
    if (nbCommandes < 3) {
        fprintf(stderr, "Erreur : Au minimum 3 commandes requises (trouvé : %d)\n",
            nbCommandes);
        return 1;
    }

    return 0;
}


int valider_commande(const char* str, Commande* cmd, const ConfigCommandes* cfg_commandes) {
    static const char* CommandesBase[] = { "KI", "LO", "SO", "NI", "MA" };

    // Vérifier que c'est une commande valide (parmi les 5)
    int indice_cmd = -1;
    for (int i = 0; i < 5; i++) {
        if (strcmp(str, CommandesBase[i]) == 0) {
            indice_cmd = i;
            break;
        }
    }

    // Si ce n'est pas une commande valide
    if (indice_cmd == -1) {
        fprintf(stderr, "Erreur : L'ordre %s n'existe pas\n", str);
        return 1;
    }

    // Vérifier que cette commande est autorisée dans le fichier
    int trouve = 0;
    for (int i = 0; i < cfg_commandes->nb_commandes; i++) {
        if (cfg_commandes->commandes[i] == indice_cmd) {
            trouve = 1;
            break;
        }
    }

    if (!trouve) {
        fprintf(stderr, "Erreur : L'ordre %s n'est pas disponible dans cette partie\n", str);
        return 1;
    }

    *cmd = (Commande)indice_cmd;
    return 0;
}

int executer_KI(Podium* bleu, Podium* rouge) {
    if (bleu->sommet < 0) {
        fprintf(stderr, "Erreur : Podium BLEU vide\n");
        return 1;
    }
    if (rouge->sommet + 1 >= rouge->capacite) {
        fprintf(stderr, "Erreur : Podium ROUGE plein\n");
        return 1;
    }

    int animal = bleu->animaux_ids[bleu->sommet];
    bleu->sommet--;
    rouge->sommet++;
    rouge->animaux_ids[rouge->sommet] = animal;

    return 0;
}

int executer_LO(Podium* bleu, Podium* rouge) {
    if (rouge->sommet < 0) {
        fprintf(stderr, "Erreur : Podium ROUGE vide\n");
        return 1;
    }
    if (bleu->sommet + 1 >= bleu->capacite) {
        fprintf(stderr, "Erreur : Podium BLEU plein\n");
        return 1;
    }

    int animal = rouge->animaux_ids[rouge->sommet];
    rouge->sommet--;
    bleu->sommet++;
    bleu->animaux_ids[bleu->sommet] = animal;

    return 0;
}

int executer_SO(Podium* bleu, Podium* rouge) {
    if (bleu->sommet < 0 || rouge->sommet < 0) {
        fprintf(stderr, "Erreur : Un des podiums est vide\n");
        return 1;
    }

    int temp = bleu->animaux_ids[bleu->sommet];
    bleu->animaux_ids[bleu->sommet] = rouge->animaux_ids[rouge->sommet];
    rouge->animaux_ids[rouge->sommet] = temp;

    return 0;
}

int executer_NI(Podium* bleu) {
    if (bleu->sommet < 0) {
        fprintf(stderr, "Erreur : Podium BLEU vide\n");
        return 1;
    }
    else if (bleu->sommet < 1) {
        fprintf(stderr, "Erreur : Animaux insuffisant sur le podium bleu\n");
        return 1;
    }

	int animal_bas = bleu->animaux_ids[0];
	// Décaler tous les animaux vers le bas
    for (int i = 0; i < bleu->sommet; i++) {
        bleu->animaux_ids[i] = bleu->animaux_ids[i + 1];
    }
	// Placer l'animal du bas au sommet
	bleu->animaux_ids[bleu->sommet] = animal_bas;

    return 0;
}

int executer_MA(Podium* rouge) {
    if (rouge->sommet < 0) {
        fprintf(stderr, "Erreur : Podium ROUGE vide\n");
        return 1;
    }
    else if (rouge->sommet < 1) {
        fprintf(stderr, "Erreur : Animaux insuffisant sur le podium rouge\n");
        return 1;
    }

    int animal_bas = rouge->animaux_ids[0];
    // Décaler tous les animaux vers le bas
    for (int i = 0; i < rouge->sommet; i++) {
        rouge->animaux_ids[i] = rouge->animaux_ids[i + 1];
    }
    // Placer l'animal du bas au sommet
    rouge->animaux_ids[rouge->sommet] = animal_bas;

    return 0;
}

int executer_commande(Commande cmd, Podium* bleu, Podium* rouge) {
    switch (cmd) {
    case KI: return executer_KI(bleu, rouge);
    case LO: return executer_LO(bleu, rouge);
    case SO: return executer_SO(bleu, rouge);
    case NI: return executer_NI(bleu);
    case MA: return executer_MA(rouge);
    default:
        fprintf(stderr, "Erreur : Commande inconnue\n");
        return 1;
    }
}

