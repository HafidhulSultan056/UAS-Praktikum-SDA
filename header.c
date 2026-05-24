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
