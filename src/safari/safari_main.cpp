#include<stdio.h>
#include<stdlib.h>
#include<algorithm>
#include<iostream>
#include<vector>
#include<math.h>
#include<string.h>
#include<assert.h>
#include<map>
#include<time.h>
#include<getopt.h>
#include"safari.h"

using namespace std;

static struct option long_options[] =
{
    {"user_number", required_argument, NULL, 0},
    {"item_number", required_argument, NULL, 0},
    {"k", required_argument, NULL, 0},
    {"epsilon1", required_argument, NULL, 0},
    {"epsilon2", required_argument, NULL, 0},
    {"input_file", required_argument, NULL, 0},
    {"output_file", required_argument, NULL, 0},
    {"method", required_argument, NULL, 0},
    {"use_lasso", required_argument, NULL, 0}
};

int user_number=-1, item_number=-1, k=-1, option_index;
char input_file[100]="",output_file[100]="",method[100]="";
char use_lasso[100]="no";
double epsilon1=-1, epsilon2=-1;
double mI[MAXRANK][MAXRANK][MAXRANK];
RankDataset real_dataset, synthetic_dataset;

int main(int argc, char * argv[])
{
    while( getopt_long(argc, argv, "", long_options, &option_index)!= -1)
    {
        printf("%s %s\n", long_options[option_index].name, optarg);
        
        switch (option_index)
        {
        case 0:
            user_number = strtol(optarg, NULL, 10);
			break;
        case 1:
            item_number = strtol(optarg, NULL, 10);
			break;
		case 2:
            k = strtol(optarg, NULL, 10);
			break;
		case 3:
            sscanf(optarg,"%lf",&epsilon1);
			break;
		case 4:
            sscanf(optarg,"%lf",&epsilon2);
			break;
		case 5:
            strcpy(input_file, optarg);
			break;
		case 6:
            strcpy(output_file, optarg);
			break;
		case 7:
            strcpy(method, optarg);
			break;
		case 8:
            strcpy(use_lasso, optarg);
			break;
		}
    }

    if( user_number == -1 || item_number == -1 || k == -1 || epsilon1 == -1 || epsilon2 == -1 
	|| input_file[0] == 0 || output_file[0] == 0 || method[0] == 0)
    {
        fprintf(stderr,"parameter error.");
        return -1;
    }
    
    random_init();

	real_dataset.init(user_number, item_number);
	synthetic_dataset.init(user_number, item_number);
    
    real_dataset.read_file(input_file);
	
	if( strcmp(use_lasso, "no") == 0 ){
		phase_one(&real_dataset, epsilon1, method, mI);
    }else if( strcmp(use_lasso, "yes") == 0 ){
		phase_one_with_lasso(&real_dataset, epsilon1, method, mI);
	}else{
		fprintf(stderr, "use lasso %s error.\n", use_lasso);
		return 0;
	}
    vector<vector<int> > hierarchical;
    learning_hierarchical(item_number, k, mI, hierarchical);
	
	phase_two(&real_dataset, hierarchical, epsilon2, method, &synthetic_dataset);
    
	pair<double, double> f1 = first_order(&real_dataset, &synthetic_dataset);
	pair<double, double> f2 = second_order(&real_dataset, &synthetic_dataset);
	
	printf("first order tvd:%f re:%f\n",f1.first,f1.second);
	printf("second order tvd:%f re:%f\n",f2.first,f2.second);
	
	FILE *fp = fopen(output_file, "a");
	fprintf(fp,"%f %f %f %f\n",f1.first,f1.second,f2.first,f2.second);
	fclose(fp);
    return 0;
}

