## prerequisites 
install valgrind, zip and unzip commands
```sh
sudo apt-get install valgrind #ubuntu
yay -S valgrind #arch
```

## setup
### automatic
```sh
sh setup.sh
```

### manual
1. run chmod lines from the setup script 
2. download archive from this [link](https://github.com/freeuni-paradigms/assn-2-six-degrees-data/archive/master.zip). extract archive. move `data` folder in the assignment folder

## development

1. build: `make`

2. run: `./six-degrees`

3. test
```sh
./six-degrees-checker64 ./six-degrees
# check for memory leaks
./six-degrees-checker64 ./six-degrees -m
```
