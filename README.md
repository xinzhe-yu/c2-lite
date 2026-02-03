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
│   ├── src/
│   │   ├── main.c          # server entry point, select() loop
│   │   ├── server.c        # socket, bind, listen, accept, interactive shell handler
│   │   ├── session.c       # client list manager (opaque struct, add/remove/get API)
│   │   ├── command.c       # command dispatcher (list, interact, kill)
│   │   └── terminal.c      # raw terminal mode helpers
│   └── include/
│       ├── server.h
│       ├── session.h       # client list public API (opaque client_list_t)
│       ├── command.h
│       └── terminal.h
├── client/
│   ├── src/
│   │   ├── main.c          # client entry point, daemon mode, reconnect loop, execve
│   │   └── client.c        # socket creation, connect
│   └── include/
│       └── client.h
├── common/
│   └── common.h            # shared constants (port, buffer size)
├── makefile
└── README.md
```

## Build

```bash
make
```

Binaries output to `build/`.

```bash
make clean    # remove build directory
```

## Usage

Start the listener:

```bash
./build/server_bin
```

The server provides an interactive prompt (`c2>`) with commands:
- `list` — show all active sessions
- `interact <id>` — interact with a specific session
- `kill <id>` — terminate a session

Run the implant (update `SERVER_IP` in `client/main.c` before building):

```bash
./build/client_bin
```

The client automatically:
- Backgrounds itself as a daemon process
- Reconnects every 3 seconds if the connection drops
- Redirects all I/O to `/dev/null` for stealth

## Concepts Practiced

- Multi-file C project structure with `src/`/`include/` layout and separate compilation
- Encapsulation via opaque data structures and clean C APIs (forward declarations, accessor functions)
- POSIX socket programming (TCP client/server)
- Process management with `fork()`, `execve()`, `waitpid()`, `setsid()`
- Daemon process creation and backgrounding
- I/O multiplexing with `select()`
- File descriptor manipulation with `dup2()`
- Client session management with persistent IDs and command dispatching
- Raw terminal mode with `termios`
- Automatic reconnection logic
- Makefile build systems with pattern rules, dependency tracking (`-MMD`), and order-only prerequisites

## License

For educational use only. See disclaimer above.
