#include <stdio.h>
#include "produit.h"
#include "types.h"
#include <string.h>

void ajouterProduit(Boutique *b, Produit p) {
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, p.reference) == 0){  
        printf("Erreur : un produit avec la même référence existe déjà.\n");
            return;
        }
    }

    if (p.quantiteStock < 0) {
        printf("Erreur : la quantité en stock ne peut pas être négative.\n");
        return;
    }

    if (p.prixUnitaireHT < 0) {
        printf("Erreur : le prix unitaire ne peut pas être négatif.\n");
        return;
    }

    if (p.designation[0] == '\0') {
        printf("Erreur : la désignation ne peut pas être vide.\n");
        return;
    }
    if (p.categorie[0] == '\0') {
        printf("Erreur : la catégorie ne peut pas être vide.\n");
        return;
    }


    if (b->nbProduits >= b->capaciteProduits) {
        b->capaciteProduits *= 2;
        Produit *temp = realloc(b->produits, b->capaciteProduits * sizeof(Produit));
        if (temp == NULL) {
            printf("Erreur : impossible d'allouer de la mémoire pour les produits.\n");
            exit(1);
        }
        b->produits = temp; 
    }

    printf("Produit ajouté.\n");
}

void modifierProduit(Boutique *b, Produit p) {
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, p.reference) == 0) {
            b->produits[i] = p;
            printf("Produit modifié.\n");
            return;
        }
    }
    printf("Erreur : produit non trouvé.\n");
}


void supprimerProduit(Boutique *b, Produit p){
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, p.reference) == 0) {
            for (int j = i; j < b->nbProduits - 1; j++) {
                b->produits[j] = b->produits[j + 1];
            }
            b->nbProduits--;
            printf("Produit supprimé.\n");
            return;
        }
    }
    printf("Erreur : produit non trouvé.\n");

}

void afficherProduit(Produit p) {
    printf("Référence : %s\n", p.reference);
    printf("Désignation : %s\n", p.designation);
    printf("Catégorie : %s\n", p.categorie);
    printf("Prix unitaire HT : %.2f\n", p.prixUnitaireHT);
    printf("Quantité en stock : %d\n", p.quantiteStock);
    printf("Seuil d'alerte : %d\n", p.seuilAlerte);
    printf("Actif : %d\n", p.actif);
}

/*
void rechercherProduit(Boutique *b, Produit p) {
    int modeRecherche;
    printf("Choisissez le mode de recherche :\n");
    printf("1. Par référence\n");
    printf("2. Par mots clés\n");
    scanf("%d", &modeRecherche);

    if (modeRecherche == 1) {
        for (int i = 0; i < b->nbProduits; i++) {
            if (b->produits[i].reference == p.reference) {
                afficherProduit(b->produits[i]);
                return;
            }
        }
    } else if (modeRecherche == 2) {
        char motsCles[100];
        printf("Entrez les mots clés de recherche : ");
        scanf("%s", motsCles);

        for (int i = 0; i < b->nbProduits; i++) {
            return;
        }
    } else {
        printf("Erreur : mode de recherche invalide.\n");
        return;
    }
    
}
    */
