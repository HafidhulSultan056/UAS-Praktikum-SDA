#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1

// Struktur untuk menu makanan
typedef struct Menu {
    char nama[50];
    float harga;
    int stok;
    int terjual;
    struct Menu* next; // Pointer ke menu berikutnya
} Menu;

// Struktur untuk node linked list pembeli
typedef struct Node {
    char nama_pembeli [50];
    char item[50];
    float total_harga;
    char hari[20];
    struct Node* next;
}

Menu* hashTable[TABLE_SIZE]; // Hash table untuk menyimpan menu makanan
Node* headQueue = NULL; // Head dari linked list pembeli
Node* tailQueue = NULL; // Tail dari linked list pembeli
Node* headHistory = NULL; // Head dari linked list history pembelian
Node* topStack = NULL; // Top dari stack untuk pembeli terakhir

void initHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

// 2. Hash Table & Hashing: Mengubah string nama menu jadi index array
int hashFunction(char* nama) {
    int hash = 0;
    // Menggunakan penjumlahan nilai ASCII dari setiap karakter
    for (int i = 0; i < strlen(nama); i++) {
        hash += nama[i];
    }
    return hash % TABLE_SIZE;
}

// 3. Insert & Collision: Memasukkan data ke Hash Table dengan sistem Chaining
void insertMenu(char* nama, float harga, int stok) {
    int index = hashFunction(nama);

    // Alokasi memori untuk node menu baru
    Menu* newMenu = (Menu*)malloc(sizeof(Menu));
    strcpy(newMenu->nama, nama);
    newMenu->harga = harga;
    newMenu->stok = stok;
    newMenu->terjual = 0; // Default awal
    newMenu->next = NULL;

    // Handle Collision dengan Chaining (Insert at Head)
    if (hashTable[index] == NULL) {
        hashTable[index] = newMenu;
    } else {
        newMenu->next = hashTable[index];
        hashTable[index] = newMenu;
    }
}

// 4. File I/O: Membaca data.txt saat program pertama kali jalan
void loadDataFromFile() {
    // Panggil init dulu biar hash table bersih
    initHashTable();

    FILE* file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("Peringatan: File data.txt tidak ditemukan. Database kosong!\n");
        return;
    }

    char nama[50];
    float harga;
    int stok;

    // Membaca per baris dengan format: Nama Menu,Harga,Stok
    // %49[^,] berarti membaca string sampai ketemu tanda koma (berguna jika ada spasi pada nama)
    while (fscanf(file, " %49[^,],%f,%d", nama, &harga, &stok) == 3) {
        insertMenu(nama, harga, stok);
    }

    fclose(file);
    printf("Sukses: Database menu jajanan berhasil dimuat dari data.txt!\n");
}
