<h1 align="center">Seahorse Programming Language</h1>
<h4 align="center"><i>Também disponível em <a href="./README_PTBR.md">Português</a>.</i></h4>
<p align="center">Transpiled programming language with a focus on simplicity and practicity.</p>

## Introduction
Seahorse is a programming language designed for creating simple and versatile scripts.

Seahorse is a language transpiled into the [Lua](https://www.lua.org) programming language.

The code in this repository implements a simple compiler for the Seahorse language in Go.

Unfortunately, the compiler is not yet finished, let alone stable. Therefore, I do not recommend that you try to run any of the code in this repository **for now**.

## Examples
- Hello World:
  ```
  printLog("Hello World!");
  ```
- Variables:
  ```
  var i = "hello";
  printLog(i);
  ```
- I/O:
  ```
  var i = input("Enter your name:");
  printLog(i);
  ```

More examples soon...

---

> [MIT License](./LICENSE)