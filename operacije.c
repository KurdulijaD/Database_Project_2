#include "operacije.h"

FILE *otvoriDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "rb+");
	if (fajl == NULL) {
		printf("Doslo je do greske pri otvaranju datoteke\n");
	} else {
		printf("Datoteka uspesno otvorena!\n");
	}
	return fajl;
}

void kreirajDatoteku(char *filename) {
	FILE *fajl = fopen(filename, "wb");
	if (fajl == NULL) {
		printf("Greska prilikom kreiranja datoteke\n");
	} else {
		BLOK blok;
		blok.slogovi[0].sifraUverenja = OZNAKA_KRAJA_DATOTEKE;
		fwrite(&blok, sizeof(BLOK), 1, fajl);
		printf("Datoteka uspesno kreirana.\n");
		fclose(fajl);
	}
}

void dodajSlog(FILE *fajl, SLOG *slog) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	//Treba pronaci poziciju prvog sloga sa vrednoscu kljuca vecom od vrednosti
	//kljuca sloga koji upisujemo u datoteku. Na to mesto treba smestiti slog
	//koji upisujemo, a slog koji je vec bio na toj poziciji upisati u datoteku
	//na isti nacin.

	//Slog koji se trenutno upisuje ce se nalaziti u promenljivoj "slogKojiUpisujemo"
	SLOG slogKojiUpisujemo;
	memcpy(&slogKojiUpisujemo, slog, sizeof(SLOG));

	BLOK blok;
	fseek(fajl, 0, SEEK_SET);
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {

			if (blok.slogovi[i].sifraUverenja ==  OZNAKA_KRAJA_DATOTEKE) {
				//Ako smo stigli do kraja datoteke, znaci da nema slogova
				//u datoteci sa vrednoscu kljuca vecom od vrednosti kljuca
				//sloga koji upisujemo; na to mesto cemo staviti
				//slog koji se upisuje u datoteku, a oznaku kraja
				//cemo pomeriti za jedno mesto unapred.
				memcpy(&blok.slogovi[i], &slogKojiUpisujemo, sizeof(SLOG));

				//Na mesto ispred unetog, sad treba staviti slog sa oznakom kraja datoteke;
				//Kako to izvesti, zavisi od toga da li ima praznih mesta u trenutnom bloku.

				if (i != FBLOKIRANJA-1) {
					//Ako tekuci slog nije poslednji slog u bloku, ima jedno
					//prazno mesto posle njega, pa tu mozemo smestiti slog
					//sa oznakom kraja datoteke.
					blok.slogovi[i+1].sifraUverenja = OZNAKA_KRAJA_DATOTEKE;

					//To je to, mozemo vratiti blok u datoteku
					fseek(fajl, -sizeof(BLOK), SEEK_CUR);
					fwrite(&blok, sizeof(BLOK), 1, fajl);

					printf("Novi slog evidentiran u datoteci.\n");
					return;

				} else {
					//Inace, nemamo vise mesta u ovom bloku.
					//Oznaku kraja moramo prebaciti u sledeci blok
					//(koji prvo moramo napraviti).

					//Kao prvo, trenutni blok je pun, pa ga mozemo vratiti
					//nazad u datoteku.
					fseek(fajl, -sizeof(BLOK), SEEK_CUR);
					fwrite(&blok, sizeof(BLOK), 1, fajl);

					//Sada mozemo napraviti novi blok i upisati u njega slog
					//sa oznakom kraja datoteke (na prvu poziciju)
					BLOK noviBlok;
					noviBlok.slogovi[0].sifraUverenja = OZNAKA_KRAJA_DATOTEKE;
					fwrite(&noviBlok, sizeof(BLOK), 1, fajl);

					printf("Novi slog evidentiran u datoteci.\n");
					printf("(dodat novi blok)\n");
					return;

				}

			} else if (blok.slogovi[i].sifraUverenja == slogKojiUpisujemo.sifraUverenja) {

                //printf("vec postoji %s == %s \n", blok.slogovi[i].evidBroj, slogKojiUpisujemo.evidBroj);

                if (!blok.slogovi[i].deleted) {
                    printf("Slog sa tom vrednoscu kljuca vec postoji!\n");
                    return;
                } else {
                    //Imamo logicki obrisan slog sa istom vrednoscu kljuca sloga
                    //kao slog koji upisujemo u datoteku. Na to mesto treba prepisati
                    //novi slog preko tog logicki izbrisanog.
                    memcpy(&blok.slogovi[i], &slogKojiUpisujemo, sizeof(SLOG));

                    //Sad samo vratimo ceo taj blok u datoteku i to je to:
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
					fwrite(&blok, sizeof(BLOK), 1, fajl);

					printf("Novi slog evidentiran u datoteci.\n");
					printf("(prepisan preko logicki izbrisanog)\n");
					return;
                }

            } else if (blok.slogovi[i].sifraUverenja > slogKojiUpisujemo.sifraUverenja) {
				//Pronasli smo prvi slog sa kljucem vecim od ovog
				//koji upisujemo u datoteku. Na to mesto smo planirali
				//da smestimo novi slog. Pre nego sto to uradimo, sacuvacemo
				//slog koji se vec nalazi na tom mestu, u promenljivu tmp,
				//a potom ga odatle prepisati u "slogKojiUpisujemo", jer cemo njega
				//sledeceg upisivati (na isti nacin kao i prethodni slog)
				SLOG tmp;
				memcpy(&tmp, &blok.slogovi[i], sizeof(SLOG));
				memcpy(&blok.slogovi[i], &slogKojiUpisujemo, sizeof(SLOG));
				memcpy(&slogKojiUpisujemo, &tmp, sizeof(SLOG));

				//Sada u "slogKojiUpisujemo" stoji slog koji je bio na tekucoj
				//poziciji (pre nego sto smo na nju upisali onaj koji
				//dodajemo). U narednoj iteraciji, taj slog ce se dodavati u datoteku
				//na isti nacin.

				//Takodje, ako je to bio poslednji slog u bloku, mozemo ceo taj
				//blok vratiti u datoteku.
				if (i == FBLOKIRANJA-1) {
					fseek(fajl, -sizeof(BLOK), SEEK_CUR);
					fwrite(&blok, sizeof(BLOK), 1, fajl);
					fflush(fajl);
				}
			}
		}
	}
}

