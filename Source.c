#include <stdio.h>
#include <string.h>
#include <locale.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

void mLCS(const char* x, const char* y, char* lcs) {
    int m = strlen(x);
    int n = strlen(y);
    int dmtx[100][100] = { 0 };
    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            dmtx[i][j] = 0;
        }
    }

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (x[i - 1] == y[j - 1]) {
                dmtx[i][j] = dmtx[i - 1][j - 1] + 1;
            }
            else {
                dmtx[i][j] = max(dmtx[i - 1][j], dmtx[i][j - 1]);
            }
        }
    }

    int len = dmtx[m][n];
    lcs[len] = '\0';

    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (x[i - 1] == y[j - 1]) {
            lcs[--len] = x[i - 1];
            --i;
            --j;
        }
        else if (dmtx[i - 1][j] > dmtx[i][j - 1]) {
            --i;
        }
        else {
            --j;
        }
    }
}

void testing() {
    struct TestCase {
        const char* x;
        const char* y;
        const char* z;
    };

    struct TestCase tests[] = {
        {"11011011", "11101111", "1101111"},
        {"10101", "11111", "111"},
        {"0", "1", ""},
        {"1011111", "10111111", "1011111"},
        {"", "", ""},
    };

    for (int i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) {
        char result[100];
        mLCS(tests[i].x, tests[i].y, result);
        if (strcmp(result, tests[i].z) == 0) {
            printf("Тест %d успешен\n", i + 1);
        }
        else {
            printf("Ошибка в тесте %d: ожидалось %s, получено %s\n", i + 1, tests[i].z, result);
        }
    }
}

int main() {
    setlocale(LC_CTYPE, "Russian");
    printf("Цителадзе 5030102/20001\nЗадание 1, вариант 9\n\n");
    testing(); 
    return 0;
}
