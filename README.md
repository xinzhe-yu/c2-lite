# c2-lite

A lightweight reverse shell written in C from scratch. Built as an educational project to learn systems programming, network programming, and offensive security fundamentals.

## ⚠️ Disclaimer

This project is built strictly for **educational purposes** as part of coursework in an NSA/DHS designated Center of Academic Excellence in Cyber Operations (CAE-CO) program. It is intended for use in controlled lab environments only. Do not use this tool against systems you do not own or have explicit written authorization to test. Unauthorized access to computer systems is illegal.

## What This Is

A reverse shell consisting of two binaries:

- **Server (listener)** — accepts incoming connections, forks a child process per session, and provides an interactive operator interface using `select()` for multiplexed I/O
- **Client (implant)** — connects back to the server, redirects standard file descriptors to the socket, and spawns a shell via `execve`

## Project Structure

```
c2-lite/
├── server/
│   ├── main.c          # server entry point, fork logic
│   ├── server.c         # socket, bind, listen, accept
│   ├── server.h
│   ├── session.c        # interactive select() loop
│   └── session.h
├── client/
│   ├── main.c          # client entry point, reconnect loop, execve
│   ├── client.c         # socket creation, connect
│   └── client.h
├── common/
│   └── common.h        # shared constants (port, buffer size)
├── Makefile
└── README.md
```

## Build

```bash
make
```

Binaries output to `build/`.

```bash
make clean    # remove object files and binaries
make debug    # build with debug symbols
```

## Usage

Start the listener:

```bash
./build/server
```

Run the implant (update `SERVER_IP` in `client/main.c` before building):

```bash
./build/client
```

## Concepts Practiced

- Multi-file C project structure with headers and separate compilation
- POSIX socket programming (TCP client/server)
- Process management with `fork()`, `execve()`, `waitpid()`
- I/O multiplexing with `select()`
- File descriptor manipulation with `dup2()`
- Makefile build systems with pattern rules and separate targets

## License

For educational use only. See disclaimer above.
