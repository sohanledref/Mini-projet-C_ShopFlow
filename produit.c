
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "produit.h"
#include "type."


/*Toutes les fonctions moins annodines ont été expliquée dans le fichier client.c, par exemple, strcspn, strchr, getgs, getchar etc...*/


static void lireTexte(char *texte, int taille, const char *message){ 
    int c;

    printf("%s", message);

    if (fgets(texte, taille, stdin) != NULL){
        if (strchr(texte, '\n') != NULL){
            texte[strcspn(texte, "\n")] = '\0';
        } else {
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
        }
    } else {
        texte[0] = '\0';
    }
}


static int referenceExiste(Boutique *b, const char *reference) {
    int i;

    for (i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, reference) == 0) {
            return 1;
        }
    }

    return 0;
}


static int lireEntierPositifOuZero(const char *message){
    char texte[50];
    char *fin;
    long valeur;

    do {
        lireTexte(texte, sizeof(texte), message);

        valeur = strtol(texte, &fin, 10);

        if (texte[0] != '\0' && *fin == '\0' && valeur >= 0 && valeur <= 2147483647L) {
            return (int) valeur;
        }

        printf("Veuillez saisir un entier positif ou nul.\n");

    } while (1);
}


static float lirePrix(const char *message)
{
    char texte[50];
    char *fin;
    float prix;

    do{
        lireTexte(texte, sizeof(texte), message);

        prix = strtof(texte, &fin);

        if (texte[0] != '\0' && *fin == '\0' && isfinite(prix) && prix > 0){
            return prix;
        }

        printf("Veuillez saisir un prix strictement positif.\n");

    } while (1);
}


static int produitUtiliseDansCommande(Boutique *b, const char *reference)
{
    int i, j, k;

    for (i = 0; i < b->nbClients; i++){
        for (j = 0; j < b->clients[i].nbCommandes; j++){
            Commande *c = &b->clients[i].commandes[j];

            if (strcmp(c->statut, "ANNULEE") != 0){
                for (k = 0; k < c->nbLignes; k++){
                    if (strcmp(c->lignes[k].refProduit, reference) == 0){
                        return 1;
                    }
                }
            }
        }
    }

    return 0;
}


/* FONCTION 1 : AJOUTER UN PRODUIT */


void ajouterProduit(Boutique *b)
{
    Produit nouveau;
    Produit *temp;
    int capacite;
    
    if (b == NULL){
        return;
    }

    lireTexte(nouveau.reference, sizeof(nouveau.reference), "Reference : ");

    if (nouveau.reference[0] == '\0') {
        printf("La reference ne peut pas etre vide.\n");
        return;
    }

    if (referenceExiste(b, nouveau.reference)) {
        printf("Cette reference existe deja.\n");
        return;
    }

    lireTexte(nouveau.designation, sizeof(nouveau.designation), "Designation : ");

    lireTexte(nouveau.categorie, sizeof(nouveau.categorie), "Categorie : ");

    if (nouveau.designation[0] == '\0' || nouveau.categorie[0] == '\0'){
        printf("La designation et la categorie sont obligatoires.\n");
        return;
    }

    nouveau.prixUnitaireHT = lirePrix("Prix unitaire HT : ");

    nouveau.quantiteStock = lireEntierPositifOuZero("Quantite en stock : ");

    nouveau.seuilAlerte = lireEntierPositifOuZero("Seuil d'alerte : ");

    if (b->nbProduits >= b->capaciteProduits){
        capacite = (b->capaciteProduits == 0) ? 2 : b->capaciteProduits * 2;

        temp = realloc(b->produits, capacite * sizeof(Produit));

        if (temp == NULL){
            printf("Erreur d'allocation memoire.\n");
            return;
        }

        b->produits = temp;
        b->capaciteProduits = capacite;
    }

    nouveau.actif = 1;

    b->produits[b->nbProduits] = nouveau;
    b->nbProduits++;

    printf("Produit ajoute avec succes.\n");
}


/* FONCTION 2 : MODIFIER UN PRODUIT */


void modifierProduit(Boutique *b, const char *reference) {
    int i;

    if (b == NULL || reference == NULL) {
        return;
    }

    for (i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, reference) == 0 && b->produits[i].actif == 1) {
            printf("\nModification du produit : %s\n", reference);

            b->produits[i].prixUnitaireHT = lirePrix("Nouveau prix HT : ");

            b->produits[i].quantiteStock = lireEntierPositifOuZero("Nouveau stock : ");

            b->produits[i].seuilAlerte = lireEntierPositifOuZero("Nouveau seuil d'alerte : ");

            printf("Produit modifie avec succes.\n");

            return;
        }
    }

    printf("Produit introuvable ou inactif.\n");
}


/* FONCTION 3 : SUPPRIMER UN PRODUIT */


void supprimerProduit(Boutique *b, const char *reference){
    int i;

    if (b == NULL || reference == NULL) {
        return;
    }

    for (i = 0; i < b->nbProduits; i++) {
        if (strcmp(b->produits[i].reference, reference) == 0 && b->produits[i].actif == 1) {
            if (produitUtiliseDansCommande(b, reference)){
                printf("Suppression refusee : produit utilise dans une commande non annulee.\n");
                return;
            }

            b->produits[i].actif = 0;

            printf("Produit supprime avec succes.\n");

            return;
        }
    }

    printf("Produit introuvable ou deja inactif.\n");
}


/* FONCTION 4 : AFFICHER LA LISTE DES PRODUITS */


void afficherListeProduits(Boutique *b){
    int i;
    float valorisation;

    if (b == NULL) {
        return;
    }

    printf("\n========== CATALOGUE DES PRODUITS ==========\n");

    for (i = 0; i < b->nbProduits; i++) {
        if (b->produits[i].actif == 1){
            valorisation = b->produits[i].prixUnitaireHT * b->produits[i].quantiteStock;
            printf("\nReference : %s\n", b->produits[i].reference);
            printf("Designation : %s\n", b->produits[i].designation);
            printf("Categorie : %s\n", b->produits[i].categorie);           
            printf("Prix unitaire HT : %.2f euros\n", b->produits[i].prixUnitaireHT);       
            printf("Quantite en stock : %d\n", b->produits[i].quantiteStock);
            printf("Seuil d'alerte : %d\n", b->produits[i].seuilAlerte);           
            printf("Valorisation du stock : %.2f euros HT\n", valorisation);

            if (b->produits[i].quantiteStock <= b->produits[i].seuilAlerte) {
                printf("ALERTE : stock faible !\n");
            }
        }
    }

    printf("\n============================================\n");
}


/* FONCTION 5 : RECHERCHER UN PRODUIT */


void rechercherProduit(Boutique *b, const char *motCle) {
    int i;
    int trouve = 0;

    if (b == NULL || motCle == NULL || motCle[0] == '\0') {
        return;
    }

    for (i = 0; i < b->nbProduits; i++) {
        if (b->produits[i].actif == 1) {
            if (strcmp(b->produits[i].reference, motCle) == 0 || strstr(b->produits[i].designation, motCle) != NULL) {*
                printf("\nReference : %s\n", b->produits[i].reference);
                printf("Designation : %s\n", b->produits[i].designation);
                printf("Categorie : %s\n", b->produits[i].categorie);
                printf("Prix HT : %.2f euros\n", b->produits[i].prixUnitaireHT);
                printf("Stock : %d\n", b->produits[i].quantiteStock);

                trouve = 1;
            }
        }
    }

    if (trouve == 0) {
        printf("Aucun produit trouve.\n");
    }
}
