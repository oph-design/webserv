#!/usr/bin/python3

from random import choice
import sys

# Read from stdin until EOF (Ctrl+D on Unix-like systems or Ctrl+Z on Windows)
comment = [
    'Nice individual linter u got there',
    'You know we were supposed to write c++ code rigth? But maybe it compiles anyway. \n\
\n\
On a serious note:\n\
It`s a great summary and is in accordance with what I read so far.\n\
Only thing I`d add would be examples for request-, response headers,etc.\n\
But we can do that later, enjoy your sunday! ^^',
    '5 failed pushes? \n\
Learn how to code inside the norm',
    'The code looks ok',
    'Ok ...\n\
random ass pull request, but Ok',
    'What a wonderful contribution\n\
I`m sure we`re not going to wait to use it for the next 3 weeks',
    'LGTM',
    'changed a file unrelated to the topic of the PR.\n\
If you weren`t a junior you would be fired now.',
    'I guess because you couldn`t get through our code styling requirements you had to change them. Very smart ... very smart',
    'ok, but next time pls not decide something like that on the fly.\n\
Instead setup a meeting in 3 days with at least 2 follow up meetings and 3 unnecessary attendants to make the decision',
    'JA GANZ TOLL!',
    'Bro what`s wrong with this test case',
    'These changes will boost our performance by at least 0,01 micro seconds per iteration in each loop. Mind Blowing 🤯',
    'Let`s trust that it works somehow',
    'Pls stay closer to our styling requirements in the future',
    'Ok but can you now FINALY work on webserv?',
    'I get payed way to less for this stuff',
    'And I thought cpplint was annoying',
    'I was about to cancel your vacation, but you did a decent job. Consider yourself patted on the back.',
    'Understood nothing, but it looks fine I guess',
    'i press green button',
    'Keep in mind working hourse start at 9 and wont be paid before',
    'wo sassy comment?',
    'I especially liked that you removed the empty space in the Makefile after the main. Mistakes like those really sometimes are hard to find, especially when the linter just gives you a vague description. At least thats what i heard',
    'I didn`t even know we had a include folder',
    'urgh, so many colors',
    'If there are 18 commits and 12 of them are from me. Is it really YOUR Pullrequest?',
    'ubuntu? Where is your ambition ...'
           ]

currentComment = choice(comment)

stdin_contents = sys.stdin.read()

print('Content-Type: text/plain')
print(f'Content-Length: {len(currentComment)}')
print('Connection: keep-alive')
print('')
print("Content in stdin:")
print(stdin_contents)
print (currentComment)
