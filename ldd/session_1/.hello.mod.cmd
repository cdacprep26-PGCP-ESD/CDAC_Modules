savedcmd_/home/desd/ldd/session_1/hello.mod := printf '%s\n'   hello.o | awk '!x[$$0]++ { print("/home/desd/ldd/session_1/"$$0) }' > /home/desd/ldd/session_1/hello.mod