SLOG *pronadjiSlog(FILE *fajl, int sifra) {
	if (fajl == NULL) {
		return NULL;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraUverenja == OZNAKA_KRAJA_DATOTEKE ||blok.slogovi[i].sifraUverenja > sifra) {
				return NULL;
			} else if (blok.slogovi[i].sifraUverenja == sifra) {
				if (!blok.slogovi[i].deleted) {
                    SLOG *slog = (SLOG *)malloc(sizeof(SLOG));
                    memcpy(slog, &blok.slogovi[i], sizeof(SLOG));
                    return slog;
				}
			}
		}
	}

	return NULL;
}

void ispisiSveSlogove(FILE *fajl) {
	if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int rbBloka = 0;
    printf("Sifra        Prez.Meh      Datum.Dol         Cena          Prez.Vla        Oznaka\n");
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraUverenja == OZNAKA_KRAJA_DATOTEKE) {
				printf("B%d S%d *\n", rbBloka, i);
                return;
			} else if (!blok.slogovi[i].deleted) {
                printf("B%d S%d ", rbBloka, i);
                printf("%d      %s         %s        %.2lf        %s          %s",
                blok.slogovi[i].sifraUverenja,
                blok.slogovi[i].prezimeMehanicara,
                blok.slogovi[i].datum,
                blok.slogovi[i].cena,
                blok.slogovi[i].prezimeVlasnika,
                blok.slogovi[i].oznakaVrsteVozila);
                printf("\n");
            }
		}

		rbBloka++;
	}
}

