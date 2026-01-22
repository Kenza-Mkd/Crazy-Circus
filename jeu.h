#pragma once
#include "types.h"
#include <stdbool.h> // Pour utiliser 'bool'

/**
 * @brief Initialise un podium avec une capacité maximale donnée.
 * Alloue la mémoire nécessaire et positionne le podium dans un état
 * vide prêt à recevoir des animaux.
 * @param[out] p           Pointeur vers le podium à initialiser.
 * @param[in]  capacite_max Capacité maximale du podium.
 */
void init_podium(Podium* p, int capacite_max);

/**
 * @brief Libère la mémoire utilisée par un podium.
 * Libère la mémoire dynamique associée au podium et le remet dans un état invalide ou vide.
 * @param[in,out] p Pointeur vers le podium à libérer.
 */
void liberer_podium(Podium* p);

/**
 * @brief Indique si un podium est vide.
 * Teste si le podium ne contient aucun animal.
 * @param[in] p Pointeur constant vers le podium à tester.
 * @return true si le podium est vide, false sinon.
 */
bool est_vide(const Podium* p);

/**
 * @brief Indique si un podium est plein.
 * Vérifie si le podium a atteint sa capacité maximale (cas limite).
 * @param[in] p Pointeur constant vers le podium à tester.
 * @return true si le podium est plein, false sinon.
 */
bool est_plein(const Podium* p);

/**
 * @brief Empile un animal au sommet du podium.
 * Ajoute l'identifiant d'un animal au sommet de la pile si le podium n'est pas plein.
 * @param[in,out] p         Pointeur vers le podium à modifier.
 * @param[in]     id_animal Identifiant de l'animal à empiler.
 * @return true en cas de succès, false si le podium est plein.
 */
bool empiler(Podium* p, int id_animal);

/**
 * @brief Dépile l'animal au sommet du podium.
 * Retire et retourne l'identifiant de l'animal au sommet de la pile.
 * @param[in,out] p Pointeur vers le podium à modifier.
 * @return L'identifiant de l'animal dépilé, ou -1 si le podium est vide.
 */
int depiler(Podium* p);

/**
 * @brief Donne l'animal au sommet sans le dépiler.
 * Consulte l'identifiant de l'animal au sommet du podium sans le retirer.
 * @param[in] p Pointeur constant vers le podium à inspecter.
 * @return L'identifiant de l'animal au sommet, ou -1 si le podium est vide.
 */
int sommet_podium(const Podium* p);
