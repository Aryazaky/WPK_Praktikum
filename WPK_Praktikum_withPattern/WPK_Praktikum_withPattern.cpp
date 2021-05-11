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
// A 2D double vector with assignable names
class VectorD {
    vector<vector<double>> vec;
    vector<string> vec_names;
    template<typename T>
    bool isValid(vector<T> _vector, int index) {
        if (!isReady) {
            cout << "Vector is empty!" << endl;
            return false;
        }
        if (index >= _vector.size()) {
            cout << "Out of range. Max index was " << _vector.size() << ". index given = " << index << endl;
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
    int GetSize() { return vec.size(); }
    int GetSize(int index) { return vec[index].size(); }
    void Push(double x, int index) {
        if (isValid(vec, index)) {
            vec[index].push_back(x);
        }
    }
    void Push(double x, int index, string new_name) {
        if (isValid(vec, index)) {
            vec[index].push_back(x);
            SetVecName(index, new_name);
        }
    }
    // Returns a copy of a vector on the specified index
    const vector<double> Get(int index) {
        if (isValid(vec, index)) {
            return vec[index];
        }
    }
    void AddEmptyVec(int count) {
        while (count--) {
            vec.push_back(vector<double>());
            vec_names.push_back("vec" + GetSize());
            isReady = true;
        }
    }
    void SetVecName(int index, string new_name) {
        if (isValid(vec_names, index)) {
            vec_names[index] = new_name;
        }
    }
    void PopBackVec(int count) {
        if (isValid(vec, count - 1)) {
            while (count--) {
                vec.pop_back();
            }
        }
    }
    string GetVecName(int index) {
        return vec_names[index];
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
    bool isFilled;
    virtual double CalculateDifference(double x, double delta_x) = 0;
    virtual double ErrorValue(double x, double delta_x) {
        return abs(CalculateDifference(x, delta_x) - (func->Fx(x + delta_x) - func->Fx(x - delta_x)) / delta_x / 2);
    }
public:
    Method(Function& function, int vec_size) : func(&function), vec(vec_size), isFilled(false) { }
    void ChangeFunction(Function& new_function) { func = &new_function; }
    virtual void PrintTable() {
        if (!isFilled) {
            cout << "[ERROR] Table not yet generated\n";
            return;
        }
        cout << "\nResult Table:\n";
        for (int i = 0; i < vec.GetSize(); i++) {
            cout << vec.GetVecName(i) << "\t";
        }
        cout << endl;
        for (int i = 0; i < vec.GetSize(0); i++) {
            for (int o = 0; o < vec.GetSize(); o++) {
                cout << vec.Get(o)[i] << "\t";
            }
            cout << endl;
        }
        cout << endl;
    }
    virtual void run(double x) { cout << "[ERROR] Not implemented\n"; }
    virtual void run(double a, double b, double h) {
        if (b < a) {
            cout << "B must be larger than A\n";
            return;
        }
        if (h <= 0) {
            cout << "H must be more than 0\n";
            return;
        }
        run(a, b, floor((b - a) / h));
    }
    virtual void run(double a, double b, int n) {
        if (b < a) {
            cout << "B must be larger than A\n";
            return;
        }
        if (n < 1) {
            cout << "N must be at least 1\n";
            return;
        }

        double h = (b - a) / n;

        vec.Clear();
        vec.Resize(5);
        // Isi tabel: 
        // X; F(x); F'(x); F'eksak; error;

        // Copy x, f(x), dan f'(x) ke tabel
        for (int i = 0; i < n + 1; i++) {
            double x = a + (h * i);
            vec.Push(x, 0, "X"); // 0 itu vector X
            vec.Push(func->Fx(x), 1, "F(X)"); // 1 itu vector F(X)
            vec.Push(CalculateDifference(x, h), 2, "F'(X)");
            vec.Push((func->Fx(x + h) - func->Fx(x - h)) / h / 2, 3, "F'eksak(X)");
            vec.Push(ErrorValue(x, h), 4, "error");
            isFilled = true;
        }
    }
    virtual void run(double a, double b, double h, int n) { cout << "[ERROR] Not implemented\n"; }
};
class Program {
    Method* method;
public:
    Program(Method& _method) : method(&_method){}
    void SetMethod(Method& _method) { method = &_method; }
    void Run(double a, double b, double h) { method->run(a, b, h); }
    void Run(double a, double b, int n) { method->run(a, b, n); }
    void Print() { method->PrintTable(); }
};

// -------------------------------------------------------------------
// Fungsi dan method di bawah
// -------------------------------------------------------------------

// Fungsi A
class FX_expmXsin2Xp1 : public Function {
public:
    double Fx(double x) {
        return exp(-x) * sin(2 * x) + 1;
    }
};
// Turunan fungsi A
class FX_expmX2cos2Xmsin2X : public Function {
public:
    double Fx(double x) {
        return exp(-x) * (2 * cos(2 * x) - sin(2 * x));
    }
};
// Turunan dari turunan fungsi A
class FX_mexpmX3sin2Xp4cos2X : public Function {
public:
    double Fx(double x) {
        return -exp(-x) * (3 * sin(2 * x) + 4 * cos(2 * x));
    }
};

class ForwardNumericalDifferentiation : public Method {
    double CalculateDifference(double x, double delta_x) {
        return (func->Fx(x + delta_x) - func->Fx(x)) / delta_x;
    }
public:
    ForwardNumericalDifferentiation(Function& function) : Method(function, 5) {}
};

class MiddleNumericalDifferentiation : public Method {
    double CalculateDifference(double x, double delta_x) {
        return (func->Fx(x + delta_x) - func->Fx(x - delta_x)) / delta_x / 2;
    }
public:
    MiddleNumericalDifferentiation(Function& function) : Method(function, 5) {}
};

class BackwardNumericalDifferentiation : public Method {
    double CalculateDifference(double x, double delta_x) {
        return (func->Fx(x) - func->Fx(x - delta_x)) / delta_x;
    }
public:
    BackwardNumericalDifferentiation(Function& function) : Method(function, 5) {}
};

int main()
{
    cout << "Hello World!\n";
    Function* fx;
    fx = new FX_expmXsin2Xp1();
    ForwardNumericalDifferentiation forwardNumDif(*fx);
    MiddleNumericalDifferentiation middleNumDif(*fx);
    BackwardNumericalDifferentiation backwardNumDif(*fx);

    Program program(forwardNumDif);

    program.Run(0, 1, 10);
    program.Print();

    delete(fx);
}