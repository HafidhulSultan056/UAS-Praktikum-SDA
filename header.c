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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- TAMBAHAN STRUCT KHUSUS STACK (PENTING!) ---
// Struct ini cuma nyimpen 'alamat memori' dari Node Riwayat
typedef struct StackNode {
    struct Node* historyRef; 
    struct StackNode* next;
} StackNode;

// Ubah variabel global topStack di kodingan gabungan jadi seperti ini:
StackNode* topStackData = NULL; 

// ==========================================
// KODE BAGIAN ERDIAN (LINKED LIST & STACK UNDO)
// ==========================================

// 1. Simpan Riwayat (Linked List)
// Fungsi ini dipanggil sama Ejott (Reza) SETELAH dia berhasil nge-Dequeue antrean
Node* simpanRiwayat(char* nama_pembeli, char* item, float total, char* hari) {
    // Bikin node riwayat baru
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->nama_pembeli, nama_pembeli);
    strcpy(newNode->item, item);
    newNode->total = total;
    strcpy(newNode->hari, hari);
    newNode->next = NULL;

    // Masukin ke Linked List Riwayat (Insert at Tail biar urut dari transaksi pertama ke terakhir)
    if (headHistory == NULL) {
        headHistory = newNode;
    } else {
        Node* temp = headHistory;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    
    // Wajib di-return pointernya biar bisa dilempar ke fungsi Push Stack
    return newNode; 
}

// 2. Push Stack (LIFO)
// Menerima alamat pointer dari fungsi simpanRiwayat di atas
void pushUndo(Node* nodeRiwayat) {
    StackNode* newStack = (StackNode*)malloc(sizeof(StackNode));
    newStack->historyRef = nodeRiwayat; // Simpan alamat aslinya
    newStack->next = topStackData;
    topStackData = newStack;
}

// Fitur Bantuan: Cari Menu di Hash Table (Biar bisa balikin stok)
// Kita panggil fungsi hashFunction() dari kodinganmu (Hafidhul)
Menu* cariMenuUntukUndo(char* nama_item) {
    int index = hashFunction(nama_item); 
    Menu* temp = hashTable[index];
    
    while (temp != NULL) {
        if (strcmp(temp->nama, nama_item) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL; // Kalau anehnya nggak ketemu
}

// 3. Fitur Undo (Batal) - Dipanggil di Main Menu
void undoTransaksi() {
    // Cek dulu, ada yang bisa di-undo nggak?
    if (topStackData == NULL) {
        printf("Gagal: Tidak ada riwayat transaksi yang bisa dibatalkan!\n");
        return;
    }

    // A. Pop dari Stack: Ambil data pointer paling atas (terakhir transaksi)
    StackNode* poppedStack = topStackData;
    Node* targetNode = poppedStack->historyRef; // Ini alamat node di Linked List
    topStackData = poppedStack->next; // Turunin top Stack-nya

    // B. Balikin jumlah stok jajanan (Update Database)
    Menu* menuTerkait = cariMenuUntukUndo(targetNode->item);
    if (menuTerkait != NULL) {
        menuTerkait->stok += 1;    // Balikin stok 1 (Asumsi per transaksi 1 item)
        menuTerkait->terjual -= 1; // Kurangi jumlah terjual
    }

    // C. Hapus nodenya dari Linked List Riwayat
    if (headHistory == targetNode) {
        // Kalau yang mau dihapus kebetulan ada di ujung depan (head)
        headHistory = headHistory->next;
    } else {
        // Cari node sebelumnya (prev) buat nyambungin jembatan yang putus
        Node* temp = headHistory;
        while (temp != NULL && temp->next != targetNode) {
            temp = temp->next;
        }
        if (temp != NULL) {
            temp->next = targetNode->next; // Lompatin targetNode
        }
    }

    printf("\n[!] UNDO SUKSES [!]\n");
    printf("Transaksi atas nama '%s' (Item: %s) berhasil dibatalkan.\n", targetNode->nama_pembeli, targetNode->item);
    if (menuTerkait != NULL) {
        printf("Stok '%s' telah dikembalikan menjadi %d.\n", menuTerkait->nama, menuTerkait->stok);
    }

    // Bersihin sampah memori biar efisien
    free(targetNode);
    free(poppedStack);
}
