#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <random>
#include <string>
#include <windows.h>
#include <clocale>

using namespace std;
using namespace sf;

// Глобальные константы для настройки размера поля и окна
const int GRID_SIZE = 30; // основной размер поля NxN для случайных режимов
const int CELL_SIZE = 25;  // размер одной ячейки в пикселях для поля 30x30
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE; // итоговый размер окна в пикселях

const int TASK_SIZE = 10;     // размер картинки варианта 23: 10x10
const int MIN_WEIGHT = 1;     // минимальный вес клетки на случайной карте
const int MAX_WEIGHT = 10;    // максимальный вес клетки на случайной карте
const int INF = 1000000000;   // большое число, используемое как "бесконечность" для g и f

// Типы ячеек на поле. По ним определяется и логика алгоритма, и цвет при отрисовке.
enum CellType {
    EMPTY,     // пустая проходимая ячейка
    OBSTACLE,  // препятствие, через которое нельзя пройти
    START,     // начальная точка
    END,       // конечная точка
    PATH,      // клетка найденного пути
    VISITED,   // клетка, которую алгоритм уже обработал
    IN_QUEUE   // клетка, которая добавлена в очередь на рассмотрение
};

// Структура для представления одной ячейки поля
struct Cell {
    int x, y;        // координаты ячейки в сетке
    CellType type;   // текущее состояние ячейки
    int f, g, h;     // значения для A*: g - стоимость пути от старта, h - эвристика, f = g + h
    Cell* parent;    // указатель на предыдущую клетку, нужен для восстановления пути

    // Конструктор: задаёт координаты, делает клетку пустой и обнуляет параметры A*
    Cell(int x, int y) : x(x), y(y), type(EMPTY), f(0), g(0), h(0), parent(nullptr) {}

    // Перегрузка оператора == для сравнения двух ячеек по координатам
    bool operator==(const Cell& other) const {
        return x == other.x && y == other.y;
    }
};

// Режим работы программы
enum WorkMode {
    VARIANT_23_FIELD, // поле из картинки варианта 23
    RANDOM_FIELD,     // случайное поле: препятствия есть, но веса всех клеток равны 1
    RANDOM_MAP        // случайная карта: препятствия есть, у проходимых клеток есть вес
};

// Функция для вычисления эвристики.
// Используется манхэттенское расстояние.
int heuristic(const Cell& a, const Cell& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Функция проверяет, находится ли ячейка внутри поля 30x30.
bool isValid(int x, int y) {
    return x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE;
}

// Узел для приоритетной очереди. В очередь кладём не саму Cell, а координаты и значения f, g, h.
struct QueueNode {
    int x, y;       // координаты клетки
    int f, g, h;    // значения A* для этой клетки
    int order;      // порядок добавления: нужен, если у клеток одинаковые f и h
};

// Сравнение для priority_queue. 
struct CompareNode {
    bool operator()(const QueueNode& a, const QueueNode& b) const {
        if (a.f != b.f) return a.f > b.f;
        if (a.h != b.h) return a.h > b.h;
        return a.order > b.order;
    }
};

// Создание сетки GRID_SIZE x GRID_SIZE. Каждая ячейка получает свои координаты.
vector<vector<Cell>> create_grid() {
    vector<vector<Cell>> grid;
    for (int i = 0; i < GRID_SIZE; ++i) {
        vector<Cell> row;
        for (int j = 0; j < GRID_SIZE; ++j) {
            row.push_back(Cell(i, j));
        }
        grid.push_back(row);
    }
    return grid;
}

// Полная очистка поля перед загрузкой нового режима. 
void reset_grid(vector<vector<Cell>>& grid, vector<vector<int>>& weights) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j].type = EMPTY;
            grid[i][j].f = 0;
            grid[i][j].g = 0;
            grid[i][j].h = 0;
            grid[i][j].parent = nullptr;
            weights[i][j] = 1;
        }
    }
}

// Очистка только следов прошлого запуска A*: PATH, VISITED и IN_QUEUE.
void clear_search_marks(vector<vector<Cell>>& grid) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j].type == PATH || grid[i][j].type == VISITED || grid[i][j].type == IN_QUEUE) {
                grid[i][j].type = EMPTY;
            }

            // Сбрасываем параметры A*, чтобы новый запуск не зависел от старого
            grid[i][j].f = INF;
            grid[i][j].g = INF;
            grid[i][j].h = 0;
            grid[i][j].parent = nullptr;
        }
    }
}

