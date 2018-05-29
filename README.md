# SAFARI(paper name)

This is a C++ implementation of SAFARI in "[SAFARI (paper name)](https://arxiv.org/abs/1706.03762)". 

If there is any suggestion or error, feel free to fire an issue to let me know. :)

# Requirement
- g++  5.1.0+

# Usage


## Download ranking dataset

The example below uses the Sushi ranking dataset and Jester ranking dataset(https://pan.baidu.com/s/1NKsVOW8eZ_2ajBYr4-6RmA, password:tnp9) 

```bash
unzip dataset.zip
```

> You can also use your own dataset, your  dataset should meet the following format. Each line in your dataset file should be one of data. The data format is item's rank (i.e. a permutation of [0~n-1]).
```bash
0 1 3 2
3 2 0 1
1 0 2 3
...
```

## Build code
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
>> user_number : the number of data in dataset file.  
>> item_number : the number of variable in one data.  
>> epsilon : The privacy budget.  
>> input_file : The file name of dataset file.  
>> output_file : The file name of output file.  
>> method : The method of local differential privacy. You can chosse "rappor", "sh", "olh", "grr" or "noldp" (i.e. don't use local differential privacy).

## Run the safari model
```bash
./bin/safari --user_number 1000 --item_number 4 --k 1 --epsilon1 0.5 --epsilon2 0.5 --input_file ./demo_dataset.txt --output_file out.txt --method rappor --use_lasso yes
```
> Explanation of the parameters :  
>> user_number : the number of data in dataset file.  
>> item_number : the number of variable in one data.  
>> k : Build a k-thin chain model in the first phase (i.e. the cliques are are never allowed to have more than k variables).   
>> epsilon1 : The privacy budget in the first phase.  
>> epsilon2 : The privacy budget in the second phase.  
>> input_file : The file name of dataset file.  
>> output_file : The file name of output file.  
>> method : The method of local differential privacy. You can chosse "rappor", "sh", "grr" or "noldp" (i.e. don't use local differential privacy).  
>> use_lasso : "yes" or "no", which means whether you use lasso in the first phase.

# Documentation
- ./include/dataset.h ./bin/dataset.cpp :  
    Build ranking dataset, and evaluate the difference between two ranking datasets.
- ./include/lasso.h ./bin/lasso.cpp :  
    The implementation of lasso regression.
- ./include/ldp.h ./bin/grr.cpp ./bin/rappor.cpp ./bin/olh.cpp ./bin/sh.cpp:  
    The implementation of four methods of local differential privacy. (Rappor, SH, GRR, OLH)
- ./include/util.h ./src/util.cpp:  
    Some tools, sush as calculating mutual information and Cantor expansion.
- ./include/safari.h ./src/safair/phase_one.cpp ./src/safair/phase_one_with_lasso.cpp ./src/safair/phase_two.cpp ./src/safair/learning_hierarchi.cpp:  
    The implementation of Safari. phase_one.cpp in the first phase without lasso. 
    phase_one_with_lasso.cpp in the first phase with lasso. 
    learning_hierarchi.cpp learn the k-thin chain model.
    phase_two.cpp in the second phase without lasso. 
- ./src/naive/naive_main.cpp :  
    The main of naive method.
- ./src/safari/safari_main.cpp :  
    The main of safari method.




