## setup
0. install valgrind
```sh
sudo apt-get install valgrind #ubuntu
yay -S valgrind #arch
```

change script permissions
```sh
chmod u+x rsgChecker*
chmod u+x rsg-sample*
```

1. build: `make`

2. run: `./rsg data/bionic.g`

3. test
```sh
./rsgChecker32 ./rsg data/bionic.g
./rsgChecker64 ./rsg data/bionic.g
```

4. test all
```sh
for i in $(/bin/ls data); do
	echo $i
	./rsgChecker64 ./rsg data/$i
done
```

one line
```sh
for i in $(/bin/ls data); do echo $i; ./rsgChecker64 ./rsg data/$i; done
```
