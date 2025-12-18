#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

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
// fungsi utilities
void pause();
void save();
void load();
void cekKapasistas();
void hapusMemori();
void getTanggalHariIni(char tanggal[]);
void toLowerCase(char *str);
void formatNama(char *nama);

// fungsi input
int inputInt(char *perintah);
void inputString(char *string, int size, const char *prompt);
void inputNik(char *nik, int size, const char *perintah);

// fungsi pencarian
int cariIndexNIK(const char *nik);
int cariIndexNama();

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

// fungsi tarik saldo
void tarikSaldo();

// fungsi riwayat transaksi
void tampilRiwayatByNIK();

#endif