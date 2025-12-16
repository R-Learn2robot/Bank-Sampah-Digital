#include "../include/function.h"

Warga   *data              = NULL;
int      jlhWarga          = 0;
int      kapasitasWarga    = 0;

Riwayat *transaksi         = NULL;   // kalau nanti dipakai sebagai array
int      jlhTransaksi      = 0;
int      kapasitasTransaksi = 0;

// ================= FUNGSI LAIN-LAIN =================

void pause() { // fungsi agar setelah setiap fungsi selesai dia berhenti sejenak baru melakukan clear screen
    printf("\ntekan enter untuk kembali...");
    getchar();
}

void save() { // menyimpan data warga ke DATA_WARGA
    FILE *pF = fopen(DATA_WARGA, "w");
    if (!pF) {
        printf("file tidak ditemukan!");
        return;
    }

    for (int i = 0; i < jlhWarga; i++) {
        fprintf(
            pF,
            "%s|%s|%s|%s|%s|%d|\n",
            data[i].nama,
            data[i].nik,
            data[i].rw,
            data[i].rt,
            data[i].alamat,
            data[i].saldo
        );
    }

    fclose(pF);
}

void load() { // membaca file dan memasukkannya ke array of struct warga data[]
    FILE *pF = fopen(DATA_WARGA, "r");
    if (!pF) {
        printf("file tidak ditemukan!");
        return;
    }

    char line[255];
    jlhWarga  = 0;

    while (fgets(line, sizeof(line), pF)) {
        cekKapasistas();
        sscanf(
            line,
            "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|",
            data[jlhWarga].nama,
            data[jlhWarga].nik,
            data[jlhWarga].rw,
            data[jlhWarga].rt,
            data[jlhWarga].alamat,
            &data[jlhWarga].saldo
        );
        jlhWarga++;
    }

    fclose(pF);
}

void cekKapasistas() { // fungsi mengecek apakah kapasitas array masih cukup
    if (jlhWarga >= kapasitasWarga) {
        kapasitasWarga = (kapasitasWarga == 0) ? 2 : kapasitasWarga * 2;
        Warga *temp = realloc(data, kapasitasWarga * sizeof(Warga));
        if (temp == NULL) {
            printf("\ngagal mengalokasikan data");
            exit(1);
        }
        data = temp;
    }
}

void hapusMemori() { // fungsi membersihkan memori setelah program selesai agar tidak memory leak
    free(data);
    data = NULL;
    kapasitasWarga = 0;
    jlhWarga = 0;
}

// ================= FUNGSI PENCARIAN =================

int cariIndexNIK(const char *nik) { // fungsi cari index array berdasarkan NIK
    for (int i = 0; i < jlhWarga; i++) {
        if (strcmp(data[i].nik, nik) == 0) {
            return i;
        }
    }
    return -1;
}

// cari index berdasarkan nama lengkap, jika ada lebih dari satu, user pilih mana
int cariIndexNamaPilihan() {
    char namaCari[101];
    int indexKetemu[200];   // menampung index-index yang cocok
    int jumlahKetemu = 0;
    int pilihan;
    int i;

    system(CLEAR);
    printf("=== PENCARIAN DATA WARGA BERDASARKAN NAMA ===\n");
    printf("Masukkan nama lengkap yang ingin dicari: ");
    getchar();                          // buang newline sisa scanf sebelumnya
    fgets(namaCari, sizeof(namaCari), stdin);

    // buang '\n' di akhir fgets jika ada
    size_t len = strlen(namaCari);
    if (len > 0 && namaCari[len - 1] == '\n') {
        namaCari[len - 1] = '\0';
    }

    // cari semua yang namanya sama persis
    for (i = 0; i < jlhWarga; i++) {
        if (strcmp(data[i].nama, namaCari) == 0) {
            indexKetemu[jumlahKetemu] = i;
            jumlahKetemu++;
        }
    }

    if (jumlahKetemu == 0) {
        printf("\nTidak ada data dengan nama \"%s\".\n", namaCari);
        pause();
        return -1;
    }

    // kalau cuma satu, langsung pakai itu
    if (jumlahKetemu == 1) {
        int idx = indexKetemu[0];
        printf("\nDitemukan 1 data:\n");
        printf("Index  : %d\n", idx);
        printf("Nama   : %s\n", data[idx].nama);
        printf("NIK    : %s\n", data[idx].nik);
        printf("RW     : %s\n", data[idx].rw);
        printf("RT     : %s\n", data[idx].rt);
        printf("Alamat : %s\n", data[idx].alamat);
        printf("Saldo  : %d\n", data[idx].saldo);
        pause();
        return idx;
    }

    // kalau lebih dari satu, tampilkan tabel dan minta user pilih
    printf("\nDitemukan %d data dengan nama \"%s\":\n", jumlahKetemu, namaCari);
    printf("==========================================================================\n");
    printf("| No | Index |      NIK       | Nama                           | Saldo |\n");
    printf("==========================================================================\n");
    for (i = 0; i < jumlahKetemu; i++) {
        int idx = indexKetemu[i];
        printf("| %-2d | %-5d | %-13s | %-30s | %-5d |\n",
               i + 1,
               idx,
               data[idx].nik,
               data[idx].nama,
               data[idx].saldo);
    }
    printf("==========================================================================\n");

    // minta pilihan
    do {
        printf("Pilih nomor data yang ingin diubah (1-%d), atau 0 untuk batal: ",
               jumlahKetemu);

        if (scanf("%d", &pilihan) != 1) {
            while (getchar() != '\n');   // bersihkan input jika bukan angka
            printf("Input tidak valid.\n");
            continue;
        }

        if (pilihan == 0) {
            printf("Dibatalkan.\n");
            pause();
            return -1;
        }

        if (pilihan < 1 || pilihan > jumlahKetemu) {
            printf("Pilihan di luar jangkauan.\n");
        } else {
            int idxTerpilih = indexKetemu[pilihan - 1];
            printf("Anda memilih data:\n");
            printf("Nama   : %s\n", data[idxTerpilih].nama);
            printf("NIK    : %s\n", data[idxTerpilih].nik);
            printf("RW     : %s\n", data[idxTerpilih].rw);
            printf("RT     : %s\n", data[idxTerpilih].rt);
            printf("Alamat : %s\n", data[idxTerpilih].alamat);
            printf("Saldo  : %d\n", data[idxTerpilih].saldo);
            pause();
            return idxTerpilih;
        }
    } while (1);
}

