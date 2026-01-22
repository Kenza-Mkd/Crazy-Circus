#pragma once
// Énumération des commandes autorisées
typedef enum { KI, LO, SO, NI, MA } Commande;

// Structure pour stocker les commandes du fichier
typedef struct {
    Commande commandes[5];  // Maximum 5 ordres (KI, LO, SO, NI, MA)
    int nb_commandes;       // Nombre réel d'ordres lus
} ConfigCommandes;

// Structure pour stocker les noms des animaux (lus depuis crazy.cfg)
typedef struct {
    char** noms;
    int nombre_total;
} ConfigAnimaux;

// Structure pour un podium (Bleu ou Rouge)
typedef struct {
    int* animaux_ids; // Tableau alloué dynamiquement (malloc)
    int sommet;       // Indice du prochain emplacement libre (-1 si vide)
    int capacite;     // Nombre max d'animaux
} Podium;

// Structure de l'état du jeu
typedef struct {
    Podium bleu;
    Podium rouge;
    // ... autres données comme le nombre de joueurs, scores, etc.
} EtatJeu;