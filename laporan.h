#ifndef LAPORAN_H
#define LAPORAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================================
// STRUKTUR MENU MAKANAN
// ================================
typedef struct Menu {
    char nama[50];
    float harga;
    int stok;
    int terjual;
    struct Menu* next;
} Menu;

// ================================
// FUNGSI LAPORAN
// ================================
void insertionSort(Menu arr[], int n);
void tampilkanLaporan(Menu* head);
float hitungTotalPendapatan(Menu arr[], int n);
int hitungJumlahMenu(Menu* head);
void copyLinkedListKeArray(Menu* head, Menu arr[]);

#endif
