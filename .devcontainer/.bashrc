if [ "$TERM" = xterm-256color ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
alias ls='ls -Aph --group-directories-first --color=auto'
alias hexdump='hexdump -C'