void obrisiSlogFizicki(FILE *fajl, int sifra) {

    SLOG *slog = pronadjiSlog(fajl, sifra);
    if (slog == NULL) {
        printf("Slog koji zelite obrisati ne postoji!\n");
        return;
    }

    //Trazimo slog sa odgovarajucom vrednoscu kljuca, i potom sve
    //slogove ispred njega povlacimo jedno mesto unazad.

    BLOK blok, naredniBlok;
    int sifraZaBrisanje = sifra;

    fseek(fajl, 0, SEEK_SET);
    while (fread(&blok, 1, sizeof(BLOK), fajl)) {
        for (int i = 0; i < FBLOKIRANJA; i++) {

            if (blok.slogovi[i].sifraUverenja == OZNAKA_KRAJA_DATOTEKE) {

                if (i == 0) {
                    //Ako je oznaka kraja bila prvi slog u poslednjem bloku,
                    //posle brisanja onog sloga, ovaj poslednji blok nam
                    //vise ne treba;
                    printf("(skracujem fajl...)\n");
                    fseek(fajl, -sizeof(BLOK), SEEK_END);
					long bytesToKeep = ftell(fajl);
					ftruncate(fileno(fajl), bytesToKeep);
					fflush(fajl); //(da bi se primenile promene usled poziva truncate)
                }

                printf("Slog je fizicki obrisan.\n");
                return;
            }

            if (blok.slogovi[i].sifraUverenja == sifraZaBrisanje) {

                //Obrisemo taj slog iz bloka tako sto sve slogove ispred njega
                //povucemo jedno mesto unazad.
                for (int j = i+1; j < FBLOKIRANJA; j++) {
                    memcpy(&(blok.slogovi[j-1]), &(blok.slogovi[j]), sizeof(SLOG));
                }

                //Jos bi hteli na poslednju poziciju u tom bloku upisati prvi
                //slog iz narednog bloka, pa cemo zato ucitati naredni blok...
                int podatakaProcitano = fread(&naredniBlok, sizeof(BLOK), 1, fajl);

                //...i pod uslovom da uopste ima jos blokova posle trenutnog...
                if (podatakaProcitano) {

                    //(ako smo nesto procitali, poziciju u fajlu treba vratiti nazad)
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);

                    //...prepisati njegov prvi slog na mesto poslednjeg sloga u trenutnom bloku.
                    memcpy(&(blok.slogovi[FBLOKIRANJA-1]), &(naredniBlok.slogovi[0]), sizeof(SLOG));

                    //U narednoj iteraciji while loopa, brisemo prvi slog iz narednog bloka.
                    sifraZaBrisanje= naredniBlok.slogovi[0].sifraUverenja;
                }

                //Vratimo trenutni blok u fajl.
                fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                fwrite(&blok, sizeof(BLOK), 1, fajl);
                fflush(fajl);

                if (!podatakaProcitano) {
                    //Ako nema vise blokova posle trentnog, mozemo prekinuti algoritam.
                    printf("Slog je fizicki obrisan.\n");
                    return;
                }

                //To je to, citaj sledeci blok
                break;
            }

        }
    }
}
double PronadjiProsjekZaLM(FILE *fajl){
  if(fajl == NULL){
        printf("Nesto ne valja \n");
        return;
    }

    double prosjekLM= 0;
    int brojLm = 0;
	fseek(fajl, 0, SEEK_SET);
	BLOK blok;

	while (fread(&blok, sizeof(BLOK), 1, fajl)) {

		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraUverenja ==  OZNAKA_KRAJA_DATOTEKE) {
                prosjekLM = prosjekLM/(double)brojLm;
                return prosjekLM;
			}

            if(strcmp(blok.slogovi[i].oznakaVrsteVozila, "LM") == 0 && blok.slogovi[i].deleted==0){
                      prosjekLM += (double)blok.slogovi[i].cena;
                      brojLm++;
            }
		}
	}
}
void ObrisiUvjerenjaSaCenomManomOd0(FILE *fajl){
    if (fajl == NULL)
    {
        printf("Fajl nije otvoren \n");
        return;
    }

    BLOK blok;
    int br=0;
    fseek(fajl, 0, SEEK_SET);

    while(fread(&blok, sizeof(BLOK), 1, fajl))
    {
        for (int i = 0; i < FBLOKIRANJA; i++)
        {
            if(blok.slogovi[i].sifraUverenja==OZNAKA_KRAJA_DATOTEKE){
                return;
            }
            if (blok.slogovi[i].deleted==0)
            {
                if(blok.slogovi[i].cena <= 0){;
                    blok.slogovi[i].deleted =1;
                    fseek(fajl, -sizeof(BLOK), SEEK_CUR);
                    fwrite(&blok, sizeof(BLOK), 1, fajl);
                    fseek(fajl, 0, SEEK_CUR);
                    printf("Slog %d u bloku %d izbrisan\n",i,br);
                }

            }
        }
        br++;
    }
}

