#ifndef _UTIL_H
#define _UTIL_H

const int MAXRANK = 60;

void random_init();
double random_p();

class KangTuo{
public:
	int n;
    int base[MAXRANK];
    void init(int nn);
    int Hash(int *a);
    void get_hash(int h,int *a);
};

#endif
