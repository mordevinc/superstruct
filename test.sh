#!/bin/bash

rm -f data/data.json
make clean && make

# массив
./bin/dbms --file data.json --query 'MPUSH myarray apple'
./bin/dbms --file data.json --query 'MPUSH myarray banana'
./bin/dbms --file data.json --query 'MPUSH myarray cherry'
./bin/dbms --file data.json --query 'MPUSH myarray date'
./bin/dbms --file data.json --query 'MPUSH myarray elderberry'

# односвязный список
./bin/dbms --file data.json --query 'FPUSH mylist one'
./bin/dbms --file data.json --query 'FPUSH mylist two'
./bin/dbms --file data.json --query 'FPUSH mylist three'
./bin/dbms --file data.json --query 'FPUSH_FRONT mylist zero'

# двусвязный список
./bin/dbms --file data.json --query 'LPUSH mydlist first'
./bin/dbms --file data.json --query 'LPUSH mydlist second'
./bin/dbms --file data.json --query 'LPUSH mydlist third'
./bin/dbms --file data.json --query 'LPUSH_FRONT mydlist zero'

# стек
./bin/dbms --file data.json --query 'SPUSH mystack bottom'
./bin/dbms --file data.json --query 'SPUSH mystack middle'
./bin/dbms --file data.json --query 'SPUSH mystack top'

# очередь
./bin/dbms --file data.json --query 'QPUSH myqueue first'
./bin/dbms --file data.json --query 'QPUSH myqueue second'
./bin/dbms --file data.json --query 'QPUSH myqueue third'

# двусвязная очередь
./bin/dbms --file data.json --query 'DPUSH_BACK mydeque back1'
./bin/dbms --file data.json --query 'DPUSH_BACK mydeque back2'
./bin/dbms --file data.json --query 'DPUSH_FRONT mydeque front1'
./bin/dbms --file data.json --query 'DPUSH_FRONT mydeque front2'

# дерево
./bin/dbms --file data.json --query 'TINSERT mytree 50'
./bin/dbms --file data.json --query 'TINSERT mytree 30'
./bin/dbms --file data.json --query 'TINSERT mytree 70'
./bin/dbms --file data.json --query 'TINSERT mytree 20'
./bin/dbms --file data.json --query 'TINSERT mytree 40'
./bin/dbms --file data.json --query 'TINSERT mytree 60'
./bin/dbms --file data.json --query 'TINSERT mytree 80'

# показать всё
./bin/dbms --file data.json --query 'PRINT'
cat data/data.json