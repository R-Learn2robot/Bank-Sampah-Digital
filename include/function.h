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
    int saldo;          // saldo warga
} Warga;

typedef struct {
    char nik[20];
    char nama[100];     // kalau tidak dipakai, boleh diabaikan
    char jenis[10];     // "masuk" / "keluar"
    int saldo;          // perubahan saldo pada transaksi ini
    char tanggal[15];   // "dd/mm/yyyy" atau "yyyy-mm-dd"
} Riwayat;

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

// === VARIABEL GLOBAL ===
extern Warga   *data;
extern int      jlhWarga;
extern int      kapasitasWarga;

extern Riwayat *transaksi;
extern int      jlhTransaksi;
extern int      kapasitasTransaksi;

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

// fungsi edit data
void masukkanDataBaru(const int index);
void editDataWarga();

// fungsi transaksi
void tambahTransaksi();   // kalau nanti kamu implementasikan
void setorSampah();
void tampilRiwayatByNIK(const char *filename, const char *nikCari);

#endif