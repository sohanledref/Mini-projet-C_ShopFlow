#ifndef STATISTIQUE_H
#define STATISTIQUE_H

#include "types.h"

void afficherChiffreAffaires(Boutique *b);

void afficherChiffreAffairesPeriode(Boutique *b, int mois, int annee);

void afficherTop5Produits(Boutique *b);

void afficherClassementClients(Boutique *b);

#endif
