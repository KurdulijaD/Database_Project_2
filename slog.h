#ifndef SLOG_H_INCLUDED
#define SLOG_H_INCLUDED

#define FBLOKIRANJA 3
#define OZNAKA_KRAJA_DATOTEKE -1

typedef struct {
    int sifraUverenja;
    char prezimeMehanicara[11];
    char datum[11];
    float cena;
    char prezimeVlasnika[11];
    char oznakaVrsteVozila[3];
    int deleted;
} SLOG;

typedef struct Blok {
    SLOG slogovi[FBLOKIRANJA];
} BLOK;

#endif // SLOG_H_INCLUDED
