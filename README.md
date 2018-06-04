# Collecting Preference Rankings under Local Differential Privacy
 
[Collecting Preference Rankings under Local Differential Privacy (technical report)](https://github.com/cheng-lab-at-bupt/SAFARI/blob/master/Collecting%20Preference%20Rankings%20under%20Local%20Differential%20Privacy%20(technical%20report).pdf)  
The source code of **SAFARI** is publicly available.

# Experimental Requirement
- g++  5.1.0+

# Usage
An example of running the experimets is shown as follows.

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


## 1) Guideline for running the baseline approaches 
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
./bin/safari --user_number 1000 --item_number 4 --k 1 --epsilon1 0.5 --epsilon2 0.5 --input_file ./demo_dataset.txt --output_file out.txt --method rappor --use_lasso yes
```
> Explanation of the parameters :  
>> **user_number** : the total number of users   
>> **item_number** : the number of item in the item set  
>> **k** : the maximum size of a leaf item set   
>> **epsilon1** : the privacy budget in the **Phase 2**  
>> **epsilon2** : the privacy budget in the **Phase 4**  
>> **input_file** : the file name of dataset  
>> **output_file** : the file name of output  
>> **method** : frequency estimation for multiple attributes. (e.g., "grr" represents SAFA, "sh" represents harmony)  
>> **use_lasso** : "yes" or "no", which represents whether you use lasso regression model (i.e. **Rule I** or **Rule I***)

# Documentation
- **./include/dataset.h ./bin/dataset.cpp** :  
    synthesizng a ranking dataset, examining the accuracy of the first-order marginals and the second-order marginals of the synthetic ranking dataset
- **./include/lasso.h ./bin/lasso.cpp** :  
    implementation of lasso regression model
- **./include/ldp.h ./bin/grr.cpp ./bin/rappor.cpp ./bin/olh.cpp ./bin/sh.cpp** :  
    implementation of Rappor, SH, OLH and GRR. (rappor.cpp, sh.cpp, olh.cpp and grr.cpp)
- **./include/util.h ./src/util.cpp** :  
    tripletwise mutual information computation, data structure of rankings (Cantor expansion)
- **./include/safari.h** :  
    implementation of SAFARI
    - **./src/safair/phase_one_with_lasso.cpp** :  
        implementation of the Phase 1 of SAFARI (with lasso regression model)
    - **./src/safair/phase_one.cpp** :  
        implementation of another version of Phase 1 of SAFARI (without lasso regression model)
    - **./src/safair/learning_hierarchi.cpp** :  
        implementation of construct the k-thin chain in Phase 2 of SAFARI
    - **./src/safair/phase_two.cpp** :  
        implementation of Phase 4 and 5 of SAFARI
- **./src/naive/naive_main.cpp** :  
    main function used for baseline approaches
- **./src/safari/safari_main.cpp** :  
    main function used for SAFARI




