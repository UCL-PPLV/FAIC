class Y {  
public: void doSomething();  
};

void z() { Y y; y.doSomething(); }

int doSomething(int i) {  
    if (i == 0) return 0;
    return 1 + doSomething(i--);
}

int main() {  
    return doSomething(2);
}
