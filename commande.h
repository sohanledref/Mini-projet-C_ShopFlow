#ifndef COMMANDE_H
#define COMMANDE_H

#include "types.h"

void creerCommande(Boutique *b);
void ajouterLigneCommande(Boutique *b, int numero);
void supprimerLigneCommande(Boutique *b, int numero);
void changerStatutCommande(Boutique *b, int numero);
void annulerCommande(Boutique *b, int numero);
void afficherCommande(Boutique *b, int numero);
void calculerCommande(Commande *c);

#endif
