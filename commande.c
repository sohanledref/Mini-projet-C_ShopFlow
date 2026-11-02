#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commande.h"
#include "types.h"

static Client *chercherClient(Boutique *b, int idClient) {
    for (int i = 0; i < b->nbClients; i++) {
        if (b->clients[i].idClient == idClient && b->clients[i].actif == 1) {
            return &b->clients[i];
        }
    }

    return NULL;
}

static Produit *chercherProduit(Boutique *b, const char *reference) {
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, reference) == 0 && b->produits[i].actif == 1) {
            return &b->produits[i];
        }
    }

    return NULL;
}

static Commande *chercherCommande(Boutique *b, int numero) {
    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            if (b->clients[i].commandes[j].numero == numero) {
                return &b->clients[i].commandes[j];
            }
        }
    }

    return NULL;
}

static int ligneExiste(Commande *c, const char *reference) {
    for (int i = 0; i < c->nbLignes; i++) {
        if (strcmp(c->lignes[i].refProduit, reference) == 0) {
            return 1;
        }
    }

    return 0;
}

void calculerCommande(Commande *c) {
    c->montantBrutHT = 0.0f;

    for (int i = 0; i < c->nbLignes; i++) {
        c->lignes[i].sousTotalHT = c->lignes[i].quantite * c->lignes[i].prixUnitaireHT;
        c->montantBrutHT += c->lignes[i].sousTotalHT;
    }

    if (c->montantBrutHT >= 600) {
        c->tauxRemise = 15;
    } else if (c->montantBrutHT >= 300) {
        c->tauxRemise = 10;
    } else if (c->montantBrutHT >= 100) {
        c->tauxRemise = 5;
    } else {
        c->tauxRemise = 0;
    }

    c->montantRemise = c->montantBrutHT * c->tauxRemise / 100;
    c->montantHT = c->montantBrutHT - c->montantRemise;
    c->montantTVA = c->montantHT * 0.20f;

    if (c->montantBrutHT >= 60) {
        c->fraisPort = 0;
    } else {
        c->fraisPort = 5.90f;
    }

    c->montantTTC = c->montantHT + c->montantTVA + c->fraisPort;
}

