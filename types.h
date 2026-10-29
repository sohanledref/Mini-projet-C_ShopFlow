#ifndef TYPES_H
#define TYPES_H

struct Date{
    int jour;
    int mois;
    int annee;
};

struct Adresse {
    char rue[80];
    char ville[50];
    char codePostal[6];
    char pays[50];
};

struct Produit{
    char reference[12];
    char designation[60];
    char categorie[30];
    float prixUnitaireHT;
    int quantiteStock;
    int seuilAlerte;
    int actif;
};

struct LigneCommande{
    char refProduit[12];
    char designation[60];
    int quantite;
    float prixUnitaireHT;
    float sousTotalHT;
};

struct Commande{
    int numero;
    int idClient;
    struct Date dateCommande;
    char statut[20];
    int nbLignes;
    int capaciteLignes;
    struct LigneCommande *lignes;
    float montantBrutHT;
    float tauxRemise;
    float montantRemise;
    float montantHT;
    float montantTVA;
    float montantTTC;
    float fraisPort;
    struct Adresse adresseLivraison;
};

struct Client {
    int idClient;
    char nom[50];
    char prenom[50];
    char email[80];
    char telephone[15];
    struct Adresse adresse;
    int pointsFidelite;
    int nbCommandes;
    int capaciteCommandes;
    struct Commande *commandes;
    int actif;
};

struct Boutique {
    char nom[50];
    struct Produit *produits;
    int nbProduits;
    int capaciteProduits;
    struct Client *clients;
    int nbClients;
    int capaciteClients;
    int numeroSuivant;
    int idClientSuivant;
};

#endif 