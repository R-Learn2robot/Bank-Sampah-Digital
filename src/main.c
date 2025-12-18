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
        printf("| 4) Hapus data warga                   |\n");
        printf("| 5) Edit data warga                    |\n");
        printf("| 6) Tarik saldo                        |\n");
        printf("| 7) Riwayat transaksi                  |\n");
        printf("| 8) Laporan total sampah (kg)          |\n");
        printf("| 0) Exit                               |\n");
        printf("=========================================\n");
        input = inputInt("Masukkan input anda (1-7): ");
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
            hapusDataWarga();
            pause();
            break;

        case 5:
            editDataWarga();
            pause();
            break;

        case 6:
            tarikSaldo();
            break;

        case 7:
            tampilRiwayatByNIK();
            pause();
            break;

        case 8:
            menuLaporanSampah();
            break;

        case 0:
            save();
            hapusMemori();
            printf("\nProgram selesai");
            return 0;

        // case untuk error
        case -1:
            printf("\nInput tidak boleh kosong!\n");
            pause();
            break;
        
        case -2:
            printf("\nInput hanya boleh angka!\n");
            pause();
            break;
        
        default:
            printf("\nInput harus 1-7\n");
            getchar();
            pause();
            break;
        }

    } while (1);
}