## What is MemEater?
MemEater is a small DOS utility to allocate extended memory (XMS). If you have a lot of memory in your DOS machine and it causes problems with older programs, MemEater allows you to decrease the amount of available XMS by allocating some of it.

MemEater works in MS-DOS, FreeDOS and DOSBox.

## Background
Some old DOS programs will not work when the size of the free extended memory exceeds 32MB. One example of such software is a famous game Aladdin by Disney Interactive. If you try to run it on a computer which has more than 32MB of free memory, you will get "XMS allocation error" message and the game will quit. To solve this problem, you can use MemEater to decrease the amount of available memory to 32MB or less.

## Detailed explanation
According to XMS specification, information about available extended memory is returned through CPU registers. DX register contains a total amount of free extended memory (in kilobytes), and AX register contains the size of the largest free extended memory block (also in kilobytes). Those registers are 16-bit, so they can store numbers from 0 to 65535. However, some programs use a 16-bit **signed** integer (int), instead of **unsigned** integer (unsigned int), to fetch the contents of those registers. A signed integer can store numbers from -32768 to 32767. The highest bit in the signed integer is used to represent a +/- sign, so if you try to read unsigned number larger than 32767 as a signed value, it has the highest bit set to 1, and it's interpreted by a program as a negative number. This way, by programmer's mistake, a program thinks that the amount of available memory is less than zero. Such errors went unnoticed for years, because back in the days very few home computers had more than just a couple of megabytes of memory.

## Usage
Using MemEater is very simple:

```C:\> memaeter.exe 20000```

will permanently allocate 20000 kB of extended memory. The memory will be released when you reboot the computer.

## Known issues
In FreeDOS MemEater will report 64MB of free memory even if a computer has more than 64MB of memory installed. It's because the original XMS specification used 16-bit registers to operate on 1kB memory blocks, which gives a maximum of 64MB (65536 \* 1024 bytes) available for programs. FreeDOS memory manager uses 32-bit registers, which allows for storing much larger values. However MemEater, for compatibility reasons, is a 16-bit real-mode program, and making it 32-bit would require dropping support for CPUs older than 386. As a workaround, you can use a built-in "mem" command in FreeDOS.

Despite this limitation, memory allocation in FreeDOS works correctly. If you request more than 64MB memory, it will simply be allocated in a few 64MB chunks.

## Source code
The source code is available as public domain and comes with no warranty. The executable was produced with Turbo C 2.01, which can be downloaded for free from Embarcadero Antique Software collection.
