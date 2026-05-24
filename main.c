#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

// ==========================================
// 1. DEKLARASI STRUCT & VARIABEL GLOBAL
// ==========================================

// Struktur untuk menu makanan
typedef struct Menu {
    char nama[50];
    float harga;
    int stok;
    int terjual;
    struct Menu* next; // Pointer untuk chaining Hash Table
} Menu;

// Struktur untuk node linked list pembeli & antrean
typedef struct Node {
    char nama_pembeli[50];
    char item[50];
    float total_harga;
    char hari[20];
    struct Node* next;
} Node;

// Tambahan Struct khusus Stack untuk fitur Batal (Erdian)
typedef struct StackNode {
    struct Node* historyRef; 
    struct StackNode* next;
} StackNode;

// Struct khusus untuk fitur Analisis & DSS (Junita)
typedef struct {
    char item[50];
    int stok;
    int totalTerjual;
    int prediksi;
    int rekomendasiStok;
} Analisis;

// Variabel Global
Menu* hashTable[TABLE_SIZE];   // Hash table untuk menyimpan menu makanan
Node* headQueue = NULL;        // Head dari linked list pembeli
Node* tailQueue = NULL;        // Tail dari linked list pembeli
Node* headHistory = NULL;      // Head dari linked list history pembelian
StackNode* topStackData = NULL;// Top dari stack untuk undo transaksi

// ==========================================
// 2. KODE BAGIAN HAFIDHUL SULTAN (HASH TABLE & I/O)
// ==========================================
void initHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

int hashFunction(char* nama) {
    int hash = 0;
    for (int i = 0; i < strlen(nama); i++) {
        hash += nama[i];
    }
    return hash % TABLE_SIZE;
}

void insertMenu(char* nama, float harga, int stok) {
    int index = hashFunction(nama);
    Menu* newMenu = (Menu*)malloc(sizeof(Menu));
    strcpy(newMenu->nama, nama);
    newMenu->harga = harga;
    newMenu->stok = stok;
    newMenu->terjual = 0; 
    newMenu->next = NULL;

    if (hashTable[index] == NULL) {
        hashTable[index] = newMenu;
    } else {
        newMenu->next = hashTable[index];
        hashTable[index] = newMenu;
    }
}

void loadDataFromFile() {
    initHashTable();
    FILE* file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("\nPeringatan: File data.txt tidak ditemukan. Database kosong!\n");
        return;
    }
    char nama[50];
    float harga;
    int stok;
    while (fscanf(file, " %49[^,],%f,%d", nama, &harga, &stok) == 3) {
        insertMenu(nama, harga, stok);
    }
    fclose(file);
    printf("\nSukses: Database menu berhasil dimuat dari data.txt!\n");
}

// Fungsi Bantuan untuk mencari Menu (Dipakai Erdian & Reza)
Menu* cariMenuDiHash(char* nama_item) {
    int index = hashFunction(nama_item); 
    Menu* temp = hashTable[index];
    while (temp != NULL) {
        if (strcmp(temp->nama, nama_item) == 0) return temp;
        temp = temp->next;
    }
    return NULL; 
}

void tambahStok() {
    char target[50];
    int tambahan;
    
    printf("\n--- FITUR TAMBAH STOK ---\n");
    printf("Masukkan nama menu yang mau ditambah stoknya: ");
    scanf(" %[^\n]", target);

    // Langsung tembak pake fungsi cari yang udah ada
    Menu* menuTerkait = cariMenuDiHash(target);
    
    if (menuTerkait != NULL) {
        printf("Stok '%s' saat ini: %d porsi\n", menuTerkait->nama, menuTerkait->stok);
        printf("Masukkan jumlah stok yang baru datang: ");
        scanf("%d", &tambahan);
        
        menuTerkait->stok += tambahan; // Ini intinya, nambahin stok!
        
        printf("\n[+] SUKSES: Stok '%s' berhasil ditambah! Total sekarang: %d porsi.\n", 
            menuTerkait->nama, menuTerkait->stok);
    } else {
        printf("\n[-] GAGAL: Menu '%s' nggak ketemu Jot! Cek lagi huruf kapitalnya.\n", target);
    }
}


// ==========================================
// 3. KODE BAGIAN SHIFA (BINARY SEARCH)
// ==========================================
int kumpulkanMenuKeArray(Menu* arr[]) {
    int count = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Menu* temp = hashTable[i];
        while (temp != NULL) {
            arr[count] = temp;
            count++;
            temp = temp->next;
        }
    }
    return count; 
}

