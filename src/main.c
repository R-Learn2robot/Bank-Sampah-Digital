#include "../include/function.h"

int main() {
    int input;

    load();
    sortRWRTNama();
    save();

    do {
        system(CLEAR);
        printf("=========================================\n");
        printf("|                MENU                   |\n");
        printf("=========================================\n");
        printf("| 1) Tambah data warga                  |\n");
        printf("| 2) Melihat data warga                 |\n");
        printf("| 3) Setor sampah                       |\n");
        printf("| 4) Hapus data warga (WIP)             |\n");
        printf("| 5) Edit data warga                    |\n");
        printf("| 6) Tarik saldo                        |\n");
        printf("| 7) Riwayat transaksi                  |\n");
        printf("| 0) Exit                               |\n");
        printf("=========================================\n");
        printf("Masukkan input anda: ");
        scanf("%d", &input);

        switch (input) {
        case 1:
            tambahWarga();
            pause();
            break;

        case 2:
            sortRWRTNama();
            save();
            liatDataWarga();
            pause();
            break;

        case 3:
            setorSampah();
            break;

        case 4:
            // blm ad weh
            break;

        case 5:
            editDataWarga();
            pause();
            break;

        case 6:
            // blm ada fungsi
            pause();
            break;

        case 7:
            tampilRiwayatByNIK();
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
}