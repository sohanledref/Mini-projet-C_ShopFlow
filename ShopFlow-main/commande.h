#ifndef COMMANDE_H
#define COMMANDE_H

#include "types.h"

void initialiserLignesCommande(Commande *commande);
int ajouterLigneCommande(Commande *commande, LigneCommande ligne);
void libererLignesCommande(Commande *commande);

#endif

