/* produit.c - module catalogue produits (aucun main, aucune dependance a initialisation) */
#define _CRT_SECURE_NO_WARNINGS /* evite les avertissements scanf/strcpy sous MSVC (viens de claude) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "produit.h"

Produit saisirProduit(void) {
    Produit p;
    memset(&p, 0, sizeof(Produit)); //
    printf("Reference : ");
    scanf(" %11s", p.reference); //limiter la saisie a 11 caracteres pour reference sinon tab deborde.
    printf("Designation : ");
    scanf(" %59s", p.designation);
    printf("Categorie : ");
    scanf(" %29s", p.categorie);
    printf("Prix unitaire HT : ");
    scanf("%f", &p.prixUnitaireHT);
    printf("Quantite en stock : ");
    scanf("%d", &p.quantiteStock);
    printf("Seuil d'alerte : ");
    scanf("%d", &p.seuilAlerte);
    p.actif = 1;
    return p;
}

void ajouterProduit(Boutique *b, Produit p) {
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, p.reference) == 0) {
            printf("Erreur : un produit avec la meme reference existe deja.\n");
            return;
        }
    }

    if (p.reference[0] == '\0') {
        printf("Erreur : la reference ne peut pas etre vide.\n");
        return;
    }
    if (p.quantiteStock < 0) {
        printf("Erreur : la quantite en stock ne peut pas etre negative.\n");
        return;
    }
    if (p.prixUnitaireHT <= 0) {
        printf("Erreur : le prix unitaire doit etre strictement positif.\n");
        return;
    }
    if (p.designation[0] == '\0') {
        printf("Erreur : la designation ne peut pas etre vide.\n");
        return;
    }
    if (p.categorie[0] == '\0') {
        printf("Erreur : la categorie ne peut pas etre vide.\n");
        return;
    }

    if (b->nbProduits == b->capaciteProduits) {
        int nouvelleCapacite = (b->capaciteProduits == 0) ? 2 : b->capaciteProduits * 2;
        Produit *temp = (Produit *)realloc(b->produits, nouvelleCapacite * sizeof(Produit)); //realloc pour doubler la capacite
        if (temp == NULL) {
            printf("Erreur : impossible d'allouer de la memoire pour les produits.\n");
            return; 
        }
        b->produits = temp;
        b->capaciteProduits = nouvelleCapacite;
    }

    b->produits[b->nbProduits++] = p;
    printf("Produit ajoute.\n");
}

void modifierProduit(Boutique *b, Produit p) {
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, p.reference) == 0) {
            b->produits[i] = p;
            printf("Produit modifie.\n");
            return;
        }
    }
    printf("Erreur : produit non trouve.\n");
}

void supprimerProduit(Boutique *b, Produit p) {
    for (int i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, p.reference) == 0) {
            for (int j = i; j < b->nbProduits - 1; j++) {
                b->produits[j] = b->produits[j + 1];
            }
            b->nbProduits--;
            printf("Produit supprime.\n");
            return;
        }
    }
    printf("Erreur : produit non trouve.\n");
}

void afficherProduit(Produit p) {
    printf("Reference : %s\n", p.reference);
    printf("Designation : %s\n", p.designation);
    printf("Categorie : %s\n", p.categorie);
    printf("Prix unitaire HT : %.2f\n", p.prixUnitaireHT);
    printf("Quantite en stock : %d\n", p.quantiteStock);
    printf("Seuil d'alerte : %d\n", p.seuilAlerte);
    printf("Actif : %d\n", p.actif);
}


int main() {
    Boutique b;
    b.nbProduits = 0;
    b.capaciteProduits = 2;
    b.produits = (Produit *)malloc(b.capaciteProduits * sizeof(Produit));
    if (b.produits == NULL) return 1;

    Produit p1 = {"REF001", "Golden Delicious", "Pomme", 10.0f, 100, 10, 1};
    Produit p2 = {"REF002", "Royal Gala", "Pomme", 12.0f, 80, 5, 1};
    Produit p3 = {"REF003", "Williams", "Poire", 8.0f, 50, 5, 1};

    printf("--- Ajouts (3 produits) ---\n");
    ajouterProduit(&b, p1);
    ajouterProduit(&b, p2);
    ajouterProduit(&b, p3);
    printf("nbProduits = %d (attendu : 3), capacite = %d (attendu : 4)\n",
           b.nbProduits, b.capaciteProduits);

    printf("\n--- Doublon (doit etre refuse) ---\n");
    ajouterProduit(&b, p1);

    printf("\n--- Prix negatif (doit etre refuse) ---\n");
    Produit pBad = {"REF999", "Test", "Test", -5.0f, 10, 1, 1};
    ajouterProduit(&b, pBad);
    printf("nbProduits = %d (attendu : 3)\n", b.nbProduits);

    printf("\n--- Modification de REF001 ---\n");
    Produit pModif = {"REF001", "Granny Smith", "Pomme", 12.0f, 80, 5, 1};
    modifierProduit(&b, pModif);
    afficherProduit(b.produits[0]);

    printf("\n--- Modification d'une ref inexistante ---\n");
    Produit pInconnu = {"REF404", "X", "X", 1.0f, 1, 1, 1};
    modifierProduit(&b, pInconnu);

    printf("\n--- Suppression de REF002 ---\n");
    supprimerProduit(&b, p2);
    printf("nbProduits = %d (attendu : 2)\n", b.nbProduits);

    printf("\n--- Saisie manuelle ---\n");
    Produit pSaisi = saisirProduit();
    ajouterProduit(&b, pSaisi);
    afficherProduit(b.produits[b.nbProduits - 1]);

    free(b.produits);
    return 0;
}
