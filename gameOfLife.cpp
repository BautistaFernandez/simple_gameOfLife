#include <iostream>
#include <random>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>

using namespace std;

void dead_state(int a, int b);
void random_state(int a, int b);
int random();
char render(int state);
void next_state();
void clear_console();

const int alive = 1, dead = 0;
constexpr int width = 20, height = 100;

int board_state[width][height];
char str_state[width][height];

int main() {
    random_state(width, height);

    while (true) {
        clear_console();

        next_state();

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                cout << str_state[i][j];
            }
            cout << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(200));
    }
    return 0;
}

// Inicializa las posiciones del tablero a 0.
void dead_state(int a, int b) {
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            board_state[i][j] = 0; // Estado muerto
            str_state[i][j] = ' '; // Inicializar a muertas
        }
    }
}

// Asigna un estado aleatorio a las células.
void random_state(int a, int b) {
    dead_state(a, b);
    int numAlive = (width * height) / 4;
    vector<pair<int, int>> positions;

    // Llenar todas las posiciones posibles
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            positions.push_back({i, j});
        }
    }

    // Mezclar posiciones
    random_device rd;
    mt19937 gen(rd());
    shuffle(positions.begin(), positions.end(), gen);

    // Marcar las primeras numAlive posiciones como vivas
    for (int i = 0; i < numAlive; ++i) {
        int x = positions[i].first;
        int y = positions[i].second;
        str_state[x][y] = '#'; // Marca la célula como viva
        board_state[x][y] = 1;  // Estado vivo
    }
}

// Cambia números y formatea la tabla.
char render(int state) {
    return (state == 0) ? ' ' : '#'; // Cambié el renderizado
}

// filas = width - columnas = height
void next_state() {
    char temp_state[width][height];

    auto getWrappedIndex = [](int index, int limit) {
        return (index + limit) % limit;
    };

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int aliveCount = 0;

            // Revisar los 8 vecinos.
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di != 0 || dj != 0) {
                        int ni = getWrappedIndex(i + di, width);
                        int nj = getWrappedIndex(j + dj, height);

                        if (str_state[ni][nj] == '#') {
                            aliveCount++;
                        }
                    }
                }
            }

            // Aplicar condiciones.
            if (str_state[i][j] == ' ') {
                if (aliveCount == 3) {
                    temp_state[i][j] = '#'; // Nace una célula
                } else {
                    temp_state[i][j] = ' '; // Permanece muerta
                }
            } else if (str_state[i][j] == '#') {
                if (aliveCount < 2 || aliveCount > 3) {
                    temp_state[i][j] = ' '; // Muere
                } else {
                    temp_state[i][j] = '#'; // Sobrevive
                }
            }
        }
    }

    // Copiar estado temporal al estado original.
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            str_state[i][j] = temp_state[i][j];
        }
    }
}

void clear_console() {
    system("cls");
}