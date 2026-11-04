#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "initialisation.h"
#include "commande.h"   
#include "types.h"



static Client *trouverClientInit(Boutique *b, int idClient)
{
    for (int i = 0; i < b->nbClients; i++) {
        if (b->clients[i].idClient == idClient) {
            return &b->clients[i];
        }
    }
    return NULL;
}

static Produit *trouverProduitInit(Boutique *b, const char *reference)
{
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, reference) == 0) {
            return &b->produits[i];
        }
    }
    return NULL;
}


static int ajouterClientInit(Boutique *b, const char *nom, const char *prenom,
                             const char *email, const char *telephone,
                             const char *rue, const char *ville,
                             const char *codePostal, const char *pays)
{
    Client *c;

    if (b->nbClients >= b->capaciteClients) {
        return 0;
    }

    c = &b->clients[b->nbClients];
    memset(c, 0, sizeof(Client));

    c->idClient = b->idClientSuivant++;
    strcpy(c->nom, nom);
    strcpy(c->prenom, prenom);
    strcpy(c->email, email);
    strcpy(c->telephone, telephone);
    strcpy(c->adresse.rue, rue);
    strcpy(c->adresse.ville, ville);
    strcpy(c->adresse.codePostal, codePostal);
    strcpy(c->adresse.pays, pays);

    c->pointsFidelite = 0;
    c->nbCommandes = 0;
    c->capaciteCommandes = 0;
    c->commandes = NULL;
    c->actif = 1;

    b->nbClients++;
    return 1;
}


static int ajouterCommandeInit(Boutique *b, int idClient,
                               int jour, int mois, int annee,
                               const char *statut,
                               const char *const refs[], const int quantites[],
                               int nbLignes)
{
    Client *client = trouverClientInit(b, idClient);
    Commande c;

    if (client == NULL) {
        return 0;
    }

    memset(&c, 0, sizeof(Commande));

    c.numero = b->numeroSuivant++;
    c.idClient = idClient;
    c.dateCommande = (Date){jour, mois, annee};
    strcpy(c.statut, statut);
    c.adresseLivraison = client->adresse;

    c.capaciteLignes = nbLignes;
    c.lignes = (LigneCommande *)malloc(c.capaciteLignes * sizeof(LigneCommande));
    if (c.lignes == NULL) {
        printf("Erreur d'allocation memoire (lignes de commande).\n");
        return 0;
    }
    c.nbLignes = 0;

    for (int i = 0; i < nbLignes; i++) {
        Produit *p = trouverProduitInit(b, refs[i]);

        if (p == NULL) {
            free(c.lignes);
            return 0;
        }

        LigneCommande *l = &c.lignes[c.nbLignes];
        strcpy(l->refProduit, p->reference);
        strcpy(l->designation, p->designation);
        l->quantite = quantites[i];
        l->prixUnitaireHT = p->prixUnitaireHT;
        l->sousTotalHT = l->quantite * l->prixUnitaireHT;
        c.nbLignes++;
    }

    calculerCommande(&c);

    if (client->nbCommandes >= client->capaciteCommandes) {
        int nouvelleCapacite = (client->capaciteCommandes == 0) ? 2 : client->capaciteCommandes * 2;
        Commande *temp = (Commande *)realloc(client->commandes, nouvelleCapacite * sizeof(Commande));

        if (temp == NULL) {
            printf("Erreur d'allocation memoire (commandes du client).\n");
            free(c.lignes);
            return 0;
        }
        client->commandes = temp;
        client->capaciteCommandes = nouvelleCapacite;
    }

    client->commandes[client->nbCommandes] = c;
    client->nbCommandes++;

    if (strcmp(statut, "ANNULEE") != 0) {
        client->pointsFidelite += (int)(c.montantTTC / 10);
    }

    return 1;
}


