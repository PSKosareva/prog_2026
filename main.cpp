#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <windows.h>
#include <cctype>
#include <limits>

using namespace std;

const WORD ROW_COLORS[4] = {
    FOREGROUND_RED | FOREGROUND_INTENSITY,
    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
};
const WORD WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

static unsigned int decodeUtf8(const string& s, int& len) {
    if (s.empty()) { len = 0; return 0; }
    unsigned char c = (unsigned char)s[0];

    if (c < 0x80) { len = 1; return c; }
    if ((c & 0xE0) == 0xC0) {
        if (s.size() < 2) { len = 1; return c; }
        len = 2;
        return ((c & 0x1Fu) << 6) | ((unsigned char)s[1] & 0x3Fu);
    }
    if ((c & 0xF0) == 0xE0) {
        if (s.size() < 3) { len = 1; return c; }
        len = 3;
        return ((c & 0x0Fu) << 12)
            | (((unsigned char)s[1] & 0x3Fu) << 6)
            | ((unsigned char)s[2] & 0x3Fu);
    }
    if ((c & 0xF8) == 0xF0) {
        if (s.size() < 4) { len = 1; return c; }
        len = 4;
        return ((c & 0x07u) << 18)
            | (((unsigned char)s[1] & 0x3Fu) << 12)
            | (((unsigned char)s[2] & 0x3Fu) << 6)
            | ((unsigned char)s[3] & 0x3Fu);
    }
    len = 1;
    return c;
}

static string encodeUtf8(unsigned int cp) {
    string out;
    if (cp < 0x80) {
        out += (char)cp;
    }
    else if (cp < 0x800) {
        out += (char)(0xC0 | (cp >> 6));
        out += (char)(0x80 | (cp & 0x3F));
    }
    else if (cp < 0x10000) {
        out += (char)(0xE0 | (cp >> 12));
        out += (char)(0x80 | ((cp >> 6) & 0x3F));
        out += (char)(0x80 | (cp & 0x3F));
    }
    else {
        out += (char)(0xF0 | (cp >> 18));
        out += (char)(0x80 | ((cp >> 12) & 0x3F));
        out += (char)(0x80 | ((cp >> 6) & 0x3F));
        out += (char)(0x80 | (cp & 0x3F));
    }
    return out;
}

static bool isLetter(unsigned int cp) {
    if ((cp >= 'A' && cp <= 'Z') || (cp >= 'a' && cp <= 'z')) return true;
    if (cp >= 0x0410 && cp <= 0x044F) return true;
    if (cp == 0x0401 || cp == 0x0451) return true;
    return false;
}

class JaggedArray {
    vector<vector<string>> data;

    static bool isEmpty(const string& s) {
        for (char c : s)
            if (!isspace(static_cast<unsigned char>(c))) return false;
        return true;
    }

public:
    JaggedArray() {}
    JaggedArray(const vector<vector<string>>& d) : data(d) {}

    class Row {
        vector<string>& r;
    public:
        Row(vector<string>& row) : r(row) {}
        string& operator[](int j) {
            if (j < 0 || j >= (int)r.size()) throw out_of_range("индекс j вне границ");
            return r[j];
        }
    };
    class ConstRow {
        const vector<string>& r;
    public:
        ConstRow(const vector<string>& row) : r(row) {}
        const string& operator[](int j) const {
            if (j < 0 || j >= (int)r.size()) throw out_of_range("индекс j вне границ");
            return r[j];
        }
    };

    Row operator[](int i) {
        if (i < 0 || i >= (int)data.size()) throw out_of_range("индекс i вне границ");
        return Row(data[i]);
    }
    ConstRow operator[](int i) const {
        if (i < 0 || i >= (int)data.size()) throw out_of_range("индекс i вне границ");
        return ConstRow(data[i]);
    }

    int rows() const { return (int)data.size(); }

