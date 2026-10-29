#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "client.h"

static void lireTexte(char *texte, int taille, const char *message){
    int c;

    printf("%s", message);

    if (fgets(texte, taille, stdin) != NULL){ /*fgestet stdin permettent de lire les inuputs, mieux que scanf car l'utilisateur peut mettre des espaces*/
        if (strchr(texte, '\n') != NULL){
            texte[strcspn(texte, "\n")] = '\0'; /*remplace le saut de ligne de fgets par le signe de fin de chaine*/
        } else {
            /*Enleve les caractères en trop pour pas impacté les prochaines saisies*/
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
        }
    } else {
        texte[0] = '\0';
    }
}

static int emailExiste(Boutique *b, const char *email, int indiceIgnore){
    int i;

    for (i = 0; i < b->nbClients; i++){
        if (i != indiceIgnore && strcmp(b->clients[i].email, email) == 0){
            return 1;
        }
    }

    return 0;
}

static int telephoneValide(const char *telephone){
    int i;

    if (strlen(telephone) != 10){
        return 0;
    }
    
    for (i = 0; telephone[i] != '\0'; i++) {  /*telephone[i] != '/0' est mieux que i < 10 niveau optimisation*/
        if (!isdigit((unsigned char)telephone[i])){ /*isdigi permet de savoir si l'éléments est un chiffre*/
            return 0;
        }
    }

    return 1;
}

static int codePostalValide(const char *codePostal){
    int i;

    if (strlen(codePostal) != 5){
        return 0;
    }
    for (i = 0; codePostal[i] != '\0'; i++){
        if (!isdigit((unsigned char)codePostal[i])){
            return 0;
        }
    }

    return 1;
}

static void saisirAdresse(Adresse *adresse)
{
    lireTexte(adresse->rue, sizeof(adresse->rue), "Rue : ");
    lireTexte(adresse->ville, sizeof(adresse->ville), "Ville : ");

    do
    {
        lireTexte(adresse->codePostal, sizeof(adresse->codePostal), "Code postal (5 chiffres) : ");

        if (!codePostalValide(adresse->codePostal)){
            printf("Code postal invalide.\n");
        }
    } while (!codePostalValide(adresse->codePostal));

    lireTexte(adresse->pays, sizeof(adresse->pays), "Pays : ");
}

void ajouterClient(Boutique *b){
    Client nouveau;
    Client *temp;
    int capacite;
    char email[80];

    if (b == NULL){
        return;
    }

    lireTexte(nouveau.nom, sizeof(nouveau.nom), "Nom : ");
    lireTexte(nouveau.prenom, sizeof(nouveau.prenom), "Prenom : ");

    do {
        lireTexte(email, sizeof(email), "Email : ");

        if (strchr(email, '@') == NULL){/*Vérifie qu'il y a bien l'@*/
            printf("Adresse email invalide.\n");
        } else if (emailExiste(b, email, -1)) {
            printf("Cet email existe deja.\n");
        } else {
            break;
        }

    } while (1); /*on sort avec le break*/

    strcpy(nouveau.email, email);

    do {
        lireTexte(nouveau.telephone, sizeof(nouveau.telephone), "Telephone (10 chiffres) : ");

        if (!telephoneValide(nouveau.telephone)) {
            printf("Telephone invalide.\n");
        };
    } while (!telephoneValide(nouveau.telephone));

    saisirAdresse(&nouveau.adresse);

    if (b->nbClients >= b->capaciteClients){
        capacite = (b->capaciteClients == 0) ? 2 : b->capaciteClients * 2; /*si c'est vrai -> 2 sinon b-> capaciteClients *2 */

        temp = realloc(b->clients, capacite * sizeof(Client));

        if (temp == NULL){
            printf("Erreur d'allocation memoire.\n");
            return;
        }

        b->clients = temp;
        b->capaciteClients = capacite;
    }

    nouveau.idClient = b->idClientSuivant++; /*On incremente après l'utitilisation*/
    nouveau.pointsFidelite = 0;
    nouveau.nbCommandes = 0;
    nouveau.capaciteCommandes = 0;
    nouveau.commandes = NULL;
    nouveau.actif = 1;

    b->clients[b->nbClients] = nouveau;
    b->nbClients++;

    printf("Client ajoute avec succes. ID : %d\n", nouveau.idClient);
}

