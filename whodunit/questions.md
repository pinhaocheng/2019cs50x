# Questions

## What's `stdint.h`?

A header file.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

To declare the way to use the data in a specific way. uint8_t means an 8 bit wide integer, and as follows.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?
| name | desciption |
| ----- | ----- |
| BYTE | 1 byte |
| DWORD | 4 bytes |
| LONG | 4 bytes |
| WORD | 2 bytes |

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

WORD bfTypre, which means the file type. It must be BM. 

## What's the difference between `bfSize` and `biSize`?
| name | desciption |
| ----- | ----- |
| bfSize | The size, in bytes, of the bitmap file. |
| biSize | The number of bytes required by the structure. |

## What does it mean if `biHeight` is negative?

If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount: The number of bits-per-pixel. The biBitCount member of the BITMAPINFOHEADER structure determines the number of bits that define each pixel and the maximum number of colors in the bitmap.

## Why might `fopen` return `NULL` in `copy.c`?

`fopen` will return `NULL` when the file couldn't be opened.

## Why is the third argument to `fread` always `1` in our code?

The third argument is about the number of elements, each one with a size of second argument `sizeof(RGBTRIPLE)`  bytes. 
This means that 1 block of size `sizeof(RGBTRIPLE)` (aka 3 bytes) will be read from `inptr` and written into `&triple`.

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

As the code is 
```
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
```
When `bi.biWidth` is `3`, `sizeof(RGBTRIPLE)` is `3`, padding will yield `3`.

## What does `fseek` do?

`fseek` moves the file pointer to specified location. In this case, it skips over the padding int and
    looks for next RGBTRIPLE to read.

## What is `SEEK_CUR`?

The current position of the file pointer.
