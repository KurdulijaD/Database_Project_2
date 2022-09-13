#ifndef OPERACIJE_H_INCLUDED
#define OPERACIJE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "slog.h"

void kreirajDatoteku(char *filename);
FILE *otvoriDatoteku(char *filename);
SLOG *pronadjiSlog(FILE *fajl, int sifraUverenja);
void dodajSlog(FILE *fajl, SLOG *slog);
void ispisiSveSlogove(FILE *fajl);
void obrisiSlogFizicki(FILE *fajl, int sifraUverenja);


double PronadjiProsjekZaLM(FILE *fajl);
void ObrisiUvjerenjaSaCenomManomOd0(FILE *fajl);
void PrikaziMehanicare(FILE* fajl);
#endif // OPERACIJE_H_INCLUDED
