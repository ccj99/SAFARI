#ifndef _LASSO_H
#define _LASSO_H

class Lasso{
public:
    const static double eps = 0;
	//par_x size m*n; par_y size m; par_b size n
	double **par_x , *par_y , *par_b, *K;
    bool *in;
    int n,m;
    void init(int _n,int _m);
    double cos2(int j);
    double dist();
    void Least_Angle_Regression();
};

#endif