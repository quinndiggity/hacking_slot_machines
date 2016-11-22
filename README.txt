**** QUIZVIZ README ********************************
[THIS VERSION AS AT 29/07/15]

How to build (in Linux):
************************
1 Ensure cmake and the OpenCV library are installed

2 In the root directory (above src), make a directory called 'build'

3 Navigate to build

4 Execute command 'cmake ..'

5 Navigate to [root]

6 Ensure a prototype image 'prototype.png' exists in this directory

How to run (in Linux):
**********************
From root directory, execute 'build/src/./quizviz [a] [b] [c]', replacing [a], 
[b] and [c] with a 0 or 1, depending on the answers to the questions 
[verbose?] [debug?] [use_vid?]. If running using video, ensure there is a
video 'input.avi' in the root directory.

How to rebuild (in Linux):
**************************
After making changes to the code, it may be necessary to rebuild. To do so,
delete the 'build' directory and then follow the directions above. This can be
automated using the provided 'rebuild.sh' shell script using the following
command: 'sh rebuild.sh'.

