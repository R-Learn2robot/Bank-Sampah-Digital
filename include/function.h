#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char nama[101];
    char nik[17];
    char rt[3];
    char rw[3];
    char alamat[101];
    int poin;
}Warga;
typedef struct {
    char nik[20];
    char nama[100];
    char jenis[10];   
    int poin;
    char tanggal[15]; 
} Riwayat;

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

// variabel global
extern Warga *data; // array of struct          
extern int jlhWarga; // ada berapa warga yg terdaftar         
extern int kapasitasWarga; // kapasitas data

extern Riwayat *transaksi;
extern int jlhTransaksi;
extern int kapasitasTransaksi;

#define DATA_WARGA "data/dataWarga.txt"
#define FILE_TRANSAKSI "data/transaksi.txt"//Faiq coba tambahakan ini


// === DEKLARASI FUNGSI ===
// fungsi lain-lain
void save();
void load();
void pause(); 
void cekKapasistas();
void hapusMemori();

// fungsi pencarian
int cariIndexNIK(const char *nik);

//fungsi sorting
void sortRWRTNama();

// fungsi tambah data warga
void tambahWarga();

// fungsi menampilkan data warga
void tampilkanListWarga();
void pencarianDataNIK();
void liatDataWarga();

// fungsi edit data
void masukkanDataBaru(const int index);
void editDataWarga();

// fungsi transaksi
void tambahTransaksi(); // <--- Tambahkan baris ini di sini

#endif