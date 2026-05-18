#include "laporan.h"

// ======================================
// Membuat node menu baru
// ======================================
Menu* buatMenu(
    char nama[],
    float harga,
    int stok,
    int terjual
) {

    Menu* baru =
        (Menu*) malloc(sizeof(Menu));

    strcpy(baru->nama, nama);

    baru->harga = harga;
    baru->stok = stok;
    baru->terjual = terjual;

    baru->next = NULL;

    return baru;
}

// ======================================
// Menambahkan menu ke linked list
// ======================================
void tambahMenu(
    Menu** head,
    char nama[],
    float harga,
    int stok,
    int terjual
) {

    Menu* baru =
        buatMenu(
            nama,
            harga,
            stok,
            terjual
        );

    // Jika list kosong
    if (*head == NULL) {

        *head = baru;
        return;
    }

    // Cari node terakhir
    Menu* temp = *head;

    while (temp->next != NULL) {

        temp = temp->next;
    }

    temp->next = baru;
}

// ======================================
// Menghitung jumlah menu
// ======================================
int hitungJumlahMenu(Menu* head) {

    int jumlah = 0;

    while (head != NULL) {

        jumlah++;
        head = head->next;
    }

    return jumlah;
}

// ======================================
// Copy linked list ke array
// ======================================
void copyLinkedListKeArray(
    Menu* head,
    Menu arr[]
) {

    int i = 0;

    while (head != NULL) {

        arr[i] = *head;

        head = head->next;
        i++;
    }
}

// ======================================
// INSERTION SORT
// Mengurutkan berdasarkan jumlah
// terjual terbesar ke terkecil
// ======================================
void insertionSort(
    Menu arr[],
    int n
) {

    int i;
    int j;

    Menu key;

    for (i = 1; i < n; i++) {

        key = arr[i];

        j = i - 1;

        while (
            j >= 0 &&
            arr[j].terjual < key.terjual
        ) {

            arr[j + 1] = arr[j];

            j--;
        }

        arr[j + 1] = key;
    }
}

// ======================================
// Menghitung total pendapatan
// ======================================
float hitungTotalPendapatan(
    Menu arr[],
    int n
) {

    float total = 0;

    for (int i = 0; i < n; i++) {

        total +=
            arr[i].harga *
            arr[i].terjual;
    }

    return total;
}

// ======================================
// Menampilkan laporan penjualan
// ======================================
void tampilkanLaporan(
    Menu* head
) {

    if (head == NULL) {

        printf(
            "\nData menu kosong!\n"
        );

        return;
    }

    int jumlahMenu =
        hitungJumlahMenu(head);

    Menu arr[jumlahMenu];

    copyLinkedListKeArray(
        head,
        arr
    );

    insertionSort(
        arr,
        jumlahMenu
    );

    float totalPendapatan =
        hitungTotalPendapatan(
            arr,
            jumlahMenu
        );

    printf(
        "\n===================================================="
    );

    printf(
        "\n           LAPORAN PENJUALAN KANTIN"
    );

    printf(
        "\n====================================================\n"
    );

    printf(
        "%-5s %-20s %-10s %-10s %-15s\n",
        "No",
        "Nama Menu",
        "Harga",
        "Terjual",
        "Pendapatan"
    );

    printf(
        "-------------------------------------------------------------\n"
    );

    for (int i = 0; i < jumlahMenu; i++) {

        float pendapatan =
            arr[i].harga *
            arr[i].terjual;

        printf(
            "%-5d %-20s Rp%-8.0f %-10d Rp%-10.0f\n",
            i + 1,
            arr[i].nama,
            arr[i].harga,
            arr[i].terjual,
            pendapatan
        );
    }

    printf(
        "-------------------------------------------------------------\n"
    );

    printf(
        "TOTAL PENDAPATAN : Rp%.0f\n",
        totalPendapatan
    );

    printf(
        "====================================================\n"
    );
}
