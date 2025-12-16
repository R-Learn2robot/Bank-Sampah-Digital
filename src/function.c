#include "../include/function.h" // Pastikan path ini benar untuk mengakses header

// Variabel Global
Warga *data = NULL;
int jlhWarga = 0;
int kapasitasWarga = 0;

// Fungsi Lain-Lain
void pause() {
    printf("\ntekan enter untuk kembali...");
    getchar();
}

void save() { // menyimpan data warga ke DATA_WARGA
    FILE *pF = fopen(DATA_WARGA, "w");
    if (!pF) {
        printf("file tidak ditemukan!");
        return;
    }
    for (int i = 0; i < jlhWarga; i++){
        fprintf(pF, "%s|%s|%s|%s|%s|%d|\n", data[i].nama, data[i].nik, data[i].rw, 
            data[i].rt, data[i].alamat, data[i].saldo);

    fclose(pF);
}

void load() { // membaca file dan memasukkannya ke array of struct warga data[]
    FILE *pF = fopen(DATA_WARGA, "r");
    if (!pF) {
        printf("file tidak ditemukan!");
        return;
    }

    char line[255];
    jlhWarga = 0;

    while (fgets(line, sizeof(line), pF)){
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|", &data[jlhWarga].nama, &data[jlhWarga].nik, &data[jlhWarga].rw, 
            &data[jlhWarga].rt, &data[jlhWarga].alamat, &data[jlhWarga].saldo);
        jlhWarga++;
    }

    fclose(pF);
}

void cekKapasistas() {
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
                        Warga temp = data[j];
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
    char nama[100];
    char nik[100];
    char rt[100];
    char rw[100];
    char alamat[100];
    int saldo;
    int count = 1;

    while (fgets(line, sizeof(line), pF)){
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|", &nama, &nik, &rw, &rt, &alamat, &saldo);
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

void pencarianDataNIK() {
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

void liatDataWarga() {
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
        printf("Saldo  : %d\n", data[index].saldo); // <-- Diubah: Tampilkan saldo

        printf("\nMasukkan data baru\n");
        printf("1) Nama\n");
        printf("2) NIK\n");
        printf("3) RW\n");
        printf("4) RT\n");
        printf("5) Alamat\n");
        // printf("6) Saldo\n"); // Opsional: Jika ingin bisa edit saldo langsung
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
        printf("0) Kembali\n");
        printf("Masukkan input anda: ");
        scanf("%d", &input);
        switch (input) {
            case 1:
                printf("\n=== PENCARIAN DATA WARGA BERDASARKAN NIK ===\n");
                printf("Masukkan NIK yang ingin dicari: ");
                scanf(" %16s", &cariNIK);
                index = cariIndexNIK(cariNIK);
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

// fitur setor sampah
void setorSampah() {
    char input_nik[17];
    int index_warga;
    char jenis_sampah[10];
    float berat_sampah;
    int hari, bulan, tahun;
    char tanggal_str[15]; // Format YYYY-MM-DD
    int saldo_per_kg = 100; // Atau tentukan logika perhitungan saldo lainnya
    int total_saldo;
    int input_menu;

    system(CLEAR);
    printf("=== SETOR SAMPAH ===\n");

    // 1. Input NIK
    printf("Masukkan NIK Anda: ");
    scanf(" %16s", input_nik);

    // 2. Cek NIK
    index_warga = cariIndexNIK(input_nik);
    if (index_warga == -1) {
        printf("NIK tidak ditemukan. Proses setor sampah dibatalkan.\n");
        pause();
        return;
    }

    printf("Warga ditemukan: %s (NIK: %s)\n", data[index_warga].nama, data[index_warga].nik);

    // Sub Menu 1: Jenis Sampah (Diubah menjadi 4 pilihan)
    printf("\nPilih Jenis Sampah:\n");
    printf("1) Plastik (Rp 4,000/kg)\n");
    printf("2) Besi (Rp 3,500/kg)\n");
    printf("3) Tembaga (Rp 70,000/kg)\n");
    printf("4) Kardus (Rp 1,800/kg)\n");
    printf("Pilihan (1-4): ");
    scanf("%d", &input_menu);

    switch (input_menu) {
        case 1:
            strcpy(jenis_sampah, "Plastik");
            harga_per_kg = 4000.0;
            break;
        case 2:
            strcpy(jenis_sampah, "Besi");
            harga_per_kg = 3500.0;
            break;
        case 3:
            strcpy(jenis_sampah, "Tembaga");
            harga_per_kg = 70000.0;
            break;
        case 4:
            strcpy(jenis_sampah, "Kardus");
            harga_per_kg = 1800.0;
            break;
        default:
            printf("Pilihan jenis tidak valid. Proses setor sampah dibatalkan.\n");
            pause();
            return;
    }

    // 3. Input Berat Sampah
    printf("\nMasukkan berat sampah (kg): ");
    scanf("%f", &berat_sampah);
    if (berat_sampah <= 0) {
        printf("Berat tidak valid. Proses setor sampah dibatalkan.\n");
        pause();
        return;
    }

    // 4. Input Tanggal
    printf("\nMasukkan tanggal setor:\n");
    printf("Hari (1-31): ");
    scanf("%d", &hari);
    printf("Bulan (1-12): ");
    scanf("%d", &bulan);
    printf("Tahun (YYYY): ");
    scanf("%d", &tahun);

    // Validasi tanggal dasar
    if (hari < 1 || hari > 31 || bulan < 1 || bulan > 12 || tahun < 1000 || tahun > 9999) {
        printf("Tanggal tidak valid. Proses setor sampah dibatalkan.\n");
        pause();
        return;
    }
    sprintf(tanggal_str, "%04d-%02d-%02d", tahun, bulan, hari);

    // 5. Hitung saldo
    total_saldo = (int)(berat_sampah * saldo_per_kg); // Pembulatan ke bawah

    // 6. Simpan ke Riwayat Transaksi
    FILE *pF_transaksi = fopen(FILE_TRANSAKSI, "a");
    if (!pF_transaksi) {
        printf("Gagal membuka file transaksi (%s) untuk ditambahkan!\n", FILE_TRANSAKSI);
        pause();
        return;
    }

    // Format: NIK|saldo|Tanggal|
    fprintf(pF_transaksi, "%s|masuk|%.2f|%s|\n",
            data[index_warga].nik,
            total_saldo,
            tanggal_str);

    fclose(pF_transaksi);

    // 7. Update saldo Warga di Array dan Simpan
    data[index_warga].saldo += total_saldo;
    save(); // Pastikan perubahan saldo disimpan ke dataWarga.txt

    printf("\n--- Konfirmasi Setoran ---\n");
    printf("Nama: %s\n", data[index_warga].nama);
    printf("NIK: %s\n", data[index_warga].nik);
    printf("Jenis: %s\n", jenis_sampah); // Hanya jenis, tanpa kondisi
    printf("Berat: %.2f kg\n", berat_sampah);
    printf("Total saldo Ditambahkan: %d\n", total_saldo);
    printf("saldo Terbaru Anda: %d\n", data[index_warga].saldo);
    printf("Tanggal: %s\n", tanggal_str);
    printf("Setoran berhasil dicatat.\n");

    pause();
}

// fitur riwayat transaksi
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
