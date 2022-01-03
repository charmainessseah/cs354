
// for each of the following functions 
// correct the prototype
// write function body
// replace return with the correct return statement
//
// you may write as many helper functions here as you wish

int algorithm_1() {
    return 8000 + 63;  
}

int algorithm_2(int x, int y) {
    if (x >= y) {
        return x / y;
    } else {
        int sum = x * 3;
        sum += y;
        return sum;
    }
}

int algorithm_3(int x, int y) {
    int sum = 0;
    while (y > 0) {
        sum += x;
        y--;
    }
    return sum;
}

void algorithm_4(int x, int *p) {
    int newVal = x * 3;
    *p = newVal;
}



