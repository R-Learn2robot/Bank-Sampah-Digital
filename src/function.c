#include "../include/function.h"

// === VARIABEL GLOBAL ===
Warga *data = NULL;
int jlhWarga = 0;
int kapasitasWarga = 0;

// === FUNGSI UTILITIES ===
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
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|", data[jlhWarga].nama, data[jlhWarga].nik, data[jlhWarga].rw,
            data[jlhWarga].rt, data[jlhWarga].alamat, &data[jlhWarga].saldo);
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

void getTanggalHariIni(char tanggal[]) { // fungsi mengambil tanggal hari ini
    time_t sekarang = time(NULL);
    struct tm *info = localtime(&sekarang);

    sprintf(tanggal, "%02d/%02d/%04d", info->tm_mday, info->tm_mon + 1, info->tm_year + 1900);
}

void toLowerCase(char *str) { // agar string jadi lowercase
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void formatNama(char *nama) { // agar string ter format
    int setelahSpasi = 1; 

    for (int i = 0; nama[i] != '\0'; i++) {
        if (isspace((unsigned char)nama[i])) {
            setelahSpasi = 1;
        } else {
            if (setelahSpasi) {
                nama[i] = toupper((unsigned char)nama[i]);
                setelahSpasi = 0;
            } else {
                nama[i] = tolower((unsigned char)nama[i]);
            }
        }
    }
}

// === FUNGSI INPUT(blm di implementasi)x` ===
int inputInt(char *perintah) { //fungsi mengecek apakah yang dimasukkan hanya angka
    char string[100];
    int angka;
    char sisa;

    while (1) {
        printf("%s", perintah);
        if (fgets(string, sizeof(string), stdin) == NULL) {
            return -3;
        }

        if (string[0] == '\n') {
            return -1;
        }

        int hasil = sscanf(string, "%d %c", &angka, &sisa);

        if (hasil == 1) {
            return angka;
        } else {
            return -2;
        }
    }
}

void inputString(char *string, int size, const char *perintah) { //fungsi mengecek apakah yang dimasukkan hanya huruf
    int valid;

    do {
        valid = 1;
        printf("%s", perintah);

        if (fgets(string, size, stdin) == NULL) {
            printf("Gagal membaca input.\n");
            continue;
        }

        string[strcspn(string, "\n")] = '\0';

        if (strlen(string) == 0) {
            printf("Input tidak boleh kosong!\n");
            valid = 0;
            continue;
        }

        for (int i = 0; string[i] != '\0'; i++) {
            if (!isalpha((unsigned char)string[i]) && string[i] != ' ') {
                printf("Input hanya boleh berisi huruf dan spasi!\n");
                valid = 0;
                break;
            }
        }

    } while (!valid);
}

void inputNik(char *nik, int size, const char *perintah) { //fungsi mengecek apakah yang dimasukkan hanya huruf
    int valid;

    do {
        valid = 1;
        printf("%s", perintah);

        if (fgets(nik, size, stdin) == NULL) {
            printf("Gagal membaca input.\n");
            continue;
        }

        nik[strcspn(nik, "\n")] = '\0';

        if (strlen(nik) == 0) {
            printf("Input tidak boleh kosong!\n");
            valid = 0;
            continue;
        }

        for (int i = 0; nik[i] != '\0'; i++) {
            if (!isdigit((unsigned char)nik[i])) {
                printf("Input hanya boleh berisi huruf dan spasi!\n");
                valid = 0;
                break;
            }
        }

    } while (!valid);
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
    printf("=== PENCARIAN DATA WARGA BERDASARKAN NAMA ===\n");
    printf("Masukkan nama lengkap (contoh: \"ahmad rovii\"): ");
    scanf(" %100[^\n]", cariNama);
    getchar();

    if (strlen(cariNama) == 0) {
        printf("Anda tidak menginput apapun.\n");
        pause();
        return -1;
    }

    // cari semua nama yang sama
    for (i = 0; i < jlhWarga; i++) {
        if (strcmp(data[i].nama, cariNama) == 0) {
            indexKetemu[jumlahKetemu] = i;
            jumlahKetemu++;
        }
    }

    if (jumlahKetemu == 0) {
        printf("\nTidak ada data dengan nama \"%s\".\n", cariNama);
        pause();
        return -1;
    }

    // kalau cuma satu, langsung return
    if (jumlahKetemu == 1) {
        int idx = indexKetemu[0];
        printf("\nDitemukan 1 data:\n");
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
        printf("Pilih nomor data yang diinginkan (1-%d), atau 0 untuk batal: ",
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
        printf("=== MENAMBAHKAN DATA WARGA ===\n");
        inputNik(nikInput, sizeof(nikInput), "Masukkan NIK: ");
        if (cariIndexNIK(nikInput) != -1) {
            printf("\nNIK %s sudah terdaftar!\n", nikInput);
            printf("Silakan masukkan NIK lain.\n\n");
            getchar();
            pause();
        }
    } while (cariIndexNIK(nikInput) != -1);
    strcpy(data[jlhWarga].nik, nikInput);
    printf("Nama   : ");
    scanf(" %100[^\n]", data[jlhWarga].nama);
    printf("RW     : ");
    scanf(" %02s", data[jlhWarga].rw);
    printf("RT     : ");
    scanf(" %02s", data[jlhWarga].rt);
    printf("Alamat : ");
    scanf(" %100[^\n]", data[jlhWarga].alamat);
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
        printf("%d)  Nama   : %s\n", i+1, data[i].nama);
        printf("    NIK    : %s\n", data[i].nik);
        printf("    RW     : %s\n", data[i].rw);
        printf("    RT     : %s\n", data[i].rt);
        printf("    alamat : %s\n", data[i].alamat);
        printf("    Saldo  : %d\n\n", data[i].saldo);
    }
}

void pencarianDataNIK() { // fungsi cari data berdasarkan NIK
    char cariNIK[17];
    system(CLEAR);
    printf("=== PENCARIAN DATA WARGA BERDASARKAN NIK ===\n");
    printf("Masukkan NIK yang ingin dicari: ");
    scanf(" %16s", &cariNIK);

    int index = cariIndexNIK(cariNIK);

    if (index == -1) {
        printf("\nData warga dengan NIK %s tidak ditemukan.\n", cariNIK);
    } else {
        printf("\nData warga ditemukan:\n");
        printf("nama   : %s\n", data[index].nama);
        printf("NIK    : %s\n", data[index].nik);
        printf("RW     : %s\n", data[index].rw);
        printf("RT     : %s\n", data[index].rt);
        printf("alamat : %s\n", data[index].alamat);
        printf("saldo  : %d\n", data[index].saldo);
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

    printf("\nWarga ditemukan: %s (NIK: %s)\n", data[index_warga].nama, data[index_warga].nik);
    pause();

    int berjalan1 = 1;
    do
    {
        system(CLEAR);
        printf("\nPilih Jenis Sampah:\n");
        printf("1) Plastik (Rp 4,000/kg)\n");
        printf("2) Besi (Rp 3,500/kg)\n");
        printf("3) Tembaga (Rp 70,000/kg)\n");
        printf("4) Kardus (Rp 1,800/kg)\n");
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
        getchar();
        if (berat_sampah <= 0) {
            printf("Berat harus lebih dari 0.\n");
            pause();
            continue;
        } else {
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
    system(CLEAR);
    printf("\n=== Konfirmasi Setoran ===\n");
    printf("Nama                   : %s\n", data[index_warga].nama);
    printf("NIK                    : %s\n", data[index_warga].nik);
    printf("Jenis                  : %s\n", jenis_sampah);
    printf("Berat                  : %.2f kg\n", berat_sampah);
    printf("Harga                  : Rp %d/kg\n", harga_per_kg);
    printf("Total Saldo Ditambahkan: Rp %d\n", total_saldo);
    printf("Saldo Terbaru Anda     : Rp %d\n", data[index_warga].saldo);
    printf("Tanggal                : %s\n", tanggal);
    printf("\nSetoran berhasil dicatat.\n");

    pause();
}

// === FUNGSI HAPUS DATA WARGA ===
void hapusByNIK() {// fungsi hapus data warga berdasarkan NIK
    char cariNIK[17];
    char pilih;
    system(CLEAR);
    printf("\n=== PENGHAPUSAN DATA WARGA BERDASARKAN NIK ===\n");
    printf("Masukkan NIK yang ingin dihapus: ");
    scanf(" %16s", &cariNIK);
        
    int index = cariIndexNIK(cariNIK);
    
    if (index == -1) {
        printf("\nData warga dengan NIK %s tidak ditemukan.\n", cariNIK);
        return;
    }
    printf("\nData warga ditemukan:\n");
    printf("Nama   : %s\n", data[index].nama);
    printf("NIK    : %s\n", data[index].nik);
    printf("RW     : %s\n", data[index].rw);
    printf("RT     : %s\n", data[index].rt);
    printf("Alamat : %s\n", data[index].alamat);
    printf("Saldo  : %d\n", data[index].saldo);
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
        printf("=== PENGHAPUSAN DATA WARGA ===\n");
        printf("1) Hapus data berdasarkan NIK \n");
        printf("2) Hapus semua\n");
        printf("0) Kembali\n");
        printf("Masukkan input anda ");
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
        printf("\nData Warga:\n");
        printf("Nama   : %s\n", data[index].nama);
        printf("NIK    : %s\n", data[index].nik);
        printf("RW     : %s\n", data[index].rw);
        printf("RT     : %s\n", data[index].rt);
        printf("Alamat : %s\n", data[index].alamat);
        printf("Saldo  : %d\n", data[index].saldo);

        printf("\nMasukkan data baru\n");
        printf("1) Nama\n");
        printf("2) NIK\n");
        printf("3) RW\n");
        printf("4) RT\n");
        printf("5) Alamat\n");
        printf("0) Selesai\n");
        printf("Data mana yang ingin diganti: ");
        if (scanf(" %d", &input) != 1) {
            printf("Input harus berupa angka!\n");
            while (getchar() != '\n'); // bersihkan buffer
            pause();
            continue;
        }
        switch (input) {
        case 1:
            printf("Masukkan Nama Baru: ");
            scanf(" %100[^\n]", data[index].nama);
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

    sortRWRTNama();
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
        printf("=== EDIT DATA WARGA ===\n");
        printf("1) Cari berdasarkan NIK\n");
        printf("2) Cari berdasarkan nama\n");
        printf("0) Kembali\n");
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
            printf("\n=== PENCARIAN DATA WARGA BERDASARKAN NIK ===\n");
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

// === FUNGSI TUKAR SALDO ===
void tarikSaldo() {
    char cariNIK[17];
    int index;
    int saldoDiTukar;
    int temp;
    char tanggal[15];
    
    system(CLEAR);
    printf("=== PENARIKAN SALDO ===\n");
    printf("Masukkan NIK warga: ");
    scanf("%16s", &cariNIK);
    getchar();
    index = cariIndexNIK(cariNIK);
    if (index == -1) {
        printf("\nData warga dengan NIK %s tidak ditemukan.\n", cariNIK);
        pause();
        return;
    }
    int berjalan = 1;
    int input;
    do
    {
        system(CLEAR);
        printf("\nData warga ditemukan:\n");
        printf("nama   : %s\n", data[index].nama);
        printf("NIK    : %s\n", data[index].nik);
        printf("RW     : %s\n", data[index].rw);
        printf("RT     : %s\n", data[index].rt);
        printf("alamat : %s\n", data[index].alamat);
        printf("saldo  : %d\n", data[index].saldo);
        printf("\n=== PILIH ===\n");
        printf("1) Tukar saldo spesifik\n");
        printf("2) Tukar semua saldo \n");
        printf("0) Kembali \n");
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
            if (scanf("%d", &saldoDiTukar) != 1) {
                printf("Input harus berupa angka!\n");
                while (getchar() != '\n'); // bersihkan buffer
                pause();
                continue;
            }
            if (saldoDiTukar <= 0) {
                printf("\nJumlah penarikan harus lebih dari 0!\n");
                getchar();
                pause();
                continue;
            }
            temp = data[index].saldo - saldoDiTukar;
            if (temp < 0) {
                printf("\nSaldo anda tidak cukup!\n");
            } else {
                data[index].saldo = temp;
                printf("\n\n=== KONFIRMASI TUKAR SALDO ===\n");
                printf("nama               : %s\n", data[index].nama);
                printf("NIK                : %s\n", data[index].nik);
                printf("saldo yang ditukar : %d\n", saldoDiTukar);
                printf("saldo terbaru      : %d\n", data[index].saldo);
                save();
            }
            pause();
            berjalan = 0;
            break;
        case 2:
            if (data[index].saldo <= 0) {
                printf("\nSaldo tidak cukup!\n");
            } else {
                saldoDiTukar = data[index].saldo;
                data[index].saldo -= saldoDiTukar;
                printf("\n=== KONFIRMASI TUKAR SALDO ===\n");
                printf("nama               : %s\n", data[index].nama);
                printf("NIK                : %s\n", data[index].nik);
                printf("saldo yang ditukar : %d\n", saldoDiTukar);
                printf("saldo terbaru      : %d\n", data[index].saldo);
            }
            getchar();
            pause();
            berjalan = 0;
            break;
        case 0:
            return;
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

// === FUNGSI RIWAYAT TRANSAKSI ===
void tampilRiwayatByNIK() {
    FILE *pF = fopen(FILE_TRANSAKSI, "r");
    if (!pF) {
        printf("File transaksi (%s) tidak ditemukan.\n", FILE_TRANSAKSI);
        return;
    }

    //input nik
    char nikCari[17];
    printf("Masukkan NIK yang ingin dicari: ");
    scanf(" %16s", &nikCari);
    getchar();

    // Cari nama warga berdasarkan NIK yang dicari
    int idx_nama = cariIndexNIK(nikCari);
    if (idx_nama == -1) {
        printf("\nNIK %s tidak ditemukan dalam daftar warga.\n", nikCari);
        fclose(pF);
        return;
    }

    system(CLEAR);
    printf("=== RIWAYAT TRANSAKSI ===\n");
    printf("Nama : %s\n", data[idx_nama].nama);
    printf("NIK  : %s\n\n", nikCari);

    printf("==========================================\n");
    printf("| %-12s | %-10s | %-10s |\n", "Tanggal", "Jenis", "Saldo");
    printf("==========================================\n");

    char line[255];
    char nik[17];
    char jenis[10];
    int saldo;
    char tanggal[15];
    int ketemu = 0;

    while (fgets(line, sizeof(line), pF)) {
        // Format file: NIK|Jenis|Saldo|Tanggal|
        int cekFormat = sscanf(line, "%[^|]|%[^|]|%d|%[^|]\n", &nik, &jenis, &saldo, &tanggal);
        if (cekFormat == 4 && strcmp(nik, nikCari) == 0) { // Cocokkan NIK
            printf("| %-12s | %-10s | %10d |\n", tanggal, jenis, saldo);
            ketemu = 1;
        }
    }

    if (!ketemu) {
        printf("Tidak ditemukan riwayat transaksi untuk NIK %s.\n", nikCari);
    }

    fclose(pF);
    getchar();
}

int hitungTotalSaldoByNIK(const char *nikTarget) {
    FILE *fp = fopen(FILE_TRANSAKSI, "r");
    if (!fp) return 0;

    RiwayatTransaksi trx;
    int total = 0;

    while (fscanf(fp, "%[^;];%[^;];%d;%[^;\n]\n",
            trx.nik, trx.jenis, &trx.saldo, trx.tanggal) == 4) {
        if (strcmp(trx.nik, nikTarget) == 0) {
            total += trx.saldo;
        }
    }

    fclose(fp);
    return total;
}

void laporanTotalSampahPerOrang() {
    // Pastikan sudah ada data warga
    if (jlhWarga == 0) {
        printf("\n");
        printf("=================================\n");
        printf("|   BELUM ADA DATA WARGA        |\n");
        printf("=================================\n");
        return;
    }

    // Cek apakah ada transaksi
    FILE *fp = fopen(FILE_TRANSAKSI, "r");
    if (!fp) {
        printf("\n");
        printf("=================================\n");
        printf("| BELUM ADA DATA TRANSAKSI      |\n");
        printf("=================================\n");
        return;
    }
    fclose(fp);

    printf("\n");
    printf("==========================================================================\n");
    printf("|                   TOTAL SALDO SAMPAH PER ORANG                          |\n");
    printf("==========================================================================\n");
    printf("| %-3s | %-20s | %-16s | %-5s | %-5s | %-12s |\n",
           "No", "Nama", "NIK", "RW", "RT", "Total (Rp)");
    printf("--------------------------------------------------------------------------\n");

    int nomor = 1;
    for (int i = 0; i < jlhWarga; i++) {
        int totalSaldo = hitungTotalSaldoByNIK(data[i].nik);
        if (totalSaldo > 0) {
            printf("| %-3d | %-20s | %-16s | %-5s | %-5s | %-12d |\n",
                nomor,
                data[i].nama,
                data[i].nik,
                data[i].rw,
                data[i].rt,
                totalSaldo);
            nomor++;
        }
    }

    printf("==========================================================================\n");
}

void laporanTotalSampahPerRT() {
    if (jlhWarga == 0) {
        printf("\n");
        printf("=================================\n");
        printf("|   BELUM ADA DATA WARGA        |\n");
        printf("=================================\n");
        return;
    }

    FILE *fp = fopen(FILE_TRANSAKSI, "r");
    if (!fp) {
        printf("\n");
        printf("=================================\n");
        printf("| BELUM ADA DATA TRANSAKSI      |\n");
        printf("=================================\n");
        return;
    }
    fclose(fp);

    char daftarRW[200][3];
    char daftarRT[200][3];
    int  jlhRTUnik = 0;

    // Kumpulkan kombinasi RW/RT unik dari data warga
    for (int i = 0; i < jlhWarga; i++) {
        int sudahAda = 0;
        for (int k = 0; k < jlhRTUnik; k++) {
            if (strcmp(daftarRW[k], data[i].rw) == 0 &&
                strcmp(daftarRT[k], data[i].rt) == 0) {
                sudahAda = 1;
                break;
            }
        }
        if (!sudahAda) {
            strcpy(daftarRW[jlhRTUnik], data[i].rw);
            strcpy(daftarRT[jlhRTUnik], data[i].rt);
            jlhRTUnik++;
        }
    }

    printf("\n");
    printf("=========================================================\n");
    printf("|              TOTAL SALDO SAMPAH PER RT                |\n");
    printf("=========================================================\n");
    printf("| %-3s | %-5s | %-5s | %-12s |\n",
           "No", "RW", "RT", "Total (Rp)");
    printf("---------------------------------------------------------\n");

    int nomor = 1;
    for (int x = 0; x < jlhRTUnik; x++) {
        long totalRT = 0;

        // Untuk setiap warga di RW/RT ini, jumlahkan semua transaksinya
        for (int i = 0; i < jlhWarga; i++) {
            if (strcmp(data[i].rw, daftarRW[x]) == 0 &&
                strcmp(data[i].rt, daftarRT[x]) == 0) {

                totalRT += hitungTotalSaldoByNIK(data[i].nik);
            }
        }

        if (totalRT > 0) {
            printf("| %-3d | %-5s | %-5s | %-12ld |\n",
                   nomor,
                   daftarRW[x],
                   daftarRT[x],
                   totalRT);
            nomor++;
        }
    }

    printf("=========================================================\n");
}

void laporanTotalSampahPerRW() {
    if (jlhWarga == 0) {
        printf("\n");
        printf("=================================\n");
        printf("|   BELUM ADA DATA WARGA        |\n");
        printf("=================================\n");
        return;
    }

    FILE *fp = fopen(FILE_TRANSAKSI, "r");
    if (!fp) {
        printf("\n");
        printf("=================================\n");
        printf("| BELUM ADA DATA TRANSAKSI      |\n");
        printf("=================================\n");
        return;
    }
    fclose(fp);

    char daftarRW[200][3];
    int  jlhRWUnik = 0;

    // Kumpulkan RW unik
    for (int i = 0; i < jlhWarga; i++) {
        int sudahAda = 0;
        for (int k = 0; k < jlhRWUnik; k++) {
            if (strcmp(daftarRW[k], data[i].rw) == 0) {
                sudahAda = 1;
                break;
            }
        }
        if (!sudahAda) {
            strcpy(daftarRW[jlhRWUnik], data[i].rw);
            jlhRWUnik++;
        }
    }

    printf("\n");
    printf("===============================================\n");
    printf("|           TOTAL SALDO SAMPAH PER RW         |\n");
    printf("===============================================\n");
    printf("| %-3s | %-5s | %-12s |\n",
           "No", "RW", "Total (Rp)");
    printf("-----------------------------------------------\n");

    int nomor = 1;
    for (int x = 0; x < jlhRWUnik; x++) {
        long totalRW = 0;

        for (int i = 0; i < jlhWarga; i++) {
            if (strcmp(data[i].rw, daftarRW[x]) == 0) {
                totalRW += hitungTotalSaldoByNIK(data[i].nik);
            }
        }

        if (totalRW > 0) {
            printf("| %-3d | %-5s | %-12ld |\n",
                   nomor,
                   daftarRW[x],
                   totalRW);
            nomor++;
        }
    }

    printf("===============================================\n");
}

void laporanTotalSampahPerKelurahan() {
    if (jlhWarga == 0) {
        printf("\n");
        printf("=================================\n");
        printf("|   BELUM ADA DATA WARGA        |\n");
        printf("=================================\n");
        return;
    }

    FILE *fp = fopen(FILE_TRANSAKSI, "r");
    if (!fp) {
        printf("\n");
        printf("=================================\n");
        printf("| BELUM ADA DATA TRANSAKSI      |\n");
        printf("=================================\n");
        return;
    }
    fclose(fp);

    char daftarKel[200][50];
    int  jlhKelUnik = 0;

    // Kumpulkan kelurahan unik dari data warga
    for (int i = 0; i < jlhWarga; i++) {
        int sudahAda = 0;
        for (int k = 0; k < jlhKelUnik; k++) {
            if (strcmp(daftarKel[k], data[i].kelurahan) == 0) {
                sudahAda = 1;
                break;
            }
        }
        if (!sudahAda) {
            strcpy(daftarKel[jlhKelUnik], data[i].kelurahan);
            jlhKelUnik++;
        }
    }

    printf("\n");
    printf("=============================================================\n");
    printf("|         TOTAL SALDO SAMPAH PER KELURAHAN                  |\n");
    printf("=============================================================\n");
    printf("| %-3s | %-25s | %-12s |\n",
           "No", "Kelurahan", "Total (Rp)");
    printf("-------------------------------------------------------------\n");

    int nomor = 1;
    for (int x = 0; x < jlhKelUnik; x++) {
        long totalKel = 0;

        for (int i = 0; i < jlhWarga; i++) {
            if (strcmp(data[i].kelurahan, daftarKel[x]) == 0) {
                totalKel += hitungTotalSaldoByNIK(data[i].nik);
            }
        }

        if (totalKel > 0) {
            printf("| %-3d | %-25s | %-12ld |\n",
                   nomor,
                   daftarKel[x],
                   totalKel);
            nomor++;
        }
    }

    printf("=============================================================\n");
}

void menuLaporanTotalSampah() {
    int pilih;
    do {
        printf("\n");
        printf("=================================\n");
        printf("|      LAPORAN TOTAL SAMPAH     |\n");
        printf("=================================\n");
        printf("| 1) Per Orang (NIK)            |\n");
        printf("| 2) Per RT                     |\n");
        printf("| 3) Per RW                     |\n");
        printf("| 4) Per Kelurahan              |\n");
        printf("| 0) Kembali                    |\n");
        printf("=================================\n");
        printf("Masukkan pilihan anda: ");
        pilih = inputInt("");

        switch (pilih) {
            case 1: laporanTotalSampahPerOrang();      break;
            case 2: laporanTotalSampahPerRT();         break;
            case 3: laporanTotalSampahPerRW();         break;
            case 4: laporanTotalSampahPerKelurahan();  break;
            case 0: break;
            default:
                printf("Pilihan tidak valid.\n");
        }

    } while (pilih != 0);
}

