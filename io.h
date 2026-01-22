#pragma once
#include "types.h"

/**
 * @brief Lit le fichier de configuration des animaux.
 * Lit le fichier crazy.cfg puis alloue et remplit une structure
 * ConfigAnimaux à partir de son contenu. Retourne NULL en cas d'erreur (par exemple si le fichier est introuvable ou invalide).
 * @param[in] filename Chemin vers le fichier crazy.cfg à lire.
 * @return Pointeur vers une structure ConfigAnimaux correctement
 */
ConfigAnimaux* lire_config_animaux(const char* filename);

/**
 * @brief Libère la configuration des animaux.
 * Libère toute la mémoire allouée dynamiquement par lire_config_animaux pour la configuration passée en paramètre.
 * @param[in,out] config Pointeur vers la configuration à libérer.           
 */
void liberer_config_animaux(ConfigAnimaux* config);

/**
 * @brief Affiche l'état actuel et l'objectif côte à côte.
 * Affiche les deux états des podiums (actuel et objectif) à l'écran, en utilisant la configuration pour traduire les indices en noms d'animaux.
 * @param[in] actuel   État courant des podiums du joueur.
 * @param[in] objectif État des podiums à atteindre.
 * @param[in] config   Configuration contenant les noms des animaux.
 */
void afficher_plateau(const EtatJeu* actuel, const EtatJeu* objectif, const ConfigAnimaux* config);