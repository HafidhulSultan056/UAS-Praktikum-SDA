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

// 1. Fungsi Binary Search Utama 
// Menerima array of pointer ke Menu yang SUDAH TERURUT alfabetis
Menu* binarySearch(Menu* arr[], int n, char* targetNama) {
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        // Implementasi strcmp: membandingkan nama menu dengan target
        int cmp = strcmp(arr[mid]->nama, targetNama);

        if (cmp == 0) {
            return arr[mid]; // Data ditemukan!
        } 
        else if (cmp < 0) {
            low = mid + 1;   // Target lebih besar (secara alfabet), geser batas bawah
        } 
        else {
            high = mid - 1;  // Target lebih kecil (secara alfabet), geser batas atas
        }
    }
    
    return NULL; // Data tidak ditemukan
}

// 2. Fungsi Bantuan: Mengumpulkan data dari Hash Table  ke Array
int kumpulkanMenuKeArray(Menu* arr[]) {
    int count = 0;
    
    // Looping seluruh index Hash Table
    for (int i = 0; i < TABLE_SIZE; i++) {
        Menu* temp = hashTable[i];
        
        // Looping chaining (Linked List) di setiap index
        while (temp != NULL) {
            arr[count] = temp;
            count++;
            temp = temp->next;
        }
    }
    return count; // Mengembalikan total menu yang berhasil dikumpulkan
}

// 3. Fungsi Bantuan: Mengurutkan Array Alfabetis agar bisa di-Binary Search
void sortMenuArray(Menu* arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Jika string pertama lebih besar dari string kedua, tukar posisinya
            if (strcmp(arr[j]->nama, arr[j+1]->nama) > 0) {
                Menu* temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

// --- CONTOH CARA SHIFA MEMANGGIL FUNGSINYA DI DALAM main() ---
/*
    // Siapkan array kosong sebesar jumlah kemungkinan menu
    Menu* arrayPencarian[100]; 
    
    // Kumpulkan dan hitung total data
    int totalMenu = kumpulkanMenuKeArray(arrayPencarian);
    
    // Urutkan datanya
    sortMenuArray(arrayPencarian, totalMenu);
    
    // Cari menunya pakai Binary Search
    char target[50] = "Mie Goreng"; // Contoh yang dicari
    Menu* hasil = binarySearch(arrayPencarian, totalMenu, target);
    
    if (hasil != NULL) {
        printf("Ketemu! %s harganya %.2f\n", hasil->nama, hasil->harga);
    } else {
        printf("Menu tidak ditemukan di database!\n");
    }
*/