// Восстановление пути от финиша к старту через указатели parent. Сначала путь получается в обратном порядке, затем разворачивается.
vector<pair<int, int>> reconstruct_path(Cell* finish) {
    vector<pair<int, int>> path;
    Cell* current = finish;

    while (current != nullptr) {
        path.push_back({ current->x, current->y });
        current = current->parent;
    }

    reverse(path.begin(), path.end());
    return path;
}

// Проверка достижимости для случайной генерации. Используется обычный обход в ширину, чтобы убедиться, что от старта до финиша можно дойти.
bool has_path(const vector<vector<Cell>>& grid, pair<int, int> start, pair<int, int> finish) {
    vector<vector<bool>> visited(GRID_SIZE, vector<bool>(GRID_SIZE, false));
    queue<pair<int, int>> q;

    // Начинаем обход со стартовой клетки
    q.push(start);
    visited[start.first][start.second] = true;

    // Направления движения: вверх, вниз, влево, вправо
    int dx[4] = { -1, 1, 0, 0 };
    int dy[4] = { 0, 0, -1, 1 };

    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();

        // Если дошли до финиша, путь существует
        if (current == finish) return true;

        for (int k = 0; k < 4; ++k) {
            int nx = current.first + dx[k];
            int ny = current.second + dy[k];

            // Добавляем соседнюю клетку, если она внутри поля, ещё не посещена и не является препятствием
            if (isValid(nx, ny) && !visited[nx][ny] && grid[nx][ny].type != OBSTACLE) {
                visited[nx][ny] = true;
                q.push({ nx, ny });
            }
        }
    }

    return false;
}

// Загрузка поля по варианту 23.
void load_variant23_field(vector<vector<Cell>>& grid,
    vector<vector<int>>& weights,
    pair<int, int>& startPos,
    pair<int, int>& finishPos) {
    reset_grid(grid, weights);

    // Картинка варианта имеет размер 10x10. Основная сетка программы 30x30, поэтому клетки за пределами 10x10 закрываем препятствиями.
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (i >= TASK_SIZE || j >= TASK_SIZE) {
                grid[i][j].type = OBSTACLE;
            }
        }
    }

    // Серые клетки с картинки варианта 23.
    vector<pair<int, int>> obstacles = {
        {6, 0}, {8, 0},
        {0, 1}, {3, 1}, {7, 1}, {9, 1},
        {0, 2}, {4, 2}, {6, 2},
        {4, 3},
        {1, 4}, {3, 4}, {8, 4},
        {5, 5}, {7, 5}, {9, 5},
        {4, 6},
        {8, 7},
        {3, 8}, {4, 8}, {5, 8}, {7, 8},
        {1, 9}, {4, 9}, {6, 9}
    };

    // Помечаем перечисленные клетки как препятствия
    for (auto [x, y] : obstacles) {
        grid[x][y].type = OBSTACLE;
    }

    // Старт и финиш для варианта 23
    startPos = { 0, 6 };
    finishPos = { 9, 9 };

    // Устанавливаем специальные типы для стартовой и конечной клеток
    grid[startPos.first][startPos.second].type = START;
    grid[finishPos.first][finishPos.second].type = END;
}

// Генерация случайного поля.
void generate_random_field(vector<vector<Cell>>& grid,
    vector<vector<int>>& weights,
    pair<int, int>& startPos,
    pair<int, int>& finishPos) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> obstacle_dist(0, 4); // вероятность препятствия примерно 20%

    // Для случайных режимов старт находится в левом верхнем углу, а финиш — в правом нижнем
    startPos = { 0, 0 };
    finishPos = { GRID_SIZE - 1, GRID_SIZE - 1 };

    bool correct = false;

    // Генерируем поле до тех пор, пока между стартом и финишем не появится путь
    while (!correct) {
        reset_grid(grid, weights);

        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                weights[i][j] = 1; // в обычном поле все веса равны 1

                // С некоторой вероятностью превращаем клетку в препятствие
                if (obstacle_dist(gen) == 0) {
                    grid[i][j].type = OBSTACLE;
                }
            }
        }

        // Гарантируем, что старт и финиш не являются препятствиями
        grid[startPos.first][startPos.second].type = START;
        grid[finishPos.first][finishPos.second].type = END;

        // Проверяем, существует ли путь
        correct = has_path(grid, startPos, finishPos);
    }
}

