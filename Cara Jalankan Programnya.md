Cara Menjalankan Program Bank Sampah di Terminal (Windows)
Struktur folder project:

Text
Bank-Sampah-Digital/
├─ app.exe (bisa belum ada, akan dibuat setelah compile)
├─ Cara Jalankan Programnya.md
├─ README.md
├─ include/
│ └─ function.h
├─ src/
│ ├─ main.c
│ └─ function.c
└─ data/
├─ dataWarga.txt
└─ transaksi.txt
Pastikan folder data dan file dataWarga.txt, transaksi.txt sudah ada.
Boleh kosong dulu, nanti akan terisi oleh program.

1. Buka Terminal di Folder Project
   Buka File Explorer.

Masuk ke folder:

Text
C:\Proyek Akhir\Bank-Sampah-Digital
Klik di address bar (kolom path di atas), ketik:

Text
cmd
lalu tekan Enter.

Akan muncul Command Prompt dengan posisi:

Text
C:\Proyek Akhir\Bank-Sampah-Digital> 2. Masuk ke Folder src
Di Command Prompt, ketik:

bat
cd src
Sekarang posisi menjadi:

Text
C:\Proyek Akhir\Bank-Sampah-Digital\src>
Untuk mengecek isi folder:

bat
dir
Harus muncul minimal:

main.c
function.c 3. Compile Program dengan GCC
Pastikan gcc sudah terpasang (MinGW / TDM-GCC).
Di Command Prompt (masih di folder src), jalankan:

bat
gcc main.c function.c -I../include -o app.exe
Keterangan:

main.c function.c → dua file sumber program.
-I../include → memberi tahu compiler bahwa file header (function.h) ada di folder ..\include.
-o app.exe → nama file output program.
Jika berhasil, tidak ada pesan error, dan akan muncul file app.exe di folder src.

4. Menjalankan Program
   Masih di folder src, jalankan:

bat
app.exe
Program Bank Sampah Digital akan berjalan di terminal.

5. Menggunakan Fitur Laporan Total Sampah
   Di dalam program:

Dari menu utama, pilih menu yang mengarah ke Laporan / Report
(misalnya: Laporan Total Sampah atau Menu Laporan).
Di Menu Laporan Total Sampah, biasanya ada pilihan:
1 → Laporan total sampah per orang (per NIK)
2 → Laporan total sampah per RT
3 → Laporan total sampah per RW
4 → Laporan total sampah per kelurahan
Pilih angka yang diinginkan, lalu tekan Enter.
Program akan menampilkan tabel laporan sesuai pilihan.
Catatan:

Agar laporan tidak kosong, pastikan sudah ada data warga dan transaksi setor sampah yang tersimpan di file data/dataWarga.txt dan data/transaksi.txt (dari hasil menjalankan menu Tambah Warga dan Setor Sampah di program). 6. Menjalankan Program dari Folder Utama (opsional)
Jika ingin menjalankan dari folder utama:

bat
cd "C:\Proyek Akhir\Bank-Sampah-Digital"
src\app.exe 7. Jika Terjadi Error Saat Compile
Beberapa error umum:

fatal error: function.h: No such file or directory
→ Pastikan perintah compile memakai -I../include dan file function.h ada di folder include.

undefined reference to ...
→ Pastikan semua file .c yang dibutuhkan sudah dimasukkan di perintah gcc (misalnya main.c function.c).
