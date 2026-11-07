*This project has been created as part of the 42 curriculum by `hlassiou`*

# Libft

## Description

Libft is a C programming project at 42/1337 that aims to build my own C library by reimplementing standard C library functions and creating additional utility functions.

The goal of this project is to understand how common functions work internally, improve my knowledge of C programming, and learn how to manage memory, manipulate strings, work with pointers, and handle linked lists.

The library, `libft.a`, can be reused in future C projects instead of rewriting the same utility functions every time.

### Library Contents

**Part 1 — Libc Functions**

Reimplementing standard C library functions, including:

- Character checks and conversions: `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint`, `ft_toupper`, `ft_tolower`
- Memory manipulation: `ft_memset`, `ft_bzero`, `ft_memcpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp`, `ft_calloc`
- String manipulation: `ft_strlen`, `ft_strchr`, `ft_strrchr`, `ft_strncmp`, `ft_strlcpy`, `ft_strlcat`, `ft_strnstr`, `ft_strdup`
- Conversion: `ft_atoi`

**Part 2 — Additional Functions**

Creating extra functions for string manipulation, conversion, and output:

- `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`
- `ft_itoa`, `ft_strmapi`, `ft_striteri`
- `ft_putchar_fd`, `ft_putstr_fd`, `ft_putendl_fd`, `ft_putnbr_fd`

**Part 3 — Linked Lists**

Implementing functions to create, access, modify, traverse, and delete singly linked lists using the `t_list` structure:

- `ft_lstnew`
- `ft_lstadd_front`
- `ft_lstsize`
- `ft_lstlast`
- `ft_lstadd_back`
- `ft_lstdelone`
- `ft_lstclear`
- `ft_lstiter`
- `ft_lstmap`

These functions help me understand pointers, dynamic memory allocation, function pointers, and how nodes are connected through a linked list.

## Instructions

### Compilation

Compile the library using the Makefile:

```bash
make all
```

This generates the static library `libft.a`.

Other Makefile commands:

```bash
make clean   # Remove object files
make fclean  # Remove object files and libft.a
make re      # Rebuild the library
```

### Using the Library

Include the library header in your C source file:

```c
#include "libft.h"
```

Compile your program with the library:

```bash
cc main.c -L. -lft -o program
```

Run the program:

```bash
./program
```

Replace `main.c` with your own source file. Make sure `libft.h` is available in the include path used by your project.

### Compilation Rules

The project must compile with the following flags:

```bash
-Wall -Wextra -Werror
```

The library is created using `ar`. All allocated memory must be handled correctly, and the functions must follow the 42/1337 project requirements.

### Testing

I use test programs and a Libft tester to check the behavior of my functions, including edge cases, NULL pointers where applicable, and memory-management errors. I also use Valgrind to help detect memory leaks and invalid memory accesses.

## Resources

### Documentation and Tutorials

- [42 Libft Project Subject](https://cdn.intra.42.fr/pdf/pdf/96030/en.subject.pdf) — Project requirements and function descriptions.
- [Linked Lists in C — YouTube Tutorial](https://www.youtube.com/watch?v=mkZYMKwKkvI\&t=1222s) — A simple explanation of linked lists, how they work, and how to implement their operations.

### AI Usage

I used ChatGPT and Claude as learning and debugging assistants during this project. They helped me:

- Understand difficult C concepts, especially pointers, function pointers, and dynamic memory allocation.
- Generate edge cases and testing ideas to check the behavior of my functions.
- Understand memory-management problems, including memory leaks, invalid memory accesses, and correct use of `malloc` and `free`.
- Review errors and clarify the reasoning behind different implementations, particularly in Part 2.

I use these tools to support my understanding and testing. I still need to understand the code, verify the results, and be able to explain my implementations during peer evaluation.

---

## Author

**`hlassiou`**

42/1337 — Libft Project