rm /var/www/sabay/data/www/statservd/data/out.bin.snap
rm /var/www/sabay/data/www/statservd/data/out.bin.snap.idx
g++ -ggdb -lsnappy main.cpp -o main.exe;./main.exe