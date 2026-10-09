```
ai generated
usei so o subject
pra visualizar o que falta
```

# ft_irc implementation checklist

This checklist is based on `en.subject.txt`. It covers the mandatory project only; the bonus features (file transfer and bot) are intentionally excluded.

## 1. Project-wide requirements

- [ ] Build an IRC server named `ircserv` in C++98; do not build an IRC client.
- [ ] Accept exactly the runtime inputs `./ircserv <port> <password>` and validate them before starting.
- [ ] Use TCP/IP (IPv4 or IPv6) and support multiple simultaneous clients without hanging.
- [ ] Use non-blocking file descriptors and one `poll()` (or equivalent such as `select`, `kqueue`, or `epoll`) mechanism for all I/O, including the listening socket.
- [ ] Do not fork. Do not perform socket reads or writes outside the readiness-driven event loop. Do not use `errno` after `read`/`recv` or `write`/`send` to decide whether to retry or what to do next.
- [ ] Handle partial incoming data by buffering until complete IRC commands can be parsed. Handle partial outgoing writes by retaining unsent data.
- [ ] Do not crash or unexpectedly exit on malformed input, disconnects, or ordinary resource errors; clean up affected clients and report startup/runtime errors appropriately.
- [ ] Use only C++98 and the functions/libraries authorized by the subject. No external or Boost libraries.
- [ ] Compile with `c++ -Wall -Wextra -Werror` (and ensure `-std=c++98` works).
- [ ] Provide a Makefile with `ircserv`, `all`, `clean`, `fclean`, and `re` targets; avoid unnecessary relinking.
- [ ] Select a reference IRC client and verify that it connects and works without errors. Record the client and connection instructions in the README.
- [ ] Keep code clean, test edge cases, and ensure every team member can explain their contribution.

## 2. Suggested modules and ownership

Assign one owner per module. Agree on the shared contracts in section 3 before implementation so modules can be developed independently. Names below are suggestions, not required class names.

### Module A — Server startup and event loop

**Owns:** `main`, argument validation, listening socket, polling, client socket lifecycle, dispatching readable/writable events.

- [ ] Validate the port and required password; fail clearly on invalid arguments or socket setup failure.
- [ ] Create, configure, bind, and listen on the server socket.
- [ ] Put the listening socket and every accepted client socket in non-blocking mode.
- [ ] Keep one poller for the listening socket and all client sockets; update its interest set as clients connect, disconnect, or have pending output.
- [ ] Accept new clients without blocking and cleanly handle disconnects and socket errors.
- [ ] Read available bytes into each client's input buffer; pass only complete lines/commands to the protocol layer.
- [ ] Queue outbound data and write when the poller reports readiness; preserve data not yet sent.
- [ ] Route parsed commands to the relevant handler and route responses/broadcasts back through the output queue.
- [ ] Remove disconnected clients from poller and shared state exactly once.

**Done when:** several clients can connect concurrently, slow/partial I/O does not hang the server, and disconnecting one client does not affect others.

### Module B — IRC framing and command parsing

**Owns:** per-client input buffering, line framing, command/parameter parsing, and serialization helpers if agreed.

- [ ] Aggregate packets until a complete IRC line is available; do not assume one `recv` equals one command.
- [ ] Support multiple complete commands arriving in one read.
- [ ] Parse command names, middle parameters, and the optional trailing parameter (introduced by `:`) without losing spaces in the trailing parameter.
- [ ] Define and enforce a maximum line/buffer size to prevent unbounded memory growth; handle invalid or oversized input safely.
- [ ] Normalize command names for dispatch without changing the content of user-provided parameters unnecessarily.
- [ ] Provide a typed command representation and a single agreed formatting path for server replies/messages.
- [ ] Leave command semantics and channel/user authorization to the relevant command handlers.

**Done when:** split commands, batched commands, empty/malformed lines, and trailing parameters are handled predictably.

### Module C — Client session, registration, and identity

**Owns:** client registration state, password authentication, nicknames, usernames, and direct messages.

- [ ] Track each connection until it has supplied the required registration information.
- [ ] Implement `PASS` authentication against the server password supplied at startup.
- [ ] Implement `NICK` and `USER`; validate required parameters and enforce nickname uniqueness.
- [ ] Prevent unregistered clients from using commands that require registration; allow the registration sequence to arrive in multiple commands or packets.
- [ ] Notify a registering client when registration succeeds, using replies compatible with the selected reference client.
- [ ] Implement private `PRIVMSG` delivery to a target user; report missing/unknown targets and invalid requests appropriately.
- [ ] On disconnect/`QUIT`, remove the user's nickname and session state and notify any relevant channel members (coordinate with Module D).

**Done when:** the chosen IRC client can authenticate, register, set a unique nick, and send/receive private messages.

### Module D — Channels, membership, and channel messaging

**Owns:** channel state, membership, joining/leaving, channel message fan-out, and coordination with operator state.

