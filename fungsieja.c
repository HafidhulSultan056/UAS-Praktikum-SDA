
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==========================================
// KODE BAGIAN EJOT / REZA (QUEUE & MAIN MENU)
// ==========================================

// 1. Enqueue: Masukin pembeli ke antrean belakang (FIFO)
void enqueue(char* nama_pembeli, char* item, char* hari) {
    // Alokasi memori untuk orang yang antre
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->nama_pembeli, nama_pembeli);
    strcpy(newNode->item, item);
    newNode->total = 0; // Total harga di-set 0 dulu, nanti dihitung pas Dequeue
    strcpy(newNode->hari, hari); 
    newNode->next = NULL;

    // Logika Queue (FIFO)
    if (tailQueue == NULL) {
        // Kalau antrean kosong
        headQueue = tailQueue = newNode;
    } else {
        // Kalau udah ada yang antre, taruh di paling belakang
        tailQueue->next = newNode;
        tailQueue = newNode;
    }
    printf("\n[+] BEMHASIL: Pesanan atas nama '%s' telah masuk ke antrean belakang!\n", nama_pembeli);
}

// 2. Dequeue: Manggil pembeli terdepan dan proses pesanannya
void dequeueProcess() {
    // Cek apakah antrean kosong [cite: 230]
    if (headQueue == NULL) {
        printf("\n[-] GAGAL: Antrean saat ini kosong. Tidak ada pesanan untuk diproses.\n");
        return;
    }

    // Ambil data orang terdepan
    Node* temp = headQueue;
    headQueue = headQueue->next;
    
    // Kalau antrean jadi kosong setelah orang ini dipanggil
    if (headQueue == NULL) {
        tailQueue = NULL;
    }

    // --- CARI HARGA & UPDATE STOK DARI DATABASE HAFIDHUL ---
    float total_harga = 0;
    Menu* menuDipesan = cariMenuUntukUndo(temp->item); // Pakai fungsi pencarian yang udah ada
    
    if (menuDipesan != NULL) {
        total_harga = menuDipesan->harga;
        menuDipesan->stok -= 1;     // Kurangi stok jajanan
        menuDipesan->terjual += 1;  // Tambah statistik penjualan
    } else {
        printf("\n[!] Peringatan: Menu '%s' tidak ditemukan di database, harga di-set 0.\n", temp->item);
    }

    // --- PANGGIL FUNGSI ERDIAN (CATAT SEJARAH) ---
    // 1. Simpan ke Linked List Riwayat
    Node* riwayatBaru = simpanRiwayat(temp->nama_pembeli, temp->item, total_harga, temp->hari);
    // 2. Simpan alamatnya ke Stack buat fitur Batal/Undo
    pushUndo(riwayatBaru);

    // Cetak Struk Sederhana [cite: 236]
    printf("\n====================================\n");
    printf("         PESANAN DIPROSES!\n");
    printf("====================================\n");
    printf("Nama Pembeli : %s\n", temp->nama_pembeli);
    printf("Item Pesanan : %s\n", temp->item);
    printf("Total Bayar  : Rp %.2f\n", total_harga);
    printf("====================================\n");

    // Bebaskan memori node antrean karena datanya sudah disalin ke riwayat Erdian
    free(temp); 
}

// 3. Main Menu: Dalang dari segala fungsi
int main() {
    int pilihan;
    char nama[50], item[50], hari[20];

    // 1. Inisialisasi Data Saat Program Jalan (Fungsi Hafidhul) [cite: 215]
    loadDataFromFile();

    // 2. Looping Menu Utama do-while
    do {
        printf("\n=== KANTIN FAKULTAS MIPA USK ===\n");
        printf("1. Cari Menu (Fitur Hafidhul & Shifa)\n");
        printf("2. Tambah Pesanan Baru (Masuk Antrean)\n");
        printf("3. Proses Antrean (Layanin)\n");
        printf("4. Batal / Undo Transaksi Terakhir (Fitur Erdian)\n");
        printf("5. Laporan & Rekomendasi (Fitur Aulia & Junita)\n");
        printf("6. Keluar\n");
        printf("================================\n");
        printf("Pilih aksi (1-6): ");
        scanf("%d", &pilihan);

        // Bersihkan buffer enter dari scanf sebelumnya biar gets/scanf string aman
        getchar(); 

        switch (pilihan) {
            case 1:
                // TODO: Panggil fungsi Binary Search punya Shifa [cite: 222]
                printf("\nFitur Pencarian masih dalam tahap pengerjaan (Tunggu Shifa!)...\n");
                break;
                
            case 2:
                // Cabang 2: Tambah Pesanan Baru [cite: 226]
                printf("\n--- FORM PESANAN BARU ---\n");
                printf("Masukkan Nama Pembeli : ");
                scanf("%[^\n]s", nama);
                getchar(); // Bersihkan buffer
                
                printf("Masukkan Nama Jajanan : ");
                scanf("%[^\n]s", item);
                getchar(); 
                
                // INI YANG WAJIB DITANYAIN SESUAI GAMBAR!
                printf("Masukkan Hari Pembelian (cth: Senin): ");
                scanf("%[^\n]s", hari);
                getchar(); 

                enqueue(nama, item, hari);
                break;
                
            case 3:
                // Cabang 3: Pemrosesan Antrean [cite: 229]
                dequeueProcess();
                break;
                
            case 4:
                // Cabang 4: Fitur Batal / Undo [cite: 237]
                undoTransaksi();
                break;
                
            case 5:
                // Cabang 5: Laporan Tren Penjualan Harian [cite: 242]
                // TODO: Panggil fungsi Quick Sort & Print punya kelompok Laporan
                printf("\nFitur Laporan masih dalam tahap pengerjaan...\n");
                break;
                
            case 6:
                // Cabang 6: Keluar Sistem [cite: 246]
                printf("\nMenutup sistem... Sampai jumpa!\n");
                break;
                
            default:
                printf("\n[-] Pilihan tidak valid! Silakan masukkan angka 1-6.\n");
        }
    } while (pilihan != 6);

    return 0;
}
