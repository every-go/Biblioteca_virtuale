Progetto di PAO anno accademico 2024/2025

Questo progetto riguarda la biblioteca virtuale

Comprensivo di .gitignore funzionante

Se trovate bug/problemi non fate problemi ad aprire una issue, grazie

# Doppio progetto

Ho deciso di fare un doppio progetto, quello 'ProgettoJSON' è quello richiesto per il corso 'Programmazione ad Oggetti' nell'A.A 2024/2025, invece il 'ProgettoDB' è stato completato dopo aver seguito il corso 'Base di Dati' e, invece di usare un file JSON, usa un DB con PostgreSQL (non testato con MySQL, SQLite ecc). Ovviamente è compreso il file .sql per creare il server necessario, modificare le macro (gli #DEFINE) presenti in _dbmanager.cpp_ con le proprie credenziali per far funzionare tutto correttamente. L'interfaccia grafica non subisce modifiche rispetto al progettoJSON, tutto ciò che c'è di diverso è nella parte back-end piuttosto che nella front-end.

## 🐳 Avviare l'app con Docker (supporto X11) (solo su Progetto_JSON)

Questa applicazione richiede l'accesso al server X11 per mostrare un'interfaccia grafica.

### 1. Assicurati di avere i prerequisiti:

- Docker installato
- Ambiente desktop (non headless)
- Server X11 in funzione (normale su Linux desktop)

Se vuoi ricreare l'immagine da 0, assicurati di essere nella cartella del progetto (quella che contiene il Dockerfile), quindi esegui::
```docker build -t biblioteca-virtuale:1.0 .```


### 2. Consenti a Docker di accedere al display:

```bash
xhost +local:docker
```

### 3. Eseguire il container:

docker run -it --rm \
  -v "$(pwd)/dati.json:/app/dati.json" \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  -v $HOME/.Xauthority:/root/.Xauthority \
  biblioteca-virtuale:1.0

Il file dati.json deve trovarsi nella stessa cartella da cui lanci il comando.

### 4. Dopo l'uso, puoi revocare l'accesso al server X11::
```bash
xhost -local:docker
```


## 🪟 Esecuzione su Windows (tramite WSL2) (non testata)

Se usi **Windows con WSL2**, per eseguire l'app con interfaccia grafica Docker servono alcuni passaggi extra.

### ✅ Prerequisiti

1. Installa un server X11 su Windows, ad esempio:
   - [VcXsrv (gratuito)](https://sourceforge.net/projects/vcxsrv/)
   - [X410 (a pagamento, Microsoft Store)](https://x410.dev/)

2. Avvia il server X11 (ad esempio VcXsrv) e **abilita “Disable access control”** per permettere connessioni dal WSL.

### ⚙️ Configura WSL2

Apri una shell WSL2 e imposta la variabile DISPLAY:

```bash
export DISPLAY=$(grep nameserver /etc/resolv.conf | awk '{print $2}'):0
```

```
docker build -t biblioteca-virtuale:1.0 .
```

```
docker run -it --rm \
  -v "$(pwd)/dati.json:/app/dati.json" \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  biblioteca-virtuale:1.0
```

## 🍎 Esecuzione su macOS (non testata)

macOS non ha un server X11 nativo, quindi per eseguire applicazioni Docker con interfaccia grafica X11 devi installare un server X11 esterno.

### ✅ Prerequisiti

1. Installa [XQuartz](https://www.xquartz.org/), il server X11 ufficiale per macOS.
2. Dopo l'installazione, **riavvia il computer** o effettua il logout/login per applicare le modifiche.
3. Avvia XQuartz prima di eseguire il container.

### ⚙️ Configurazione DISPLAY

In macOS, apri il terminale e imposta la variabile DISPLAY con:

```bash
export DISPLAY=host.docker.internal:0
```

```
docker build -t biblioteca-virtuale:1.0 .
```

```
docker run -it --rm \
  -v "$(pwd)/dati.json:/app/dati.json" \
  -e DISPLAY=$DISPLAY \
  -v /tmp/.X11-unix:/tmp/.X11-unix \
  biblioteca-virtuale:1.0
```

### Note importanti
XQuartz deve essere avviato prima di eseguire il container.

Potresti dover abilitare l'accesso in preferenze di sicurezza di macOS per XQuartz.

Se hai problemi con permessi o autenticazione X11, prova ad abilitare “Allow connections from network clients” nelle preferenze di XQuartz (XQuartz > Preferences > Security).

Assicurati che il file dati.json sia presente nella cartella di lavoro.
