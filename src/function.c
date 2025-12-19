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
        fprintf(pF, "%s|%s|%s|%s|%s|%d|%d|\n", data[i].nama, data[i].nik, data[i].rw,
            data[i].rt, data[i].alamat, data[i].saldo, data[i].totalSampahKg);

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
            continue;
        }

        if (string[0] == '\n') {
            printf("\nInput tidak boleh kosong!\n");
            continue;
        }

        int hasil = sscanf(string, "%d %c", &angka, &sisa);

        if (hasil == 1) {
            return angka;
        } else {
            printf("\nInput hanya boleh angka!\n");
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

        if (valid) {
            toLowerCase(string);
        }

    } while (!valid);
}

void inputAngkaString(char *nik, int size, const char *perintah) { //fungsi mengecek apakah yang dimasukkan hanya huruf
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
                printf("Input hanya boleh berisi angka!\n");
                valid = 0;
                break;
            }
        }

    } while (!valid);
}

void inputAlamat(char *string, int size, const char *perintah) { //fungsi mengecek apakah yang dimasukkan hanya huruf
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

        if (valid) {
            toLowerCase(string);
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
    inputString(cariNama, sizeof(cariNama), "Masukkan nama lengkap: ");
    formatNama(cariNama);
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
        printf("\n+===============================================+\n");
        printf("|               DITEMUKAN 1 DATA               |\n");
        printf("+----------------------+-----------------------+\n");
        printf("| %-20s | %-21s |\n", "Nama",   cariNama);
        printf("| %-20s | %-21s |\n", "NIK",    data[idx].nik);
        printf("| %-20s | %-21s |\n", "RW",     data[idx].rw);
        printf("| %-20s | %-21s |\n", "RT",     data[idx].rt);
        printf("| %-20s | %-21s |\n", "Alamat", data[idx].alamat);
        printf("| %-20s | %-21d |\n", "Saldo",  data[idx].saldo);
        printf("+----------------------+-----------------------+\n");
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
        char namaKetemu [101];
        strcpy(namaKetemu, data[idx].nik);
        formatNama(namaKetemu);
        printf("| %-2d | %-13s | %-30s | %-8d |\n",
            i + 1,
            data[idx].nik,
            namaKetemu,
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
            printf("+===============================================+\n");
            printf("|              ANDA MEMILIH DATA                |\n");
            printf("+----------------------+-----------------------+\n");
            printf("| %-20s | %-21s |\n", "Nama",   data[idxTerpilih].nama);
            printf("| %-20s | %-21s |\n", "NIK",    data[idxTerpilih].nik);
            printf("| %-20s | %-21s |\n", "RW",     data[idxTerpilih].rw);
            printf("| %-20s | %-21s |\n", "RT",     data[idxTerpilih].rt);
            printf("| %-20s | %-21s |\n", "Alamat", data[idxTerpilih].alamat);
            printf("| %-20s | %-21d |\n", "Saldo",  data[idxTerpilih].saldo);
            printf("+----------------------+-----------------------+\n");
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

    system(CLEAR);
    printf("=== MENAMBAHKAN DATA WARGA ===\n");
    inputString(data[jlhWarga].nama, sizeof(data[jlhWarga].nama), "Nama   :");
    do {
        inputAngkaString(nikInput, sizeof(nikInput), "NIK    : ");
        if (cariIndexNIK(nikInput) != -1) {
            printf("\nNIK %s sudah terdaftar!\n", nikInput);
            pause();
            continue;
        }
    } while (cariIndexNIK(nikInput) != -1);
    strcpy(data[jlhWarga].nik, nikInput);
    inputAngkaString(data[jlhWarga].rw, sizeof(data[jlhWarga].rw), "RW     : ");
    inputAngkaString(data[jlhWarga].rt, sizeof(data[jlhWarga].rt), "RT     : ");
    inputAlamat(data[jlhWarga].alamat, sizeof(data[jlhWarga].alamat), "Alamat : ");
    data[jlhWarga].saldo = 0;
    data[jlhWarga].totalSampahKg = 0.0;

    char namaFormat[101];
    strcpy(namaFormat, data[jlhWarga].nama);
    formatNama(namaFormat);
    printf("\n+===============================================+\n");
    printf("|             DATA WARGA TERSIMPAN             |\n");
    printf("+----------------------+-----------------------+\n");
    printf("| %-20s | %-21s |\n", "Nama",   namaFormat);
    printf("| %-20s | %-21s |\n", "NIK",    data[jlhWarga].nik);
    printf("| %-20s | %-21s |\n", "RW",     data[jlhWarga].rw);
    printf("| %-20s | %-21s |\n", "RT",     data[jlhWarga].rt);
    printf("| %-20s | %-21s |\n", "Alamat", data[jlhWarga].alamat);
    printf("| %-20s | %-21d |\n", "Saldo",  data[jlhWarga].saldo);
    printf("+----------------------+-----------------------+\n\n");;
    
    jlhWarga++;
    sortRWRTNama();
    save();
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
        char namaFormat[101];
        strcpy(namaFormat, data[i].nama);
        formatNama(namaFormat);
        for (int i = 0; i < jlhWarga; i++) {
    char namaFormat[101];
    strcpy(namaFormat, data[i].nama);
    formatNama(namaFormat);

    printf("\n+===============================================+\n");
    printf("|              DATA WARGA DITEMUKAN            |\n");
    printf("+----------------------+-----------------------+\n");
    printf("| %-20s | %-21s |\n", "Nama",   namaFormat);
    printf("| %-20s | %-21s |\n", "NIK",    data[i].nik);
    printf("| %-20s | %-21s |\n", "RW",     data[i].rw);
    printf("| %-20s | %-21s |\n", "RT",     data[i].rt);
    printf("| %-20s | %-21s |\n", "Alamat", data[i].alamat);
    printf("| %-20s | %-21d |\n", "Saldo",  data[i].saldo);
    printf("+----------------------+-----------------------+\n");
}
    }
}

void pencarianDataNIK() { // fungsi cari data berdasarkan NIK
    char cariNIK[17];
    system(CLEAR);
    printf("=== PENCARIAN DATA WARGA BERDASARKAN NIK ===\n");
    inputAngkaString(cariNIK, sizeof(cariNIK), "Masukkan NIK yang ingin dicari: ");

    int index = cariIndexNIK(cariNIK);

    if (index == -1) {
        printf("\nData warga dengan NIK %s tidak ditemukan.\n", cariNIK);
    } else {
        char namaFormat[101];
        strcpy(namaFormat, data[index].nama);
        formatNama(namaFormat);
        printf("\nData warga ditemukan:\n");
        printf("Nama   : %s\n", namaFormat);
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
        printf("+====================================+\n");
        printf("|        MELIHAT DATA WARGA          |\n");
        printf("+------------------------------------+\n");
        printf("| 1) Tampilkan data semua warga      |\n");
        printf("| 2) Cari data warga                 |\n");
        printf("| 0) Kembali                         |\n");
        printf("+------------------------------------+\n");
        input = inputInt("Masukkan input anda: ");
        switch (input) {
        case 1:
            tampilkanListWarga();
            pause();
            break;
        case 2:
            pencarianDataNIK();
            pause();
            break;
        case 0:
            berjalan = 0;
            break;
        default:
            printf("Input Invalid");
            pause();
            break;
        }
    } while (berjalan);
}

// === FUNGSI SETOR SAMPAH ===
void setorSampah() {
    char inputNik[17];
    int indexWarga;
    char jenisSampah[10];
    float beratSampah;
    char tanggal[15];
    int hargaPerKg;
    int totalSaldo;
    int inputMenu;

    system(CLEAR);
    printf("=== SETOR SAMPAH ===\n");

    // 1. Input NIK
    printf("Masukkan NIK Anda: ");
    scanf(" %16s", inputNik);
    getchar();

    // 2. Cek NIK
    indexWarga = cariIndexNIK(inputNik);
    if (indexWarga == -1) {
        printf("\nNIK tidak ditemukan. Proses setor sampah dibatalkan.\n");
        pause();
        return;
    }
    char namaFormat[101];
    strcpy(namaFormat, data[indexWarga].nama);
    formatNama(namaFormat);
    printf("\nWarga ditemukan: %s (NIK: %s)\n", namaFormat, data[indexWarga].nik);
    pause();

    int berjalan1 = 1;
    do
    {
        system(CLEAR);
        printf("\n+===========================================+\n");
        printf("|           PILIH JENIS SAMPAH             |\n");
        printf("+-------------------------------------------+\n");
        printf("| 1) Plastik   (Rp 4,000/kg)                |\n");
        printf("| 2) Besi      (Rp 3,500/kg)                |\n");
        printf("| 3) Tembaga   (Rp 70,000/kg)               |\n");
        printf("| 4) Kardus    (Rp 1,800/kg)                |\n");
        printf("+===========================================+\n");
                inputMenu = inputInt("Masukkan input anda: ");
        switch (inputMenu) {
            case 1:
                strcpy(jenisSampah, "Plastik");
                hargaPerKg = 4000;
                berjalan1 = 0;
                break;
            case 2:
                strcpy(jenisSampah, "Besi");
                hargaPerKg = 3500;
                berjalan1 = 0;
                break;
            case 3:
                strcpy(jenisSampah, "Tembaga");
                hargaPerKg = 70000;
                berjalan1 = 0;
                break;
            case 4:
                strcpy(jenisSampah, "Kardus");
                hargaPerKg = 1800;
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
        if (scanf("%f", &beratSampah) != 1) {
            printf("Input harus berupa angka!\n");
            while (getchar() != '\n'); // bersihkan buffer
            pause();
            continue;
        }
        getchar();
        if (beratSampah <= 0) {
            printf("Berat harus lebih dari 0.\n");
            pause();
            continue;
        } else {
            berjalan2 = 0;
        }
    } while (berjalan2);

    getTanggalHariIni(tanggal);

    
    totalSaldo = beratSampah * hargaPerKg;

    data[indexWarga].totalSampahKg += beratSampah;
    data[indexWarga].saldo += totalSaldo;
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
            data[indexWarga].nik,
            totalSaldo,
            tanggal);

    fclose(pF_transaksi);
    system(CLEAR);
    printf("\n+===========================================================+\n");
    printf("|                    KONFIRMASI SETORAN                    |\n");
    printf("+-------------------------------+---------------------------+\n");
    printf("| %-29s | %-25s |\n", "Nama", namaFormat);
    printf("| %-29s | %-25s |\n", "NIK", data[indexWarga].nik);
    printf("| %-29s | %-25s |\n", "Jenis", jenisSampah);
    printf("| %-29s | %8.2f kg               |\n", "Berat", beratSampah);
    printf("| %-29s | Rp %-20d|\n", "Harga", hargaPerKg);
    printf("| %-29s | Rp %-20d|\n", "Total Saldo Ditambahkan", totalSaldo);
    printf("| %-29s | Rp %-20d|\n", "Saldo Terbaru Anda", data[indexWarga].saldo);
    printf("| %-29s | %-25s |\n", "Tanggal", tanggal);
    printf("+-------------------------------+---------------------------+\n");
    printf("| Setoran berhasil dicatat.                                 |\n");
    printf("+===========================================================+\n");
    printf("\nSetoran berhasil dicatat.\n");
    pause();
}

// === FUNGSI HAPUS DATA WARGA ===
void hapusByNIK() {// fungsi hapus data warga berdasarkan NIK
    char cariNIK[17];
    char pilih;
    system(CLEAR);
    printf("\n+=======================================================+\n");
    printf("|   PENGHAPUSAN DATA WARGA BERDASARKAN NIK               |\n");
    printf("+=======================================================+\n");
        inputAngkaString(cariNIK, sizeof(cariNIK), "Masukkan NIK yang ingin dihapus: ");
        
    int index = cariIndexNIK(cariNIK);
    
    if (index == -1) {
        printf("\nData warga dengan NIK %s tidak ditemukan.\n", cariNIK);
        return;
    }
    char namaFormat[101];
    strcpy(namaFormat, data[index].nama);
    formatNama(namaFormat);
    printf("\n+===============================================+\n");
    printf("|              DATA WARGA DITEMUKAN            |\n");
    printf("+----------------------+-----------------------+\n");
    printf("| %-20s | %-21s |\n", "Nama",   namaFormat);
    printf("| %-20s | %-21s |\n", "NIK",    data[index].nik);
    printf("| %-20s | %-21s |\n", "RW",     data[index].rw);
    printf("| %-20s | %-21s |\n", "RT",     data[index].rt);
    printf("| %-20s | %-21s |\n", "Alamat", data[index].alamat);
    printf("| %-20s | %-21d |\n", "Saldo",  data[index].saldo);
    printf("+----------------------+-----------------------+\n");
    printf("Anda yakin hapus data warga ini (y/n): ");
    scanf(" %c", &pilih);
    if (pilih == 'y' || pilih == 'Y') {
        for (int i = index; i < jlhWarga - 1; i++) {
            data[i] = data[i + 1];
        }
        jlhWarga--;
        save();
        printf("data warga telah dihapus\n");
        getchar();
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
        printf("+=======================================+\n");
        printf("|       PENGHAPUSAN DATA WARGA          |\n");
        printf("+---------------------------------------+\n");
        printf("| 1) Hapus data berdasarkan NIK         |\n");
        printf("| 2) Hapus semua                        |\n");
        printf("| 0) Kembali                            |\n");
        printf("+=======================================+\n");
        input = inputInt("Masukkan input anda: ");
        switch (input) {
        case 1:
            hapusByNIK();
            pause();
            break;
        case 2:
            hapusSemua();
            pause();
            break;
        case 0:
            berjalan = 0;
            break;
        default:
            printf("Input harus 1 atau 2");
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
        char namaFormat[101];
        strcpy(namaFormat, data[index].nama);
        formatNama(namaFormat);
        system(CLEAR);
        printf("\n+===============================================+\n");
        printf("|                 DATA WARGA                   |\n");
        printf("+----------------------+-----------------------+\n");
        printf("| %-20s | %-21s |\n", "Nama",   data[index].nama);
        printf("| %-20s | %-21s |\n", "NIK",    data[index].nik);
        printf("| %-20s | %-21s |\n", "RW",     data[index].rw);
        printf("| %-20s | %-21s |\n", "RT",     data[index].rt);
        printf("| %-20s | %-21s |\n", "Alamat", data[index].alamat);
        printf("| %-20s | %-21d |\n", "Saldo",  data[index].saldo);
        printf("+----------------------+-----------------------+\n");

        printf("\n+===========================+\n");
        printf("|     MASUKKAN DATA BARU   |\n");
        printf("+---------------------------+\n");
        printf("| 1) Nama                  |\n");
        printf("| 2) NIK                   |\n");
        printf("| 3) RW                    |\n");
        printf("| 4) RT                    |\n");
        printf("| 5) Alamat                |\n");
        printf("| 0) Selesai               |\n");
        printf("+---------------------------+\n");
        input = inputInt("Data mana yang ingin diganti: ");
        switch (input) {
        case 1:
            inputString(data[index].nama, sizeof(data[index].nama), "Masukkan Nama Baru: ");
            break;
        case 2:
            inputAngkaString(nikInput, sizeof(nikInput), "Masukkan NIK Baru");
            if (cariIndexNIK(nikInput) != -1) {
                printf("\nNIK %s sudah terdaftar!\n", nikInput);
                printf("Silakan masukkan NIK lain.\n\n");
                pause();
                break;;
            } else {
                strcpy(data[index].nik, nikInput);
            }
            break;
        case 3:
            inputAngkaString(data[index].rw, sizeof(data[index].rw), "Masukkan RW baru: ");
            break;
        case 4:
            inputAngkaString(data[index].rt, sizeof(data[index].rt), "Masukkan RT baru: ");
            break;
        case 5:
            inputString(data[index].alamat, sizeof(data[index].alamat), "Masukkan alamat Baru: ");
            break;
        case 0:
            berjalan = 0;
            break;
        default:
            printf("Input hanya boleh 1-5");
            pause();
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
        printf("+==========================+\n");
        printf("|      EDIT DATA WARGA     |\n");
        printf("+--------------------------+\n");
        printf("| 1) Cari berdasarkan NIK  |\n");
        printf("| 2) Cari berdasarkan nama |\n");
        printf("| 0) Kembali               |\n");
        printf("+--------------------------+\n");
        input = inputInt("Masukkan input anda: ");
        switch (input) {
        case 1:
        system(CLEAR);
            printf("\n+=================================================+\n");
            printf("|   PENCARIAN DATA WARGA BERDASARKAN NIK          |\n");
            printf("+-------------------------------------------------+\n");
            printf("| Masukkan NIK yang ingin dicari: ");
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

// === FUNGSI TARIK SALDO ===
void tarikSaldo() {
    char cariNIK[17];
    int index;
    int saldoDiTukar;
    int temp;
    char tanggal[15];
    
    system(CLEAR);
    printf("+=======================+\n");
    printf("|    PENARIKAN SALDO    |\n");
    printf("+=======================+\n");
    inputAngkaString(cariNIK, sizeof(cariNIK), "Masukkan NIK yang ingin dihapus: ");
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
        char namaFormat[101];
        strcpy(namaFormat, data[index].nama);
        formatNama(namaFormat);
        printf("\n+===============================================+\n");
        printf("|              DATA WARGA DITEMUKAN            |\n");
        printf("+----------------------+-----------------------+\n");
        printf("| %-20s | %-21s |\n", "Nama",   namaFormat);
        printf("| %-20s | %-21s |\n", "NIK",    data[index].nik);
        printf("| %-20s | %-21s |\n", "RW",     data[index].rw);
        printf("| %-20s | %-21s |\n", "RT",     data[index].rt);
        printf("| %-20s | %-21s |\n", "Alamat", data[index].alamat);
        printf("| %-20s | %-21d |\n", "Saldo",  data[index].saldo);
        printf("+----------------------+-----------------------+\n");

        printf("\n+============== PILIH ===============+\n");
        printf("| 1) Tukar saldo spesifik            |\n");
        printf("| 2) Tukar semua saldo               |\n");
        printf("| 0) Kembali                         |\n");
        printf("+====================================+\n");
        input = inputInt("Masukkan input anda: ");
        switch (input) {
        case 1:
            saldoDiTukar = inputInt("\nMasukkan jumlah saldo yang diinginkan: ");
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
                char namaFormat[101];
                strcpy(namaFormat, data[index].nama);
                formatNama(namaFormat);
                printf("\n+===========================================+\n");
                printf("|        KONFIRMASI TUKAR SALDO            |\n");
                printf("+-------------------------------------------+\n");
                printf("| Nama               : %s\n", namaFormat);
                printf("| NIK                : %s\n", data[index].nik);
                printf("| Saldo yang ditukar : %d\n", saldoDiTukar);
                printf("| Saldo terbaru      : %d\n", data[index].saldo);
                printf("+===========================================+\n");
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
                char namaFormat[101];
                strcpy(namaFormat, data[index].nama);
                formatNama(namaFormat);
                ("\n=== KONFIRMASI TUKAR SALDO ===\n");
                printf("\n+===========================================+\n");
                printf("|        KONFIRMASI TUKAR SALDO            |\n");
                printf("+-------------------------------------------+\n");
                printf("| Nama               : %s\n", namaFormat);
                printf("| NIK                : %s\n", data[index].nik);
                printf("| Saldo yang ditukar : %d\n", saldoDiTukar);
                printf("| Saldo terbaru      : %d\n", data[index].saldo);
                printf("+===========================================+\n");
            }
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

    FILE *pF_transaksi = fopen(FILE_TRANSAKSI, "a");printf;
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
    system(CLEAR);
    printf("=== TAMPILKAN RIWAYAT TRANSAKSI ===\n");
    inputAngkaString(nikCari, sizeof(nikCari), "Masukkan NIK yang ingin dicari: ");

    // Cari nama warga berdasarkan NIK yang dicari
    int idxNama = cariIndexNIK(nikCari);
    if (idxNama == -1) {
        printf("\nNIK %s tidak ditemukan dalam daftar warga.\n", nikCari);
        fclose(pF);
        return;
    }
    char namaFormat[101];
    strcpy(namaFormat, data[idxNama].nama);
    formatNama(namaFormat);
    system(CLEAR);

    printf("+===========================+\n");
    printf("|      RIWAYAT TRANSAKSI   |\n");
    printf("+---------------------------+\n");
    printf("| Nama : %s\n", namaFormat);
    printf("| NIK  : %s\n\n", nikCari);
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

// === FUNGSI LAPORAN SAMPAH TERKUMPUL ===
void laporanPerWarga() {
    char nik[17];
    inputAngkaString(nik, sizeof(nik), "Masukkan NIK warga: ");
    
    int idx = cariIndexNIK(nik);
    if (idx == -1) {
        printf("NIK tidak ditemukan!\n");
        return;
    }
    printf("\nLaporan untuk %s (RT %s/RW %s):\n", data[idx].nama, data[idx].rt, data[idx].rw);
    printf("Total sampah terkumpul: %d kg\n", data[idx].totalSampahKg);
}

void laporanPerRT() {
    char rt[3], rw[3];
    float totalSampah = 0.0;
    inputAngkaString(rw, sizeof(rw), "Masukkan RW: ");
    inputAngkaString(rt, sizeof(rt), "Masukkan RT: ");

    printf("\n=== Laporan RT %s RW %s ===\n", rt, rw);
    for (int i = 0; i < jlhWarga; i++) {
        if (strcmp(data[i].rt, rt) == 0 && strcmp(data[i].rw, rw) == 0) {
            totalSampah += data[i].totalSampahKg;
        }
    }
    printf("Total Nilai Sampah RT %s: Rp %ld\n", rt, totalSampah);
}

void laporanPerRW() {
    char rw[3];
    float totalSampah = 0.0;
    inputAngkaString(rw, sizeof(rw), "Masukkan RW: ");

    for (int i = 0; i < jlhWarga; i++) {
        if (strcmp(data[i].rw, rw) == 0) {
            totalSampah += totalSampah += data[i].totalSampahKg;
        }
    }
    printf("\nTotal Nilai Sampah RW %s: Rp %ld\n", rw, totalSampah);
}

void laporanSeluruhKelurahan() {
    float totalSampah = 0;
    for (int i = 0; i < jlhWarga; i++) {
        totalSampah += data[i].totalSampahKg;
    }

    printf("\n+===============================================+\n");
    printf("|     LAPORAN KESELURUHAN KELURAHAN            |\n");
    printf("+-----------------------------------------------+\n");
    printf("| Total Nilai Seluruh Sampah: Rp %-13ld |\n", totalSampah);
    printf("+===============================================+\n");
}

void menuTotalSampah() {
    int input;
    int berjalan = 1;
    do
    {
        system(CLEAR);

        printf("+=================================+\n");
        printf("|        MENU LAPORAN DATA        |\n");
        printf("+---------------------------------+\n");
        printf("| 1) Laporan total                |\n");
        printf("| 2) Laporan per RW               |\n");
        printf("| 3) Laporan per RT               |\n");
        printf("| 4) Laporan per warga            |\n");
        printf("| 0) Kembali                      |\n");
        printf("+---------------------------------+\n");
        input = inputInt("Masukkan input anda: ");
        switch (input) {
        case 1:
            laporanSeluruhKelurahan();
            pause();
            break;
        case 2:
            laporanPerRW();
            pause();
            break;
        case 3:
            laporanPerRT();
            pause();
            break;
        case 4:
            laporanPerWarga();
            pause();
            break;
        case 0:
            berjalan = 0;
            break;
        default:
            printf("\nInput harus 1-7\n");
            pause();
            break;
        }
    } while (berjalan);
}