// ================= FUNGSI SORTING =================

void sortRWRTNama() { // sorting data berdasarkan RW lalu RT lalu nama
    Warga temp;

    if (jlhWarga <= 1) {
        return;
    }

    for (int i = 0; i < jlhWarga - 1; i++) {
        for (int j = 0; j < jlhWarga - i - 1; j++) {
            int cmpRW = strcmp(data[j].rw, data[j + 1].rw);

            if (cmpRW > 0) {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            } else if (cmpRW == 0) {
                int cmpRT = strcmp(data[j].rt, data[j + 1].rt);

                if (cmpRT > 0) {
                    temp = data[j];
                    data[j] = data[j + 1];
                    data[j + 1] = temp;
                } else if (cmpRT == 0) {
                    int cmpNama = strcmp(data[j].nama, data[j + 1].nama);

                    if (cmpNama > 0) {
                        temp = data[j];
                        data[j] = data[j + 1];
                        data[j + 1] = temp;
                    }
                }
            }
        }
    }
}

// ================= FUNGSI TAMBAH WARGA =================

void tambahWarga() { // fungsi tambah data warga ke file.txt
    system(CLEAR);
    cekKapasistas();
    char nikInput[17];

    printf("=== MENAMBAHKAN DATA WARGA ===\n");
    printf("Nama   : ");
    scanf(" %100[^\n]", data[jlhWarga].nama);

    do {
        printf("NIK    : ");
        scanf(" %16s", nikInput);

        if (cariIndexNIK(nikInput) != -1) {
            printf("\nNIK %s sudah terdaftar!\n", nikInput);
            printf("Silakan masukkan NIK lain.\n\n");
        }

    } while (cariIndexNIK(nikInput) != -1);
    strcpy(data[jlhWarga].nik, nikInput);

    printf("RW     : ");
    scanf(" %2s", data[jlhWarga].rw);

    printf("RT     : ");
    scanf(" %2s", data[jlhWarga].rt);

    printf("Alamat : ");
    scanf(" %100[^\n]", data[jlhWarga].alamat);

    data[jlhWarga].saldo = 0;
    jlhWarga++;

    sortRWRTNama();
    save();
}

// ================= FUNGSI TAMPIL DATA =================

void tampilkanListWarga() { // fungsi tampilkan semua data dari file
    FILE *pF = fopen(DATA_WARGA, "r");
    if (!pF) {
        printf("file tidak ditemukan");
        return;
    }

    if (jlhWarga == 0) {
        printf("belum ada warga yang terdaftar");
        fclose(pF);
        return;
    }

    system(CLEAR);
    printf("\n=== LIST WARGA ===\n");

    char line[255];
    char nama[100], nik[100], rt[100], rw[100], alamat[100];
    int saldo;
    int count = 1;

    while (fgets(line, sizeof(line), pF)) {
        sscanf(
            line,
            "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|",
            nama, nik, rw, rt, alamat, &saldo
        );
        printf("%d) Nama   : %s\n", count, nama);
        printf("    NIK    : %s\n", nik);
        printf("    RW     : %s\n", rw);
        printf("    RT     : %s\n", rt);
        printf("    Alamat : %s\n", alamat);
        printf("    Saldo  : %d\n", saldo);
        count++;
    }
    fclose(pF);
}

void pencarianDataNIK() { // fungsi cari data warga berdasarkan NIK
    char cariNIK[17];
    system(CLEAR);
    printf("=== PENCARIAN DATA WARGA BERDASARKAN NIK ===\n");
    printf("Masukkan NIK yang ingin dicari: ");
    scanf(" %16s", cariNIK);

    int index = cariIndexNIK(cariNIK);

    if (index == -1) {
        printf("\nData warga dengan NIK %s tidak ditemukan.\n", cariNIK);
    } else {
        printf("\nData warga ditemukan:\n");
        printf("Nama   : %s\n", data[index].nama);
        printf("NIK    : %s\n", data[index].nik);
        printf("RW     : %s\n", data[index].rw);
        printf("RT     : %s\n", data[index].rt);
        printf("Alamat : %s\n", data[index].alamat);
        printf("Saldo  : %d\n", data[index].saldo);
    }
}

