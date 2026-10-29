#include <stdio.h>
#include "type.h"
#include <string.h>


static void titreEtape(const char *titre)
{
    printf("\n========== %s ==========\n", titre);
}

static void attendreEntree(void)
{
    printf("\n[Entree pour continuer]");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void modeDemonstration(Boutique *b)
{
    titreEtape("1. Catalogue des produits");
    /* afficherProduits(b); */
    attendreEntree();

    titreEtape("2. Recherche : categorie Audio");
    rechercherProduits(b, "Audio", 0, "");
    attendreEntree();

    titreEtape("3. Recherche : produits a moins de 50 EUR");
    rechercherProduits(b, "", 50.0f, "");
    attendreEntree();

    titreEtape("4. Une commande avec remise et frais de port");
    /* afficherCommande(...); */
    attendreEntree();

    titreEtape("5. Palmares et alertes de stock");
    /* afficherPalmaresProduits(b); */
    /* afficherAlertesStock(b); */
    attendreEntree();
}
