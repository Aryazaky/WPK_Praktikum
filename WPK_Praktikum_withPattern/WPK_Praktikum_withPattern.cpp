#include <iostream>
#include <vector>
using namespace std;

class Vector {
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
    Vector(int size) {
        while (size--) {
            vec.push_back(vector<double>());
        }
        isReady = true;
    }
    Vector() {
        isReady = false;
    }
    const vector<vector<double>> getFullVec() { return vec; }
    void push(double x, int index) {
        if (isValid(index)) {
            vec[index].push_back(x);
        }
    }
    vector<double> getVec(int index) {
        if (isValid(index)) {
            return vec[index];
        }
    }
};
class Function {
public: 
    virtual double Fx(double x) = 0;
};
class Method {
    Vector vec;
    Function* func;
public:
    Method(Function& function, int vec_size) {
        func = &function;
        vec = Vector(vec_size);
    }
    virtual void run() = 0;
    virtual void run(double x) = 0;
};

int main()
{
    cout << "Hello World!\n";
}