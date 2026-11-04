#ifndef PROMOTION_H
#define PROMOTION_H

#include "types.h"

/* Ajouter une nouvelle promotion */
int ajouterPromotion(Boutique *b, Promotion nouvellePromotion);

/* Afficher toutes les promotions */
void afficherPromotions(const Boutique *b);

/* Vérifier si un code promotionnel est valide */
int verifierCodePromo(
    const Boutique *b,
    const char *code,
    Date dateActuelle,
    Promotion *promotionTrouvee
);

/* Appliquer une promotion à une commande */
int appliquerCodePromo(Commande *c, const Promotion *p);

#endif
