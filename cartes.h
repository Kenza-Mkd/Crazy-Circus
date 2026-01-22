#pragma once
#include "types.h"

extern int nb_cartes_restantes;


/**
 * @brief Génère toutes les configurations objectifs possibles.
 * Calcule et génère toutes les permutations possibles des animaux en
 * utilisant l'algorithme de Heap itératif. Pour chaque permutation,
 * toutes les répartitions possibles entre les deux podiums sont créées
 * et stockées dans une liste globale d'objectifs.
 * @param[in] config Structure contenant les noms des animaux et leur nombre total N.
 */
void generer_tous_objectifs(ConfigAnimaux* config);

/**
 * @brief Pioche un objectif aléatoire parmi ceux générés.
 * Sélectionne uniformément une configuration de jeu parmi la liste des
 * objectifs préalablement générés. Cette fonction est appelée au début
 * de chaque tour pour déterminer la situation à atteindre.
 * @return Un EtatJeu représentant l'objectif à atteindre.
 */
EtatJeu piocher_objectif_aleatoire();

/**
 * @brief Libère la mémoire associée à la liste des objectifs.
 * Libère toute la mémoire allouée pour stocker les différentes
 * configurations objectifs générées par generer_tous_objectifs et
 * réinitialise le compteur nb_cartes_restantes si nécessaire.
 */
void liberer_liste_objectifs();

// --- Fonctions internes (utilisées par l'algorithme) ---

/**
 * @brief Enregistre les configurations issues d'une permutation.
 * Pour une permutation donnée (tableau d'indices A de taille n),
 * génère toutes les coupes possibles entre le podium bleu et le
 * podium rouge, puis enregistre chaque configuration correspondante
 * dans la liste globale d'objectifs.
 * @param[in] A Tableau d'indices représentant une permutation des animaux.
 * @param[in] n Taille du tableau A (nombre d'animaux).
 */
void enregistrer_configurations_pour_permutation(int* A, int n);