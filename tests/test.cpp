class Y {  
public: void doMaths();
public: void printHelloWorld();  
};

class RandomClassHere {}; 

void z() { Y y; y.doMaths(); }

int doMaths(int i) {  
    if (i == 0) return 0;
    return 1 + doMaths(i--);
}

void printHelloWorld() { }

void unusedFunc() { }

int main() { 
	printHelloWorld();
	unusedFunc(); 
    return doMaths(2);
}
