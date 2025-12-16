
#include "../include/function.h"

Warga data[MAX_WARGA];
int jlhWarga = 0;

//fungsi agar setelah setiap fungsi selesai dia berhenti sejenak baru melakukan clear screen
void pause() {
    printf("\ntekan enter untuk kembali...");
    getchar();
}

//fungsi tambah data warga ke file.txt
void tambahWarga(){
    printf("nama: ");
    scanf(" %[^\n]", &data[jlhWarga].nama);
    printf("NIK: ");
    scanf(" %s", &data[jlhWarga].nik);
    printf("RW: ");
    scanf(" %s", &data[jlhWarga].rw);
    printf("RT: ");
    scanf(" %s", &data[jlhWarga].rt);
    printf("alamat: ");
    scanf(" %[^\n]", &data[jlhWarga].alamat);
    data[jlhWarga].poin = 0;
    jlhWarga++;
    
    sortRWRTNama();
    save();
    
}

//fungsi tampilkan data dari file
void tampilData() {
    FILE *pF = fopen(FILE_NAME, "r");
    if (!pF) {
        printf("file tidak ditemukan");
        return;
    }

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
        printf("%d) nama   : %s\n   NIK    : %s\n   RW     : %s\n   RT     : %s\n   alamat : %s\n   poin   : %d\n\n", count, nama, nik, rw, rt, alamat, poin);
        count++;
    }
    fclose(pF);

}

//menyimpan data kedalam file.txt
void save() {
    FILE *pF = fopen(FILE_NAME, "w");
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

// membaca file dan memasukkannya ke array of struck warga data[]
void load(){
    FILE *pF = fopen(FILE_NAME, "r");
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

// sorting data berdasarkan RW lalu RT lalu nama
void sortRWRTNama() {
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

// Fungsi pembantu untuk mencari indeks array berdasarkan NIK
int findIndexByNIK(const char *nik) {
    for (int i = 0; i < jlhWarga; i++) {
        if (strcmp(data[i].nik, nik) == 0) {
            return i; // NIK ditemukan di indeks i
        }
    }
    return -1; // NIK tidak ditemukan
}

void editWarga() {
    if (jlhWarga == 0) {
        printf("⚠️ Belum ada data warga untuk diedit.\n");
        return;
    }
    
    char nik_cari[15];
    printf("\n--- Edit Data Warga ---\n");
    printf("Masukkan NIK data yang ingin diedit: ");
    scanf(" %s", nik_cari);
    
    int indeks = findIndexByNIK(nik_cari);

    if (indeks == -1) {
        printf("!!! Data warga dengan NIK %s tidak ditemukan.\n", nik_cari);
        return;
    }

    Warga *w = &data[indeks];
    printf("\nData ditemukan: Nama: %s, RW/RT: %s/%s\n", w->nama, w->rw, w->rt);
    printf("Biarkan kosong atau masukkan 0 jika tidak ingin mengubah field tertentu.\n");
    
    // --- Edit Nama ---
    char temp_nama[100];
    printf("Masukkan NAMA BARU (Saat ini: %s): ", w->nama);
    // Membersihkan buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 

    // Membaca input, menggunakan fgets untuk menangani spasi/enter
    if (fgets(temp_nama, sizeof(temp_nama), stdin) != NULL) {
        temp_nama[strcspn(temp_nama, "\n")] = 0; // Menghapus newline
        if (strlen(temp_nama) > 0) {
            strcpy(w->nama, temp_nama);
        }
    }

    // --- Edit RW ---
    char temp_rw[5];
    printf("Masukkan RW BARU (Saat ini: %s): ", w->rw);
    scanf(" %4s", temp_rw);
    if (strlen(temp_rw) > 0) {
        strcpy(w->rw, temp_rw);
    }

    // --- Edit RT ---
    char temp_rt[5];
    printf("Masukkan RT BARU (Saat ini: %s): ", w->rt);
    scanf(" %4s", temp_rt);
    if (strlen(temp_rt) > 0) {
        strcpy(w->rt, temp_rt);
    }
    
    // --- Edit Alamat ---
    char temp_alamat[100];
    printf("Masukkan ALAMAT BARU (Saat ini: %s): ", w->alamat);
    // Membersihkan buffer
    while ((c = getchar()) != '\n' && c != EOF); 
    
    if (fgets(temp_alamat, sizeof(temp_alamat), stdin) != NULL) {
        temp_alamat[strcspn(temp_alamat, "\n")] = 0; // Menghapus newline
        if (strlen(temp_alamat) > 0) {
            strcpy(w->alamat, temp_alamat);
        }
    }

    // --- Edit Poin ---
    int temp_poin;
    printf("Masukkan POIN BARU (Saat ini: %d): ", w->poin);
    scanf("%d", &temp_poin);
    if (temp_poin >= 0) {
        w->poin = temp_poin;
    }

    // Setelah edit, kita sort dan save data
    sortRWRTNama();
    save();
    printf("\n✅ Data warga dengan NIK %s berhasil diubah dan disimpan.\n", nik_cari);
}