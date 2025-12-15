#include <stdio.h>

// Definisi kode warna
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define RESET   "\033[0m"

int main() {
    printf(RED "Ini teks berwarna merah\n" RESET);
    printf(GREEN "Ini teks berwarna hijau\n" RESET);
    printf(BLUE "Ini teks berwarna biru\n" RESET);
    
    printf("Kembali ke warna normal\n");

    // Kombinasi background kuning dengan tulisan biru
    printf("\033[43;34m Teks Biru di Background Kuning \033[0m\n");

    return 0;
}