# Лабораторная работа 7

## Задание 1

**1.1**


```
./uclient 127.0.0.1
Enter string
Hello World!
^C
```
```
./userver 
SERVER starts...
REQUEST Hello World!
FROM 127.0.0.1 : 50221
^C
```


## Задание 2

Команда гуишная тулза netactview тащит (я использовал ее вместо netstat -a –p):

tcp 	49420 	ESTABLISHED 	127.0.0.1 	30012 	localhost 	3901 	tclient
tcp 	30012 	ESTABLISHED 	127.0.0.1 	49420 	localhost 	3900 	tserver

