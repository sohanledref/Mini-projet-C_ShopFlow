void afficherStatsCategories(Boutique *b)
{
    int nb;
    StatCategorie *tab = calculerStatsCategories(b, &nb);
 
    printf("\n--- STATISTIQUES PAR CATEGORIE ---\n");
 
    if (tab == NULL) {
        printf("Aucune vente a afficher.\n");
        return;
    }
 
    printf("%-15s %10s %12s\n", "Categorie", "Quantite", "CA HT");
    for (int i = 0; i < nb; i++) {
        printf("%-15s %10d %12.2f\n", tab[i].categorie, tab[i].quantiteVendue, tab[i].caHT);
    }
 
    free(tab);
}
