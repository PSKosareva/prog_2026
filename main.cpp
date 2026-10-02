#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <windows.h>
#include <cctype>

using namespace std;

const WORD ROW_COLORS[4] = {
    FOREGROUND_RED | FOREGROUND_INTENSITY,
    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY
};
const WORD WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;

class JaggedArray {
    vector<vector<string>> data;
public:
    JaggedArray() {}
    JaggedArray(const vector<vector<string>>& d) : data(d) {}

    class Row {
        vector<string>& r;
    public:
        Row(vector<string>& row) : r(row) {}
        string& operator[](int j) {
            if (j < 0 || j >= (int)r.size()) throw out_of_range("Индекс j вне границ");
            return r[j];
        }
    };
    class ConstRow {
        const vector<string>& r;
    public:
        ConstRow(const vector<string>& row) : r(row) {}
        const string& operator[](int j) const {
            if (j < 0 || j >= (int)r.size()) throw out_of_range("Индекс j вне границ");
            return r[j];
        }
    };

    Row operator[](int i) {
        if (i < 0 || i >= (int)data.size()) throw out_of_range("Индекс i вне границ");
        return Row(data[i]);
    }
    ConstRow operator[](int i) const {
        if (i < 0 || i >= (int)data.size()) throw out_of_range("Индекс i вне границ");
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
        if (k < 0 || k >= (int)data.size()) throw out_of_range("Индекс k вне границ");
        data[k].push_back(item);
    }

    void sortRows() {
        for (auto& r : data) sort(r.begin(), r.end());
    }

    JaggedArray operator+(const JaggedArray& other) const {
        int n = (int)max(data.size(), other.data.size());
        JaggedArray res;
        res.data.resize(n);
        for (int i = 0; i < n; i++) {
            int na = (i < (int)data.size()) ? (int)data[i].size() : 0;
            int nb = (i < (int)other.data.size()) ? (int)other.data[i].size() : 0;
            int m = min(na, nb);

            for (int j = 0; j < m; j++) {
                res.data[i].push_back(data[i][j] + other.data[i][j]);
            }
        }
        return res;
    }

    JaggedArray& operator++() {
        for (auto& r : data)
            for (auto& s : r)
                if (!s.empty()) {
                    unsigned char c = (unsigned char)s[0];
                    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
                        s[0] = (char)(c + 1);
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
            for (auto& s : data[i]) cout << s << " ";
            cout << "\n";
        }
        SetConsoleTextAttribute(h, WHITE);
    }

    static JaggedArray loadFromTxt(const string& filename) { //загрузка с .txt
        ifstream f(filename);
        if (!f) throw runtime_error("Не удалось открыть файл: " + filename);

        JaggedArray arr;
        string line;
        while (getline(f, line)) {
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
        ofstream f("A.txt");
        f << "alpha beta gamma\n";
        f << "delta epsilon\n";
        f << "zeta eta theta iota\n";
    }
    ifstream t2("B.txt");
    if (!t2) {
        ofstream f("B.txt");
        f << "one two\n";
        f << "three four five\n";
        f << "six seven\n";
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    ensureSampleFiles();
    JaggedArray A, B;
    try {
        A = JaggedArray::loadFromTxt("A.txt");
        B = JaggedArray::loadFromTxt("B.txt");
    }
    catch (const exception& e) {
        cout << "Ошибка загрузки: " << e.what() << "\n";
        return 1;
    }

    A.print("Массив A:");   cout << "\n";
    B.print("Массив B:");   cout << "\n";

    cout << "A[2][2] = " << A[2][2] << "  -> меняем на \"NEW\"\n";
    A[2][2] = "NEW";
    A.print("После изменения A[2][2]:");  cout << "\n";

    try { A[99][0]; }
    catch (const out_of_range&) { cout << "A[99][0] -> ошибка: индекс вне границ\n\n"; }

    A.Delete(0, 1);
    A.print("После Delete(0, 1):"); cout << "\n";

    int removed = A.Delete("NEW");
    A.print("После Delete(\"NEW\"), удалено " + to_string(removed) + ":"); cout << "\n";

    A.add_endline(0, "tail");
    A.print("После add_endline(0, \"tail\"):"); cout << "\n";

    JaggedArray C = A + B;
    C.print("C = A + B (поэлементная конкатенация):"); cout << "\n";

    // префиксный
    ++A;
    A.print("После ++A (первый символ каждого элемента +1):"); cout << "\n";

    // постфиксный
    JaggedArray oldA = A++;
    oldA.print("A++ вернул прежнее значение:"); cout << "\n";
    A.print("A после A++:"); cout << "\n";

    A.sortRows();
    A.print("A после сортировки внутри строк:"); cout << "\n";

    system("pause");
    return 0;
}