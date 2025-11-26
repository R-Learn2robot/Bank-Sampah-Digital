🏷️ Bank Sampah Digital – Terminal App (C Project)
♻️ Sistem Manajemen Bank Sampah Berbasis CLI Menggunakan Bahasa C

Program ini adalah aplikasi terminal yang dibuat untuk membantu proses digitalisasi Bank Sampah di lingkungan masyarakat, khususnya di tingkat RT/RW atau komunitas kecil. Aplikasi ini memungkinkan pengelola bank sampah mencatat data warga, menyimpan transaksi penyetoran sampah, menghitung saldo tabungan berdasarkan jenis sampah, dan menyimpan data secara permanen menggunakan file handling.

Aplikasi ini dikembangkan sebagai Proyek Akhir Mata Kuliah Algoritma dan Pemrograman, dan mengikuti pendekatan System Development Life Cycle (SDLC).

👥 Pembuat Program

Program ini dibuat oleh:
Kelompok 2

Anggota:

1. Ahmad Faiq Zidane
2. Radit
3. Fadla
4. rouf
5. Dina
6. Nadisya Azkia Syifana
   


🚀 Fitur Utama
| No | Fitur                                          |
| -- | ---------------------------------------------- |
| 1  | Tambah dan kelola data warga                   |
| 2  | Input setoran sampah berdasarkan jenis & berat |
| 3  | Perhitungan saldo otomatis                     |
| 4  | Pencarian data warga (Searching)               |
| 5  | Pengurutan saldo (Bubble Sort)                 |
| 6  | Riwayat transaksi                              |
| 7  | Simpan dan muat data menggunakan file          |

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
| Calon Pengguna              | Examples                |
| --------------------------- | ----------------------- |
| RT/RW & Pengurus Lingkungan | Bank Sampah warga       |
| Organisasi Sosial           | PKK, Karang Taruna      |
| Sekolah                     | Program 3R & eco-school |
| Komunitas Lingkungan        | Volunteer, Eco movement |