    bool Delete(int i, int j) {
        if (i < 0 || i >= (int)data.size())          return false;
        if (j < 0 || j >= (int)data[i].size())       return false;
        data[i].erase(data[i].begin() + j);
        return true;
    }

    int Delete(const string& item) {
        int cnt = 0;
        for (auto& r : data) {
            auto it = remove(r.begin(), r.end(), item);
            cnt += (int)distance(it, r.end());
            r.erase(it, r.end());
        }
        return cnt;
    }

    void add_endline(int k, const string& item) {
        if (k < 0 || k >= (int)data.size()) throw out_of_range("индекс вне границ");
        data[k].push_back(item);
    }

    void sortRows() {
        for (auto& r : data) sort(r.begin(), r.end());
    }

    JaggedArray operator+(const JaggedArray& other) const {
        JaggedArray res;
        int n = max(data.size(), other.data.size());
        res.data.resize(n);

        for (int i = 0; i < n; i++) {
            int m1 = (i < (int)data.size()) ? (int)data[i].size() : 0;
            int m2 = (i < (int)other.data.size()) ? (int)other.data[i].size() : 0;
            int m = max(m1, m2);
            res.data[i].resize(m);

            for (int j = 0; j < m; j++) {
                string a = (j < m1) ? data[i][j] : "";
                string b = (j < m2) ? other.data[i][j] : "";

                if (a.empty() && !b.empty()) {
                    res.data[i][j] = b;
                }
                else if (isEmpty(a) && a != "") {
                    res.data[i][j] = a;
                }
                else {
                    res.data[i][j] = a + b;
                }
            }
        }
        return res;
    }

    JaggedArray& operator++() {
        for (auto& r : data)
            for (auto& s : r) {
                if (s.empty()) continue;
                int len = 0;
                unsigned int cp = decodeUtf8(s, len);
                if (isLetter(cp)) {
                    string newFirst = encodeUtf8(cp + 1);
                    s = newFirst + s.substr(len);
                }
            }
        return *this;
    }
    JaggedArray operator++(int) {
        JaggedArray old(*this);
        ++(*this);
        return old;
    }

    void print(const string& title = "") const {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!title.empty()) cout << title << "\n";
        for (int i = 0; i < (int)data.size(); i++) {
            SetConsoleTextAttribute(h, ROW_COLORS[i % 4]);
            cout << "[" << i << "]  ";
            for (auto& s : data[i]) cout << "\"" << s << "\" ";
            cout << "\n";
        }
        SetConsoleTextAttribute(h, WHITE);
    }

    void inputRow() {
        cout << "введите элементы строки через запятую: ";
        string line;
        getline(cin, line);

        vector<string> row;
        stringstream ss(line);
        string item;
        while (getline(ss, item, ',')) {
            size_t l = item.find_first_not_of(" \t");
            size_t r = item.find_last_not_of(" \t");
            if (l == string::npos) {
                row.push_back("");
            }
            else {
                row.push_back(item.substr(l, r - l + 1));
            }
        }
        data.push_back(row);
        cout << "строка успешно добавлена.\n";
    }

    void inputEdit(int i, int j) {
        if (i < 0 || i >= (int)data.size() || j < 0 || j >= (int)data[i].size())
            throw out_of_range("индекс вне диапазона");

        cout << "текущее значение A[" << i << "][" << j << "] = \"" << data[i][j] << "\"\n";
        cout << "введите новое значение: ";
        string value;
        getline(cin, value);
        data[i][j] = value;
        cout << "значение изменено.\n";
    }

    void inputEndline(int k) {
        if (k < 0 || k >= (int)data.size()) throw out_of_range("индекс вне границ");

        cout << "введите элемент для добавления в конец строки " << k << ": ";
        string item;
        getline(cin, item);
        data[k].push_back(item);
        cout << "элемент \"" << item << "\" добавлен.\n";
    }

    static JaggedArray loadFromTxt(const string& filename) {
        ifstream f(filename);
        if (!f) throw runtime_error("не удалось открыть файл: " + filename);

        JaggedArray arr;
        string line;
        while (getline(f, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            for (auto& c : line) if (c == ',') c = ' ';

            istringstream iss(line);
            vector<string> row;
            string tok;
            while (iss >> tok) row.push_back(tok);
            if (!row.empty()) arr.data.push_back(row);
        }
        return arr;
    }
};

