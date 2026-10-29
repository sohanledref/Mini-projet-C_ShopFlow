#ifndef TYPE_H
#define TYPE_H

#define TVA 20.0f
#define Frais_de_port 5.90f
#define Frais_de_port_offert 60.0f
#define seuil_remise_5 100.0f
#define seuil_remise_10 300.0f
#define seuil_remise_15 600.0f
#define remise_0 0.0f
#define remise_5 5.0f
#define remise_10 10.0f
#define remise_15 15.0f
#define EN_COURS "EN_COURS"
#define EXPEDIEE "EXPEDIEE"
#define LIVREE "LIVREE"
#define ANNULEE "ANNULEE"

typedef struct { int Jour ; int Mois ; int Annee ; } Date;
typedef struct { char rue[80] ; char ville[50] ; char codePostal[6] ; char pays[50] ; } Adresse;
typedef struct { char reference[12] ; char designation[60] ; char categorie[30] ; float prixUnitaireHT ; int quantiteStock ; int seuilAlerte ; int actif ; } Produit;
typedef struct { char refProduit[12] ; char designation[60] ; int quantite ; float prixUnitaireHT ; float sousTotalHT ; } LigneCommande;
typedef struct { int numero ; int idClient ; Date dateCommande ; char statut[20] ; int nbLignes ; int capaciteLignes ; LigneCommande *lignes ; float montantBrutHT ; float tauxRemise ; float montantRemise ; float montantHT ; float montantTVA ; float montantTTC ; float fraisPort ; Adresse adresseLivraison ; } Commande;
typedef struct { int idClient ; char nom[50] ; char prenom[50] ; char email[80] ; char telephone[15] ; Adresse adresse ; int pointsFidelite ; int nbCommandes ; int capaciteCommandes ; Commande *commandes ; int actif ; } Client;
typedef struct { char nom[50] ; Produit *produits ; int nbProduits ; int capaciteProduits ; Client *clients ; int nbClients ; int capaciteClients ; int numeroSuivant ; int idClientSuivant ; } Boutique;

#endif
