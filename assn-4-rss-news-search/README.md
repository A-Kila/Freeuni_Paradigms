## instructions
- clone the repository
- open terminal and run `sh setup.sh` ან `./setup.sh`

### development

prerequisits. valgrind, zip, curl

```sh
sudo apt-get install valgrind #ubuntu
yay -S valgrind #arch

sudo apt-get install libcurl4-gnutls-dev #ubuntu
(32-ბიტიანი ვერსია: sudo apt-get install libcurl4-gnutls-dev:i386 #ubuntu)
sudo pacman -S libcurl-gnutls #arch
```

build

```sh
make
```

test

```sh
./assn-4-checker ./rss-news-search
./assn-4-checker ./rss-news-search -m
```
