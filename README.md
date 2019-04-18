# socketServCli

## Instructions

### Task 1:  

- Site: https://beej.us/guide/bgnet/html/single/bgnet.html#clientserver
- Compile the examples server.c, client.c, listener.c and talker.c using gcc and a makefile.
- When using gcc, turn on as many warning options as possible. Include these warning options in your makefile.
- Use gdb and the gcc warnings to help you debug, so that when you compile your code, gcc emits as few warnings as possible.

NOTE:
When you run gcc, you can include command options. Lines 44-48 of the installing_openssl text file show you examples of command options. These are also known as compilation flags.

https://gcc.gnu.org/onlinedocs/gcc/Invoking-GCC.htmlThese
 compilation flags control how your code is compiled.When you write your makefile, include compilation flags so that the compiler gives as many warnings as possible.

https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#Warning-Options
Warnings are different from errors. Errors must be fixed because your code will not compile and your program will not run if errors are not fixed. It is often possible for your code to compile even if it is badly written or does the wrong thing. This is because the compiler is only able to catch syntax errors in the code, but not other errors. Using command options in gcc to give you more warnings can help reduce the risk of these non-syntax errors.The aim of the exercise is to get used to having the compiler give you as many warnings as possible, and fixing your code so that warnings are minimised. This is good practice, because it can minimise bigger problems in the code if the code ends up being used for a long time.

See these links for more information:

https://stackoverflow.com/questions/32585072/how-to-make-gcc-in-cmd-exe-shows-all-errors
https://stackoverflow.com/questions/154630/recommended-gcc-warning-options-for-cif 

you use a good linter with your IDE, it can warn you in advance of the things that the compiler will complain about. I use Atom as an IDE and my linters are the following packages: linter 2.2.0, linter-ui-default 1.7.1, linter-gcc 0.7.1 (for c and c++), linter-pylama (for python)


### Task 2:  

- Add the following features to your server.c (TCP) and listener.c (UDP) server programs:
    - The port that the server programs listen on is to be set at the command line, e.g. ./server -p 5678
     - reverse a string sent by the client.
- Add the following features to your client.c (TCP) and talker.c (UDP) client programs:
   - The port that the client connects to is also to be set at the command line, e.g. ./client -h 127.0.0.1:9000
- Use gcc, gdb and make to help you.


### Task 3: The dreaded OpenSSL Library  

If you get to this point, give yourself a pat on the back, network programming is hard. Now to dive into the hell that is the OpenSSL source code.
- Download the latest long-term support version of OpenSSL (now 1.1.1b) and install it on your computer so that it does not conflict with your system installation. (instructions are attached)
- Use the TLS API to secure the communication between your TCP programs server.c and client.c.

## Help

Use gcc, gdb and make to help you.Here are some links to help you:
- Beej's Guide to Network Programming, or how many of us got through network programming at school: https://beej.us/guide/bgnet/
A French Translation: http://vidalc.chez.com/lf/socket.html#reference
- Another tutorial: http://www.lowtek.com/sockets/
- The bible on Network Programming: http://www.masterraghu.com/subjects/np/introduction/unix_network_programming_v1.3/
- Beej's Guide to GDB: https://beej.us/guide/bggdb/
- guide to gcc and make: https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html
- http://mrbook.org/blog/tutorials/make/
- THE documentation on make: https://www.gnu.org/software/make/manual/make.html
- gdb cheatsheet: http://darkdust.net/files/GDB%20Cheat%20Sheet.pdf
- The gdbinit file I use: https://gist.github.com/CocoaBeans/1879270 It steps through the instructions not by line, but by machine instruction and shows you the registers (!!)
- Atom Text Editor - what I use for programming - you might have your favourite IDE or text editor: https://atom.io/
- OpenSSL: https://www.openssl.org/source/
- OpenSSL manpages (lists most, but not all functions): https://www.openssl.org/docs/manmaster/man3/
- Parsing command line options in C: https://www.gnu.org/software/libc/manual/html_node/Getopt.html#Getopt
- Warning options for gcc:
https://stackoverflow.com/questions/154630/recommended-gcc-warning-options-for-c
https://stackoverflow.com/questions/32585072/how-to-make-gcc-in-cmd-exe-shows-all-errors
- Online manpages for Linux, also lists many C functions: http://man7.org/linux/man-pages/dir_all_alphabetic.html
- The book about C programming in Unix, it's the only textbook I still use.: http://www.codeman.net/wp-content/uploads/2014/04/APUE-3rd.pdf- OpenSSL examples:
https://wiki.openssl.org/index.php/SSL/TLS_Client
https://wiki.openssl.org/index.php/Simple_TLS_Server
http://fm4dd.com/openssl/sslconnect.htm
http://www.cs.odu.edu/~cs772/fall08/lectures/ssl_programming.pdf
https://www.linuxjournal.com/article/4822
It is said that the source code of OpenSSL Is the documentation, so don't be shy to jump in and look at it "grep -r "search term" /path/to/openssl"
https://www.linuxjournal.com/article/4822
https://people.freebsd.org/~syrinx/presentations/openssl/OpenSSL-Programming-20140424-01.pdf
http://www.informit.com/articles/article.aspx?p=22079

