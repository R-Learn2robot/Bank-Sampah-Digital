#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_WARGA 500
#define MAX_TRANSAKSI 2000
#define NAMA_LEN 60
#define ALAMAT_LEN 100
#define JENIS_LEN 20
#define TGL_LEN 20

#define HARGA_PLASTIK 1500.0f
#define HARGA_KARDUS 1000.0f
#define HARGA_GELAS 2000.0f
#define HARGA_LOGAM 5000.0f
#define HARGA_ORGANIK 500.0f

char* strcasestr_custom(const char* haystack, const char* needle) {
    if (!haystack || !needle) return NULL;
    size_t nlen = strlen(needle);
    if (nlen == 0) return (char*)haystack;
    for (; *haystack; haystack++) {
        if (tolower(*haystack) == tolower(*needle)) {
            if (strncasecmp(haystack, needle, nlen) == 0) {
                return (char*)haystack;
            }
        }
    }
    return NULL;
}

typedef struct {
    int id;
    char nama[NAMA_LEN];
    char alamat[ALAMAT_LEN];
    float saldo;
} Warga;

typedef struct {
    int idTransaksi;
    int wargaID;
    char jenis[JENIS_LEN];
    float berat;
    float total;
    char tanggal[TGL_LEN];
} Transaksi;

Warga wargaList[MAX_WARGA];
int jumlahWarga = 0;

Transaksi transaksiList[MAX_TRANSAKSI];
int jumlahTransaksi = 0;
int nextTransaksiID = 1;

void menu();
void tambahWarga();
int cariIndexWargaByID(int id);
int cariIndexWargaByNama(char *nama);
void tampilkanSemuaWarga();
void setorSampah();
float hargaPerKg(char *jenis);
void lihatProfilSaldo();
void tampilkanTransaksi(int idx);
void simpanData();
void muatData();
void cariWargaMenu();
void sortingRankingBySaldo();
void swapWarga(Warga *a, Warga *b);
void pause();

int main() {
    muatData();
    menu();
    return 0;
}

void menu() {
    int pilihan;
    do {
        system("cls");
        printf("=========================================\n");
        printf("         BANK SAMPAH DIGITAL (CLI)      \n");
        printf("=========================================\n");
        printf("1. Tambah Data Warga\n");
        printf("2. Tampilkan Semua Warga\n");
        printf("3. Setor Sampah\n");
        printf("4. Lihat Profil & Saldo Warga\n");
        printf("5. Cari Data Warga\n");
        printf("6. Ranking Saldo (Sort)\n");
        printf("7. Riwayat Semua Transaksi\n");
        printf("8. Simpan Data ke File\n");
        printf("9. Muat Data dari File\n");
        printf("0. Keluar\n");
        printf("-----------------------------------------\n");
        printf("Pilih menu: ");
        scanf("%d", &pilihan);
        getchar();

        switch(pilihan) {
            case 1: tambahWarga(); break;
            case 2: tampilkanSemuaWarga(); break;
            case 3: setorSampah(); break;
            case 4: lihatProfilSaldo(); break;
            case 5: cariWargaMenu(); break;
            case 6: sortingRankingBySaldo(); break;
            case 7:
                printf("\n--- Semua Riwayat Transaksi ---\n");
                for (int i = 0; i < jumlahTransaksi; i++) tampilkanTransaksi(i);
                pause();
                break;
            case 8: simpanData(); break;
            case 9: muatData(); break;
            case 0:
                printf("Simpan data sebelum keluar? (y/n): ");
                char c;
                c = getchar();
                if (c == 'y' || c == 'Y') simpanData();
                break;
        }
    } while (pilihan != 0);
}

void pause() {
    printf("\nTekan ENTER untuk kembali...");
    getchar();
}

