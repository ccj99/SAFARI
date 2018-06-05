# Collecting Preference Rankings under Local Differential Privacy
 
[Collecting Preference Rankings under Local Differential Privacy (technical report)](https://github.com/cheng-lab-at-bupt/SAFARI/blob/master/Collecting%20Preference%20Rankings%20under%20Local%20Differential%20Privacy%20(technical%20report).pdf)  
The source code of the **SAFARI** approach is publicly available.

# Experimental requirement
- g++  5.1.0+

# Usage
An example of running the experiments is shown as follows.

## Ranking dataset

The test ranking datasets are extracted from the [Sushi dataset](http://www.kamishima.net/sushi/) and [Jester dataset](http://goldberg.berkeley.edu/jester-data/). The path of these ranking datasets is:
```bash
./SAFARI_code/dataset
```

## Build code
The code can be built by the following steps:
```bash
cd ./SAFARI_code/
mkdir obj
mkdir bin
make ./bin/naive 
make ./bin/safari 
make clean
```


## 1) Guideline for running the baselines 
```bash
cd ./SAFARI_code/
./bin/naive --user_number 1000 --item_number 4 --epsilon 1.0 --input_file ./demo_dataset.txt --output_file out.txt --method rappor
```
> Explanation of the parameters :  
>> **user_number** : the total number of users  
>> **item_number** : the total number of items   
>> **epsilon** : the privacy budget  
>> **input_file** : the file name of dataset  
>> **output_file** : the file name of output  
>> **method** : the baseline approach ("rappor", "sh" or "olh")  

## 2) Guideline for running SAFARI
```bash
cd ./SAFARI_code/
./bin/safari --user_number 1000 --item_number 4 --k 1 --epsilon1 0.5 --epsilon2 0.5 --input_file ./demo_dataset.txt --output_file out.txt --method safa --use_lasso yes
```
> Explanation of the parameters :  
>> **user_number** : the total number of users   
>> **item_number** : the total number of items  
>> **k** : the maximum size of a leaf item set   
>> **epsilon1** : the privacy budget used in the **Phase 2** of SAFARI  
>> **epsilon2** : the privacy budget used in the **Phase 4** of SAFARI  
>> **input_file** : the file name of dataset  
>> **output_file** : the file name of output  
>> **method** : frequency estimation for multiple attributes ("safa" or "harmony")  
>> **use_lasso** : "yes" or "no", which indicates whether the lasso regression model is used (i.e. **Rule I** or **Rule I***)

# Documentation
- **./include/dataset.h ./bin/dataset.cpp** :  
    synthesizng a ranking dataset, examining the accuracy of the first-order marginals and the second-order marginals of the synthetic ranking dataset
- **./include/lasso.h ./bin/lasso.cpp** :  
    implementation of the lasso regression model
- **./include/ldp.h ./bin/grr.cpp ./bin/rappor.cpp ./bin/olh.cpp ./bin/sh.cpp** :  
    implementations of the Rappor, SH, OLH and GRR, respectively (rappor.cpp, sh.cpp, olh.cpp and grr.cpp)
- **./include/util.h ./src/util.cpp** :  
    tripletwise mutual information computation, data structure of rankings (Cantor expansion)
- **./include/safari.h** :  
    implementation of the SAFARI approach
    - **./src/safari/first_collect_with_lasso.cpp** :  
        implementation of the Phase 1 of SAFARI (with the lasso regression model)
    - **./src/safari/first_collect.cpp** :  
        implementation of another version of the Phase 1 of SAFARI (without the lasso regression model)
    - **./src/safari/learning_hierarchi.cpp** :  
        implementation of the k-thin chain construction in the Phase 2 of SAFARI
    - **./src/safari/second_collect.cpp** :  
        implementation of the Phases 3-5 of SAFARI
- **./src/naive/naive_main.cpp** :  
    main function used for the baselines
- **./src/safari/safari_main.cpp** :  
    main function used for SAFARI




