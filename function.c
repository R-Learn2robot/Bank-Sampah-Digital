
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

void pencarianData(){ // input nik
    char cariNIK[100];
    int ketemu = 0;

    printf("\n=== PENCARIAN DATA WARGA BERDASARKAN NIK ===\n");
    printf("Masukkan NIK yang ingin dicari: ");
    scanf(" %s", cariNIK);

    // loop array data[]
    for (int i = 0; i < jlhWarga; i++) {
        if (strcmp(data[i].nik, cariNIK) == 0) {
            ketemu = 1;
            printf("\nData warga ditemukan:\n");
            printf("nama   : %s\n", data[i].nama);
            printf("NIK    : %s\n", data[i].nik);
            printf("RW     : %s\n", data[i].rw);
            printf("RT     : %s\n", data[i].rt);
            printf("alamat : %s\n", data[i].alamat);
            printf("poin   : %d\n", data[i].poin);
            break; // berhenti karena sudah ketemu
        }
    }

    if (!ketemu) {
        printf("\nData warga dengan NIK %s tidak ditemukan.\n", cariNIK);
    }

    pause(); // supaya berhenti sejenak sebelum kembali ke menu
}