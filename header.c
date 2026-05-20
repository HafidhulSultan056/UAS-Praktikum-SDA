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

typedef struct {
    char item[50];
    int stok;
    int totalTerjual;
    int prediksi;
    int rekomendasiStok;
} Analisis;

// INIT HASH TABLE
void initHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

// HASH FUNCTION
int hashFunction(char* nama) {
    int hash = 0;
    for (int i = 0; i < strlen(nama); i++) {
        hash += nama[i];
    }
    return hash % TABLE_SIZE;
}

// INSERT MENU (DARI FILE)
void insertMenu(char* nama, float harga, int stok) {
    int index = hashFunction(nama);

    Menu* newMenu = (Menu*)malloc(sizeof(Menu));
    strcpy(newMenu->nama, nama);
    newMenu->harga = harga;
    newMenu->stok = stok;
    newMenu->terjual = stok / 10; // simulasi penjualan
    newMenu->next = NULL;

    if (hashTable[index] == NULL) {
        hashTable[index] = newMenu;
    } else {
        newMenu->next = hashTable[index];
        hashTable[index] = newMenu;
    }
}

// LOAD DATA DARI data.txt
void loadDataFromFile() {

    FILE* file = fopen("data.txt", "r");

    if (file == NULL) {
        printf("data.txt tidak ditemukan!\n");
        return;
    }

    char nama[50];
    float harga;
    int stok;

    while (fscanf(file, " %49[^,],%f,%d", nama, &harga, &stok) == 3) {
        insertMenu(nama, harga, stok);
    }

    fclose(file);
}

// AMBIL DATA DARI HASH TABLE
int ambilData(Menu* arr[]) {

    int count = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {

        Menu* temp = hashTable[i];

        while (temp != NULL) {
            arr[count++] = temp;
            temp = temp->next;
        }
    }

    return count;
}

// ANALISIS STRUCT BUILD
void decisionSupportSystem() {

    Menu* arrMenu[100];
    int jumlahMenu = ambilData(arrMenu);

    Analisis data[100];
    int jumlah = 0;

    // KONVERSI MENU -> ANALISIS
    for (int i = 0; i < jumlahMenu; i++) {

        int found = 0;

        for (int j = 0; j < jumlah; j++) {
            if (strcmp(data[j].item, arrMenu[i]->nama) == 0) {
                data[j].totalTerjual += arrMenu[i]->terjual;
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy(data[jumlah].item, arrMenu[i]->nama);
            data[jumlah].stok = arrMenu[i]->stok;
            data[jumlah].totalTerjual = arrMenu[i]->terjual;
            jumlah++;
        }
    }

    // SORTING (DESC TERLARIS)
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - i - 1; j++) {
            if (data[j].totalTerjual < data[j + 1].totalTerjual) {
                Analisis temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }

    // OUTPUT DSS
    printf("\n===== ANALISIS TREN PENJUALAN KANTIN =====\n");

    for (int i = 0; i < jumlah; i++) {

        data[i].prediksi = data[i].totalTerjual + 2;

        if (data[i].totalTerjual >= data[i].stok * 0.8)
            data[i].rekomendasiStok = data[i].stok + 3;
        else if (data[i].totalTerjual >= data[i].stok * 0.5)
            data[i].rekomendasiStok = data[i].stok;
        else
            data[i].rekomendasiStok = data[i].stok - 2;

        printf("\nMenu : %s", data[i].item);
        printf("\nStok : %d", data[i].stok);
        printf("\nTerjual : %d", data[i].totalTerjual);
        printf("\nPrediksi : %d", data[i].prediksi);
        printf("\nRekomendasi Stok : %d\n", data[i].rekomendasiStok);
    }

    printf("\n===== MENU TERLARIS =====\n");
    printf("%s (%d terjual)\n", data[0].item, data[0].totalTerjual);

    printf("\n===== MENU KURANG DIMINATI =====\n");
    printf("%s (%d terjual)\n", data[jumlah - 1].item, data[jumlah - 1].totalTerjual);
}

int main() {

    initHashTable();
    loadDataFromFile();
    decisionSupportSystem();

    return 0;
}

