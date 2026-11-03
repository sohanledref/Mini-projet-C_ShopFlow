
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "promotion.h"

int ajouterPromotion(Boutique *b, Promotion nouvellePromotion) {

    if (b == NULL) {
        return 0;
    }

    if (nouvellePromotion.code[0] == '\0') {
        return 0;
    }

    if (nouvellePromotion.tauxReduction <= 0 ||
        nouvellePromotion.tauxReduction > 100) {
        return 0;
    }

    // Vérifier si le code existe déjà
    for (int i = 0; i < b->nbPromotions; i++) {
        if (strcmp(b->promotions[i].code, nouvellePromotion.code) == 0) {
            return 0;
        }
    }

    // Agrandir le tableau si nécessaire
    if (b->nbPromotions >= b->capacitePromotions) {

        int nouvelleCapacite;

        if (b->capacitePromotions == 0) {
            nouvelleCapacite = 2;
        } else {
            nouvelleCapacite = b->capacitePromotions * 2;
        }

        Promotion *temp = realloc(
            b->promotions,
            nouvelleCapacite * sizeof(Promotion)
        );

        if (temp == NULL) {
            return 0;
        }

        b->promotions = temp;
        b->capacitePromotions = nouvelleCapacite;
    }

    // Ajouter la promotion
    b->promotions[b->nbPromotions] = nouvellePromotion;
    b->nbPromotions++;

    return 1;
}
