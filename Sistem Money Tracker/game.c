#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // Untuk input keyboard (_kbhit, _getch)
#include <windows.h> // Untuk fungsi Sleep dan kontrol kursor
#include <time.h>   // Untuk angka acak

// Konfigurasi Area Game
#define WIDTH 10
#define HEIGHT 20

// Definisi kode warna
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define RESET   "\033[0m"

int playerX;
int score;
int coinX, coinY;
int gameOver;
int speed = 50;

// Fungsi untuk menyembunyikan kursor agar tampilan lebih bersih
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

// Fungsi untuk memindahkan posisi kursor (menghindari flickering dibanding system("cls"))
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setup() {
    gameOver = 0;
    score = 0;
    playerX = WIDTH / 2; // Pemain mulai di tengah
    
    // Posisi awal koin
    srand(time(0));
    coinX = rand() % (WIDTH - 2) + 1;
    coinY = 0;
    
    hideCursor();
}

void draw() {
    // Kembali ke titik 0,0 daripada menghapus layar (mengurangi kedip)
    gotoxy(0, 0);

    // Gambar batas atas
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    // Gambar area permainan
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) printf("#"); // Dinding kiri

            if (i == coinY && j == coinX) {
                printf(YELLOW "$" RESET); // Gambar Koin
            }
            else if (i == HEIGHT - 1 && j == playerX) {
                printf("U"); // Gambar Pemain (Keranjang)
            }
            else {
                printf(" "); // Ruang kosong
            }

            if (j == WIDTH - 1) printf("#"); // Dinding kanan
        }
        printf("\n");
    }

    // Gambar batas bawah
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    // Tampilkan Skor & Kontrol
    printf("Skor: %d\n", score);
    printf("Kontrol: 'a' (Kiri) | 'd' (Kanan) | 'x' (Keluar)\n");
}

void input() {
    if (_kbhit()) { // Mengecek apakah ada tombol yang ditekan
        char current = _getch();
        switch (current) {
            case 'a': // Gerak Kiri
                if (playerX > 0) playerX--;
                break;
            case 'd': // Gerak Kanan
                if (playerX < WIDTH - 1) playerX++;
                break;
            case 'x': // Keluar game
                gameOver = 1;
                break;
        }
    }
}

void logic() {
    // Gerakkan koin ke bawah
    coinY++;

    // Cek apakah koin tertangkap
    if (coinY == HEIGHT - 1) {
        if (coinX == playerX) {
            score += 10; // Tambah skor
            // Reset koin ke atas di posisi acak baru
            coinX = rand() % (WIDTH - 2) + 1;
            coinY = 0;
        }
    }

    // Cek jika koin menyentuh tanah (melewati pemain)
    if (coinY >= HEIGHT) {
        // Game Over jika koin jatuh (opsional, bisa diubah jadi kurangi nyawa)
        gameOver = 1; 
    }
}

int main() {
    setup();

    while (!gameOver) {
        draw();
        input();
        logic();
        Sleep(speed); // Mengatur kecepatan game (makin kecil makin cepat)
    }

    // Pesan Game Over
    gotoxy(0, HEIGHT + 4);
    printf("\n=== GAME OVER ===\n");
    printf("Skor Akhir Anda: %d\n", score);
    printf("Tekan apa saja untuk keluar...");
    _getch();

    return 0;
}