void PrikazMehanicara(FILE* fajl){
    if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}
	int z=0;
    char vrste[30][3];
	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {
		for (int i = 0; i < FBLOKIRANJA; i++) {
            if (blok.slogovi[i].sifraUverenja == OZNAKA_KRAJA_DATOTEKE ) {
                break;
            }
            int ind=0;
            if (blok.slogovi[i].deleted==0) {
                for(int j=0;j<z;j++){
                    if(strcmp(vrste[j],blok.slogovi[i].oznakaVrsteVozila)==0){
                        ind=1;
                    }
                }
                if(ind==0){
                    strcpy(vrste[z],blok.slogovi[i].oznakaVrsteVozila);
                    z++;
                }
		   }
	    }

	}

	for(int i=0;i<z;i++){
        fseek(fajl, 0, SEEK_SET);
        int rbBloka = 0;
        printf("Za vrstu %s\n",vrste[i]);
        printf("Sifra   Prez.Meh      Datum.Dol  Cena  Prez.Vla  Oznaka\n");
        while (fread(&blok, sizeof(BLOK), 1, fajl)) {
            for (int i = 0; i < FBLOKIRANJA; i++) {
                if (blok.slogovi[i].sifraUverenja== OZNAKA_KRAJA_DATOTEKE) {
                    printf("B%d S%d *\n", rbBloka, i);
                    break; //citaj sledeci blok
                }
                if (blok.slogovi[i].deleted==0 && strcmp(blok.slogovi[i].oznakaVrsteVozila,vrste[i])==0) {
                    printf("B%d S%d ", rbBloka, i);
                    printf("B%d S%d ", rbBloka, i);
                    printf("%d      %s      %s     %lf     %s      %s",
                    blok.slogovi[i].sifraUverenja,
                    blok.slogovi[i].prezimeMehanicara,
                    blok.slogovi[i].datum,
                    blok.slogovi[i].cena,
                    blok.slogovi[i].prezimeVlasnika,
                    blok.slogovi[i].oznakaVrsteVozila);
                    printf("\n");
                }
            }

            rbBloka++;
        }
	}
}

void PrikaziMehanicare(FILE* fajl){
    char vv[150][3];
    if (fajl == NULL) {
		printf("Datoteka nije otvorena!\n");
		return;
	}

	fseek(fajl, 0, SEEK_SET);
	BLOK blok;
	int b=0;
	while (fread(&blok, sizeof(BLOK), 1, fajl)) {
		for (int i = 0; i < FBLOKIRANJA; i++) {
			if (blok.slogovi[i].sifraUverenja == OZNAKA_KRAJA_DATOTEKE) {
                break;
			} else if (!blok.slogovi[i].deleted) {
                int ind=0;
                for(int k = 0;k<b;k++){
                    if(strcmp(vv[k],blok.slogovi[i].oznakaVrsteVozila)==0){
                        ind=1;
                    }
                }
			    if(ind==0)
                {
                    strcpy(vv[b],blok.slogovi[i].oznakaVrsteVozila);
                    b++;
                }
            }
		}
	}

    for(int i =0 ;i<b;i++){
        if (fajl == NULL) {
            printf("Datoteka nije otvorena!\n");
            return;
        }

        fseek(fajl, 0, SEEK_SET);
        BLOK blok;
        int rbBloka = 0;
        printf("Ozn. voz. %s\n",vv[i]);
        printf("     Prezime:\n");
        while (fread(&blok, sizeof(BLOK), 1, fajl)) {
            for (int j = 0; j < FBLOKIRANJA; j++) {
                if (blok.slogovi[j].sifraUverenja == OZNAKA_KRAJA_DATOTEKE) {
                    break;
                } else if (!blok.slogovi[j].deleted && strcmp(blok.slogovi[j].oznakaVrsteVozila,vv[i])==0) {
                    printf("     %s\n",blok.slogovi[j].prezimeMehanicara);
                }
            }
        }
        printf("\n");
    }
}


