#include <stdlib.h>
#include "commande.h"

void initialiserLignesCommande(Commande *commande) {
    if (commande == NULL) return;
    commande->nbLignes = 0;
    commande->capaciteLignes = 0;
    commande->lignes = NULL;
}

int ajouterLigneCommande(Commande *commande, LigneCommande ligne) {
    if (commande == NULL) return 0;

    if (commande->nbLignes == commande->capaciteLignes) {
        int nouvelleCapacite = (commande->capaciteLignes == 0) ? 2 : commande->capaciteLignes * 2;
        LigneCommande *nouvellesLignes = realloc(commande->lignes, nouvelleCapacite * sizeof(LigneCommande));
        if (nouvellesLignes == NULL) return 0;
        commande->lignes = nouvellesLignes;
        commande->capaciteLignes = nouvelleCapacite;
    }

    commande->lignes[commande->nbLignes] = ligne;
    commande->nbLignes++;
    return 1;
}

void libererLignesCommande(Commande *commande) {
    if (commande == NULL) return;
    free(commande->lignes);
    commande->lignes = NULL;
    commande->nbLignes = 0;
    commande->capaciteLignes = 0;
}

