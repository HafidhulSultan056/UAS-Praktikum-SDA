#SISTEM ANALISIS TREN PENJUALAN KANTIN

Aplikasi berbasis **CLI (Command Line Interface)** Menggunakan bahasa pemrograman C. Sistem ini mengintegrasikan berbagai konsep struktur data seperti *Hash Table*, *Linked List*, *Stack*, dan beberapa algortima *Sorting* dan *Searching* untuk menangani antrean pembeli, pencatatan transaksi, manajemen inventaris, dan fitur pembatalan transaksi (*Undo*).

## Arsitektur Struktur Data

Untuk memastikan efisiensi memori dan kecepatan pemrosesan data, program ini mengombinasikan beberapa struktur data:
* **Hash Table (Chaining):** Manajemen penyimpanan primer data menu makanan.
* **Queue (Linked List Based):** Struktur dinamis untuk mengelola antrean pelanggan yang masuk.
* **Linked List:** Penyimpanan sekunder untuk merekam riwayat transaksi yang sukses secara kronologis.
* **Stack (Pointer-Based):** Penyimpanan referensi memori (*LIFO*) khusus untuk memfasilitasi mekanisme pembatalan (*Undo*).

Program analisis tren ini memuat beberapa fitur seperti:
1. Fitur Cari Menu
2. Fitur Tambah Pesanan (Enqueue)
3. Fitur Proses Antrean (Dequeue)
4. Fitur Batal Pesanan
5. Fitur Laporan Keuangan
