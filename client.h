#ifndef CLIENT_H
#define CLIENT_H

#include "types.h"

void ajouterClient(Boutique *b);
void modifierClient(Boutique *b, int idClient);
void supprimerClient(Boutique *b, int idClient);
void afficherListe(Boutique *b);
void afficherClient(Boutique *b, int idClient);

#endif