void tambahWarga() {
    Warga w;
    w.id = jumlahWarga == 0 ? 1 : wargaList[jumlahWarga-1].id + 1;

    printf("Nama: ");
    fgets(w.nama, NAMA_LEN, stdin);
    w.nama[strcspn(w.nama, "\n")] = 0;

    printf("Alamat: ");
    fgets(w.alamat, ALAMAT_LEN, stdin);
    w.alamat[strcspn(w.alamat, "\n")] = 0;

    w.saldo = 0;

    wargaList[jumlahWarga++] = w;
    printf("Data warga berhasil ditambahkan.\n");
    pause();
}

int cariIndexWargaByID(int id) {
    for (int i = 0; i < jumlahWarga; i++) {
        if (wargaList[i].id == id) return i;
    }
    return -1;
}

int cariIndexWargaByNama(char *nama) {
    for (int i = 0; i < jumlahWarga; i++) {
        if (strcasecmp(wargaList[i].nama, nama) == 0) return i;
    }
    return -1;
}

void tampilkanSemuaWarga() {
    printf("\nDaftar Warga:\n");
    for (int i = 0; i < jumlahWarga; i++) {
        printf("%d | %s | Rp %.2f\n", wargaList[i].id, wargaList[i].nama, wargaList[i].saldo);
    }
    pause();
}

float hargaPerKg(char *jenis) {
    char j[32];
    strcpy(j, jenis);
    for (int i = 0; j[i]; i++) j[i] = tolower(j[i]);

    if (strstr(j, "plast")) return HARGA_PLASTIK;
    if (strstr(j, "kard")) return HARGA_KARDUS;
    if (strstr(j, "gelas")) return HARGA_GELAS;
    if (strstr(j, "logam")) return HARGA_LOGAM;
    if (strstr(j, "organik")) return HARGA_ORGANIK;
    return -1;
}

void setorSampah() {
    int id;
    printf("Masukkan ID warga: ");
    scanf("%d", &id);
    getchar();

    int idx = cariIndexWargaByID(id);
    if (idx == -1) {
        printf("Warga tidak ditemukan.\n");
        pause();
        return;
    }

    char jenis[JENIS_LEN];
    float berat;
    char tanggal[TGL_LEN];

    printf("Jenis sampah: ");
    fgets(jenis, JENIS_LEN, stdin);
    jenis[strcspn(jenis, "\n")] = 0;

    printf("Berat (kg): ");
    scanf("%f", &berat);
    getchar();

    printf("Tanggal: ");
    fgets(tanggal, TGL_LEN, stdin);
    tanggal[strcspn(tanggal, "\n")] = 0;

    float harga = hargaPerKg(jenis);
    if (harga < 0) {
        printf("Jenis sampah tidak dikenali.\n");
        pause();
        return;
    }

    float total = harga * berat;
    wargaList[idx].saldo += total;

    Transaksi t;
    t.idTransaksi = nextTransaksiID++;
    t.wargaID = id;
    strcpy(t.jenis, jenis);
    t.berat = berat;
    t.total = total;
    strcpy(t.tanggal, tanggal);
    transaksiList[jumlahTransaksi++] = t;

    printf("Transaksi berhasil dicatat.\n");
    pause();
}

void tampilkanTransaksi(int idx) {
    Transaksi t = transaksiList[idx];
    int i = cariIndexWargaByID(t.wargaID);
    printf("%d | %s | %.2f kg | Rp %.2f | %s\n",
           t.idTransaksi,
           wargaList[i].nama,
           t.berat,
           t.total,
           t.tanggal);
}

void lihatProfilSaldo() {
    int id;
    printf("Masukkan ID warga: ");
    scanf("%d", &id);
    getchar();

    int idx = cariIndexWargaByID(id);
    if (idx == -1) {
        printf("Warga tidak ditemukan.\n");
        pause();
        return;
    }

    Warga w = wargaList[idx];

    printf("\nProfil Warga:\n");
    printf("ID: %d\nNama: %s\nAlamat: %s\nSaldo: Rp %.2f\n\n",
           w.id, w.nama, w.alamat, w.saldo);

    printf("Riwayat Transaksi:\n");
    for (int i = 0; i < jumlahTransaksi; i++) {
        if (transaksiList[i].wargaID == w.id) tampilkanTransaksi(i);
    }

    pause();
}

