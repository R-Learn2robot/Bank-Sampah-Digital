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

