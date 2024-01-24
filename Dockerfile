FROM debian
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    automake \
    gdb \
    gdb-multiarch \
    gcc-arm-none-eabi \
    autoconf \
    flex

# Copie du code source dans le conteneur
WORKDIR /app
COPY . /app

CMD ["bash"]