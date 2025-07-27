FROM ubuntu:22.04

# Installa Qt6 e dipendenze
RUN apt-get update && apt-get install -y \
    qt6-base-dev \
    qt6-base-dev-tools \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Crea cartella di lavoro
WORKDIR /app

# Copia TUTTO il progetto (compreso dati.json)
COPY . .

# Compila (assumendo che usi qmake)
RUN qmake6 && make

# Punto di ingresso
CMD ["./biblioteca_virtuale"]