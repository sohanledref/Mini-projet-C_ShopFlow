#ifndef PRODUIT_H
#define PRODUIT_H

#include "types.h"

void ajouterProduit(Boutique *b);

void modifierProduit(Boutique *b, const char *reference);

void supprimerProduit(Boutique *b, const char *reference);

void afficherListeProduits(Boutique *b);

void rechercherProduit(Boutique *b, const char *motCle);

#endif
    