// Генерация случайной карты. В отличие от случайного поля, здесь каждая клетка имеет вес от 1 до 10.
void generate_random_map(vector<vector<Cell>>& grid,
    vector<vector<int>>& weights,
    pair<int, int>& startPos,
    pair<int, int>& finishPos) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> obstacle_dist(0, 4); // вероятность препятствия примерно 20%
    uniform_int_distribution<int> weight_dist(MIN_WEIGHT, MAX_WEIGHT); // случайный вес клетки

    // Для случайных режимов старт находится в левом верхнем углу, а финиш — в правом нижнем
    startPos = { 0, 0 };
    finishPos = { GRID_SIZE - 1, GRID_SIZE - 1 };

    bool correct = false;

    // Генерируем карту до тех пор, пока путь от старта к финишу не будет существовать
    while (!correct) {
        reset_grid(grid, weights);

        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                weights[i][j] = weight_dist(gen); // задаём случайный вес клетки от 1 до 10

                // С некоторой вероятностью клетка становится препятствием
                if (obstacle_dist(gen) == 0) {
                    grid[i][j].type = OBSTACLE;
                }
            }
        }

        // Гарантируем, что старт и финиш остаются проходимыми
        grid[startPos.first][startPos.second].type = START;
        grid[finishPos.first][finishPos.second].type = END;

        // Проверяем, что сгенерированная карта имеет путь
        correct = has_path(grid, startPos, finishPos);
    }
}

// Алгоритм A*. На вход получает поле, веса клеток, координаты старта и финиша.
vector<pair<int, int>> a_star_algorithm(vector<vector<Cell>>& grid,
    const vector<vector<int>>& weights,
    pair<int, int> startPos,
    pair<int, int> finishPos,
    int& totalCost) {
    clear_search_marks(grid); // очищаем старые отметки предыдущего запуска

    // Получаем указатели на стартовую и конечную клетки
    Cell* start = &grid[startPos.first][startPos.second];
    Cell* finish = &grid[finishPos.first][finishPos.second];

    // Очередь с приоритетом хранит клетки, которые алгоритм должен рассмотреть
    priority_queue<QueueNode, vector<QueueNode>, CompareNode> open_set;

    // closed[x][y] показывает, была ли клетка уже окончательно обработана
    vector<vector<bool>> closed(GRID_SIZE, vector<bool>(GRID_SIZE, false));

    // Для стартовой клетки расстояние от старта равно 0
    start->g = 0;

    // h — эвристическая оценка расстояния от старта до финиша
    start->h = heuristic(*start, *finish);

    // f = g + h — итоговая оценка клетки
    start->f = start->g + start->h;

    int pushOrder = 0; // счётчик порядка добавления в очередь

    // Добавляем стартовую клетку в очередь
    open_set.push({ start->x, start->y, start->f, start->g, start->h, pushOrder++ });

    // Возможные направления движения: вправо, влево, вниз, вверх
    int dx[4] = { 0, 0, 1, -1 };
    int dy[4] = { 1, -1, 0, 0 };

    // Основной цикл A*: пока есть клетки для проверки
    while (!open_set.empty()) {
        // Берём клетку с наименьшим значением f
        QueueNode currentNode = open_set.top();
        open_set.pop();

        Cell* current = &grid[currentNode.x][currentNode.y];

        // Если клетка уже была обработана ранее, пропускаем её
        if (closed[current->x][current->y]) {
            continue;
        }

        // Помечаем текущую клетку как обработанную
        closed[current->x][current->y] = true;

        // Если это не старт и не финиш, отмечаем клетку как посещённую
        if (current->type != START && current->type != END) {
            current->type = VISITED;
        }

        // Если текущая клетка является финишем, путь найден
        if (*current == *finish) {
            vector<pair<int, int>> path = reconstruct_path(finish); // восстанавливаем путь
            totalCost = finish->g; // стоимость пути равна g у конечной клетки

            // Отмечаем клетки найденного пути на поле
            for (auto [x, y] : path) {
                if (grid[x][y].type != START && grid[x][y].type != END) {
                    grid[x][y].type = PATH;
                }
            }

            return path;
        }

        // Просматриваем соседей текущей клетки
        for (int k = 0; k < 4; ++k) {
            int nx = current->x + dx[k];
            int ny = current->y + dy[k];

            // Если сосед выходит за границы поля, пропускаем его
            if (!isValid(nx, ny)) {
                continue;
            }

            // Если сосед является препятствием или уже обработан, пропускаем его
            if (grid[nx][ny].type == OBSTACLE || closed[nx][ny]) {
                continue;
            }

            Cell* neighbor = &grid[nx][ny];

            // Новая возможная стоимость пути до соседа: стоимость пути до текущей клетки + вес соседней клетки
            int tentative_g = current->g + weights[nx][ny];

            // Если новый путь до соседа дешевле старого, обновляем данные соседа
            if (tentative_g < neighbor->g) {
                neighbor->parent = current; // запоминаем, откуда пришли в эту клетку
                neighbor->g = tentative_g; // обновляем стоимость пути от старта
                neighbor->h = heuristic(*neighbor, *finish); // считаем эвристику до финиша
                neighbor->f = neighbor->g + neighbor->h; // пересчитываем итоговую оценку

                // Добавляем соседа в очередь на дальнейшее рассмотрение
                open_set.push({ neighbor->x, neighbor->y, neighbor->f, neighbor->g, neighbor->h, pushOrder++ });

                // Отмечаем клетку как находящуюся в очереди, если это не старт и не финиш
                if (neighbor->type != START && neighbor->type != END) {
                    neighbor->type = IN_QUEUE;
                }
            }
        }
    }

    // Если очередь закончилась, а финиш не найден, пути нет
    totalCost = -1;
    return {};
}