void creerCommande(Boutique *b) {
    int idClient;
    int nombreLignes;
    char reference[12];
    int quantite;

    Client *client;
    Produit *produit;
    Commande nouvelleCommande;

    if (b == NULL) {
        return;
    }

    printf("\n--- CREATION D'UNE COMMANDE ---\n");
    printf("Identifiant du client : ");

    if (scanf("%d", &idClient) != 1) {
        printf("Identifiant invalide.\n");
        return;
    }

    client = chercherClient(b, idClient);

    if (client == NULL) {
        printf("Client introuvable ou inactif.\n");
        return;
    }

    printf("Nombre de lignes a ajouter : ");

    if (scanf("%d", &nombreLignes) != 1 || nombreLignes <= 0) {
        printf("Nombre de lignes invalide.\n");
        return;
    }

    memset(&nouvelleCommande, 0, sizeof(Commande));

    nouvelleCommande.numero = b->numeroSuivant;
    nouvelleCommande.idClient = client->idClient;

    strcpy(nouvelleCommande.statut, "EN_COURS");

    nouvelleCommande.capaciteLignes = nombreLignes;
    nouvelleCommande.lignes = malloc(nombreLignes * sizeof(LigneCommande));

    if (nouvelleCommande.lignes == NULL) {
        printf("Erreur d'allocation memoire.\n");
        return;
    }

    nouvelleCommande.nbLignes = 0;
    nouvelleCommande.adresseLivraison = client->adresse;

    for (int i = 0; i < nombreLignes; i++) {
        printf("\nReference du produit : ");

        if (scanf("%11s", reference) != 1) {
            printf("Reference invalide.\n");
            free(nouvelleCommande.lignes);
            return;
        }

        produit = chercherProduit(b, reference);

        if (produit == NULL) {
            printf("Produit introuvable ou inactif.\n");
            free(nouvelleCommande.lignes);
            return;
        }

        if (ligneExiste(&nouvelleCommande, reference)) {
            printf("Ce produit existe deja dans la commande.\n");
            free(nouvelleCommande.lignes);
            return;
        }

        printf("Quantite : ");

        if (scanf("%d", &quantite) != 1 || quantite <= 0 || quantite > produit->quantiteStock) {
            printf("Quantite invalide ou stock insuffisant.\n");
            free(nouvelleCommande.lignes);
            return;
        }

        LigneCommande *ligne = &nouvelleCommande.lignes[nouvelleCommande.nbLignes];

        strcpy(ligne->refProduit, produit->reference);
        strcpy(ligne->designation, produit->designation);

        ligne->quantite = quantite;
        ligne->prixUnitaireHT = produit->prixUnitaireHT;
        ligne->sousTotalHT = quantite * produit->prixUnitaireHT;

        nouvelleCommande.nbLignes++;
        produit->quantiteStock -= quantite;
    }

    calculerCommande(&nouvelleCommande);

    if (client->nbCommandes >= client->capaciteCommandes) {
        int nouvelleCapacite;

        if (client->capaciteCommandes == 0) {
            nouvelleCapacite = 2;
        } else {
            nouvelleCapacite = client->capaciteCommandes * 2;
        }

        Commande *temp = realloc(client->commandes, nouvelleCapacite * sizeof(Commande));

        if (temp == NULL) {
            for (int i = 0; i < nouvelleCommande.nbLignes; i++) {
                Produit *p = chercherProduit(b, nouvelleCommande.lignes[i].refProduit);

                if (p != NULL) {
                    p->quantiteStock += nouvelleCommande.lignes[i].quantite;
                }
            }

            free(nouvelleCommande.lignes);
            printf("Erreur d'allocation memoire.\n");
            return;
        }

        client->commandes = temp;
        client->capaciteCommandes = nouvelleCapacite;
    }

    client->commandes[client->nbCommandes] = nouvelleCommande;
    client->nbCommandes++;

    client->pointsFidelite += (int)(nouvelleCommande.montantTTC / 10);

    b->numeroSuivant++;

    printf("\nCommande creee avec succes !\n");
    printf("Numero de commande : %d\n", nouvelleCommande.numero);
    printf("Montant TTC : %.2f euros\n", nouvelleCommande.montantTTC);
}

void ajouterLigneCommande(Boutique *b, int numero) {
    Commande *c;
    Produit *p;
    char reference[12];
    int quantite;

    c = chercherCommande(b, numero);

    if (c == NULL) {
        printf("Commande introuvable.\n");
        return;
    }

    if (strcmp(c->statut, "EN_COURS") != 0) {
        printf("Modification impossible : commande non EN_COURS.\n");
        return;
    }

    printf("Reference du produit : ");

    if (scanf("%11s", reference) != 1) {
        printf("Reference invalide.\n");
        return;
    }

    p = chercherProduit(b, reference);

    if (p == NULL) {
        printf("Produit introuvable.\n");
        return;
    }

    if (ligneExiste(c, reference)) {
        printf("Ce produit est deja present dans la commande.\n");
        return;
    }

    printf("Quantite : ");

    if (scanf("%d", &quantite) != 1 || quantite <= 0 || quantite > p->quantiteStock) {
        printf("Quantite invalide ou stock insuffisant.\n");
        return;
    }

    if (c->nbLignes >= c->capaciteLignes) {
        int nouvelleCapacite = (c->capaciteLignes == 0) ? 2 : c->capaciteLignes * 2;

        LigneCommande *temp = realloc(c->lignes, nouvelleCapacite * sizeof(LigneCommande));

        if (temp == NULL) {
            printf("Erreur d'allocation memoire.\n");
            return;
        }

        c->lignes = temp;
        c->capaciteLignes = nouvelleCapacite;
    }

    LigneCommande *ligne = &c->lignes[c->nbLignes];

    strcpy(ligne->refProduit, p->reference);
    strcpy(ligne->designation, p->designation);

    ligne->quantite = quantite;
    ligne->prixUnitaireHT = p->prixUnitaireHT;
    ligne->sousTotalHT = quantite * p->prixUnitaireHT;

    c->nbLignes++;

    p->quantiteStock -= quantite;

    calculerCommande(c);

    printf("Ligne ajoutee avec succes.\n");
}

