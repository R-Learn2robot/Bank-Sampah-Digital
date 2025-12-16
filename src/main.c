#include "../include/function.h"

int main() {
    int input;
    char nikCari[20];

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
        printf("| 3) Hapus data warga (WIP)             |\n");
        printf("| 4) Edit data warga                    |\n");
        printf("| 5) Riwayat transaksi                  |\n");
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
            // belum ada fungsi hapus data warga
            break;

        case 4:
            editDataWarga();
            pause();
            break;

        case 5:
            printf("Masukkan NIK yang ingin dilihat riwayatnya: ");
            scanf("%19s", nikCari);
            tampilRiwayatByNIK(FILE_TRANSAKSI, nikCari);  // pakai nama file dari define
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