// Отрисовка сетки в окне SFML.
void draw_grid(RenderWindow& window,
    vector<vector<Cell>>& grid,
    const vector<vector<int>>& weights,
    WorkMode mode,
    const Font& font,
    bool fontLoaded) {
    // Для варианта 23 рисуем только область 10x10, для случайных режимов — всё поле 30x30
    int activeSize = (mode == VARIANT_23_FIELD) ? TASK_SIZE : GRID_SIZE;

    // Размер клетки подбирается так, чтобы активное поле занимало всё окно
    int drawCellSize = WINDOW_SIZE / activeSize;

    for (int i = 0; i < activeSize; ++i) {
        for (int j = 0; j < activeSize; ++j) {
            // Создаём квадрат чуть меньше ячейки, чтобы были видны границы сетки
            RectangleShape cell(Vector2f(drawCellSize - 1, drawCellSize - 1));
            cell.setPosition(i * drawCellSize, j * drawCellSize);

            // Выбираем цвет клетки по её состоянию
            switch (grid[i][j].type) {
            case EMPTY:
                cell.setFillColor(Color::White); // пустая клетка
                break;
            case OBSTACLE:
                cell.setFillColor(Color::Black); // препятствие
                break;
            case START:
                cell.setFillColor(Color(255, 165, 0)); // начало — оранжевый
                break;
            case END:
                cell.setFillColor(Color(64, 224, 208)); // конец — бирюзовый
                break;
            case IN_QUEUE:
                cell.setFillColor(Color::Green); // клетка в очереди — зелёный
                break;
            case VISITED:
                cell.setFillColor(Color::Red); // посещённая клетка — красный
                break;
            case PATH:
                cell.setFillColor(Color(128, 0, 128)); // найденный путь — фиолетовый
                break;
            }

            // Рисуем саму клетку
            window.draw(cell);

            // В режиме случайной карты показываем вес клетки числом.
            if (mode == RANDOM_MAP && fontLoaded &&
                grid[i][j].type != OBSTACLE &&
                grid[i][j].type != START &&
                grid[i][j].type != END) {

                Text weightText;
                weightText.setFont(font);
                weightText.setString(to_string(weights[i][j])); // число веса клетки
                weightText.setCharacterSize(10);
                weightText.setFillColor(Color::Black);
                weightText.setOutlineColor(Color::White);
                weightText.setOutlineThickness(1.f);

                // Центрируем текст внутри клетки
                FloatRect bounds = weightText.getLocalBounds();
                weightText.setOrigin(bounds.left + bounds.width / 2.f,
                    bounds.top + bounds.height / 2.f);

                weightText.setPosition(
                    i * drawCellSize + drawCellSize / 2.f,
                    j * drawCellSize + drawCellSize / 2.f
                );

                // Рисуем вес поверх клетки
                window.draw(weightText);
            }
        }
    }
}

// Вывод подсказки по управлению программой
void print_help() {
    cout << "Управление:" << endl;
    cout << "SPACE - запустить A*" << endl;
    cout << "R     - перегенерировать текущий режим" << endl;
    cout << "1     - поле варианта 23" << endl;
    cout << "2     - случайное поле без весов" << endl;
    cout << "3     - случайная карта с весами" << endl;
}

