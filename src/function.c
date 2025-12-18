#include "../include/function.h"
#include <time.h>
#include <stdio.h>

// === VARIABEL GLOBAL ===
Warga *data = NULL;
int jlhWarga = 0;
int kapasitasWarga = 0;

// === FUNGSI LAIN-LAIN ===
void pause() { //fungsi agar setelah setiap fungsi selesai dia berhenti sejenak baru melakukan clear screen
    printf("\ntekan enter untuk lanjut...");
    getchar();
}

void save() { //menyimpan data warga kedalam file.txt (Nama fungsi tetap 'save' untuk kompatibilitas jika dipanggil dari main.c)
    FILE *pF = fopen(DATA_WARGA, "w");
    if (!pF){
        printf("Gagal membuka file data warga (%s) untuk ditulis!\n", DATA_WARGA);
        return;
    }
    for (int i = 0; i < jlhWarga; i++){
        fprintf(pF, "%s|%s|%s|%s|%s|%d|\n", data[i].nama, data[i].nik, data[i].rw,
            data[i].rt, data[i].alamat, data[i].saldo);

    }
    fclose(pF);
}

void load() { // membaca file dan memasukkannya ke array of struct warga data[]
    FILE *pF = fopen(DATA_WARGA, "r");
    if (!pF){
        // Jika file tidak ditemukan, anggap data kosong
        printf("File data warga (%s) tidak ditemukan. Akan dibuat jika ada data baru.\n", DATA_WARGA);
        return;
    }
    char line[255];
    jlhWarga  = 0;

    while (fgets(line, sizeof(line), pF)){
        cekKapasistas();
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|", &data[jlhWarga].nama, &data[jlhWarga].nik, &data[jlhWarga].rw,
            &data[jlhWarga].rt, &data[jlhWarga].alamat, &data[jlhWarga].saldo);
        jlhWarga++;
    }

    fclose(pF);
}

void cekKapasistas() { // fungsi mengecek apakah kapasitas array warga masih cukup
    if (jlhWarga >= kapasitasWarga) {
        kapasitasWarga = (kapasitasWarga == 0) ? 2 : kapasitasWarga * 2;
        Warga *temp = realloc(data, kapasitasWarga * sizeof(Warga));
        if (temp == NULL) {
            printf("\ngagal mengalokasikan data warga");
            exit(1);
        }
        data = temp;
    }
}

void hapusMemori() { // fungsi membersihkan memori setelah program selesai agar tidak memori leak
    free(data);
    data = NULL;
    kapasitasWarga = 0;
    jlhWarga = 0;

}

void getTanggalHariIni(char tanggal[]) {
    time_t sekarang = time(NULL);
    struct tm *info = localtime(&sekarang);

    sprintf(tanggal, "%02d/%02d/%04d", info->tm_mday, info->tm_mon + 1, info->tm_year + 1900);
}


// === FUNGSI PENCARIAN ===
int cariIndexNIK(const char *nik) { // fungsi cari index dari array berdasarkan NIK
    for (int i = 0; i < jlhWarga; i++) {
        if (strcmp(data[i].nik, nik) == 0){
            return i;
        }
    }
    return -1;
}

int cariIndexNama() { // cari index berdasarkan nama
    char cariNama[101];
    int indexKetemu[200];   
    int jumlahKetemu = 0;
    int pilihan;
    int i;

    system(CLEAR);
    printf("\n");
    printf("=====================================================\n");
    printf("|    PENCARIAN DATA WARGA BERDASARKAN NAMA          |\n");
    printf("=====================================================\n");
    printf("Masukkan nama lengkap (contoh: \"ahmad rovii\"): ");
    scanf(" %100[^\n]", cariNama);

    // buang '\n' di akhir fgets jika ada
    size_t len = strlen(cariNama);
    if (len > 0 && cariNama[len - 1] == '\n') {
        cariNama[len - 1] = '\0';
    }

    if (strlen(cariNama) == 0) {
        printf("Anda tidak menginput apapun.\n");
        pause();
        return -1;
    }

    // cari semua nama yang sama
    for (i = 0; i < jlhWarga; i++) {
        if (strstr(data[i].nama, cariNama) != NULL) {   // substring match
            indexKetemu[jumlahKetemu] = i;
            jumlahKetemu++;
        }
    }

    if (jumlahKetemu == 0) {
        printf("\nTidak ada data dengan nama yang mengandung \"%s\".\n", cariNama);
        pause();
        return -1;
    }

    // kalau cuma satu, langsung return
    if (jumlahKetemu == 1) {
        int idx = indexKetemu[0];
        printf("\n");
        printf("=========================================\n");
        printf("|            DATA DITEMUKAN             |\n");
        printf("=========================================\n");
        printf("| Index  : %-27d |\n", idx);
        printf("| Nama   : %-27s |\n", data[idx].nama);
        printf("| NIK    : %-27s |\n", data[idx].nik);
        printf("| RW/RT  : %-2s / %-2s                 |\n", data[idx].rw, data[idx].rt);
        printf("| Alamat : %-27s |\n", data[idx].alamat);
        printf("| Saldo  : %-27d |\n", data[idx].saldo);
        printf("=========================================\n");
        pause();
        return idx;
    }

    // kalau lebih dari satu, tampilkan tabel dan minta user pilih
    printf("\nDitemukan %d data yang mengandung \"%s\":\n", jumlahKetemu, cariNama);
    printf("============================================================================\n");
    printf("| No |      NIK       | Nama                           | Saldo    |\n");
    printf("============================================================================\n");
    for (i = 0; i < jumlahKetemu; i++) {
        int idx = indexKetemu[i];
        printf("| %-2d | %-13s | %-30s | %-8d |\n",
            i + 1,
            data[idx].nik,
            data[idx].nama,
            data[idx].saldo);
    }
    printf("============================================================================\n");

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
            printf("\n");
            printf("=========================================\n");
            printf("|           DATA WARGA TERPILIH         |\n");
            printf("=========================================\n");
            printf("| Nama   : %-27s |\n", data[idxTerpilih].nama);
            printf("| NIK    : %-27s |\n", data[idxTerpilih].nik);
            printf("| RW/RT  : %-2s / %-2s                 |\n", data[idxTerpilih].rw, data[idxTerpilih].rt);
            printf("| Alamat : %-27s |\n", data[idxTerpilih].alamat);
            printf("| Saldo  : %-27d |\n", data[idxTerpilih].saldo);
            printf("=========================================\n");
            pause();
            return idxTerpilih;
        }
    } while (1);
}

