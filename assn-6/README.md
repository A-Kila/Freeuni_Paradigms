## instructions
- clone the repository

### development

prerequisits. valgrind

```sh
sudo apt-get install valgrind #ubuntu
yay -S valgrind #arch
```

build

```sh
make
```

test

```sh
./bankdriver -t1 -w4  
Creates 4 threads and runs the first test.  
(There are 7 different tests (from -t1 to -t7), testing can be performed using different number of threads).  
./bankdriver -t1 -w4 -b (Checks the balance).  
./bankdriver -t1 -w2 -y  
(if -y is followed by a number, for example: -y50, then there is a probability of 50% that a thread stops execution).  
valgrind --tool=helgrind ./bankdriver -r -t1 -w4
```