void sortMenuArrayAlphabet(Menu* arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j]->nama, arr[j+1]->nama) > 0) {
                Menu* temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

Menu* binarySearch(Menu* arr[], int n, char* targetNama) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(arr[mid]->nama, targetNama);
        if (cmp == 0) {
            return arr[mid]; 
        } else if (cmp < 0) {
            low = mid + 1;   
        } else {
            high = mid - 1;  
        }
    }
    return NULL; 
}

void eksekusiPencarianShifa() {
    Menu* arrayPencarian[100]; 
    int totalMenu = kumpulkanMenuKeArray(arrayPencarian);
    sortMenuArrayAlphabet(arrayPencarian, totalMenu);
    
    printf("\n--- PENCARIAN MENU ---\n");
    
    // --- TAMBAHAN DARI A.B.E BIAR MENU TAMPIL ---
    printf("Daftar Menu Tersedia di Kantin:\n");
    for (int i = 0; i < totalMenu; i++) {
        printf("%d. %s\n", i + 1, arrayPencarian[i]->nama);
    }
    printf("------------------------------\n");
    // --------------------------------------------

    char target[50];
    printf("Masukkan nama jajanan yang dicari: ");
    scanf(" %[^\n]", target);
    
    Menu* hasil = binarySearch(arrayPencarian, totalMenu, target);
    if (hasil != NULL) {
        printf("\n[DITEMUKAN] Menu: %s | Harga: Rp%.0f | Sisa Stok: %d | Terjual: %d\n", 
            hasil->nama, hasil->harga, hasil->stok, hasil->terjual);
    } else {
        printf("\n[-] Maaf, menu '%s' tidak ditemukan di pangkalan data!\n", target);
    }
}

// ==========================================
// 4. KODE BAGIAN ERDIAN (LINKED LIST & STACK)
// ==========================================
Node* simpanRiwayat(char* nama_pembeli, char* item, float total_harga, char* hari) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->nama_pembeli, nama_pembeli);
    strcpy(newNode->item, item);
    newNode->total_harga = total_harga;
    strcpy(newNode->hari, hari);
    newNode->next = NULL;

    if (headHistory == NULL) {
        headHistory = newNode;
    } else {
        Node* temp = headHistory;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
    return newNode; 
}

void pushUndo(Node* nodeRiwayat) {
    StackNode* newStack = (StackNode*)malloc(sizeof(StackNode));
    newStack->historyRef = nodeRiwayat; 
    newStack->next = topStackData;
    topStackData = newStack;
}

void undoTransaksi() {
    if (topStackData == NULL) {
        printf("\nGagal: Tidak ada riwayat transaksi yang bisa dibatalkan!\n");
        return;
    }

    StackNode* poppedStack = topStackData;
    Node* targetNode = poppedStack->historyRef; 
    topStackData = poppedStack->next; 

    Menu* menuTerkait = cariMenuDiHash(targetNode->item);
    if (menuTerkait != NULL) {
        menuTerkait->stok += 1;    
        menuTerkait->terjual -= 1; 
    }

    if (headHistory == targetNode) {
        headHistory = headHistory->next;
    } else {
        Node* temp = headHistory;
        while (temp != NULL && temp->next != targetNode) {
            temp = temp->next;
        }
        if (temp != NULL) {
            temp->next = targetNode->next; 
        }
    }

    printf("\n[!] UNDO SUKSES [!]\n");
    printf("Transaksi atas nama '%s' (Item: %s) berhasil dibatalkan.\n", targetNode->nama_pembeli, targetNode->item);
    free(targetNode);
    free(poppedStack);
}

