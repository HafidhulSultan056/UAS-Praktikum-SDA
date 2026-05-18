#include "laporan.h"

int main() {

    // ======================================
    // HEAD LINKED LIST
    // ======================================
    Menu* head = NULL;

    // ======================================
    // MENAMBAHKAN DATA MENU
    // ======================================
    tambahMenu(
        &head,
        "Ayam Geprek",
        15000,
        30,
        25
    );

    tambahMenu(
        &head,
        "Mie Ayam",
        12000,
        20,
        18
    );

    tambahMenu(
        &head,
        "Es Teh Manis",
        5000,
        50,
        40
    );

    tambahMenu(
        &head,
        "Kopi",
        7000,
        40,
        30
    );

    tambahMenu(
        &head,
        "Risol Mayo",
        3000,
        100,
        75
    );

    // ======================================
    // MENAMPILKAN LAPORAN
    // ======================================
    tampilkanLaporan(head);

    return 0;
}
