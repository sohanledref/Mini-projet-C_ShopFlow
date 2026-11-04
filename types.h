#ifndef TYPES_H
#define TYPES_H

/* =========================
   STRUCTURES DE BASE
   ========================= */

typedef struct Date {
    int jour;
    int mois;
    int annee;
} Date;

typedef struct Adresse {
    char rue[80];
    char ville[50];
    char codePostal[6];
    char pays[50];
} Adresse;


/* =========================
   PRODUITS
   ========================= */

typedef struct Produit {
    char reference[12];
    char designation[60];
    char categorie[30];
    float prixUnitaireHT;
    int quantiteStock;
    int seuilAlerte;
    int actif;
} Produit;


/* =========================
   PROMOTIONS
   ========================= */

typedef struct Promotion {
    char code[20];
    float tauxReduction;
    Date dateDebut;
    Date dateFin;
    int actif;
} Promotion;


/* =========================
   COMMANDES
   ========================= */

typedef struct LigneCommande {
    char refProduit[12];
    char designation[60];
    int quantite;
    float prixUnitaireHT;
    float sousTotalHT;
} LigneCommande;


typedef struct Commande {
    int numero;
    int idClient;
    Date dateCommande;
    char statut[20];

    int nbLignes;
    int capaciteLignes;
    LigneCommande *lignes;

    float montantBrutHT;
    float tauxRemise;
    float montantRemise;

    /* Informations du code promotionnel */
    char codePromo[20];
    float tauxPromo;
    float montantRemisePromo;

    float montantHT;
    float montantTVA;
    float montantTTC;
    float fraisPort;

    Adresse adresseLivraison;
} Commande;


/* =========================
   CLIENTS
   ========================= */

typedef struct Client {
    int idClient;
    char nom[50];
    char prenom[50];
    char email[80];
    char telephone[15];

    Adresse adresse;

    int pointsFidelite;
    int nbCommandes;
    int capaciteCommandes;
    Commande *commandes;

    int actif;
} Client;


/* =========================
   BOUTIQUE
   ========================= */

typedef struct Boutique {
    char nom[50];

    Produit *produits;
    int nbProduits;
    int capaciteProduits;

    Client *clients;
    int nbClients;
    int capaciteClients;

    /* Tableau dynamique des promotions */
    Promotion *promotions;
    int nbPromotions;
    int capacitePromotions;

    int numeroSuivant;
    int idClientSuivant;
} Boutique;


#endif