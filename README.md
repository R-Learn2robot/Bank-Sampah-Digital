🏷️ Bank Sampah Digital – Terminal App (C Project)
♻️ Sistem Manajemen Bank Sampah Berbasis CLI Menggunakan Bahasa C

Program ini adalah aplikasi terminal yang dibuat untuk membantu proses digitalisasi Bank Sampah di lingkungan masyarakat, khususnya di tingkat RT/RW atau komunitas kecil. Aplikasi ini memungkinkan pengelola bank sampah mencatat data warga, menyimpan transaksi penyetoran sampah, menghitung saldo tabungan berdasarkan jenis sampah, dan menyimpan data secara permanen menggunakan file handling.

Aplikasi ini dikembangkan sebagai Proyek Akhir Mata Kuliah Algoritma dan Pemrograman, dan mengikuti pendekatan System Development Life Cycle (SDLC).

👥 Pembuat Program

Program ini dibuat oleh:
Kelompok 2

Anggota:

1. Ahmad Faiq Zidane_2502542
2. Raditya Wicaksana Putra_2501195
3. Falda Falisha Aeni_2511508
4. Muhammad Abdur Rouf_2510726
5. Nadisya Azkia Syifana_2501186
6. Dina Aulia_2502334
7. Affan Lie Dhiyaa'ulrahman_2506463


🚀 Fitur Utama
| No | Fitur                                          |
| -- | ---------------------------------------------- |
| 1  | Tambah data warga                              |
| 2  | Cari dan list semua data warga                 |
| 3  | Setor Sampah                                   |
| 4  | Edit data warga NIK/Nama                       |
| 5  | Hapus data warga                               |
| 6  | Penarikan saldo semua/spesifik                 |
| 7  | Riwayat Transaksi                              |
| 8  | Total Sampah Terkumpul                         |

🛠️ Konsep Pemrograman yang Digunakan
| Konsep              | Implementasi                                       |
| ------------------- | -------------------------------------------------- |
| Struktur Data       | `struct` dan `array of struct`                     |
| Searching           | Sequential Search                                  |
| Sorting             | Bubble Sort                                        |
| Modular Programming | Fungsi terpisah untuk tiap fitur                   |
| Persistensi Data    | File handling (`fopen`, `fprintf`, `fscanf`, dll.) |
| User Interaction    | Sistem menu CLI                                    |

▶️ Cara Menjalankan Aplikasi
| Langkah              | Perintah                           |
| -------------------- | ---------------------------------- |
| Clone Repository     | `git clone <link-repo>`            |
| Masuk Folder         | `cd bank-sampah`                   |
| Compile              | `gcc bank_sampah.c -o bank_sampah` |
| Jalankan (Linux/Mac) | `./bank_sampah`                    |
| Jalankan (Windows)   | `bank_sampah.exe`                  |

🎯 Target Penerapan Aplikasi

Aplikasi Bank Sampah Digital ini dibuat untuk membantu Admin Bank Sampah Kelurahan Nagri Kaler dalam melakukan pencatatan dan pengelolaan data bank sampah secara sederhana menggunakan bahasa C berbasis terminal.
| Pengguna                                | Peran                                                                |
| --------------------------------------- | -------------------------------------------------------------------- |
| Admin Bank Sampah Kelurahan Nagri Kaler | Mengelola data warga, mencatat transaksi sampah, dan melihat laporan |

📌 Catatan:
Aplikasi ini tidak digunakan langsung oleh warga, seluruh proses input dan pengelolaan data dilakukan oleh admin.

🏘️ Lingkup Penerapan

Aplikasi ini dirancang dengan lingkup:

Satu kelurahan (Kelurahan Nagri Kaler)

Penggunaan oleh satu atau beberapa admin bank sampah

Skala kecil dan sederhana

Aplikasi ini bertujuan untuk membantu proses administrasi bank sampah agar lebih rapi dan terdata.


