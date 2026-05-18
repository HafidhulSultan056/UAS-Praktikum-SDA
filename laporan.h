#ifndef LAPORAN_H
#define LAPORAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

// ======================================
// STRUCT MENU
// ======================================
typedef struct Menu {

    char nama[50];
    float harga;
    int stok;
    int terjual;

    struct Menu* next;

} Menu;

// ======================================
// FUNCTION PROTOTYPE
// ======================================
int hitungJumlahMenu(Menu* head);

void copyLinkedListKeArray(
    Menu* head,
    Menu arr[]
);

void insertionSort(
    Menu arr[],
    int n
);

float hitungTotalPendapatan(
    Menu arr[],
    int n
);

void tampilkanLaporan(
    Menu* head
);

Menu* buatMenu(
    char nama[],
    float harga,
    int stok,
    int terjual
);

void tambahMenu(
    Menu** head,
    char nama[],
    float harga,
    int stok,
    int terjual
);

#endif
