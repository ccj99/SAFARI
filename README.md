# SAFARI(paper name)

This is a C++ implementation of SAFARI in "[SAFARI (paper name)](https://arxiv.org/abs/1706.03762)". 

If there is any suggestion or error, feel free to fire an issue to let me know. :)

# Requirement
- g++  5.1.0+

# Usage


## Ranking dataset

The example below uses the ranking dataset extracted from the Sushi dataset and Jester dataset. From the Sushi dataset, we generate dataset with 5,000 user and the number of items ranging from 3 to 10. Similary, from the Jester dataset, we generate dataset with 20,000 users and the number of items ranging from 3 to 10. The path of the ranking dataset is:
```bash
./dataset
```

> You can also use your own dataset, your  dataset should meet the following format. Each line in your dataset file should be one of user's data. The data format is item's rank (i.e. a permutation of [0~n-1]).
```bash
0 1 3 2
3 2 0 1
1 0 2 3
...
```

## Build code
You can build the code in the following way:
```bash
mkdir obj
mkdir bin
make ./bin/naive 
make ./bin/safari 
make clean
```


## Run the naive model
```bash
./bin/naive --user_number 1000 --item_number 4 --epsilon 1.0 --input_file ./demo_dataset.txt --output_file out.txt --method rappor
```
> Explanation of the parameters :  
>> **user_number** : the number of data in dataset file.  
>> **item_number** : the number of item in one data.  
>> **epsilon** : The privacy budget.  
>> **input_file** : The file name of dataset file.  
>> **output_file** : The file name of output file.  
>> **method** : The method of local differential privacy. You can chosse "rappor", "sh", "olh", "grr" or "noldp" (i.e. don't use local differential privacy).

## Run the safari model
```bash
./bin/safari --user_number 1000 --item_number 4 --k 1 --epsilon1 0.5 --epsilon2 0.5 --input_file ./demo_dataset.txt --output_file out.txt --method rappor --use_lasso yes
```
> Explanation of the parameters :  
>> **user_number** : the number of data in dataset file.  
>> **item_number** : the number of item in one data.  
>> **k** : Build a k-thin chain model in the first phase (i.e. the cliques are are never allowed to have more than k variables).   
>> **epsilon1** : The privacy budget in the first phase.  
>> **epsilon2** : The privacy budget in the second phase.  
>> **input_file** : The file name of dataset file.  
>> **output_file** : The file name of output file.  
>> **method** : The method of local differential privacy. You can chosse "rappor", "sh", "grr" or "noldp" (i.e. don't use local differential privacy).  
>> __use_lasso__ : "yes" or "no", which means whether you use lasso in the first phase.

# Documentation
- **./include/dataset.h ./bin/dataset.cpp** :  
    Build ranking dataset, and evaluate the difference between two ranking datasets.
- **./include/lasso.h ./bin/lasso.cpp** :  
    The implementation of lasso regression.
- **./include/ldp.h ./bin/grr.cpp ./bin/rappor.cpp ./bin/olh.cpp ./bin/sh.cpp** :  
    The implementation of four methods of local differential privacy. (Rappor, SH, GRR, OLH)
- **./include/util.h ./src/util.cpp** :  
    Some tools, sush as calculating mutual information and Cantor expansion.
- **./include/safari.h** :  
    The implementation of Safari. 
    - **./src/safair/phase_one.cpp**: The implementation of the first phase without lasso. 
    - **./src/safair/phase_one_with_lasso.cpp**: The implementation of the first phase with lasso. 
    - **./src/safair/learning_hierarchi.cpp**: The implementation of learning the k-thin chain model.
    - **./src/safair/phase_two.cpp** : The implementation of the second phase and rebuild ranking dataset. 
- **./src/naive/naive_main.cpp** :  
    The main of naive method.
- **./src/safari/safari_main.cpp** :  
    The main of safari method.




