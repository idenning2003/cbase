FROM alpine:latest

RUN apk update && \
	apk upgrade && \
    apk add --no-cache sudo bash zsh gcc make libc-dev valgrind tar git openssh-client

ARG USERNAME=user
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN addgroup -g $USER_GID $USERNAME \
    && adduser -u $USER_UID -G $USERNAME -s /bin/bash -D $USERNAME \
    && mkdir -p /etc/sudoers.d \
    && echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME
WORKDIR /home/$USERNAME

RUN chown -R $USERNAME:$USERNAME /home/$USERNAME

COPY .bashrc /home/$USERNAME
RUN mkdir -p /home/$USERNAME/.ssh && chmod 775 /home/$USERNAME/.ssh

CMD ["bash"]