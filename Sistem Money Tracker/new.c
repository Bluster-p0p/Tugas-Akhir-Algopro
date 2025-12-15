#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

// --- KONFIGURASI & STRUKTUR DATA ---

#define MAX_DAYS_IN_MONTH 31
#define FILENAME_FORMAT "keuangan_%02d_%d.txt"
#define NUM_EXPENSE_CATEGORIES 9

// Konfigurasi Area Game
#define WIDTH 10
#define HEIGHT 20

// Definisi kode warna
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define RESET   "\033[0m"

// Kategori Pengeluaran
const char *EXPENSE_CATEGORIES[] = {
    "Belanja", "Makanan", "Transportasi", "Kecantikan", 
    "Olahraga", "Pakaian", "Kesehatan", "Edukasi", "Bulanan"
};

// Struktur untuk menyimpan data keuangan per hari
typedef struct {
    int day;
    double income;
    double expenses[9];
} DailyFinance;

// --- VARIABEL GLOBAL ---
int current_month, current_year;
DailyFinance monthData[MAX_DAYS_IN_MONTH];

    // Variabel Game
int playerX;
int score;
int coinX, coinY;
int gameOver;

// --- FUNGSI GAME SEDERHANA ---
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

int runGame() {
    printf("Apakah anda ingin bermain game menangkap koin? (y/n): ");
    char choice;
    scanf(" %c", &choice);
    if (tolower(choice) != 'y') {
        return 0;
    }
    else {
        setup();

        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(120); // Mengatur kecepatan game (makin kecil makin cepat)
        }

        // Pesan Game Over
        gotoxy(0, HEIGHT + 4);
        printf("\n=== GAME OVER ===\n");
        printf("Skor Akhir Anda: %d\n", score);
        printf("Tekan apa saja untuk keluar...");
        _getch();

        return 0;
    }

}

// --- FUNGSI UTILITAS ---

// Membersihkan layar konsol
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Menahan layar agar user bisa membaca
void pause() {
    printf("\nTekan Enter untuk melanjutkan...");
    getchar(); // Menangkap newline dari input sebelumnya
    getchar(); // Menunggu user menekan Enter
}

// Memeriksa apakah tahun adalah tahun kabisat
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Mendapatkan jumlah hari dalam sebulan
int getDaysInMonth(int month, int year) {
    if (month == 2) {
        return isLeapYear(year) ? 29 : 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
    }
}

// Menghasilkan nama file berdasarkan bulan dan tahun
void getFilename(char *buffer, int month, int year) {
    sprintf(buffer, FILENAME_FORMAT, month, year);
}

// --- FUNGSI PENYIMPANAN & PEMUATAN DATA ---

