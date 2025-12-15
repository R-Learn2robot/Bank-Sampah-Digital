#include "../include/function.h"



int main() {
    int pilih;
    load();
    sortRWRTNama();
    save();
    do
    {
        system(CLEAR);
        printf("===MENU===\n");
        printf("1) tambah data\n");
        printf("2) list data\n");
        printf("3) exit\n");
        scanf("%d", &pilih);
        getchar();
        switch (pilih)
        {
        case 1:
            tambahWarga();
            getchar();
            pause();
            break;
        case 2:
            sortRWRTNama();
            save();
            tampilData();
            pause();
            break;
        case 3:
            return 0;
        
        default:
            printf("invalid");
            break;
        }
    } while (1);

}

int main() {
    int pilih;

    do {
        printf("\n=== MENU BANK SAMPAH ===\n");
        printf("1. Data warga\n");
        printf("2. Setor sampah\n");
        printf("3. Tukar poin\n");
        printf("4. Laporan\n");
        printf("5. Keluar\n");
        printf("6. Riwayat transaksi\n");  // <- misal menu 6
        printf("Pilih: ");
        scanf("%d", &pilih);

        switch (pilih) {
        case 1:
            // menu data warga
            break;
        case 2:
            // setor
            break;
        case 3:
            // tukar poin
            break;
        case 4:
            // laporan
            break;
        case 5:
            printf("Terima kasih.\n");
            break;
        case 6:
            riwayatTransaksi();   // <- panggil fungsi yang tadi kita buat
            break;
        default:
            printf("Pilihan tidak valid.\n");
        }
    } while (pilih != 5);

    return 0;
}

