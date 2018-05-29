#ifndef _UTIL_H
#define _UTIL_H

const int MAXRANK = 60;

void random_init();
double random_p();
double mutual_information(double *px,double *py,double *pxy,int lenx,int leny);
void normalize(double *x,int len);

class KangTuo{
public:
	int n;
    int base[MAXRANK];
    void init(int nn);
    int Hash(int *a);
    void get_hash(int h,int *a);
};

#endif
