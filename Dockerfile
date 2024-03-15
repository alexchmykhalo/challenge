FROM archlinux

RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm bash clang cmake ninja

COPY generator /opt

COPY CMakePresets.json /home/cpp-challenge-24/
COPY CMakeLists.txt /home/cpp-challenge-24/
COPY Sources /home/cpp-challenge-24/Sources
COPY Tests /home/cpp-challenge-24/Tests
RUN cd /home/cpp-challenge-24/Sources && \
    cmake --preset LinuxRelease && \
    cmake --build --preset LinuxRelease && \
    cmake --build --preset LinuxRelease --target install

ENTRYPOINT ["/opt/processor"]

