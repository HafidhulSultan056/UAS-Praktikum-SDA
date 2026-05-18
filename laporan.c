#include "laporan.h"
        arr[j + 1] = key;
    }
}

// =====================================================
// Menghitung total seluruh pendapatan kantin
// =====================================================
float hitungTotalPendapatan(Menu arr[], int n) {
    float total = 0;

    for (int i = 0; i < n; i++) {
        total += arr[i].harga * arr[i].terjual;
    }

    return total;
}

// =====================================================
// Menampilkan laporan penjualan harian
// =====================================================
void tampilkanLaporan(Menu* head) {

    // Mengecek apakah data kosong
    if (head == NULL) {
        printf("\nTidak ada data menu!\n");
        return;
    }

    // Menghitung jumlah menu
    int jumlahMenu = hitungJumlahMenu(head);

    // Membuat array sementara
    Menu arr[jumlahMenu];

    // Copy linked list ke array
    copyLinkedListKeArray(head, arr);

    // Sorting data
    insertionSort(arr, jumlahMenu);

    // Menghitung total pendapatan
    float totalPendapatan = hitungTotalPendapatan(arr, jumlahMenu);

    // =========================================
    // OUTPUT LAPORAN
    // =========================================
    printf("\n==============================================");
    printf("\n      LAPORAN PENJUALAN HARIAN KANTIN");
    printf("\n==============================================\n");

    printf("%-5s %-20s %-10s %-10s %-15s\n",
           "No",
           "Nama Menu",
           "Harga",
           "Terjual",
           "Pendapatan");

    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < jumlahMenu; i++) {

        float pendapatanItem = arr[i].harga * arr[i].terjual;

        printf("%-5d %-20s Rp%-8.0f %-10d Rp%-10.0f\n",
               i + 1,
               arr[i].nama,
               arr[i].harga,
               arr[i].terjual,
               pendapatanItem);
    }

    printf("---------------------------------------------------------------\n");

    printf("TOTAL PENDAPATAN KANTIN : Rp%.0f\n", totalPendapatan);

    printf("==============================================\n");