// Menyimpan data ke file teks
void saveData() {
    char filename[50];
    getFilename(filename, current_month, current_year);
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Gagal membuka file untuk menyimpan data!\n");
        return;
    }

    for (int i = 0; i < getDaysInMonth(current_month, current_year); i++) {
        fprintf(fp, "%d,%.2f", monthData[i].day, monthData[i].income);
        for (int j = 0; j < NUM_EXPENSE_CATEGORIES; j++) {
            fprintf(fp, ",%.2f", monthData[i].expenses[j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

// Memuat data dari file teks
void loadData() {
    char filename[50];
    getFilename(filename, current_month, current_year);
    FILE *fp = fopen(filename, "r");

    // Inisialisasi data bulan ini dengan 0
    for (int i = 0; i < MAX_DAYS_IN_MONTH; i++) {
        monthData[i].day = i + 1;
        monthData[i].income = 0.0;
        for (int j = 0; j < NUM_EXPENSE_CATEGORIES; j++) {
            monthData[i].expenses[j] = 0.0;
        }
    }

    if (fp == NULL) {
        printf("Data untuk bulan %d/%d tidak ditemukan. Membuat data baru.\n", current_month, current_year);
        return; // Tidak ada file, data sudah diinisialisasi dengan 0
    }

    printf("Memuat data untuk bulan %d/%d...\n", current_month, current_year);
    int day;
    while (fscanf(fp, "%d,", &day) == 1) {
        if (day > 0 && day <= MAX_DAYS_IN_MONTH) {
            monthData[day - 1].day = day;
            fscanf(fp, "%lf,", &monthData[day - 1].income);
            for (int j = 0; j < NUM_EXPENSE_CATEGORIES; j++) {
                fscanf(fp, "%lf,", &monthData[day - 1].expenses[j]);
            }
        }
    }

    fclose(fp);
}

// --- FUNGSI MENU ---

void handleIncome() {
    clearScreen();
    printf("--- TAMBAH PEMASUKAN ---\n");
    int days = getDaysInMonth(current_month, current_year);
    printf("Rentang tanggal yang valid: 1 - %d\n", days);
    
    int day;
    printf("Masukkan tanggal: ");
    scanf("%d", &day);

    if (day < 1 || day > days) {
        printf("Tanggal tidak valid!\n");
        pause();
        return;
    }

    double amount;
    printf("Masukkan nominal pemasukan: ");
    scanf("%lf", &amount);

    if (amount > 0) {
        monthData[day - 1].income += amount;
        printf("Pemasukan sebesar %.2f pada tanggal %d berhasil ditambahkan.\n", amount, day);
        saveData();
    } else {
        printf("Nominal harus positif!\n");
    }
    pause();
}

void handleExpense() {
    clearScreen();
    printf("--- TAMBAH PENGELUARAN ---\n");
    
    // Pilih kategori
    printf("Pilih jenis pengeluaran:\n");
    for (int i = 0; i < NUM_EXPENSE_CATEGORIES; i++) {
        printf("%d. %s\n", i + 1, EXPENSE_CATEGORIES[i]);
    }
    
    int category_choice;
    printf("Pilihan (1-%d): ", NUM_EXPENSE_CATEGORIES);
    scanf("%d", &category_choice);

    if (category_choice < 1 || category_choice > NUM_EXPENSE_CATEGORIES) {
        printf("Kategori tidak valid!\n");
        pause();
        return;
    }

    // Pilih tanggal
    int days = getDaysInMonth(current_month, current_year);
    printf("\nRentang tanggal yang valid: 1 - %d\n", days);
    int day;
    printf("Masukkan tanggal: ");
    scanf("%d", &day);

    if (day < 1 || day > days) {
        printf("Tanggal tidak valid!\n");
        pause();
        return;
    }

    // Masukkan nominal
    double amount;
    printf("Masukkan nominal pengeluaran: ");
    scanf("%lf", &amount);

    if (amount > 0) {
        monthData[day - 1].expenses[category_choice - 1] += amount;
        printf("Pengeluaran sebesar %.2f untuk %s pada tanggal %d berhasil ditambahkan.\n", amount, EXPENSE_CATEGORIES[category_choice - 1], day);
        saveData();
    } else {
        printf("Nominal harus positif!\n");
    }
    pause();
}

void handleEdit() {
    clearScreen();
    printf("--- EDIT DATA BULAN %d/%d ---\n", current_month, current_year);
    
    int days = getDaysInMonth(current_month, current_year);
    printf("Data Saat Ini:\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("%-7s | %-15s | %-s\n", "Tanggal", "Pemasukan", "Total Pengeluaran");
    printf("-------------------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < days; i++) {
        double total_expense = 0;
        for(int j=0; j<NUM_EXPENSE_CATEGORIES; j++) {
            total_expense += monthData[i].expenses[j];
        }
        if (monthData[i].income > 0 || total_expense > 0) {
            printf("%-7d | %-15.2f | %-15.2f\n", monthData[i].day, monthData[i].income, total_expense);
        }
    }
    printf("-------------------------------------------------------------------------------------------------\n");

    int day;
    printf("\nMasukkan tanggal yang ingin diedit (1-%d): ", days);
    scanf("%d", &day);

    if (day < 1 || day > days) {
        printf("Tanggal tidak valid!\n");
        pause();
        return;
    }

    printf("\n--- Edit Data untuk Tanggal %d ---\n", day);
    printf("Pemasukan saat ini: %.2f\n", monthData[day - 1].income);
    printf("Masukkan nominal pemasukan yang benar (masukkan 0 jika tidak ada): ");
    scanf("%lf", &monthData[day - 1].income);

    for (int i = 0; i < NUM_EXPENSE_CATEGORIES; i++) {
        printf("Pengeluaran %s saat ini: %.2f\n", EXPENSE_CATEGORIES[i], monthData[day - 1].expenses[i]);
        printf("Masukkan nominal %s yang benar (masukkan 0 jika tidak ada): ", EXPENSE_CATEGORIES[i]);
        scanf("%lf", &monthData[day - 1].expenses[i]);
    }

    printf("\nData untuk tanggal %d berhasil diperbarui.\n", day);
    saveData();
    pause();
}

void handleReport() {
    clearScreen();
    printf("--- LAPORAN KEUANGAN BULAN %d/%d ---\n", current_month, current_year);
    
    double total_income = 0, total_expense = 0;
    double weekly_income[4] = {0}, weekly_expense[4] = {0};
    double needs = 0, wants = 0;
    int days = getDaysInMonth(current_month, current_year);

    for (int i = 0; i < days; i++) {
        total_income += monthData[i].income;
        
        double day_expense = 0;
        for (int j = 0; j < NUM_EXPENSE_CATEGORIES; j++) {
            day_expense += monthData[i].expenses[j];
        }
        total_expense += day_expense;

        // Kategorisasi untuk analisis 50/30/20
        // Kebutuhan (Needs): Makanan, Transportasi, Kesehatan, Bulanan
        needs += monthData[i].expenses[1] + monthData[i].expenses[2] + monthData[i].expenses[6] + monthData[i].expenses[8];
        // Keinginan (Wants): Belanja, Kecantikan, Olahraga, Pakaian, Edukasi
        wants += monthData[i].expenses[0] + monthData[i].expenses[3] + monthData[i].expenses[4] + monthData[i].expenses[5] + monthData[i].expenses[7];
        
        // Laporan mingguan
        int week = (i / 7);
        if (week < 4) {
            weekly_income[week] += monthData[i].income;
            weekly_expense[week] += day_expense;
        }
    }

    // Tampilkan Laporan Mingguan
    printf("\n--- LAPORAN MINGGUAN ---\n");
    for (int i = 0; i < 4; i++) {
        printf("Minggu %d: Pemasukan = %.2f, Pengeluaran = %.2f\n", i + 1, weekly_income[i], weekly_expense[i]);
    }

    // Tampilkan Laporan Bulanan
    printf("\n--- LAPORAN BULANAN ---\n");
    printf("Total Pemasukan   : %.2f\n", total_income);
    printf("Total Pengeluaran : %.2f\n", total_expense);
    printf("Saldo Bersih      : %.2f\n", total_income - total_expense);

    // Analisis Kesehatan Keuangan (Aturan 50/30/20)
    printf("\n--- ANALISIS KESEHATAN KEUANGAN (Aturan 50/30/20) ---\n");
    if (total_income > 0) {
        double needs_percent = (needs / total_income) * 100;
        double wants_percent = (wants / total_income) * 100;
        double savings_percent = ((total_income - total_expense) / total_income) * 100;

        printf("Persentase Kebutuhan (Target 50%%): %.2f%%\n", needs_percent);
        printf("Persentase Keinginan (Target 30%%): %.2f%%\n", wants_percent);
        printf("Persentase Tabungan (Target 20%%) : %.2f%%\n", savings_percent);
        
        printf("\nSaran:\n");
        if (needs_percent > 50) printf("- Pengeluaran kebutuhan Anda melebihi 50%%. Cobalah untuk mengevaluasi anggaran belanjaan dan transportasi.\n");
        if (wants_percent > 30) printf("- Pengeluaran keinginan Anda melebihi 30%%. Pertimbangkan untuk mengurangi pengeluaran di kategori belanja, hiburan, dll.\n");
        if (savings_percent < 20) printf("- Tabungan Anda kurang dari 20%%. Upayakan untuk menambah pemasukan atau mengurangi pengeluaran agar bisa menabung lebih banyak.\n");
        if (needs_percent <= 50 && wants_percent <= 30 && savings_percent >= 20) {
            printf("- Hebat! Pola keuangan Anda sudah sehat dan sesuai dengan aturan 50/30/20. Pertahankan!\n");
        }
    } else {
        printf("Tidak ada pemasukan bulan ini untuk dianalisis.\n");
    }
    
    pause();
    clearScreen();
    runGame();
}

void handleChangeMonth() {
    clearScreen();
    printf("--- GANTI BULAN & TAHUN ---\n");
    printf("Bulan & Tahun saat ini: %d/%d\n", current_month, current_year);
    
    printf("Masukkan bulan baru (1-12): ");
    scanf("%d", &current_month);
    printf("Masukkan tahun baru: ");
    scanf("%d", &current_year);

    if (current_month < 1 || current_month > 12) {
        printf("Bulan tidak valid! Kembali ke bulan sebelumnya.\n");
        pause();
        return;
    }
    
    printf("Beralih ke bulan %d/%d...\n", current_month, current_year);
    loadData(); // Memuat data untuk bulan dan tahun baru
    pause();
}

// --- FUNGSI UTAMA ---

int main() {
    // Inisialisasi
    printf("=== ================================ ===\n");
    printf(YELLOW "=== PROGRAM CATATAN KEUANGAN PRIBADI ===\n" RESET);
    printf("=== ================================ ===\n");
    printf("Masukkan bulan (1-12): ");
    scanf("%d", &current_month);
    printf("Masukkan tahun: ");
    scanf("%d", &current_year);

    if (current_month < 1 || current_month > 12) {
        printf("Bulan tidak valid! Program keluar.\n");
        return 1;
    }

    loadData(); // Muat data awal

    int choice;
    do {
        clearScreen();
        printf("=== MENU UTAMA (Bulan: %d/%d) ===\n", current_month, current_year);
        printf(GREEN "1. Tambah Pemasukan\n" RESET);
        printf(RED "2. Tambah Pengeluaran\n" RESET);
        printf("3. Edit Data Bulan Ini\n");
        printf(YELLOW "4. Laporan & Analisis Keuangan\n" RESET);
        printf("5. Ganti Bulan/Tahun\n");
        printf("6. Keluar\n");
        printf("=====================================\n");
        printf("Pilihan Anda: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                handleIncome();
                break;
            case 2:
                handleExpense();
                break;
            case 3:
                handleEdit();
                break;
            case 4:
                handleReport();
                break;
            case 5:
                handleChangeMonth();
                break;
            case 6:
                printf("Menyimpan data terakhir dan keluar dari program. Sampai jumpa!\n");
                saveData(); 
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
                pause();
                break;
        }
    } while (choice != 6);

    return 0;
}