// Вывод манхэттенского расстояния между стартом и финишем.
void print_manhattan_distance(vector<vector<Cell>>& grid,
    pair<int, int> startPos,
    pair<int, int> finishPos) {
    int distance = heuristic(
        grid[startPos.first][startPos.second],
        grid[finishPos.first][finishPos.second]
    );

    cout << "Манхэттенское расстояние: " << distance << endl;
}

int main() {
    // Настройка русской кодировки для консоли Windows
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    // Создаём поле и матрицу весов
    vector<vector<Cell>> grid = create_grid();
    vector<vector<int>> weights(GRID_SIZE, vector<int>(GRID_SIZE, 1));

    pair<int, int> startPos;  // координаты старта
    pair<int, int> finishPos; // координаты финиша

    // По умолчанию запускается режим поля варианта 23
    WorkMode mode = VARIANT_23_FIELD;

    // Загружаем поле варианта 23
    load_variant23_field(grid, weights, startPos, finishPos);

    // Создаём окно SFML
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "A* variant 23");

    // Загружаем шрифт для вывода весов на случайной карте
    Font font;
    bool fontLoaded = font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

    if (!fontLoaded) {
        cout << "Шрифт не загружен. Веса на карте отображаться не будут." << endl;
    }

    // Печатаем управление и начальную информацию
    print_help();
    cout << "Режим: поле варианта 23" << endl;
    print_manhattan_distance(grid, startPos, finishPos);

    // Главный цикл программы: работает, пока открыто окно
    while (window.isOpen()) {
        Event event;

        // Обработка событий окна
        while (window.pollEvent(event)) {
            // Закрытие окна
            if (event.type == Event::Closed) {
                window.close();
            }

            // Обработка нажатий клавиш
            if (event.type == Event::KeyPressed) {
                // SPACE запускает алгоритм A* на текущем поле
                if (event.key.code == Keyboard::Space) {
                    int totalCost = 0;

                    // Запускаем A* и получаем путь
                    vector<pair<int, int>> path = a_star_algorithm(grid, weights, startPos, finishPos, totalCost);

                    // Если путь найден, выводим его стоимость и количество клеток
                    if (!path.empty()) {
                        cout << "Путь найден. Стоимость пути: " << totalCost << endl;
                        cout << "Количество клеток в пути: " << path.size() << endl;
                    }
                    else {
                        cout << "Путь не найден." << endl;
                    }
                }

                // R перегенерирует текущий режим
                if (event.key.code == Keyboard::R) {
                    if (mode == VARIANT_23_FIELD) {
                        load_variant23_field(grid, weights, startPos, finishPos);
                        cout << "Режим: поле варианта 23" << endl;
                    }
                    else if (mode == RANDOM_FIELD) {
                        generate_random_field(grid, weights, startPos, finishPos);
                        cout << "Режим: случайное поле без весов" << endl;
                    }
                    else {
                        generate_random_map(grid, weights, startPos, finishPos);
                        cout << "Режим: случайная карта с весами" << endl;
                    }

                    // После загрузки режима выводим манхэттенское расстояние
                    print_manhattan_distance(grid, startPos, finishPos);
                }

                // Клавиша 1 загружает фиксированное поле варианта 23
                if (event.key.code == Keyboard::Num1) {
                    mode = VARIANT_23_FIELD;
                    load_variant23_field(grid, weights, startPos, finishPos);
                    cout << "Режим: поле варианта 23" << endl;
                    print_manhattan_distance(grid, startPos, finishPos);
                }

                // Клавиша 2 загружает случайное поле без весов
                if (event.key.code == Keyboard::Num2) {
                    mode = RANDOM_FIELD;
                    generate_random_field(grid, weights, startPos, finishPos);
                    cout << "Режим: случайное поле без весов" << endl;
                    print_manhattan_distance(grid, startPos, finishPos);
                }

                // Клавиша 3 загружает случайную карту с весами
                if (event.key.code == Keyboard::Num3) {
                    mode = RANDOM_MAP;
                    generate_random_map(grid, weights, startPos, finishPos);
                    cout << "Режим: случайная карта с весами" << endl;
                    print_manhattan_distance(grid, startPos, finishPos);
                }
            }
        }

        // Очищаем окно перед новой отрисовкой
        window.clear(Color::White);

        // Рисуем текущее поле
        draw_grid(window, grid, weights, mode, font, fontLoaded);

        // Показываем отрисованный кадр на экране
        window.display();
    }

    return 0;
}