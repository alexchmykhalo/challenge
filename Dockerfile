FROM archlinux

RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm bash clang cmake ninja

COPY generator /opt