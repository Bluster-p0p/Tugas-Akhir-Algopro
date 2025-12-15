#include <stdio.h>
#include <omp.h> // Pustaka yang diperlukan untuk OpenMP

int main() {
    int N = 10; // Jumlah iterasi perulangan
    
    printf("--- Memulai Eksekusi Perulangan Paralel ---\n");
    
    // #pragma omp parallel for adalah 'arahan' kunci
    // yang memberitahu kompiler untuk membagi perulangan
    // di bawahnya di antara beberapa thread yang tersedia.

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        // Mendapatkan ID (nomor) dari thread yang sedang menjalankan iterasi ini
        int thread_id = omp_get_thread_num();
        
        // Mendapatkan jumlah total thread yang berjalan
        int total_threads = omp_get_num_threads();
        
        // Mencetak hasil. Karena ini berjalan secara paralel,
        // urutan cetakannya mungkin tidak berurutan (0, 1, 2, ...).
        printf("Iterasi %d dikerjakan oleh Thread %d (dari %d thread)\n", 
               i, thread_id, total_threads);
    } // Akhir dari blok paralel
    
    printf("--- Perulangan Paralel Selesai ---\n");
    
    return 0;
}