void modifierClient(Boutique *b, int idClient)
{
    int i;
    char email[80];

    if (b == NULL){
        return;
    }

    for (i = 0; i < b->nbClients; i++){
        if (b->clients[i].idClient == idClient && b->clients[i].actif == 1){
            lireTexte(b->clients[i].nom, sizeof(b->clients[i].nom), "Nouveau nom : ");

            lireTexte(b->clients[i].prenom, sizeof(b->clients[i].prenom), "Nouveau prenom : ");

            do {
                lireTexte(email, sizeof(email), "Nouvel email : ");

                if (strchr(email, '@') == NULL){
                    printf("Adresse email invalide.\n");
                } else if (emailExiste(b, email, i)){
                    printf("Cet email est deja utilise.\n");
                } else{
                    break;
                }

            } while (1);

            strcpy(b->clients[i].email, email);

            do
            {
                lireTexte(b->clients[i].telephone, sizeof(b->clients[i].telephone), "Nouveau telephone (10 chiffres) : ");

                if (!telephoneValide(b->clients[i].telephone)) {
                    printf("Telephone invalide.\n");
                }
            } while (!telephoneValide(b->clients[i].telephone));

            saisirAdresse(&b->clients[i].adresse);

            printf("Client modifie avec succes.\n");
            return;
        }
    }

    printf("Client introuvable ou inactif.\n");
}

void supprimerClient(Boutique *b, int idClient)
{
    int i, j;

    if (b == NULL)
        return;

    for (i = 0; i < b->nbClients; i++)
    {
        if (b->clients[i].idClient == idClient &&
            b->clients[i].actif == 1)
        {
            for (j = 0; j < b->clients[i].nbCommandes; j++)
            {
                if (strcmp(b->clients[i].commandes[j].statut,
                           "ANNULEE") != 0)
                {
                    printf("Suppression refusee : le client "
                           "possede une commande non annulee.\n");
                    return;
                }
            }

            b->clients[i].actif = 0;

            printf("Client supprime avec succes.\n");
            return;
        }
    }

    printf("Client introuvable ou deja inactif.\n");
}

void afficherListe(Boutique *b){
    int i,j;
    float chiffreAffaires;

    if (b == NULL) {
        return;
    }
    printf("\n========== LISTE DES CLIENTS ==========\n");

    for (i = 0; i < b->nbClients; i++){
        if (b->clients[i].actif == 1){
            printf("\nID : %d\n", b->clients[i].idClient);
            printf("Nom : %s %s\n", b->clients[i].prenom, b->clients[i].nom);
            printf("Email : %s\n", b->clients[i].email);
            printf("Telephone : %s\n", b->clients[i].telephone);
            printf("Nombre de commandes : %d\n", b->clients[i].nbCommandes);

            chiffreAffaires = 0.0;

            for (j = 0; j < b->clients[i].nbCommandes; j++){
                Commande *c = &b->clients[i].commandes[j];

                if (strcmp(c->statut, "ANNULEE") != 0){
                    chiffreAffaires += c->montantHT;
                }
            }
            printf("Chiffre d'affaires : %.2f euros HT\n", chiffreAffaires);
        }
    }

    printf("\n=======================================\n");
}

void afficherClient(Boutique *b, int idClient){
    int i, j, trouve = 0;

    for (i = 0; i < b->nbClients; i++){
        if (b->clients[i].idClient == idClient && b->clients[i].actif == 1){
            trouve = 1;

            printf("\n===== FICHE CLIENT =====\n");
            printf("ID : %d\n", b->clients[i].idClient);
            printf("Nom : %s\n", b->clients[i].nom);
            printf("Prenom : %s\n", b->clients[i].prenom);
            printf("Email : %s\n", b->clients[i].email);
            printf("Telephone : %s\n", b->clients[i].telephone);
            printf("Points de fidelite : %d\n", b->clients[i].pointsFidelite);
            printf("Nombre de commandes : %d\n", b->clients[i].nbCommandes);

            printf("\n===== HISTORIQUE DES COMMANDES =====\n");

            if (b->clients[i].nbCommandes == 0){
                printf("Aucune commande pour ce client.\n");
            } else {
                for (j = 0; j < b->clients[i].nbCommandes; j++){
                    printf("\nCommande numero : %d\n", b->clients[i].commandes[j].numero);

                    printf("Date : %02d/%02d/%04d\n", b->clients[i].commandes[j].dateCommande.jour, b->clients[i].commandes[j].dateCommande.mois, b->clients[i].commandes[j].dateCommande.annee);

                    printf("Statut : %s\n", b->clients[i].commandes[j].statut);

                    printf("Montant TTC : %.2f euros\n", b->clients[i].commandes[j].montantTTC);
                }
            }

            break;
        }
    }

    if (trouve == 0){
        printf("Client introuvable ou inactif.\n");
    }
}