void supprimerLigneCommande(Boutique *b, int numero) {
    Commande *c;
    Produit *p;
    char reference[12];
    int position = -1;

    c = chercherCommande(b, numero);

    if (c == NULL) {
        printf("Commande introuvable.\n");
        return;
    }

    if (strcmp(c->statut, "EN_COURS") != 0) {
        printf("Suppression impossible : commande non EN_COURS.\n");
        return;
    }

    printf("Reference du produit a supprimer : ");

    if (scanf("%11s", reference) != 1) {
        printf("Reference invalide.\n");
        return;
    }

    for (int i = 0; i < c->nbLignes; i++) {
        if (strcmp(c->lignes[i].refProduit, reference) == 0) {
            position = i;
            break;
        }
    }

    if (position == -1) {
        printf("Ligne introuvable.\n");
        return;
    }

    p = chercherProduit(b, reference);

    if (p != NULL) {
        p->quantiteStock += c->lignes[position].quantite;
    }

    for (int i = position; i < c->nbLignes - 1; i++) {
        c->lignes[i] = c->lignes[i + 1];
    }

    c->nbLignes--;

    calculerCommande(c);

    printf("Ligne supprimee avec succes.\n");
}

void changerStatutCommande(Boutique *b, int numero) {
    Commande *c = chercherCommande(b, numero);

    if (c == NULL) {
        printf("Commande introuvable.\n");
        return;
    }

    if (strcmp(c->statut, "EN_COURS") == 0) {
        strcpy(c->statut, "EXPEDIEE");
    } else if (strcmp(c->statut, "EXPEDIEE") == 0) {
        strcpy(c->statut, "LIVREE");
    } else {
        printf("Changement de statut impossible.\n");
        return;
    }

    printf("Nouveau statut : %s\n", c->statut);
}

void annulerCommande(Boutique *b, int numero) {
    Commande *c = chercherCommande(b, numero);
    Client *client;

    if (c == NULL) {
        printf("Commande introuvable.\n");
        return;
    }

    if (strcmp(c->statut, "EN_COURS") != 0) {
        printf("Seules les commandes EN_COURS peuvent etre annulees.\n");
        return;
    }

    client = chercherClient(b, c->idClient);

    for (int i = 0; i < c->nbLignes; i++) {
        Produit *p = chercherProduit(b, c->lignes[i].refProduit);

        if (p != NULL) {
            p->quantiteStock += c->lignes[i].quantite;
        }
    }

    if (client != NULL) {
        int points = (int)(c->montantTTC / 10);

        client->pointsFidelite -= points;

        if (client->pointsFidelite < 0) {
            client->pointsFidelite = 0;
        }
    }

    strcpy(c->statut, "ANNULEE");

    printf("Commande annulee avec succes.\n");
}

void afficherCommande(Boutique *b, int numero) {
    Commande *c = chercherCommande(b, numero);

    if (c == NULL) {
        printf("Commande introuvable.\n");
        return;
    }

    printf("\n========== COMMANDE %d ==========\n", c->numero);
    printf("Client : %d\n", c->idClient);
    printf("Statut : %s\n", c->statut);

    printf("\n--- LIGNES DE COMMANDE ---\n");

    for (int i = 0; i < c->nbLignes; i++) {
        printf("\nProduit : %s\n", c->lignes[i].designation);
        printf("Reference : %s\n", c->lignes[i].refProduit);
        printf("Quantite : %d\n", c->lignes[i].quantite);
        printf("Prix unitaire HT : %.2f euros\n", c->lignes[i].prixUnitaireHT);
        printf("Sous-total HT : %.2f euros\n", c->lignes[i].sousTotalHT);
    }

    printf("\n--- FACTURATION ---\n");
    printf("Montant brut HT : %.2f euros\n", c->montantBrutHT);
    printf("Remise : %.0f %%\n", c->tauxRemise);
    printf("Montant de la remise : %.2f euros\n", c->montantRemise);
    printf("Montant HT apres remise : %.2f euros\n", c->montantHT);
    printf("TVA : %.2f euros\n", c->montantTVA);
    printf("Frais de port : %.2f euros\n", c->fraisPort);
    printf("TOTAL TTC : %.2f euros\n", c->montantTTC);
    printf("=================================\n");
}
