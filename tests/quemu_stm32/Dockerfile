# Compile and install qemu_stm32
from fedora:latest
RUN dnf install -y \
          arm-none-eabi-gcc\
          arm-none-eabi-newlib\
          findutils\
          gcc\
          make\
          git\
          glib2-devel\
          libfdt-devel\
          pixman-devel\
          pkgconf-pkg-config\
          python2\
          zlib-devel ;\
    git clone https://github.com/beckus/qemu_stm32.git
RUN cd qemu_stm32 && ./configure --extra-cflags="-w" --extra-cflags=-DDEBUG_CLKTREE --extra-cflags=-DDEBUG_STM32_RCC --extra-cflags=-DDEBUG_STM32_UART  --extra-cflags=-DDEBUG_STM32_TIMER --enable-debug --target-list="arm-softmmu" --python="/usr/bin/python2" && make && make install

# Install demos
RUN git clone https://github.com/beckus/stm32_p103_demos.git && cd stm32_p103_demos && make
