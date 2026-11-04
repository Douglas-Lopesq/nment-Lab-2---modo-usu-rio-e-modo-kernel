savedcmd_ola.mod := printf '%s\n'   ola.o | awk '!x[$$0]++ { print("./"$$0) }' > ola.mod