// ==========================================
// 5. KODE BAGIAN AULIA (INSERTION SORT & LAPORAN)
// ==========================================
void insertionSort(Menu arr[], int n) {
    int i, j;
    Menu key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        // Urutkan descending (terjual terbanyak di atas)
        while (j >= 0 && arr[j].terjual < key.terjual) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void tampilkanLaporan() {
    Menu arrData[100];
    int jumlahMenu = 0;
    
    // Tarik data langsung dari Hash Table
    for (int i = 0; i < TABLE_SIZE; i++) {
        Menu* temp = hashTable[i];
        while (temp != NULL) {
            arrData[jumlahMenu] = *temp;
            jumlahMenu++;
            temp = temp->next;
        }
    }

    if (jumlahMenu == 0) {
        printf("\nData menu kosong!\n");
        return;
    }

    insertionSort(arrData, jumlahMenu);

    float totalPendapatan = 0;
    for (int i = 0; i < jumlahMenu; i++) {
        totalPendapatan += (arrData[i].harga * arrData[i].terjual);
    }

    printf("\n==================================================================\n");
    printf("                   LAPORAN PENJUALAN KANTIN\n");
    printf("==================================================================\n");
    printf("%-5s %-20s %-10s %-10s %-15s\n", "No", "Nama Menu", "Harga", "Terjual", "Pendapatan");
    printf("------------------------------------------------------------------\n");

    for (int i = 0; i < jumlahMenu; i++) {
        float pendapatan = arrData[i].harga * arrData[i].terjual;
        printf("%-5d %-20s Rp%-8.0f %-10d Rp%-10.0f\n", 
            i + 1, arrData[i].nama, arrData[i].harga, arrData[i].terjual, pendapatan);
    }

    printf("------------------------------------------------------------------\n");
    printf("TOTAL PENDAPATAN : Rp%.0f\n", totalPendapatan);
    printf("==================================================================\n");
}

// ==========================================
// 6. KODE BAGIAN JUNITA (QUICK SORT & DSS)
// ==========================================
void swapAnalisis(Analisis* a, Analisis* b) {
    Analisis t = *a;
    *a = *b;
    *b = t;
}

int partition(Analisis arr[], int low, int high) {
    int pivot = arr[high].totalTerjual;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        // Sortir descending
        if (arr[j].totalTerjual > pivot) {
            i++;
            swapAnalisis(&arr[i], &arr[j]);
        }
    }
    swapAnalisis(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(Analisis arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void decisionSupportSystem() {
    Analisis data[100];
    int jumlah = 0;

    // Tarik data dari Hash Table
    for (int i = 0; i < TABLE_SIZE; i++) {
        Menu* temp = hashTable[i];
        while (temp != NULL) {
            strcpy(data[jumlah].item, temp->nama);
            data[jumlah].stok = temp->stok;
            data[jumlah].totalTerjual = temp->terjual;
            jumlah++;
            temp = temp->next;
        }
    }

    if (jumlah == 0) return;

    // Gunakan Quick Sort Sesuai Proposal
    quickSort(data, 0, jumlah - 1);

    printf("\n===== SISTEM PENDUKUNG KEPUTUSAN (DSS) =====\n");
    for (int i = 0; i < jumlah; i++) {
        data[i].prediksi = data[i].totalTerjual + 2;
        if (data[i].totalTerjual >= data[i].stok * 0.8)
            data[i].rekomendasiStok = data[i].stok + 3;
        else if (data[i].totalTerjual >= data[i].stok * 0.5)
            data[i].rekomendasiStok = data[i].stok;
        else
            data[i].rekomendasiStok = data[i].stok - 2;

        if(data[i].rekomendasiStok < 0) data[i].rekomendasiStok = 0;
    }
    
    printf("1. MENU TERLARIS HARI INI: %s (%d terjual)\n", data[0].item, data[0].totalTerjual);
    printf("   => Rekomendasi: Tambah stok besok menjadi %d porsi!\n\n", data[0].rekomendasiStok);
    
    printf("2. MENU KURANG DIMINATI  : %s (%d terjual)\n", data[jumlah - 1].item, data[jumlah - 1].totalTerjual);
    printf("   => Rekomendasi: Kurangi stok besok menjadi %d porsi untuk meminimalisir sisa.\n", data[jumlah - 1].rekomendasiStok);
    printf("============================================\n");
}

// ==========================================
// 7. KODE BAGIAN EJOT / REZA (QUEUE & MAIN)
// ==========================================
void enqueue(char* nama_pembeli, char* item, char* hari) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->nama_pembeli, nama_pembeli);
    strcpy(newNode->item, item);
    newNode->total_harga = 0; 
    strcpy(newNode->hari, hari); 
    newNode->next = NULL;

    if (tailQueue == NULL) {
        headQueue = tailQueue = newNode;
    } else {
        tailQueue->next = newNode;
        tailQueue = newNode;
    }
    printf("\n[+] BERHASIL: Pesanan '%s' atas nama %s masuk ke antrean!\n", item, nama_pembeli);
}

void dequeueProcess() {
    if (headQueue == NULL) {
        printf("\n[-] GAGAL: Antrean saat ini kosong. Kantin lagi sepi, Jot!\n");
        return;
    }

    printf("\n======================================================\n");
    printf("         MEMPROSES SEMUA PESANAN DI ANTREAN!\n");
    printf("======================================================\n");

    int nomorAntrean = 1;

    // LOOPING: Bakal terus jalan sampai antrean kosong (headQueue jadi NULL)
    while (headQueue != NULL) {
        Node* temp = headQueue;
        headQueue = headQueue->next;
        
        // Kalau setelah diambil ternyata habis, tail juga di-set NULL
        if (headQueue == NULL) {
            tailQueue = NULL;
        }

        float harga_final = 0;
        Menu* menuDipesan = cariMenuDiHash(temp->item); 
        
        if (menuDipesan != NULL) {
            if(menuDipesan->stok > 0) {
                harga_final = menuDipesan->harga;
                menuDipesan->stok -= 1;     
                menuDipesan->terjual += 1;  
            } else {
                printf("[!] Peringatan: Stok '%s' habis! Transaksi %s batal.\n", temp->item, temp->nama_pembeli);
                free(temp);
                continue; // Lanjut ke orang berikutnya di antrean
            }
        } else {
            printf("[!] Peringatan: Menu '%s' nggak ada di database, harga Rp0.\n", temp->item);
        }

        // Sambung ke Fitur Erdian buat simpan history & stack undo
        Node* riwayatBaru = simpanRiwayat(temp->nama_pembeli, temp->item, harga_final, temp->hari);
        pushUndo(riwayatBaru);

        // Cetak struk massal per baris biar rapi
        printf("%d. Pembeli: %-10s | Item: %-15s | Bayar: Rp%-6.0f | Hari: %s\n", 
               nomorAntrean, temp->nama_pembeli, temp->item, harga_final, temp->hari);

        free(temp); // Jangan lupa buang sampah memori
        nomorAntrean++;
    }
    
    printf("======================================================\n");
    printf("[+] SEMUA ANTREAN BERHASIL DIPROSES!\n");
}

int main() {
    int pilihan;
    char nama[50], item[50], hari[20];

    loadDataFromFile();

    do {
        printf("\n=== KANTIN FAKULTAS MIPA USK ===\n");
        printf("1. Cari Menu (Binary Search - Shifa)\n");
        printf("2. Tambah Pesanan (Enqueue - Reza)\n");
        printf("3. Proses Antrean (Dequeue - Reza)\n");
        printf("4. Batal/Undo Transaksi (Stack - Erdian)\n");
        printf("5. Laporan Keuangan & DSS (Aulia & Junita)\n");
        printf("6. Tambah Stok Menu Kantin\n"); // <-- INI YANG BARU
        printf("7. Keluar\n");                  // <-- KELUAR JADI NOMOR 7
        printf("================================\n");
        printf("Pilih aksi (1-7): ");
        scanf("%d", &pilihan);
        getchar(); 

        switch (pilihan) {
            case 1:
                eksekusiPencarianShifa();
                break;
            case 2:
                printf("\n--- TAMBAH PESANAN KE ANTREAN ---\n");
                printf("Nama Pembeli : ");
                scanf(" %[^\n]", nama);
                
                printf("Pesanan      : ");
                scanf(" %[^\n]", item);
                
                printf("Hari         : ");
                scanf(" %[^\n]", hari);
                
                // Panggil fungsi enqueue buatan lo
                enqueue(nama, item, hari);
                break;
            case 3:
                dequeueProcess();
                break;
            case 4:
                undoTransaksi();
                break;
            case 5:
                tampilkanLaporan();         
                decisionSupportSystem();    
                break;
            case 6: // <-- CASE BARU BUAT TAMBAH STOK
                tambahStok();
                break;
            case 7: // <-- KELUAR SEKARANG DI CASE 7
                printf("\nMenutup sistem... Sampai jumpa!\n");
                break;
            default:
                printf("\n[-] Pilihan tidak valid!\n");
        }
    } while (pilihan != 7); // <-- JANGAN LUPA INI DIGANTI JADI 7 JOTT!

    return 0;
}
