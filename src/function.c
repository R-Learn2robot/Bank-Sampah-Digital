
#include "../include/function.h"

Warga *data = NULL;
int jlhWarga = 0;
int kapasitasWarga = 0;

Riwayat *transaksi = NULL;
int jlhTransaksi = 0;
int kapasitasTransaksi = 0;

//fungsi laiin-lain
void pause() { //fungsi agar setelah setiap fungsi selesai dia berhenti sejenak baru melakukan clear screen
    printf("\ntekan enter untuk kembali...");
    getchar();
}

void save() { //menyimpan data kedalam file.txt
    FILE *pF = fopen(DATA_WARGA, "w");
    if (!pF){
        printf("file tidak ditemukan!");
        return;
    }
    for (int i = 0; i < jlhWarga; i++){
        fprintf(pF, "%s|%s|%s|%s|%s|%d|\n", data[i].nama, data[i].nik, data[i].rw, 
            data[i].rt, data[i].alamat, data[i].poin);

    }
    fclose(pF);
}

void load() { // membaca file dan memasukkannya ke array of struck warga data[]
    FILE *pF = fopen(DATA_WARGA, "r");
    if (!pF){
        printf("file tidak ditemukan!");
        return;
    }
    char line[255];
    jlhWarga  = 0;

    while (fgets(line, sizeof(line), pF)){
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|", &data[jlhWarga].nama, &data[jlhWarga].nik, &data[jlhWarga].rw, 
            &data[jlhWarga].rt, &data[jlhWarga].alamat, &data[jlhWarga].poin);
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

void hapusMemori() { // fungsi membersihkan memori setelah program selesai agar tidak memori leak
    free(data);
    data = NULL;
    kapasitasWarga = 0;
    jlhWarga = 0;
}

// fungsi pencarian
int cariIndexNIK(const char *nik) { // fungsi cari index dari array berdasarkan NIK
    int index = -1;

    for (int i = 0; i < jlhWarga; i++) {
        if (strcmp(data[i].nik, nik) == 0){
            return i;
            break;
        }
    }
    return -1;    
}

// fungsi sorting (Bubble Sort)
void sortRWRTNama() { // sorting data berdasarkan RW lalu RT lalu nama
    Warga temp;

    if (jlhWarga < 0){
        return;
    }

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

//fungsi tambah data warga
void tambahWarga(){ // fungsi tambah data warga ke file.txt
    system(CLEAR);
    cekKapasistas();
    char nikInput[17];
    
    printf("=== MENAMBAHKAN DATA WARGA ===\n");
    printf("nama: ");
    scanf(" %100[^\n]", &data[jlhWarga].nama);

    do {
        printf("NIK: ");
        scanf(" %16s", nikInput);

        if (cariIndexNIK(nikInput) != -1) {
            printf("\nNIK %s sudah terdaftar!\n", nikInput);
            printf("Silakan masukkan NIK lain.\n\n");
        }

    } while (cariIndexNIK(nikInput) != -1);
    strcpy(data[jlhWarga].nik, nikInput);

    printf("RW: ");
    scanf(" %2s", &data[jlhWarga].rw);

    printf("RT: ");
    scanf(" %2s", &data[jlhWarga].rt);

    printf("alamat: ");
    scanf(" %100[^\n]", &data[jlhWarga].alamat);

    data[jlhWarga].poin = 0;
    jlhWarga++;
    
    sortRWRTNama();
    save();
    
}

void tampilkanListWarga() { // fungsi tampilkan semua data dari file
    FILE *pF = fopen(DATA_WARGA, "r");
    if (!pF) {
        printf("file tidak ditemukan");
        return;
    }

    if (jlhWarga == 0) {
        printf("belum ada warga yang terdaftar");
        return;
    }

    system(CLEAR);
    printf("\n===LIST WARGA===\n");

    char line[255];
    char nama[100];
    char nik[100];
    char rt[100];
    char rw[100];
    char alamat[100];
    int poin;
    int count = 1;

    while (fgets(line, sizeof(line), pF)){
        sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%d|", &nama, &nik, &rw, &rt, &alamat, &poin);
        printf("%d) Nama   : %s\n", count, nama);
        printf("    NIK    : %s\n", nik);
        printf("    RW     : %s\n", rw);
        printf("    RT     : %s\n", rt);
        printf("    alamat : %s\n", alamat);
        printf("    Poin   : %d\n", poin);
    }
    fclose(pF);
}

void pencarianDataNIK() { // fungsi cari data warga berdasarkan NIK
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
        printf("poin   : %d\n", data[index].poin);
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

// fungsi edit data
void masukkanDataBaru(const int index) { // fungsi masukkan data baru setelah di edit
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
            scanf(" %100[^\n]", &data[index].nama);
            break;
        case 2:
            printf("Masukkan NIK Baru: ");
            scanf(" %16s", &data[index].nik);
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

void editDataWarga() { // fungsi menu edit data warga berdasarkan NIK atau nama (WIP, kurang pencarian banyak data dari 1 input)
    char cariNIK[17];
    char cariNama[101];
    int index;
    int input;
    int berjalan = 1;
    do
    {
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