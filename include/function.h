#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char nama[100];
    char nik[16];
    char rt[2];
    char rw[2];
    char alamat[100];
    int poin;
}Warga;

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

#define MAX_WARGA 50

#define FILE_NAME "data/tes.txt"



void tambahWarga();
void tampilData();
void sortRWRTNama();
void save();
void load();
void pause();

#endif