void menuRechercheProduits(Boutique *b)
{
    char categorie[30];
    char motCle[60];
    char saisie[30];
    float prixMax;

    printf("Categorie (Entree pour ignorer) : ");
    fgets(categorie, sizeof(categorie), stdin);
    categorie[strcspn(categorie, "\n")] = '\0';   

    printf("Mot-cle dans le nom (Entree pour ignorer) : ");
    fgets(motCle, sizeof(motCle), stdin);
    motCle[strcspn(motCle, "\n")] = '\0';

    printf("Prix HT maximum (0 pour ignorer) : ");
    fgets(saisie, sizeof(saisie), stdin);
    prixMax = (float)atof(saisie);                

    rechercherProduits(b, categorie, prixMax, motCle);
}
