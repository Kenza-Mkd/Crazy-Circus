#pragma once
#include "types.h"

/**
 * @brief Vérifie qu'une chaîne est une commande valide et l'ajoute à la config
 * @param cmd Chaîne à vérifier (ex: "KI", "LO", etc.)
 * @param config Pointeur vers ConfigCommandes à remplir
 * @return 1 si valide et ajoutée, 0 sinon
 */
static int estCommandeValide(const char* cmd, ConfigCommandes* config);

/**
 * @brief Lit les commandes depuis la ligne 2 du fichier crazy.cfg.
 * Lit la seconde ligne du fichier de configuration puis remplit la
 * structure ConfigCommandes avec les commandes autorisées. Effectue
 * des vérifications de validité avant d'accepter la configuration.
 * Vérifications :
 * - Au minimum 3 commandes.
 * - Chaque commande doit être dans {KI, LO, SO, NI, MA}.
 * @param[in]  filename Nom du fichier de configuration
 * @param[out] config   Pointeur vers la structure à remplir.
 * @return 0 en cas de succès, 1 en cas d'erreur (fichier invalide ou commandes non conformes).
 */
int lire_config_commandes(const char* filename, ConfigCommandes* config);

/**
 * @brief Valide et convertit une chaîne en commande.
 * Analyse la chaîne saisie par l'utilisateur et, si elle est valide
 * et autorisée par la configuration, stocke la commande correspondante.
 * @param[in]  str            Chaîne saisie par l'utilisateur (par exemple "KI", "KILI").
 * @param[out] cmd            Pointeur vers Commande pour stocker la commande décodée.
 * @param[in]  cfg_commandes  Configuration des commandes autorisées.
 * @return 0 si la commande est valide et autorisée, 1 sinon.
 */
int valider_commande(const char* str, Commande* cmd, const ConfigCommandes* cfg_commandes);

// ===== EXÉCUTION DES COMMANDES =====

/**
 * @brief Exécute une commande sur les podiums.
 * Applique la commande donnée aux podiums bleu et rouge en modifiant leur contenu selon la règle associée.
 * @param[in]     cmd   La commande à exécuter.
 * @param[in,out] bleu  Podium bleu, potentiellement modifié.
 * @param[in,out] rouge Podium rouge, potentiellement modifié.
 * @return 0 en cas de succès, 1 en cas d'erreur (par exemple podium vide).
 */
int executer_commande(Commande cmd, Podium* bleu, Podium* rouge);

/**
 * @brief Exécute la commande KI (sommet bleu → sommet rouge).
 * Déplace l'animal au sommet du podium bleu vers le sommet du podium rouge.
 * @param[in,out] bleu  Podium bleu, source du déplacement.
 * @param[in,out] rouge Podium rouge, destination du déplacement.
 * @return 0 en cas de succès, 1 si le podium bleu est vide.
 */
int executer_KI(Podium* bleu, Podium* rouge);  // B -> R

/**
 * @brief Exécute la commande LO (sommet rouge → sommet bleu).
 * Déplace l'animal au sommet du podium rouge vers le sommet du podium bleu.
 * @param[in,out] bleu  Podium bleu, destination du déplacement.
 * @param[in,out] rouge Podium rouge, source du déplacement.
 * @return 0 en cas de succès, 1 si le podium rouge est vide.
 */
int executer_LO(Podium* bleu, Podium* rouge);  // B <- R

/**
 * @brief Exécute la commande SO (échange des sommets).
 * Échange les animaux se trouvant au sommet des deux podiums.
 * @param[in,out] bleu  Podium bleu, sommet échangé.
 * @param[in,out] rouge Podium rouge, sommet échangé.
 * @return 0 en cas de succès, 1 si au moins un podium est vide.
 */
int executer_SO(Podium* bleu, Podium* rouge);  // B <-> R

/**
 * @brief Exécute la commande NI (bas bleu → sommet bleu).
 * Fait monter l'animal situé en bas du podium bleu au sommet de ce même podium.
 * @param[in,out] bleu Podium bleu modifié.
 * @return 0 en cas de succès, 1 si le podium bleu est vide.
 */
int executer_NI(Podium* bleu);                 // B ^

/**
 * @brief Exécute la commande MA (bas rouge → sommet rouge).
 * Fait monter l'animal situé en bas du podium rouge au sommet de ce même podium.
 * @param[in,out] rouge Podium rouge modifié.
 * @return 0 en cas de succès, 1 si le podium rouge est vide.
 */
int executer_MA(Podium* rouge); // R ^