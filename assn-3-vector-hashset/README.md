## Instructions
- Clone the repository
- Open terminal and run `sh setup.sh` or `./setup.sh`

### Development

#### Prerequsits 
valgrind

```sh
sudo apt-get install valgrind #ubuntu
yay -S valgrind #arch
```

#### App
build

```sh
make
```

test

```sh
./vector-test
./hashset-test
```

To compare your output to real output, you can use program `diff` (using this command)
```sh
./vector-test | diff sample-output-vector.txt - 
./hashset-test | diff sample-output-hashset.txt -
```
