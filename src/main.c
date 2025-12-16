#include "../include/function.h"



int main() {
    int input;
    load();
    sortRWRTNama();
    save();
    do
    {
        system(CLEAR);
        printf("========= MENU =======\n");
        printf("1) Tambah data warga\n");
        printf("2) Melihat data warga\n");
        printf("3) Hapus data warga(WIP)\n");
        printf("4) Edit data warga\n");
        printf("0) exit\n");
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

}