// === FUNGSI SORTING (BUBBLE SORT) ===
void sortRWRTNama() { // sorting data berdasarkan RW lalu RT lalu nama
    if (jlhWarga <= 1) {
        return;
    }
    Warga temp;

    for(int i = 0; i < jlhWarga - 1; i++){
        for(int j = 0; j < jlhWarga - i - 1; j++) {
            int cmpRW = strcmp(data[j].rw, data[j + 1].rw);

            if (cmpRW > 0) {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            } else if (cmpRW == 0) {
                int cmpRT = strcmp(data[j].rt, data[j + 1].rt);

                if(cmpRT > 0) {
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

// === FUNGSI TAMBAH DATA WARGA ===
void tambahWarga(){ // fungsi tambah data warga ke file.txt
    system(CLEAR);
    cekKapasistas();
    char nikInput[17];

    do {
        system(CLEAR);
        printf("\n");
        printf("=================================\n");
        printf("|     MENAMBAHKAN DATA WARGA    |\n");
        printf("=================================\n");
        printf("NIK    : ");
        scanf(" %16s", nikInput);
        if (cariIndexNIK(nikInput) != -1) {
            printf("\nNIK %s sudah terdaftar!\n", nikInput);
            printf("Silakan masukkan NIK lain.\n\n");
            getchar();
            pause();
        }
    } while (cariIndexNIK(nikInput) != -1);
    strcpy(data[jlhWarga].nik, nikInput);
    printf("Nama   : ");
    scanf(" %100[^\n]", &data[jlhWarga].nama);
    printf("RW     : ");
    scanf(" %02s", &data[jlhWarga].rw);
    printf("RT     : ");
    scanf(" %02s", &data[jlhWarga].rt);
    printf("Alamat : ");
    scanf(" %100[^\n]", &data[jlhWarga].alamat);
    data[jlhWarga].saldo = 0;
    jlhWarga++;

    sortRWRTNama();
    save();

    printf("Warga berhasil disimpan");
}

// === FUNGSI MELIHAT DATA WARGA ===
void tampilkanListWarga() { // fungsi tampilkan list data dari file
    if (jlhWarga == 0) {
        system(CLEAR);
        printf("belum ada warga yang terdaftar");
        return;
    }

    system(CLEAR);
    printf("\n===LIST WARGA===\n");

    for (int i = 0; i < jlhWarga; i++) {
        printf("=========================================\n");
        printf("| Data Warga ke-%-2d                      |\n", i + 1);
        printf("=========================================\n");
        printf("| Nama   : %-27s |\n", data[i].nama);
        printf("| NIK    : %-27s |\n", data[i].nik);
        printf("| RW/RT  : %-2s / %-2s                 |\n", data[i].rw, data[i].rt);
        printf("| Alamat : %-27s |\n", data[i].alamat);
        printf("| Saldo  : %-27d |\n", data[i].saldo);
        printf("=========================================\n\n");
    }
}

void pencarianDataNIK() { // fungsi cari data berdasarkan NIK
    char cariNIK[17];
    system(CLEAR);
    printf("\n");
    printf("===============================================\n");
    printf("|   PENCARIAN DATA WARGA BERDASARKAN NIK      |\n");
    printf("===============================================\n");
    printf("Masukkan NIK yang ingin dicari: ");
    scanf(" %16s", &cariNIK);

    int index = cariIndexNIK(cariNIK);

    if (index == -1) {
        printf("\nData warga dengan NIK %s tidak ditemukan.\n", cariNIK);
    } else {
        printf("\n");
        printf("=========================================\n");
        printf("|          DATA WARGA DITEMUKAN         |\n");
        printf("=========================================\n");
        printf("| Nama   : %-27s |\n", data[index].nama);
        printf("| NIK    : %-27s |\n", data[index].nik);
        printf("| RW/RT  : %-2s / %-2s                 |\n", data[index].rw, data[index].rt);
        printf("| Alamat : %-27s |\n", data[index].alamat);
        printf("| Saldo  : %-27d |\n", data[index].saldo);
        printf("=========================================\n");
    }
}

void liatDataWarga() { // fungsi menu melihat semua data warga atau spesifik
    int input;
    int berjalan = 1;
    do {
        system(CLEAR);
        printf("\n");
        printf("=================================\n");
        printf("|        MELIHAT DATA WARGA     |\n");
        printf("=================================\n");
        printf("| 1) Tampilkan semua data warga |\n");
        printf("| 2) Cari data warga            |\n");
        printf("| 0) Kembali                    |\n");
        printf("=================================\n");
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

// === FUNGSI SETOR SAMPAH ===
void setorSampah() {
    char input_nik[17];
    int index_warga;
    char jenis_sampah[10];
    float berat_sampah;
    char tanggal[15];
    int harga_per_kg;
    int total_saldo;
    int input_menu;

    system(CLEAR);
    printf("=== SETOR SAMPAH ===\n");

    // 1. Input NIK
    printf("Masukkan NIK Anda: ");
    scanf(" %16s", input_nik);
    getchar();

    // 2. Cek NIK
    index_warga = cariIndexNIK(input_nik);
    if (index_warga == -1) {
        printf("\nNIK tidak ditemukan. Proses setor sampah dibatalkan.\n");
        pause();
        return;
    }

    printf("Warga ditemukan: %s (NIK: %s)\n", data[index_warga].nama, data[index_warga].nik);
    pause();

    int berjalan1 = 1;
    do
    {
        system(CLEAR);
        printf("\n");
        printf("=====================================\n");
        printf("|         PILIH JENIS SAMPAH        |\n");
        printf("=====================================\n");
        printf("| 1) Plastik  (Rp 4,000/kg)         |\n");
        printf("| 2) Besi     (Rp 3,500/kg)         |\n");
        printf("| 3) Tembaga  (Rp 70,000/kg)        |\n");
        printf("| 4) Kardus   (Rp 1,800/kg)         |\n");
        printf("=====================================\n");
        printf("Pilihan (1-4): ");
        if (scanf("%d", &input_menu) != 1) {
            printf("Input harus berupa angka!\n");
            while (getchar() != '\n'); // bersihkan buffer
            pause();
            continue;
        }
        switch (input_menu) {
            case 1:
                strcpy(jenis_sampah, "Plastik");
                harga_per_kg = 4000;
                berjalan1 = 0;
                break;
            case 2:
                strcpy(jenis_sampah, "Besi");
                harga_per_kg = 3500;
                berjalan1 = 0;
                break;
            case 3:
                strcpy(jenis_sampah, "Tembaga");
                harga_per_kg = 70000;
                berjalan1 = 0;
                break;
            case 4:
                strcpy(jenis_sampah, "Kardus");
                harga_per_kg = 1800;
                berjalan1 = 0;
                break;
            default:
                printf("Pilihan harus 1-4.\n");
                pause();
                break;
        }
    } while (berjalan1);
    

    // 3. Input Berat Sampah
    int berjalan2 = 1;
    do
    {
        system(CLEAR);
        printf("\nMasukkan berat sampah (kg): ");
        if (scanf("%f", &berat_sampah) != 1) {
            printf("Input harus berupa angka!\n");
            while (getchar() != '\n'); // bersihkan buffer
            pause();
            continue;
        }
        if (berat_sampah <= 0) {
            printf("Berat tidak valid.\n");
            pause();
            continue;
        } else {
            printf("Data berhasil disimpan");
            pause();
            berjalan2 = 0;
        }
    } while (berjalan2);

    getTanggalHariIni(tanggal);

    // 5. Hitung Saldo
    total_saldo = berat_sampah * harga_per_kg;

    // 6. Update Saldo Warga di Array dan Simpan
    data[index_warga].saldo += total_saldo;
    save(); 

    // 7. Tambahkan ke File Riwayat
    FILE *pF_transaksi = fopen(FILE_TRANSAKSI, "a");
    if (!pF_transaksi) {
        printf("Gagal membuka file transaksi (%s) untuk ditambahkan!\n", FILE_TRANSAKSI);
        pause();
        return;
    }

    // Format file: NIK|Jenis|Saldo|Tanggal|
    fprintf(pF_transaksi, "%s|masuk|%d|%s|\n",
            data[index_warga].nik,
            total_saldo,
            tanggal);

    fclose(pF_transaksi);

    printf("\n");
    printf("===============================================\n");
    printf("|            KONFIRMASI SETORAN               |\n");
    printf("===============================================\n");
    printf("| Nama                    : %-18s |\n", data[index_warga].nama);
    printf("| NIK                     : %-18s |\n", data[index_warga].nik);
    printf("| Jenis                   : %-18s |\n", jenis_sampah);
    printf("| Berat                   : %10.2f kg |\n", berat_sampah);
    printf("| Harga                   : Rp %-10d |\n", harga_per_kg);
    printf("| Total Saldo Ditambahkan : Rp %-10d |\n", total_saldo);
    printf("| Saldo Terbaru Anda      : Rp %-10d |\n", data[index_warga].saldo);
    printf("| Tanggal                 : %-18s |\n", tanggal);
    printf("===============================================\n");
    printf("Setoran berhasil dicatat.\n");

    pause();
}

// === FUNGSI HAPUS DATA WARGA ===
void hapusByNIK() {// fungsi hapus data warga berdasarkan NIK
    char cariNIK[17];
    char pilih;
    system(CLEAR);
    printf("\n");
    printf("===============================================\n");
    printf("|   PENGHAPUSAN DATA WARGA BERDASARKAN NIK    |\n");
    printf("===============================================\n");
    printf("Masukkan NIK yang ingin dihapus: ");
    scanf(" %16s", &cariNIK);
        
    int index = cariIndexNIK(cariNIK);
    
    if (index == -1) {
        printf("\nData warga dengan NIK %s tidak ditemukan.\n", cariNIK);
        return;
    }
    printf("\n");
    printf("=========================================\n");
    printf("|          KONFIRMASI DATA WARGA        |\n");
    printf("=========================================\n");
    printf("| Nama   : %-27s |\n", data[index].nama);
    printf("| NIK    : %-27s |\n", data[index].nik);
    printf("| RW/RT  : %-2s / %-2s                 |\n", data[index].rw, data[index].rt);
    printf("| Alamat : %-27s |\n", data[index].alamat);
    printf("| Saldo  : %-27d |\n", data[index].saldo);
    printf("=========================================\n");
    printf("Anda yakin hapus data warga ini (y/n): ");
    scanf(" %c", &pilih);
    if (pilih == 'y' || pilih == 'Y') {
        for (int i = index; i < jlhWarga - 1; i++) {
            data[i] = data[i + 1];
        }
        jlhWarga--;
        save();
        printf("data warga telah dihapus\n");
    } else {
        printf("Penghapusan dibatalkan\n");
    }
}

void hapusSemua() {// fungsi hapus semua data
    char konfirmasi;
    printf("Anda yakin ingin menghapus semua data (y/n): ");
    scanf(" %c", &konfirmasi);
    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        hapusMemori();
        FILE *pF = fopen(DATA_WARGA, "w");
        fclose(pF);
    } else {
        printf("Penghapusan dibatalkan\n");
    }
}

void hapusDataWarga() {// fungsi menu hapus data warga
    int input;
    int berjalan = 1;
    do
    {
        system(CLEAR);
        printf("\n");
        printf("=================================\n");
        printf("|     PENGHAPUSAN DATA WARGA    |\n");
        printf("=================================\n");
        printf("| 1) Hapus data berdasarkan NIK |\n");
        printf("| 2) Hapus semua                |\n");
        printf("| 0) Kembali                    |\n");
        printf("=================================\n");
        printf("Masukkan input anda: ");
        if (scanf("%d", &input) != 1) {
            printf("Input harus berupa angka!\n");
            while (getchar() != '\n'); // bersihkan buffer
            pause();
            continue;
        }
        switch (input) {
        case 1:
            hapusByNIK();
            getchar();
            pause();
            break;
        case 2:
            hapusSemua();
            getchar();
            pause();
            break;
        case 0:
            berjalan = 0;
            break;
        default:
            printf("Input harus 1 atau 2!");
            pause();
            getchar();
            break;
        }
    } while (berjalan);
}

// === FUNGSI EDIT DATA WARGA ===
void masukkanDataBaru(const int index) { // fungsi masukkan data baru setelah di edit
    int input;
    int berjalan = 1;
    char nikInput[17];
    if (index == -1) {
        printf("Data warga tidak ditemukan.\n");
        return;
    }

    do {
        system(CLEAR);
        printf("\n");
        printf("=========================================\n");
        printf("|              DATA WARGA               |\n");
        printf("=========================================\n");
        printf("| Nama   : %-27s |\n", data[index].nama);
        printf("| NIK    : %-27s |\n", data[index].nik);
        printf("| RW/RT  : %-2s / %-2s                 |\n", data[index].rw, data[index].rt);
        printf("| Alamat : %-27s |\n", data[index].alamat);
        printf("| Saldo  : %-27d |\n", data[index].saldo);
        printf("=========================================\n");
        printf("\n");

        printf("=================================\n");
        printf("|        MASUKKAN DATA BARU     |\n");
        printf("=================================\n");
        printf("| 1) Nama                       |\n");
        printf("| 2) NIK                        |\n");
        printf("| 3) RW                         |\n");
        printf("| 4) RT                         |\n");
        printf("| 5) Alamat                     |\n");
        printf("| 0) Selesai                    |\n");
        printf("=================================\n");
        printf("Data mana yang ingin diganti: ");
        if (scanf("%d", &input) != 1) {
            printf("Input harus berupa angka!\n");
            while (getchar() != '\n'); // bersihkan buffer
            pause();
            continue;
        }
        switch (input) {
        case 1:
            printf("Masukkan Nama Baru: ");
            scanf(" %100[^\n]", &data[index].nama);
            break;
        case 2:
            printf("Masukkan NIK Baru: ");
            scanf(" %16s", &nikInput);
            if (cariIndexNIK(nikInput) != -1) {
                printf("\nNIK %s sudah terdaftar!\n", nikInput);
                printf("Silakan masukkan NIK lain.\n\n");
                getchar();
                pause();
                continue;
            } else {
                strcpy(data[index].nik, nikInput);
            }
            break;
        case 3:
            printf("Masukkan RW Baru: ");
            scanf(" %2s", &data[index].rw);
            break;
        case 4:
            printf("Masukkan RT Baru: ");
            scanf(" %2s", &data[index].rt);
            break;
        case 5:
            printf("Masukkan Alamat Baru: ");
            scanf(" %100[^\n]", &data[index].alamat);
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

void editDataWarga() { // fungsi menu edit warga
    char cariNIK[17];
    int index;
    int input;
    int berjalan = 1;

    do {
        system(CLEAR);
        printf("\n");
        printf("=================================\n");
        printf("|         EDIT DATA WARGA       |\n");
        printf("=================================\n");
        printf("| 1) Cari berdasarkan NIK       |\n");
        printf("| 2) Cari berdasarkan nama      |\n");
        printf("| 0) Kembali                    |\n");
        printf("=================================\n");
        printf("Masukkan input anda: ");
        if (scanf("%d", &input) != 1) {
            printf("Input harus berupa angka!\n");
            while (getchar() != '\n'); // bersihkan buffer
            pause();
            continue;
        }
        switch (input) {
        case 1:
        system(CLEAR);
            printf("===============================================\n");
            printf("|     PENCARIAN DATA WARGA BERDASARKAN NIK    |\n");
            printf("===============================================\n");
            printf("Masukkan NIK yang ingin dicari: ");
                scanf(" %16s", cariNIK);
            index = cariIndexNIK(cariNIK);
            masukkanDataBaru(index);
            break;
        case 2:
            index = cariIndexNama();
            masukkanDataBaru(index);
            break;
        case 0:
            berjalan = 0;
            break;
        default:
            printf("Input harus 1 atau 2!");
            pause();
            break;
        }
    } while (berjalan);
}

void tarikSaldo() {
    char cariNIK[17];
    int index;
    int saldoDiTukar;
    int temp;
    int hari, bulan, tahun;
    char tanggal[15];
    
    system(CLEAR);
    printf("=========================================\n");
    printf("|            PENARIKAN SALDO            |\n");
    printf("=========================================\n");
printf("Masukkan NIK warga: ");
    scanf("%16s", &cariNIK);
    index = cariIndexNIK(cariNIK);
    if (index == -1) {
        printf("\nData warga dengan NIK %s tidak ditemukan.\n", cariNIK);
    } 
    pause();
    int berjalan = 1;
    int input;
    do
    {
        system(CLEAR);
        printf("\n");
        printf("=========================================\n");
        printf("|            DATA WARGA DITEMUKAN       |\n");
        printf("=========================================\n");
        printf("Nama   : %s\n", data[index].nama);
        printf("NIK    : %s\n", data[index].nik);
        printf("RW/RT  : %s/%s\n", data[index].rw, data[index].rt);
        printf("Alamat : %s\n", data[index].alamat);
        printf("Saldo  : %d\n", data[index].saldo);
        printf("=========================================\n");

        printf("\n");
        printf("============== PILIHAN TUKAR ============\n");
        printf("1) Tukar saldo spesifik\n");
        printf("2) Tukar semua saldo\n");
        printf("=========================================\n");
printf("Input anda: ");
        if (scanf("%d", &input) != 1) {
            printf("Input harus berupa angka!\n");
            while (getchar() != '\n'); // bersihkan buffer
            pause();
            continue;
        }
        switch (input) {
        case 1:
            printf("\nMasukkan jumlah saldo yang diinginkan: ");
            scanf(" %d", &saldoDiTukar);
            getchar();
            temp = data[index].saldo - saldoDiTukar;
            if (temp < 0) {
                printf("\nSaldo anda tidak cukup!\n");
            } else {
                data[index].saldo = temp;
                printf("\n");
                printf("=========================================\n");
                printf("|        KONFIRMASI TUKAR SALDO        |\n");
                printf("=========================================\n");
                printf("Nama               : %s\n", data[index].nama);
                printf("NIK                : %s\n", data[index].nik);
                printf("Saldo yang ditukar : %d\n", saldoDiTukar);
                printf("Saldo terbaru      : %d\n", data[index].saldo);
                printf("=========================================\n");
            }
            pause();
            berjalan = 0;
            break;
        case 2:
            if (data[index].saldo == 0) {
                printf("\nSaldo tidak cukup!\n");
            } else {
                saldoDiTukar = data[index].saldo;
                data[index].saldo -= saldoDiTukar;
                printf("\n=========================================\n");
                printf("|        KONFIRMASI TUKAR SALDO        |\n");
                printf("=========================================\n");
                printf("Nama               : %s\n", data[index].nama);
                printf("NIK                : %s\n", data[index].nik);
                printf("Saldo yang ditukar : %d\n", saldoDiTukar);
                printf("Saldo terbaru      : %d\n", data[index].saldo);
                printf("=========================================\n");

            }
            getchar();
            pause();
            berjalan = 0;
            break;
        default:
            printf("\nInput harus 1 atau 2!\n");
            pause();
            break;
        }
    } while (berjalan);

    getTanggalHariIni(tanggal);

    FILE *pF_transaksi = fopen(FILE_TRANSAKSI, "a");
    if (!pF_transaksi) {
        printf("Gagal membuka file transaksi (%s) untuk ditambahkan!\n", FILE_TRANSAKSI);
        pause();
        return;
    }

    fprintf(pF_transaksi, "%s|keluar|%d|%s|\n", data[index].nik, saldoDiTukar, tanggal);

    fclose(pF_transaksi);
}

int cariIndexWargaByNIK(const char *nik) {
    for (int i = 0; i < jlhWarga; i++) {
        if (strcmp(data[i].nik, nik) == 0) {
            return i;
        }
    }
    return -1;
}

void tampilRiwayatByNIK() {
    char nikCari[20];

    printf("Masukkan NIK yang ingin dicari: ");
    scanf("%19s", nikCari);

    FILE *pF = fopen(FILE_TRANSAKSI, "r");
    if (!pF) {
        printf("File transaksi (%s) tidak ditemukan.\n", FILE_TRANSAKSI);
        pause();
        return;
    }

    int idx_nama = cariIndexWargaByNIK(nikCari);

    system(CLEAR);
    printf("=== RIWAYAT TRANSAKSI ===\n");
    if (idx_nama != -1) {
        printf("Nama : %s\n", data[idx_nama].nama);
    }
    printf("NIK  : %s\n\n", nikCari);

    printf("==========================================\n");
    printf("| %-12s | %-10s | %-10s |\n", "Tanggal", "Jenis", "Saldo");
    printf("==========================================\n");

    char line[255];
    char nik[20];
    char jenis[10];
    int saldo;
    char tanggal[15];
    int ketemu = 0;

    while (fgets(line, sizeof(line), pF)) {
        // Format file: NIK|Jenis|Saldo|Tanggal|
        int cekFormat = sscanf(
            line,
            "%19[^|]|%9[^|]|%d|%14[^|]|",
            nik, jenis, &saldo, tanggal
        );

        if (cekFormat == 4 && strcmp(nik, nikCari) == 0) {
            printf("| %-12s | %-10s | %10d |\n", tanggal, jenis, saldo);
            ketemu = 1;
        }
    }

    if (!ketemu) {
        printf("Tidak ditemukan riwayat transaksi untuk NIK %s.\n", nikCari);
    }

    printf("==========================================\n");

    fclose(pF);
    pause();
}


void tampilRiwayatByNIK() {
    char nikCari[20];
    printf("Masukkan NIK: ");
    scanf("%19s", nikCari);

    // cari index warga berdasarkan NIK (kalau mau tampil nama)
    int idx_nama = cariIndexWargaByNIK(nikCari);

    FILE *pF = fopen(FILE_TRANSAKSI, "r");
    if (!pF) {
        printf("Gagal membuka file transaksi: %s\n", FILE_TRANSAKSI);
        pause();
        return;
    }

    system(CLEAR);
    printf("=== RIWAYAT TRANSAKSI ===\n");
    if (idx_nama != -1) {
        printf("Nama : %s\n", data[idx_nama].nama);
    }
    printf("NIK  : %s\n\n", nikCari);

    printf("==========================================\n");
    printf("| %-12s | %-10s | %-10s |\n", "Tanggal", "Jenis", "Saldo");
    printf("==========================================\n");

    char line[255];
    char nik[20];
    char jenis[10];
    int saldo;
    char tanggal[15];
    int ketemu = 0;

    while (fgets(line, sizeof(line), pF)) {
        // Format file: NIK|Jenis|Saldo|Tanggal|
        int cekFormat = sscanf(
            line,
            "%19[^|]|%9[^|]|%d|%14[^|]|",
            nik, jenis, &saldo, tanggal
        );

        if (cekFormat == 4 && strcmp(nik, nikCari) == 0) { // Cocokkan NIK
            printf("| %-12s | %-10s | %10d |\n", tanggal, jenis, saldo);
            ketemu = 1;
        }
    }

    if (!ketemu) {
        printf("Tidak ditemukan riwayat transaksi untuk NIK %s.\n", nikCari);
    }

    printf("==========================================\n");

    fclose(pF);
    getchar();  // kalau mau "pause" manual
    pause();    // kalau kamu memang punya fungsi pause()
}
    void tampilTotalSampahPerOrang() {
    char nikCari[20];
    system(CLEAR);
    
    printf("====================================================\n");
    printf("|        TOTAL SAMPAH PER ORANG (KG)               |\n");
    printf("====================================================\n");
    scanf("%19s", nikCari);

    FILE *fp = fopen(FILE_TRANSAKSI, "r");
    if (!fp) {
        printf("Gagal membuka file transaksi: %s\n", FILE_TRANSAKSI);
        pause();
        return;
    }

    char nik[20], jenis[10], tanggal[15];
    int saldoTrans;
    double totalSaldoMasuk = 0;
    double totalSaldoKeluar = 0;

    // format: nik|jenis|saldo|tanggal|
    while (fscanf(fp, "%19[^|]|%9[^|]|%d|%14[^|]|%*c",
                  nik, jenis, &saldoTrans, tanggal) == 4) {

        if (strcmp(nik, nikCari) == 0) {
            if (strcmp(jenis, "masuk") == 0) {
                totalSaldoMasuk += saldoTrans;
            } else if (strcmp(jenis, "keluar") == 0) {
                totalSaldoKeluar += saldoTrans;
            }
        }
    }
    fclose(fp);

    double totalSaldoBersih = totalSaldoMasuk - totalSaldoKeluar;
    double totalKgMasuk  = totalSaldoMasuk  / (double)SALDO_PER_KG;
    double totalKgKeluar = totalSaldoKeluar / (double)SALDO_PER_KG;
    double totalKgBersih = totalSaldoBersih / (double)SALDO_PER_KG;

    int idx = cariIndexWargaByNIK(nikCari);

        printf("\n");
        printf("====================================================\n");
        printf("|        LAPORAN TOTAL SAMPAH PER ORANG (KG)       |\n");
        printf("====================================================\n");
        printf("| NIK   : %-40s |\n", nikCari);

    if (idx != -1) {
        printf("| Nama  : %-40s |\n", data[idx].nama);
        printf("| RW/RT : %-2s / %-2s                               |\n", data[idx].rw, data[idx].rt);
    } else {
        printf("| Nama  : %-40s |\n", "(tidak ditemukan di dataWarga)");
        printf("| RW/RT : %-40s |\n", "-");
    }

    printf("====================================================\n");
    printf("| %-12s | %-12s | %-12s |\n", "Jenis", "Kg", "Saldo");
    printf("====================================================\n");
    printf("| %-12s | %10.2f | %10.0f |\n", "Masuk",  totalKgMasuk,  totalSaldoMasuk);
    printf("| %-12s | %10.2f | %10.0f |\n", "Keluar", totalKgKeluar, totalSaldoKeluar);
    printf("| %-12s | %10.2f | %10.0f |\n", "Bersih", totalKgBersih, totalSaldoBersih);
    printf("====================================================\n");

    pause();
}

void tampilTotalSampahPerRT() {
    char rwCari[3], rtCari[3];
    system(CLEAR);

    printf("=============================================\n");
    printf("|          TOTAL SAMPAH PER RT (KG)         |\n");
    printf("=============================================\n");
    printf("Masukkan RW: ");
    scanf(" %2s", rwCari);
    printf("Masukkan RT: ");
    scanf(" %2s", rtCari);


    scanf(" %2s", rtCari);

    FILE *fp = fopen(FILE_TRANSAKSI, "r");
    if (!fp) {
        printf("Gagal membuka file transaksi: %s\n", FILE_TRANSAKSI);
        pause();
        return;
    }

    char nik[20], jenis[10], tanggal[15];
    int saldoTrans;
    double totalSaldoMasuk = 0;
    double totalSaldoKeluar = 0;

    while (fscanf(fp, "%19[^|]|%9[^|]|%d|%14[^|]|%*c",
                  nik, jenis, &saldoTrans, tanggal) == 4) {

        int idx = cariIndexWargaByNIK(nik);
        if (idx == -1) continue;

        // filter RT/RW
        if (strcmp(data[idx].rw, rwCari) != 0 || strcmp(data[idx].rt, rtCari) != 0) {
            continue;
        }

        if (strcmp(jenis, "masuk") == 0) {
            totalSaldoMasuk += saldoTrans;
        } else if (strcmp(jenis, "keluar") == 0) {
            totalSaldoKeluar += saldoTrans;
        }
    }
    fclose(fp);

    double totalSaldoBersih = totalSaldoMasuk - totalSaldoKeluar;
    double totalKgMasuk  = totalSaldoMasuk  / (double)SALDO_PER_KG;
    double totalKgKeluar = totalSaldoKeluar / (double)SALDO_PER_KG;
    double totalKgBersih = totalSaldoBersih / (double)SALDO_PER_KG;

    printf("\n");
    printf("=========================================================\n");
    printf("|   LAPORAN TOTAL SAMPAH RW %-2s / RT %-2s (KG)        |\n", rwCari, rtCari);
    printf("=========================================================\n");
    printf("| %-12s | %-12s | %-12s |\n", "Jenis", "Kg", "Saldo");
    printf("=========================================================\n");
    printf("| %-12s | %10.2f | %10.0f |\n", "Masuk",  totalKgMasuk,  totalSaldoMasuk);
    printf("| %-12s | %10.2f | %10.0f |\n", "Keluar", totalKgKeluar, totalSaldoKeluar);
    printf("| %-12s | %10.2f | %10.0f |\n", "Bersih", totalKgBersih, totalSaldoBersih);
    printf("=========================================================\n");
    pause();
}

void tampilTotalSampahPerRW() {
    char rwCari[3];
    system(CLEAR);
    printf("=============================================\n");
    printf("|          TOTAL SAMPAH PER RW (KG)         |\n");
    printf("=============================================\n");
    printf("Masukkan RW: ");
    scanf(" %2s", rwCari);

    FILE *fp = fopen(FILE_TRANSAKSI, "r");
    if (!fp) {
        printf("Gagal membuka file transaksi: %s\n", FILE_TRANSAKSI);
        pause();
        return;
    }

    char nik[20], jenis[10], tanggal[15];
    int saldoTrans;
    double totalSaldoMasuk = 0;
    double totalSaldoKeluar = 0;

    while (fscanf(fp, "%19[^|]|%9[^|]|%d|%14[^|]|%*c",
                  nik, jenis, &saldoTrans, tanggal) == 4) {

        int idx = cariIndexWargaByNIK(nik);
        if (idx == -1) continue;

        if (strcmp(data[idx].rw, rwCari) != 0) {
            continue;
        }

        if (strcmp(jenis, "masuk") == 0) {
            totalSaldoMasuk += saldoTrans;
        } else if (strcmp(jenis, "keluar") == 0) {
            totalSaldoKeluar += saldoTrans;
        }
    }
    fclose(fp);

    double totalSaldoBersih = totalSaldoMasuk - totalSaldoKeluar;
    double totalKgMasuk  = totalSaldoMasuk  / (double)SALDO_PER_KG;
    double totalKgKeluar = totalSaldoKeluar / (double)SALDO_PER_KG;
    double totalKgBersih = totalSaldoBersih / (double)SALDO_PER_KG;

    printf("=============================================\n");
    printf("|      LAPORAN TOTAL SAMPAH RW %-6s       |\n", rwCari);
    printf("=============================================\n");
    printf("| %-12s | %-12s | %-12s |\n", "Jenis", "Kg", "Saldo");
    printf("=============================================\n");
    printf("| %-12s | %10.2f | %10.0f |\n", "Masuk",  totalKgMasuk,  totalSaldoMasuk);
    printf("| %-12s | %10.2f | %10.0f |\n", "Keluar", totalKgKeluar, totalSaldoKeluar);
    printf("| %-12s | %10.2f | %10.0f |\n", "Bersih", totalKgBersih, totalSaldoBersih);
    printf("=============================================\n");

    pause();
}

void tampilTotalSampahKelurahan() {
    system(CLEAR);
    printf("=== TOTAL SAMPAH SELURUH KELURAHAN (KG) ===\n");

    FILE *fp = fopen(FILE_TRANSAKSI, "r");
    if (!fp) {
        printf("Gagal membuka file transaksi: %s\n", FILE_TRANSAKSI);
        pause();
        return;
    }

    char nik[20], jenis[10], tanggal[15];
    int saldoTrans;
    double totalSaldoMasuk = 0;
    double totalSaldoKeluar = 0;

    while (fscanf(fp, "%19[^|]|%9[^|]|%d|%14[^|]|%*c",
                  nik, jenis, &saldoTrans, tanggal) == 4) {

        if (strcmp(jenis, "masuk") == 0) {
            totalSaldoMasuk += saldoTrans;
        } else if (strcmp(jenis, "keluar") == 0) {
            totalSaldoKeluar += saldoTrans;
        }
    }
    fclose(fp);

    double totalSaldoBersih = totalSaldoMasuk - totalSaldoKeluar;
    double totalKgMasuk  = totalSaldoMasuk  / (double)SALDO_PER_KG;
    double totalKgKeluar = totalSaldoKeluar / (double)SALDO_PER_KG;
    double totalKgBersih = totalSaldoBersih / (double)SALDO_PER_KG;

    printf("\n--- Laporan Total Sampah Kelurahan ---\n");
    printf("Total masuk : %.2f kg (saldo %.0f)\n", totalKgMasuk, totalSaldoMasuk);
    printf("Total keluar: %.2f kg (saldo %.0f)\n", totalKgKeluar, totalSaldoKeluar);
    printf("Total bersih: %.2f kg (saldo %.0f)\n", totalKgBersih, totalSaldoBersih);

    pause();
}

void menuLaporanSampah() {
    int input;
    int jalan = 1;

    do {
            system(CLEAR);
            printf("=============================================\n");
            printf("|        LAPORAN TOTAL SAMPAH (KG)          |\n");
            printf("=============================================\n");
            printf("| 1) Total per orang (NIK)                  |\n");
            printf("| 2) Total per RT                           |\n");
            printf("| 3) Total per RW                           |\n");
            printf("| 4) Total seluruh kelurahan                |\n");
            printf("| 0) Kembali                                |\n");
            printf("=============================================\n");
            printf("Masukkan pilihan: ");
            scanf("%d", &input);

        switch (input) {
        case 1:
            tampilTotalSampahPerOrang();
            break;
        case 2:
            tampilTotalSampahPerRT();
            break;
        case 3:
            tampilTotalSampahPerRW();
            break;
        case 4:
            tampilTotalSampahKelurahan();
            break;
        case 0:
            jalan = 0;
            break;
        default:
            printf("Input tidak valid.\n");
            pause();
            break;
        }
    } while (jalan);
}



