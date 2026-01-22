#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "types.h"
#include "io.h"
#include "jeu.h"
#include "cartes.h"
#include "commandes.h"

// Structure pour tracker un joueur
typedef struct {
    char* nom;
    int score;
    int peut_jouer;
} Joueur;

int comparer_joueurs(const void* a, const void* b) {
    const Joueur* j1 = (const Joueur*)a;
    const Joueur* j2 = (const Joueur*)b;
    if (j1->score != j2->score) {
        return j2->score - j1->score;
    }
    return strcmp(j1->nom, j2->nom);
}

int sequence_correcte(const EtatJeu* etat_apres, const EtatJeu* objectif, int nb_animaux) {
    if (etat_apres->bleu.sommet != objectif->bleu.sommet) {
        return 0;
    }
    for (int i = 0; i <= etat_apres->bleu.sommet; i++) {
        if (etat_apres->bleu.animaux_ids[i] != objectif->bleu.animaux_ids[i]) {
            return 0;
        }
    }
    if (etat_apres->rouge.sommet != objectif->rouge.sommet) {
        return 0;
    }
    for (int i = 0; i <= etat_apres->rouge.sommet; i++) {
        if (etat_apres->rouge.animaux_ids[i] != objectif->rouge.animaux_ids[i]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, const char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Erreur : Au minimum 2 joueurs requis\n");
        return 1;
    }

    int nb_joueurs = argc - 1;
    Joueur* joueurs = (Joueur*)malloc(nb_joueurs * sizeof(Joueur));
    if (!joueurs) {
        fprintf(stderr, "Erreur : Allocation mémoire joueurs\n");
        return 1;
    }

    for (int i = 0; i < nb_joueurs; i++) {
        for (int j = i + 1; j < nb_joueurs; j++) {
            if (strcmp(argv[i + 1], argv[j + 1]) == 0) {
                fprintf(stderr, "Erreur : Les joueurs doivent avoir des noms distincts\n");
                free(joueurs);
                return 1;
            }
        }
    }

    for (int i = 0; i < nb_joueurs; i++) {
        joueurs[i].nom = (char*)malloc(strlen(argv[i + 1]) + 1);
        strcpy(joueurs[i].nom, argv[i + 1]);
        joueurs[i].score = 0;
        joueurs[i].peut_jouer = 1;
    }

    ConfigAnimaux* config = lire_config_animaux("crazy.cfg");
    if (config == NULL) {
        free(joueurs);
        return 1;
    }

    ConfigCommandes cfg_commandes;
    if (lire_config_commandes("crazy.cfg", &cfg_commandes) != 0) {
        liberer_config_animaux(config);
        free(joueurs);
        return 1;
    }

    srand((unsigned int)time(NULL));

    generer_tous_objectifs(config);

    printf("KI (B -> R) | LO (B <- R) | SO (B <-> R) | NI (B ^) | MA (R ^)\n");

    EtatJeu jeu_actuel= piocher_objectif_aleatoire();

    int tour = 0;

    while (tour < factorielle( config->nombre_total +1) && nb_cartes_restantes > 0) { // MODIFIER LE NOMBRE DE CARTE POSSIBLE AVEC LES ANIMAUX
        for (int i = 0; i < nb_joueurs; i++) {
            joueurs[i].peut_jouer = 1;
        }

        EtatJeu objectif;

        do {
            objectif = piocher_objectif_aleatoire();
        } while (
            objectif.bleu.animaux_ids != NULL &&
            sequence_correcte(&jeu_actuel, &objectif, config->nombre_total)
            );


        if (objectif.bleu.animaux_ids == NULL && objectif.rouge.animaux_ids == NULL) {
            break;
        }

        afficher_plateau(&jeu_actuel, &objectif, config);

        int joueurs_restants = nb_joueurs;
        int gagnant_tour = -1;

        while (joueurs_restants > 0 && gagnant_tour == -1) {
            char input_ligne[256];
            char nom_joueur[50];
            char sequence[200];

            if (fgets(input_ligne, sizeof(input_ligne), stdin) == NULL) {
                break;
            }

            if (strncmp(input_ligne, "QUIT", 4) == 0) {
                goto fin_jeu;
            }

            if (sscanf(input_ligne, "%s %s", nom_joueur, sequence) != 2) {
                fprintf(stderr, "Erreur : Format invalide (NomJoueur SEQUENCE)\n");
                continue;
            }

            int indice_joueur = -1;
            for (int i = 0; i < nb_joueurs; i++) {
                if (strcmp(nom_joueur, joueurs[i].nom) == 0) {
                    indice_joueur = i;
                    break;
                }
            }

            if (indice_joueur == -1) {
                fprintf(stderr, "Erreur : Joueur %s inconnu\n", nom_joueur);
                continue;
            }

            if (joueurs[indice_joueur].peut_jouer == 0) {
                fprintf(stderr, "%s ne peut pas jouer\n", nom_joueur);
                continue;
            }

            EtatJeu etat_test = jeu_actuel;
            int sequence_valide = 1;

            int sequence_len = strlen(sequence);
            for (int i = 0; i < sequence_len; i += 2) {
                if (i + 1 >= sequence_len) {
                    fprintf(stderr, "Erreur : Séquence incomplète\n");
                    sequence_valide = 0;
                    break;
                }

                char cmd_str[3] = { sequence[i], sequence[i + 1], '\0' };
                Commande cmd;

                if (valider_commande(cmd_str, &cmd, &cfg_commandes) != 0) {
                    sequence_valide = 0;
                    break;
                }

                if (executer_commande(cmd, &etat_test.bleu, &etat_test.rouge) != 0) {
                    sequence_valide = 0;
                    break;
                }
            }

            if (!sequence_valide) {
                fprintf(stderr, "La sequence ne conduit pas a la situation attendue -- %s ne peut plus jouer durant ce tour\n", nom_joueur);
                joueurs[indice_joueur].peut_jouer = 0;
                joueurs_restants--;

                if (joueurs_restants == 1) {
                    for (int i = 0; i < nb_joueurs; i++) {
                        if (joueurs[i].peut_jouer == 1) {
                            printf("%s gagne un point car lui seul peut encore jouer durant ce tour\n", joueurs[i].nom);
                            joueurs[i].score++;
                            gagnant_tour = i;
                            jeu_actuel = etat_test;
                            break;
                        }
                    }
                }
            }
            else {
                if (sequence_correcte(&etat_test, &objectif, config->nombre_total)) {
                    printf("%s gagne un point\n", nom_joueur);
                    joueurs[indice_joueur].score++;
                    gagnant_tour = indice_joueur;
                    jeu_actuel = etat_test;
                }
                else {
                    fprintf(stderr, "La sequence ne conduit pas a la situation attendue -- %s ne peut plus jouer durant ce tour\n", nom_joueur);
                    joueurs[indice_joueur].peut_jouer = 0;
                    joueurs_restants--;

                    if (joueurs_restants == 1) {
                        for (int i = 0; i < nb_joueurs; i++) {
                            if (joueurs[i].peut_jouer == 1) {
                                printf("%s gagne un point car lui seul peut encore jouer durant ce tour\n", joueurs[i].nom);
                                joueurs[i].score++;
                                gagnant_tour = i;
                                jeu_actuel = etat_test;
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (gagnant_tour != -1) {
            tour++;
        }
    }

fin_jeu:
    printf("\n");

    qsort(joueurs, nb_joueurs, sizeof(Joueur), comparer_joueurs);

    for (int i = 0; i < nb_joueurs; i++) {
        printf("%s %d\n", joueurs[i].nom, joueurs[i].score);
    }

    liberer_liste_objectifs();
    liberer_config_animaux(config);
    liberer_podium(&jeu_actuel.bleu);
    liberer_podium(&jeu_actuel.rouge);

    for (int i = 0; i < nb_joueurs; i++) {
        free(joueurs[i].nom);
    }
    free(joueurs);

    return 0;
}
