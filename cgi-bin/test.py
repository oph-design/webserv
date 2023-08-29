#!/usr/bin/python3

from time import sleep

sleep(5)
print('Content-Type: text/plain')
print('Content-Length: 5')
print('Connection: keep-alive')
print('')
print ('weird')
