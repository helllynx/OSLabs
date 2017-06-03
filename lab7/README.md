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
Если запустить сервер после запуска клиента ничего не изменится, так же как и если завершить работу сервера во время работы клиента, вернее сервер просто перестанет принимать запросы и отвечать на них.

## Задание 2

```
[yen@writer lab7]$ ./tclient 127.0.0.1 30012
Input message to send
OH MY GOOD!
ITS F*** WORKING!@
^C
[yen@writer lab7]$ ./tclient 127.0.0.1 30012
Input message to send
NEW CONNECTION
^C
[yen@writer lab7]$ ./tclient 127.0.0.1 30012
Input message to send
ANOTHER NEW CONNECTION
^C
```

```
[yen@writer lab7]$ ./tserver 
connection established
OH MY GOOD!
ITS F*** WORKING!@
connection established
NEW CONNECTION
connection established
ANOTHER NEW CONNECTION
^C
```
Запустить клиент до запуска сервера не получится, т.к. он не сможет создать соединение.

Команда гуишная тулза netactview тащит (я использовал ее вместо netstat -a –p):

tcp 	49420 	ESTABLISHED 	127.0.0.1 	30012 	localhost 	3901 	tclient
tcp 	30012 	ESTABLISHED 	127.0.0.1 	49420 	localhost 	3900 	tserver

**THE END**