#include <stdio.h>
#include <stdlib.h>
#include "operacije.h"
int main()
{
    int running = 1;
	int in;

	FILE *fajl = NULL;

	while (running) {
        printf("Izaberite opciju:\n");
        printf("1 - Otvoriti datoteku\n");
        printf("2 - Kreiraj datoteku\n");
        printf("3 - Pronadji slog\n");
        printf("4 - Dodaj slog\n");
        printf("5 - Ispis svih slogova\n");
        printf("6 - Obrisati slog fizicki\n");
        printf("7 - Pronadji prosjecnu cijenu uvjerenja vozila oznake 'LM'\n");
        printf("8 - Obrisi logicki uvjerenja sa cijenom manjom ili = 0\n");
        printf("9 - Prikazati mehanicare za vrstu vozila\n");
        printf("0. Izlaz\n");
		if (fajl == NULL) {
			printf("!!! WATRNING: datoteka jos uvek nije otvorena !!!\n");
		}
		scanf("%d", &in);
		getc(stdin);

		switch(in) {
			case 1:
				{
					char filename[20];
					printf("Unesite ime datoteke\n");
					scanf("%s", &filename[0]);
					fajl = otvoriDatoteku(filename);
					printf("\n");
					break;
				}

			case 2:
				{
					char filename[20];
					printf("Unesite ime datoteke\n");
					scanf("%s", filename);
					kreirajDatoteku(filename);
					printf("\n");
					break;
				}
            case 3:
				{
					int sifraLeta;
				    printf("\nUnesi sifru uvjerenja\n");
				    scanf("%d",&sifraLeta);
				    SLOG* slog = pronadjiSlog(fajl,sifraLeta);
                    printf("%d      %s      % s     %lf %10s %2s",
                        slog->sifraUverenja,
                        slog->prezimeMehanicara,
                        slog->datum,
                        slog->cena,
                        slog->prezimeVlasnika,
                        slog->oznakaVrsteVozila);
				    break;
				}
			case 4:
				{
				    SLOG slog;
                    printf("\nUnesite sifru uverenja:\n");
                    scanf("%d",&slog.sifraUverenja);
                    printf("Unesi prezime mehanicara:\n");
                    scanf("%s",&slog.prezimeMehanicara);
                    printf("Datum: (DD-MM-YYYY)\n");
                    scanf("%s",&slog.datum);
                    printf("Cena: \n");
                    scanf("%f",&slog.cena);
                    printf("Prezime vlasnika: ");
                    scanf("%s",&slog.prezimeVlasnika);
                    printf("Oznaka vrste vozila: ");
                    scanf("%s",&slog.oznakaVrsteVozila);
                    slog.deleted=0;
                    dodajSlog(fajl,&slog);
					break;
				}
            case 5:
				{
                    ispisiSveSlogove(fajl);
                    break;
				}
            case 6:
				{
				    int sifraUvjerenja;
				    printf("\nUnesi sifru ugovora: \n");
				    scanf("%d",&sifraUvjerenja);
                    obrisiSlogFizicki(fajl,sifraUvjerenja);
					break;
				}
			case 7:
				{
				    double p = PronadjiProsjekZaLM(fajl);
				    printf("%lf",p);
					break;
				}
            case 8:
                {
                    ObrisiUvjerenjaSaCenomManomOd0(fajl);
                    break;
                }
            case 9:
                {
                    PrikaziMehanicare(fajl);
                    break;
                }
            case 0:
                {
                    fclose(fajl);
                    return 0;
                }
		}
	}
}