- [ ] Create/find channels and track their members and topic/mode state through the agreed shared model.
- [ ] Implement `JOIN` and the channel membership rules, including invite-only, channel key, and user limit checks when those modes are active.
- [ ] Ensure the first channel member (or another agreed rule) receives operator status so operator-only features are usable.
- [ ] Implement leaving (`PART`) and remove members on disconnect/`QUIT`; remove empty channels when appropriate.
- [ ] Implement channel `PRIVMSG` fan-out to every other member of that channel, not back to the sender.
- [ ] Reject messages to channels the sender has not joined, and handle unknown channels/targets appropriately.
- [ ] Keep membership and operator changes consistent when users leave or are kicked.

**Done when:** multiple clients can join the same channel and exchange messages, and membership remains correct after leaving or disconnecting.

### Module E — Channel operator commands and modes

**Owns:** channel-operator authorization, `KICK`, `INVITE`, `TOPIC`, and channel `MODE`.

- [ ] Enforce the distinction between channel operators and regular users.
- [ ] Implement `KICK` to remove a target from a channel, with operator and membership checks.
- [ ] Implement `INVITE` to invite a user to a channel and retain enough invitation state for invite-only joins.
- [ ] Implement `TOPIC` to view a channel topic; allow changing it according to the topic-restriction mode (`t`).
- [ ] Implement channel `MODE` changes for all required modes:
  - [ ] `i`: set/remove invite-only.
  - [ ] `t`: set/remove operator-only topic changes.
  - [ ] `k`: set/remove the channel key/password.
  - [ ] `o`: give/take channel-operator privilege for a member.
  - [ ] `l`: set/remove the channel user limit.
- [ ] Validate mode signs (`+`/`-`), parameters, targets, and operator permissions. Keep displayed mode state consistent with actual behavior.
- [ ] Coordinate state changes and notifications with channel membership logic; do not duplicate channel/member storage.

**Done when:** an operator can use every listed command/mode, regular users cannot perform restricted actions, and mode restrictions affect joins/topic changes as expected.

### Module F — Build, documentation, and integration tests

This module can be owned by one person or shared; it must not define separate runtime state or duplicate command behavior.

- [ ] Maintain the required Makefile targets and C++98 warning-clean build.
- [ ] Add a root `README.md` in English. Its first line must be italicized and exactly follow: `This project has been created as part of the 42 curriculum by <login1>[, <login2>[, <login3>[...]]].`
- [ ] Include README sections: **Description**, **Instructions**, and **Resources**. Describe the project, build/run steps, reference client, useful references, and how/which parts of the project used AI.
- [ ] Keep tests/scripts separate from production requirements unless the team chooses otherwise; test against the running server and the selected reference client.
- [ ] Add a repeatable integration checklist covering registration, nick conflicts, private messages, channel join/broadcast, operator commands/modes, disconnect cleanup, and malformed/fragmented input.
- [ ] On macOS, use `fcntl(fd, F_SETFL, O_NONBLOCK)` only as permitted by the subject.

## 3. Shared contracts to agree before parallel coding

Write these decisions down in the project before module work begins:

- [ ] **Client identity:** how a connection is referenced (for example, a stable client ID or file descriptor) and where nick/user/registration state lives.
- [ ] **Channel model:** one source of truth for channel membership, topic, modes, invitations, and operator status. Define who owns updates.
- [ ] **Parsed command:** exact command object passed from parser to handlers (command name, parameters, trailing parameter, originating client).
- [ ] **Output path:** handlers must enqueue complete protocol lines through one server/output interface; they must not call `send` directly.
- [ ] **Broadcast path:** define helpers for sending to one client, all channel members, or all except the sender. Define whether channel/user lookup belongs to server or domain modules.
- [ ] **Errors and replies:** agree how handlers signal errors and how server replies are formatted, so one module does not silently swallow another module's failure.
- [ ] **Ownership boundaries:** event loop owns sockets and readiness; parser owns framing/parsing; domain handlers own command semantics; shared state has a single owner/API.
- [ ] **Header/API freeze:** create minimal headers/interfaces first, then each owner implements behind them. Integrate continuously rather than waiting until all modules are complete.

## 4. End-to-end acceptance checklist

- [ ] `make` builds `ircserv` successfully with the required flags and no warnings.
- [ ] Invalid/missing arguments and invalid ports fail cleanly; a valid invocation starts listening on the requested port.
- [ ] The chosen reference client connects using the configured password and can register with `PASS`, `NICK`, and `USER`.
- [ ] Multiple clients can remain connected and interact at the same time without the server hanging.
- [ ] A client can join a channel; messages sent to that channel reach every other member.
- [ ] Clients can send private messages to each other.
- [ ] Operators can `KICK`, `INVITE`, view/change `TOPIC`, and use each required `MODE`; regular users are restricted as appropriate.
- [ ] Fragmented input (for example `com`, then `man`, then `d\n`) is assembled before command processing; multiple lines in one read are also processed.
- [ ] Partial/slow output, abrupt disconnects, unknown commands/targets, malformed parameters, and repeated connect/disconnect cycles do not hang or crash the server.
- [ ] README requirements are complete, and the repository contains the required source/header files and Makefile.
