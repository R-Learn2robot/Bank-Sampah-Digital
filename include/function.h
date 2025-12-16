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
    int saldo;
}Warga;

typedef struct {
    char nik[20];
    char jenis[10];   
    int saldo;
    char tanggal[15]; 
} RiwayatTransaksi;

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

// === VARIABEL GLOBAL ===
extern Warga   *data;
extern int      jlhWarga;
extern int      kapasitasWarga;

// === KONSTANTA FILE ===
#define DATA_WARGA      "data/dataWarga.txt"
#define FILE_TRANSAKSI  "data/transaksi.txt"

// === DEKLARASI FUNGSI ===
// fungsi lain-lain
void save();
void load();
void pause();
void cekKapasistas();
void hapusMemori();

// fungsi pencarian
int cariIndexNIK(const char *nik);

// fungsi sorting
void sortRWRTNama();

// fungsi tambah data warga
void tambahWarga();

// fungsi menampilkan data warga
void tampilkanListWarga();
void pencarianDataNIK();
void liatDataWarga();

// fungsi setor sampah
void setorSampah();

// fungsi hapus data warga
void hapusByNIK();
void hapusSemua();
void hapusDataWarga();

// fungsi edit data
void masukkanDataBaru(const int index);
void editDataWarga();

// fungsi riwayat transaksi
void tampilRiwayatByNIK();

// fungsi pencarian
int cariIndexNIK(const char *nik);
int cariIndexNamaDenganKeyword();   // NEW: cari index berdasarkan keyword nama (substring)

#endif