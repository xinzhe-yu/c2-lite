# c2-lite

A lightweight reverse shell written in C from scratch. Built as an educational project to learn systems programming, network programming, and offensive security fundamentals.

## ⚠️ Disclaimer

This project is built strictly for **educational purposes** as a personal project by a student in an NSA/DHS designated Center of Academic Excellence in Cyber Operations (CAE-CO) program. It is intended for use in controlled lab environments only. Do not use this tool against systems you do not own or have explicit written authorization to test. Unauthorized access to computer systems is illegal.

## What This Is

A reverse shell consisting of two binaries:

- **Server (listener)** — accepts incoming connections, manages multiple sessions with an interactive command interface (`list`, `interact`, `kill`), and uses `select()` for multiplexed I/O between operator input and client connections
- **Client (implant)** — runs as a daemon, automatically reconnects to the server on disconnect, redirects standard file descriptors to the socket, and spawns a shell via `execve`

## Project Structure

```
c2-lite/
├── server/
│   ├── main.c          # server entry point, session manager, select() loop
│   ├── server.c         # socket, bind, listen, accept
│   ├── server.h
│   ├── command.c        # command dispatcher (list, interact, kill)
│   └── command.h
├── client/
│   ├── main.c          # client entry point, daemon mode, reconnect loop, execve
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

The server provides an interactive prompt (`c2>`) with commands:
- `list` — show all active sessions
- `interact <id>` — interact with a specific session
- `kill <id>` — terminate a session

Run the implant (update `SERVER_IP` in `client/main.c` before building):

```bash
./build/client
```

The client automatically:
- Backgrounds itself as a daemon process
- Reconnects every 3 seconds if the connection drops
- Redirects all I/O to `/dev/null` for stealth

## Concepts Practiced

- Multi-file C project structure with headers and separate compilation
- POSIX socket programming (TCP client/server)
- Process management with `fork()`, `execve()`, `waitpid()`, `setsid()`
- Daemon process creation and backgrounding
- I/O multiplexing with `select()`
- File descriptor manipulation with `dup2()`
- Session management and command dispatching
- Automatic reconnection logic
- Makefile build systems with pattern rules and separate targets

## License

For educational use only. See disclaimer above.
