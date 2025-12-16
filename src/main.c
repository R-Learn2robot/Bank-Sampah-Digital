#include "../include/function.h"



int main() {
    int input;
    load();
    sortRWRTNama();
    save();
    do
    {
        system(CLEAR);
        printf("=========================================\n");
        printf("|                MENU                   |\n");
        printf("=========================================\n");
        printf("| 1) Tambah data warga                  |\n");
        printf("| 2) Melihat data warga                 |\n");
        printf("| 3) Hapus data warga (WIP)             |\n");
        printf("| 4) Edit data warga                    |\n");
        printf("| 0) Exit                               |\n");
        printf("=========================================\n");
        printf("Masukkan input anda: ");
        scanf("%d", &input);
        getchar();
        switch (input)
        {
        case 1:
            tambahWarga();
            getchar();
            pause();
            break;
        case 2:
            sortRWRTNama();
            save();
            liatDataWarga();
            pause();
            break;
        case 3: //belum ada fungsinya
            break;
        case 4:
            editDataWarga();
            pause();
            break;
        case 0:
            save();
            hapusMemori();
            printf("\nProgram selesai");
            return 0;
        default:
            printf("invalid");
            pause();
            break;
        }
    } while (1);

    int input;
    char nikCari[20];

    do {
        printf("=========================================\n");
        printf("| %-36s |\n", "MENU");
        printf("=========================================\n");
        printf("| %-36s |\n", "1) Tambah data warga");
        printf("| %-36s |\n", "2) Melihat data warga");
        printf("| %-36s |\n", "3) Hapus data warga (WIP)");
        printf("| %-36s |\n", "4) Edit data warga");
        printf("| %-36s |\n", "5) Riwayat transaksi");
        printf("| %-36s |\n", "0) Exit");
        printf("=========================================\n");
        printf("Masukkan input anda: ");
        scanf("%d", &input);
        getchar(); // buang newline

        switch (input) {
        case 5:
            printf("Masukkan NIK yang ingin dilihat riwayatnya: ");
            scanf("%19s", nikCari);
            tampilRiwayatByNIK("riwayat.txt", nikCari);   // sesuaikan nama file
            break;

        
        }
    } while (input != 0);

}