void cariWargaMenu() {
    char input[NAMA_LEN];
    printf("Masukkan nama/substring: ");
    fgets(input, NAMA_LEN, stdin);
    input[strcspn(input, "\n")] = 0;

    for (int i = 0; i < jumlahWarga; i++) {
        if (strcasestr_custom(wargaList[i].nama, input) != NULL) {
            printf("%d | %s | Rp %.2f\n",
                   wargaList[i].id, wargaList[i].nama, wargaList[i].saldo);
        }
    }
    pause();
}

void swapWarga(Warga *a, Warga *b) {
    Warga t = *a;
    *a = *b;
    *b = t;
}

void sortingRankingBySaldo() {
    Warga temp[MAX_WARGA];
    for (int i = 0; i < jumlahWarga; i++) temp[i] = wargaList[i];

    for (int i = 0; i < jumlahWarga - 1; i++) {
        for (int j = 0; j < jumlahWarga - i - 1; j++) {
            if (temp[j].saldo < temp[j+1].saldo) {
                swapWarga(&temp[j], &temp[j+1]);
            }
        }
    }

    printf("\nRanking Saldo:\n");
    for (int i = 0; i < jumlahWarga; i++) {
        printf("%d. %s - Rp %.2f\n", i+1, temp[i].nama, temp[i].saldo);
    }
    pause();
}

void simpanData() {
    FILE *fw = fopen("warga.txt", "w");
    for (int i = 0; i < jumlahWarga; i++) {
        fprintf(fw, "%d|%s|%s|%.2f\n",
                wargaList[i].id,
                wargaList[i].nama,
                wargaList[i].alamat,
                wargaList[i].saldo);
    }
    fclose(fw);

    FILE *ft = fopen("transaksi.txt", "w");
    for (int i = 0; i < jumlahTransaksi; i++) {
        fprintf(ft, "%d|%d|%s|%.2f|%.2f|%s\n",
                transaksiList[i].idTransaksi,
                transaksiList[i].wargaID,
                transaksiList[i].jenis,
                transaksiList[i].berat,
                transaksiList[i].total,
                transaksiList[i].tanggal);
    }
    fclose(ft);

    FILE *fm = fopen("meta.txt", "w");
    fprintf(fm, "%d\n", nextTransaksiID);
    fclose(fm);

    printf("Data berhasil disimpan.\n");
    pause();
}

void muatData() {
    FILE *fw = fopen("warga.txt", "r");
    if (fw) {
        jumlahWarga = 0;
        char line[256];
        while (fgets(line, sizeof(line), fw)) {
            Warga w;
            char *p = strtok(line, "|");
            w.id = atoi(p);
            p = strtok(NULL, "|");
            strcpy(w.nama, p);
            p = strtok(NULL, "|");
            strcpy(w.alamat, p);
            p = strtok(NULL, "|");
            w.saldo = atof(p);
            wargaList[jumlahWarga++] = w;
        }
        fclose(fw);
    }

    FILE *ft = fopen("transaksi.txt", "r");
    if (ft) {
        jumlahTransaksi = 0;
        char line[256];
        while (fgets(line, sizeof(line), ft)) {
            Transaksi t;
            char *p = strtok(line, "|");
            t.idTransaksi = atoi(p);
            p = strtok(NULL, "|");
            t.wargaID = atoi(p);
            p = strtok(NULL, "|");
            strcpy(t.jenis, p);
            p = strtok(NULL, "|");
            t.berat = atof(p);
            p = strtok(NULL, "|");
            t.total = atof(p);
            p = strtok(NULL, "|");
            strcpy(t.tanggal, p);
            transaksiList[jumlahTransaksi++] = t;
        }
        fclose(ft);
    }

    FILE *fm = fopen("meta.txt", "r");
    if (fm) {
        fscanf(fm, "%d", &nextTransaksiID);
        fclose(fm);
    }
}