static void ensureSampleFiles() {
    ifstream t1("A.txt");
    if (!t1) {
        ofstream f("A.txt", ios::binary);
        f << "а в з 3 5 / м\n";
        f << "привет мир\n";
        f << "тест строка qwerty\n";
    }
    ifstream t2("B.txt");
    if (!t2) {
        ofstream f("B.txt", ios::binary);
        f << "один два\n";
        f << "три четыре пять\n";
        f << "шесть семь\n";
    }
}

void printMenu() {
    cout << "\nМЕНЮ\n";
    cout << " 1. показать массив\n";
    cout << " 2. добавить строку\n";
    cout << " 3. добавить элемент в конец строки\n";
    cout << " 4. изменить элемент a[i][j]\n";
    cout << " 5. удалить элемент по индексу (i, j)\n";
    cout << " 6. удалить элемент по значению\n";
    cout << " 7. оператор ++ (сдвиг первой буквы)\n";
    cout << " 8. сортировка строк\n";
    cout << " 9. оператор + (сложить с копией)\n";
    cout << "10. загрузить из файла\n";
    cout << " 0. выход\n";
    cout << "выберите действие: ";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    ensureSampleFiles();
    JaggedArray A;
    try {
        A = JaggedArray::loadFromTxt("A.txt");
    }
    catch (const exception& e) {
        cout << "ошибка загрузки: " << e.what() << "\n";
    }

    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            cout << "ошибка ввода, попробуйте снова.\n";
            continue;
        }
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');

        try {
            switch (choice) {
            case 1:
                A.print("текущий массив:");
                break;

            case 2:
                A.inputRow();
                A.print("массив после добавления строки:");
                break;

            case 3: {
                cout << "введите номер строки: ";
                int k; cin >> k;
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                A.inputEndline(k);
                A.print("массив после add_endline:");
                break;
            }

            case 4: {
                cout << "введите i и j: ";
                int i, j; cin >> i >> j;
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                A.inputEdit(i, j);
                A.print("массив после изменения:");
                break;
            }

            case 5: {
                cout << "введите i и j: ";
                int i, j; cin >> i >> j;
                cin.ignore((numeric_limits<streamsize>::max)(), '\n');
                if (A.Delete(i, j)) cout << "удалено.\n";
                else cout << "не удалось удалить: неверный индекс.\n";
                A.print("массив после удаления:");
                break;
            }

            case 6: {
                cout << "введите значение для удаления: ";
                string val;
                getline(cin, val);
                int removed = A.Delete(val);
                cout << "удалено элементов: " << removed << "\n";
                A.print("массив после удаления по значению:");
                break;
            }

            case 7:
                ++A;
                A.print("после ++A:");
                break;

            case 8:
                A.sortRows();
                A.print("после сортировки внутри строк:");
                break;

            case 9: {
                JaggedArray B = A;
                JaggedArray C = A + B;
                C.print("результат A + A:");
                break;
            }

            case 10:
                try {
                    A = JaggedArray::loadFromTxt("A.txt");
                    cout << "загружено из A.txt\n";
                    A.print();
                }
                catch (const exception& e) {
                    cout << "ошибка: " << e.what() << "\n";
                }
                break;

            case 0:
                cout << "выход из программы.\n";
                break;

            default:
                cout << "неверный пункт меню.\n";
            }
        }
        catch (const exception& e) {
            cout << "ошибка: " << e.what() << "\n";
        }
    }

    return 0;
}