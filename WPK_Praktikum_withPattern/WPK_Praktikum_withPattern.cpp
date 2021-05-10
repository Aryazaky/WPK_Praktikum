#include <iostream>
#include <vector>
#include <string>
using namespace std;

class DebugLog {
    struct Message {
        string msg;
        bool isError;
    };
    vector<Message> log;
public:
    DebugLog() { }
    void PrintLogToConsole(bool printErrorOnly) {
        for (const auto& msg : log) {
            if (msg.isError && printErrorOnly) {
                cout << msg.msg << endl;
            }
            else if (!printErrorOnly) {
                cout << msg.msg << endl;
            }
        }
    }
    void Log(string msg, bool isError) {
        log.push_back(Message{ msg, isError });
    }
};
class VectorD {
    vector<vector<double>> vec;
    bool isValid(int index) {
        if (!isReady) {
            cout << "Vector is empty!" << endl;
            return false;
        }
        if (index >= vec.size()) {
            cout << "Out of range. Max index was " << vec.size() << ". index given = " << index << endl;
            return false;
        }
        return true;
    }
    bool isReady;
public:
    VectorD(int size) {
        AddEmptyVec(size);
    }
    VectorD() {
        isReady = false;
    }
    void Clear() { vec.clear(); isReady = false; }
    void Resize(int new_size) {
        if (new_size > vec.size()) {
            AddEmptyVec(new_size - vec.size());
        }
        else
        {
            PopBackVec(vec.size() - new_size);
        }
    }
    // Returns a copy of the whole 2D vector
    const vector<vector<double>> Get() { return vec; }
    void Push(double x, int index) {
        if (isValid(index)) {
            vec[index].push_back(x);
        }
    }
    // Returns a copy of a vector on the specified index
    const vector<double> At(int index) {
        if (isValid(index)) {
            return vec[index];
        }
    }
    void AddEmptyVec(int count) {
        while (count--) {
            vec.push_back(vector<double>());
            isReady = true;
        }
    }
    void PopBackVec(int count) {
        if (isValid(count - 1)) {
            while (count--) {
                vec.pop_back();
            }
        }
    }
};
class Function {
public: 
    virtual double Fx(double x) = 0;
};
class Method {
protected:
    VectorD vec;
    Function* func;
public:
    Method(Function& function, int vec_size) : func(&function), vec(vec_size) { }
    void ChangeFunction(Function& new_function) { func = &new_function; }
    virtual void run(double x) = 0;
    virtual void run(double a, double b, int n) = 0;
    virtual void run(double a, double b, int n, int m) = 0;
    virtual void run(double a, double b, double e, int n) = 0;
};

// -------------------------------------------------------------------
// Fungsi dan method di bawah
// -------------------------------------------------------------------

class FX_expsin2Xp1 : public Function {
public:
    double Fx(double x) {
        return exp(-x) * sin(2 * x) + 1;
    }
};

class ForwardNumericalDifferentiation : public Method {
    void FillDifferentiationTable() {
        for (int i = 2; i < vec.Get().size(); i++) {
            for (int o = 1; o < vec.At(i - 1).size(); o++) {
                vec.Push(vec.At(i - 1)[o] - vec.At(i - 1)[o - 1], i);
            }
        }
    }
    // Menghitung dari B hingga vector sebelum ini menggunakan rumus tertentu
    void CalculatePn1(double h) {
        vec.AddEmptyVec(1);
        for (int j = 0; j < vec.At(0).size(); j++) {
            double result = 0;
            for (int i = 1; i < vec.Get().size() - 1; i++) {
                if (i % 2 == 0) {
                    result -= h / i * vec.At(i).at(j);
                }
                else {
                    result += h / i * vec.At(i).at(j);
                }
            }
        }
        
    }
public:
    ForwardNumericalDifferentiation(Function& function) : Method(function, 4) {}
    // Batas bawah = a
    // Batas atas = b
    // Pembagi = n
    // Index yang dihitung angkanya
    void run(double a, double b, int n, int m) {
        if (b < a) {
            cout << "B must be larger than A\n";
            return;
        }
        if (n < 1) {
            cout << "N must be at least 1\n";
            return;
        }
        double h = (b - a) / n;
        // Membuat tabel newton forward

        // Ukuran vector A = n+1; isinya x yang sudah dibagi2 ke n+1 bagian
        // Ukuran vector B = vector A; isinya hasil kalkulasi F(x) dari vector A dengan index yg sama
        // Jumlah vector lain (B, C, D, dst) = n-1; isinya selisih dari vector sebelumnya index yang sama dengan index sebelumnya
        vec.Clear();
        vec.Resize(2 + n - 1);
        // Mengisi vector A dan B
        for (int i = 0; i < n + 1; i++) {
            vec.Push(a + (h * i), 0); // 0 itu vector A
            vec.Push(func->Fx(a + (h * i)), 1); // 1 itu vector B
        }
        // Mengisi vector lainnya mulai dari C
        FillDifferentiationTable();
        // Menghitung forward differentiation
    }
};

int main()
{
    cout << "Hello World!\n";
}