void initialiserDonneesTest(Boutique *b)
{
    if (b == NULL) {
        return;
    }

    strcpy(b->nom, "ShopFlow");
    b->produits = NULL;
    b->nbProduits = 0;
    b->capaciteProduits = 0;
    b->clients = NULL;
    b->nbClients = 0;
    b->capaciteClients = 0;
    b->numeroSuivant = 1001;
    b->idClientSuivant = 1;

    b->capaciteProduits = 10;
    b->produits = (Produit *)malloc(b->capaciteProduits * sizeof(Produit));
    if (b->produits == NULL) {
        printf("Erreur d'allocation memoire (produits).\n");
        b->capaciteProduits = 0;
        return;
    }

    Produit modele[] = {
        {"PRD-0001", "Clavier mecanique",   "Informatique", 79.90f,  42, 10, 1},
        {"PRD-0002", "Souris sans fil",     "Informatique", 24.50f,   8, 15, 1}, /* ALERTE */
        {"PRD-0003", "Ecran 27 pouces",     "Informatique", 199.00f,  6,  5, 1},
        {"PRD-0004", "Casque audio",        "Audio",        89.90f,  25, 10, 1},
        {"PRD-0005", "Enceinte Bluetooth",  "Audio",        59.90f,  12,  8, 1},
        {"PRD-0006", "Cable HDMI 2 m",      "Accessoires",  9.90f,  120, 30, 1},
        {"PRD-0007", "Hub USB-C 7 ports",   "Accessoires",  49.90f,   4, 10, 1}, /* ALERTE */
        {"PRD-0008", "Disque SSD 1 To",     "Stockage",     109.00f, 18,  6, 1}
    };

    for (int i = 0; i < 8; i++) {
        b->produits[b->nbProduits++] = modele[i];
    }

    b->capaciteClients = 8;
    b->clients = (Client *)malloc(b->capaciteClients * sizeof(Client));
    if (b->clients == NULL) {
        printf("Erreur d'allocation memoire (clients).\n");
        b->capaciteClients = 0;
        return;
    }

    ajouterClientInit(b, "Bernard", "Julie",   "j.bernard@mail.fr",  "0612345678",
                      "12 rue de la Paix",       "Paris",     "75002", "France");
    ajouterClientInit(b, "Martin",  "Lucas",   "l.martin@mail.fr",   "0623456789",
                      "8 avenue Victor Hugo",    "Lyon",      "69003", "France");
    ajouterClientInit(b, "Durand",  "Emma",    "e.durand@mail.fr",   "0634567890",
                      "25 boulevard Gambetta",   "Marseille", "13001", "France");
    ajouterClientInit(b, "Petit",   "Hugo",    "h.petit@mail.fr",    "0645678901",
                      "3 place du Capitole",     "Toulouse",  "31000", "France");
    ajouterClientInit(b, "Leroy",   "Camille", "c.leroy@mail.fr",    "0656789012",
                      "17 quai des Chartrons",   "Bordeaux",  "33000", "France");

    ajouterCommandeInit(b, 1, 12, 3, 2026, "LIVREE",
                        (const char *const[]){"PRD-0001", "PRD-0004", "PRD-0006"},
                        (const int[]){2, 1, 3}, 3);

    ajouterCommandeInit(b, 2, 5, 4, 2026, "LIVREE",
                        (const char *const[]){"PRD-0003", "PRD-0008", "PRD-0005"},
                        (const int[]){2, 2, 1}, 3);

    ajouterCommandeInit(b, 3, 20, 6, 2026, "EXPEDIEE",
                        (const char *const[]){"PRD-0002", "PRD-0006"},
                        (const int[]){2, 1}, 2);

    ajouterCommandeInit(b, 1, 15, 9, 2026, "EN_COURS",
                        (const char *const[]){"PRD-0005", "PRD-0007"},
                        (const int[]){2, 1}, 2);

    ajouterCommandeInit(b, 4, 2, 10, 2026, "ANNULEE",
                        (const char *const[]){"PRD-0004", "PRD-0006"},
                        (const int[]){1, 2}, 2);

    ajouterCommandeInit(b, 5, 28, 10, 2026, "EN_COURS",
                        (const char *const[]){"PRD-0008", "PRD-0001", "PRD-0002"},
                        (const int[]){1, 1, 1}, 3);

}


void detruireBoutique(Boutique *b)
{
    if (b == NULL) {
        return;
    }

    for (int i = 0; i < b->nbClients; i++) {
        for (int j = 0; j < b->clients[i].nbCommandes; j++) {
            free(b->clients[i].commandes[j].lignes);
        }
        free(b->clients[i].commandes);
    }

    free(b->clients);
    free(b->produits);

    b->clients = NULL;
    b->produits = NULL;
    b->nbClients = 0;
    b->capaciteClients = 0;
    b->nbProduits = 0;
    b->capaciteProduits = 0;
}