#ifndef _LDP_H
#define _LDP_H
#include<string.h>
#include<vector>
using namespace std;

class Ldp{
public:
    double *real_number;
    int item, user_number;
	double *ldp_number;
    virtual void init(double _epsilon,int _item){
		item = _item;
		real_number = new double[item+3];
		memset(real_number, 0, sizeof(double)*(item+3));
		
		ldp_number = new double[item+3];
		memset(ldp_number, 0, sizeof(double)*(item+3));
		
		user_number = 0;
	};
    virtual void get(int r){
		real_number[r]++;
		user_number++;
	};
    virtual void compute(){
		for(int i=0;i<item;i++)
			real_number[i] /= user_number;
	}
;
};

class Rappor: public Ldp{
public:
    double *cnt;
    double p;
    void init(double _epsilon,int _item);
    void get(int r);
    void compute();
};

class Sh: public Ldp{
public:
    double *Z;
    int **phi, m;
    double p, c;
    void init(double _epsilon,int _item);
    void get(int r);
    void compute();
};

class Olh: public Ldp{
public:
    double *C;
    double p, q;
	int g;
    void init(double _epsilon,int _item);
    void get(int r);
    void compute();
};

class Noldp: public Ldp{
public:
    void init(double _epsilon,int _item);
    void get(int r);
    void compute();
};

class SamplingLdp{
public:
    Ldp** ldp;
	int *item_number, value_number, user_number;
	vector<int>id;
	double **real_number, **ldp_number;
	void init(int *_item_number,int _value_number,double epsilon,char *method);
	void get(int *item);
	void compute();
};

#endif
