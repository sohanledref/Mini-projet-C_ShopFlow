
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "promotion.h"


// FONCTION 1 : Ajouter une promotion
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

    // Vérifier les doublons
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

    b->promotions[b->nbPromotions] = nouvellePromotion;
    b->nbPromotions++;

    return 1;
}


// FONCTION 2 : Afficher les promotions
void afficherPromotions(const Boutique *b) {

    if (b == NULL) {
        printf("Erreur : boutique inexistante.\n");
        return;
    }

    if (b->nbPromotions == 0) {
        printf("Aucune promotion disponible.\n");
        return;
    }

    printf("=== LISTE DES PROMOTIONS ===\n");

    int i = 0;

    while (i < b->nbPromotions) {

        printf("Code promo : %s\n", b->promotions[i].code);
        printf("Reduction : %.2f%%\n", b->promotions[i].tauxReduction);

        i++;
    }
}


// FONCTION 3 : Vérifier un code promotionnel
int verifierCodePromo(const Boutique *b, const char *code, Date dateActuelle, Promotion *promotionTrouvee) {

    if (b == NULL || code == NULL || promotionTrouvee == NULL) {
        return 0;
    }

    if (code[0] == '\0') {
        return 0;
    }

    int i = 0;

    while (i < b->nbPromotions) {

        if (strcmp(b->promotions[i].code, code) == 0) {

            // La vérification du statut et des dates viendra ici.

            *promotionTrouvee = b->promotions[i];
            return 1;
        }

        i++;
    }

    return 0;
}