void liatDataWarga() { // fungsi menu melihat semua data warga atau spesifik
    int input;
    int berjalan = 1;
    do {
        system(CLEAR);
        printf("=== MELIHAT DATA WARGA ===\n");
        printf("1) Tampilkan data semua warga\n");
        printf("2) Cari data warga\n");
        printf("0) Kembali\n");
        printf("Masukkan input anda: ");
        scanf("%d", &input);
        switch (input) {
        case 1:
            tampilkanListWarga();
            getchar();
            pause();
            break;
        case 2:
            pencarianDataNIK();
            getchar();
            pause();
            break;
        case 0:
            berjalan = 0;
            break;
        default:
            printf("Input Invalid");
            pause();
            getchar();
            break;
        }
    } while (berjalan);
}

// ================= FUNGSI EDIT DATA =================

void masukkanDataBaru(const int index) { // fungsi masukkan data baru setelah edit
    int input;
    int berjalan = 1;
    if (index == -1) {
        printf("Warga tidak ditemukan.\n");
        return;
    }

    do {
        system(CLEAR);
        printf("\nData Warga:\n");
        printf("Nama   : %s\n", data[index].nama);
        printf("NIK    : %s\n", data[index].nik);
        printf("RW     : %s\n", data[index].rw);
        printf("RT     : %s\n", data[index].rt);
        printf("Alamat : %s\n", data[index].alamat);

        printf("\nMasukkan data baru\n");
        printf("1) Nama\n");
        printf("2) NIK\n");
        printf("3) RW\n");
        printf("4) RT\n");
        printf("5) Alamat\n");
        printf("0) Selesai\n");
        printf("Data mana yang ingin diganti: ");
        scanf("%d", &input);
        switch (input) {
        case 1:
            printf("Masukkan Nama Baru: ");
            scanf(" %100[^\n]", data[index].nama);
            break;
        case 2:
            printf("Masukkan NIK Baru: ");
            scanf(" %16s", data[index].nik);
            break;
        case 3:
            printf("Masukkan RW Baru: ");
            scanf(" %2s", data[index].rw);
            break;
        case 4:
            printf("Masukkan RT Baru: ");
            scanf(" %2s", data[index].rt);
            break;
        case 5:
            printf("Masukkan Alamat Baru: ");
            scanf(" %100[^\n]", data[index].alamat);
            break;
        case 0:
            berjalan = 0;
            break;
        default:
            printf("Input Invalid");
            pause();
            getchar();
            break;
        }

    } while (berjalan);

    save();
    printf("Data warga berhasil diperbarui.\n");
}

void editDataWarga() { // menu edit data warga berdasarkan NIK atau nama
    char cariNIK[17];
    int index;
    int input;
    int berjalan = 1;

    do {
        system(CLEAR);
        printf("=== EDIT DATA WARGA ===\n");
        printf("1) Cari berdasarkan NIK\n");
        printf("2) Cari berdasarkan nama\n");
        printf("0) Kembali\n");
        printf("Masukkan input anda: ");
        scanf("%d", &input);
        switch (input) {
        case 1:
            printf("\n=== PENCARIAN DATA WARGA BERDASARKAN NIK ===\n");
            printf("Masukkan NIK yang ingin dicari: ");
            scanf(" %16s", cariNIK);
            index = cariIndexNIK(cariNIK);
            masukkanDataBaru(index);
            break;
        case 2:
            index = cariIndexNamaPilihan();
            masukkanDataBaru(index);
            break;
        case 0:
            berjalan = 0;
            break;
        default:
            printf("Input Invalid");
            pause();
            getchar();
            break;
        }
    } while (berjalan);
}

// ================= RIWAYAT TRANSAKSI =================

void tampilRiwayatByNIK(const char *filename, const char *nikCari) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Gagal membuka file riwayat: %s\n", filename);
        return;
    }

    Riwayat r;
    int found = 0;

    printf("============================================================\n");
    printf("| %-10s | %-8s | %-8s | %-12s |\n", "NIK", "Jenis", "Saldo", "Tanggal");
    printf("============================================================\n");

    // format file: nik|jenis|saldo|tanggal|
    while (fscanf(fp, "%19[^|]|%9[^|]|%d|%14[^|]|%*c",
                  r.nik, r.jenis, &r.saldo, r.tanggal) == 4) {

        if (strcmp(r.nik, nikCari) == 0) {
            printf("| %-10s | %-8s | %-8d | %-12s |\n",
                   r.nik, r.jenis, r.saldo, r.tanggal);
            found = 1;
        }
    }

    if (!found) {
        printf("| %-56s |\n", "Tidak ada riwayat untuk NIK tersebut.");
    }

    printf("============================================================\n");

    fclose(fp);
}