#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Struktur Warga: Ganti poin menjadi saldo
typedef struct {
    char nama[101];
    char nik[17];
    char rt[3];
    char rw[3];
    char alamat[101];
    int saldo; // <-- Diubah dari poin menjadi saldo
} Warga;

// Struktur Riwayat: Ganti poin menjadi saldo
typedef struct {
    char nik[20];
    char nama[100];
    char jenis[10];   
    int saldo;        // <-- Diubah dari poin menjadi saldo
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

// Variabel global untuk riwayat transaksi
extern Riwayat *transaksi; // <-- Ditambahkan
extern int jlhTransaksi;   // <-- Ditambahkan
extern int kapasitasTransaksi; // <-- Ditambahkan

#define DATA_WARGA "data/dataWarga.txt"
#define FILE_TRANSAKSI "data/transaksi.txt" //Faiq coba tambahakan ini


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
void tambahTransaksi();
void setorSampah(); // <-- Deklarasi fungsi setorSampah ditambahkan

#endif