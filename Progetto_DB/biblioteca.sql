DROP TABLE IF EXISTS Manga;
DROP TABLE IF EXISTS Libri;
DROP TABLE IF EXISTS Riviste;
DROP TABLE IF EXISTS Film;
DROP TABLE IF EXISTS CD;
DROP TABLE IF EXISTS Multimedia;
DROP TABLE IF EXISTS Cartaceo;
DROP TABLE IF EXISTS Biblioteca;
DROP TYPE IF EXISTS Diffusione;

CREATE TABLE Biblioteca (
   id SERIAL PRIMARY KEY,
   classe TEXT NOT NULL,
   -- classe può essere 'Film', 'CD', 'Riviste', 'Libri', 'Manga'
   titolo TEXT,
   genere TEXT,
   anno INTEGER,
   costo DECIMAL(10,2),
   disponibile BOOLEAN,
   copie_disponibili INTEGER,
   nprestiti INTEGER,
   immagine TEXT,
   CHECK(costo >= 0 AND copie_disponibili >= 0 AND nprestiti >= 0 AND nprestiti >= copie_disponibili)
);

CREATE TABLE Multimedia(
   id INTEGER PRIMARY KEY,
   durata INTEGER,
   studio TEXT,
   FOREIGN KEY (id) REFERENCES Biblioteca(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE Cartaceo(
   id INTEGER PRIMARY KEY,
   autore TEXT,
   editore TEXT,
   letto BOOLEAN NOT NULL,
   FOREIGN KEY (id) REFERENCES Biblioteca(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE Film(
   id INTEGER PRIMARY KEY,
   regista TEXT,
   attoreprotagonista TEXT,
   visto BOOLEAN NOT NULL,
   FOREIGN KEY (id) REFERENCES Multimedia(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE CD(
   id INTEGER PRIMARY KEY,
   artista TEXT,
   ascoltato BOOLEAN,
   ntracce INTEGER NOT NULL,
   FOREIGN KEY (id) REFERENCES Multimedia(id) ON DELETE CASCADE ON UPDATE CASCADE,
   CHECK (ntracce > 0)
);

CREATE TYPE Diffusione AS ENUM ('Provinciale', 'Regionale', 'Nazionale', 'Internazionale');

CREATE TABLE Riviste(
   id INTEGER PRIMARY KEY,
   diffusion Diffusione NOT NULL,
   FOREIGN KEY (id) REFERENCES Cartaceo(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE Libri(
   id INTEGER PRIMARY KEY,
   linguaoriginale TEXT,
   nvolumi INTEGER NOT NULL,
   FOREIGN KEY (id) REFERENCES Cartaceo(id) ON DELETE CASCADE ON UPDATE CASCADE,
   CHECK (nvolumi > 0)
);

CREATE TABLE Manga(
   id INTEGER PRIMARY KEY,
   concluso BOOLEAN NOT NULL,
   FOREIGN KEY (id) REFERENCES Libri(id) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE OR REPLACE FUNCTION aggiorna_disponibile() RETURNS TRIGGER AS $$
BEGIN
   IF NEW.nprestiti = 0 THEN
      NEW.disponibile := FALSE;
   ELSE
      NEW.disponibile := TRUE;
   END IF;
   RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_aggiorna_disponibile
BEFORE UPDATE OF nprestiti ON Biblioteca
FOR EACH ROW
EXECUTE FUNCTION aggiorna_disponibile();

CREATE OR REPLACE FUNCTION controlla_prestiti_copie() RETURNS TRIGGER AS $$
BEGIN
   IF NEW.nprestiti < 0 OR NEW.nprestiti < NEW.copie_disponibili THEN
      RAISE EXCEPTION 'Il numero di prestiti possibili (prenotati+prenotabili) deve essere >= 0 e >= copie disponibili';
   END IF;
   RETURN NEW;
END;
$$ LANGUAGE plpgsql;


CREATE TRIGGER trg_controlla_prestiti_copie
BEFORE UPDATE OF nprestiti ON Biblioteca
FOR EACH ROW
EXECUTE FUNCTION controlla_prestiti_copie();


-- ID 1: Film
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (1, 'Film', 'Ace Ventura', 'Commedia', 1994, 9.99, TRUE, 1, 1, ':/images/ace_ventura.png');
INSERT INTO Multimedia(id, durata, studio)
VALUES (1, 86, 'Warner Bros');
INSERT INTO Film(id, regista, attoreprotagonista, visto)
VALUES (1, 'Tom Shadyac', 'Jim Carrey', FALSE);

-- ID 2: Manga
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (2,  'Manga', 'Bleach', 'Shonen', 2001, 5.20, TRUE, 3, 3, ':/images/bleach.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (2, 'Tite Kubo', 'Shueisha', TRUE);
INSERT INTO Libri(id, linguaoriginale, nvolumi)
VALUES (2, 'Giapponese', 69);
INSERT INTO Manga(id, concluso)
VALUES (2, TRUE);

-- ID 3: Riviste
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (3, 'Riviste', 'Cook_inc', 'Culinario', 2011, 3.99, TRUE, 3, 3, ':/images/cook_inc.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (3, 'Luigi Nardi', 'Vandenberg Edizioni', FALSE);
INSERT INTO Riviste(id, diffusion)
VALUES (3, 'Regionale');

-- ID 4: Riviste
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (4, 'Riviste', 'Focus', 'Scientifico', 1992, 3.99, TRUE, 4, 4, ':/images/focus.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (4, 'Raffaele Leone', 'Mondadori', FALSE);
INSERT INTO Riviste(id, diffusion)
VALUES (4, 'Nazionale');

-- ID 5: Libri
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (5, 'Libri', 'Harry Potter', 'Romanzo', 1997, 7.99, TRUE, 5, 5, ':/images/harry_potter.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (5, 'J. K. Rowling', 'Bloomsbury Publishing', FALSE);
INSERT INTO Libri(id, linguaoriginale, nvolumi)
VALUES (5, 'Inglese', 7);

-- ID 6: Libri
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (6, 'Libri', 'I ragazzi della via Pal', 'Romanzo', 1906, 7.99, TRUE, 5, 5, ':/images/via_pal.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (6, 'Ferenc Molnár', 'Giunti', TRUE);
INSERT INTO Libri(id, linguaoriginale, nvolumi)
VALUES (6, 'Ungherese', 1);

-- ID 7: Libri (duplicato titolo ma differente immagine)
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (7, 'Libri', 'I ragazzi della via Pal', 'Romanzo', 1906, 7.99, TRUE, 5, 5, ':/images/via_pal2.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (7, 'Ferenc Molnár', 'Giunti', TRUE);
INSERT INTO Libri(id, linguaoriginale, nvolumi)
VALUES (7, 'Ungherese', 1);

-- ID 8: Libri
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (8, 'Libri', 'L''isola del tesoro', 'Romanzo', 1883, 7.99, TRUE, 3, 3, ':/images/isola_tesoro.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (8, 'Robert Louis Stevenson', 'Giunti', TRUE);
INSERT INTO Libri(id, linguaoriginale, nvolumi)
VALUES (8, 'Inglese', 1);

-- ID 9: CD
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (9, 'CD', 'La Divina Commedia', 'Rap', 2023, 8.99, TRUE, 4, 4, ':/images/la_divina_commedia2.png');
INSERT INTO Multimedia(id, durata, studio)
VALUES (9, 49, 'Sony');
INSERT INTO CD(id, artista, ascoltato, ntracce)
VALUES (9, 'Tedua', TRUE, 16);

-- ID 10: CD (duplicato titolo con immagine diversa)
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (10, 'CD', 'La Divina Commedia', 'Rap', 2023, 8.99, TRUE, 4, 4, ':/images/la_divina_commedia.png');
INSERT INTO Multimedia(id, durata, studio)
VALUES (10, 49, 'Sony');
INSERT INTO CD(id, artista, ascoltato, ntracce)
VALUES (10, 'Tedua', TRUE, 16);

-- ID 11: Libri
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (11, 'Libri', 'La Divina Commedia', 'Poema', 1472, 9.99, TRUE, 1472, 1472, ':/images/divina.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (11, 'Dante Alighieri', '', FALSE);
INSERT INTO Libri(id, linguaoriginale, nvolumi)
VALUES (11, 'Volgare fiorentino', 1);

-- ID 12: Manga
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (12, 'Manga', 'One Piece', 'Shonen', 1999, 5.20, TRUE, 3, 3, ':/images/onepiece.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (12, 'Eiichiro Oda', 'Shueisha', TRUE);
INSERT INTO Libri(id, linguaoriginale, nvolumi)
VALUES (12, 'Giapponese', 110);
INSERT INTO Manga(id, concluso)
VALUES (12, FALSE);

-- ID 13: Film
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (13, 'Film', 'Shrek', 'Animazione', 2001, 9.99, TRUE, 1, 1, ':/images/shrek.png');
INSERT INTO Multimedia(id, durata, studio)
VALUES (13, 90, 'DreamWorks Animation');
INSERT INTO Film(id, regista, attoreprotagonista, visto)
VALUES (13, 'Andrew Adamson', 'Shrek', FALSE);

-- ID 14: Film
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (14, 'Film', 'Skyscraper', 'Avventura', 2018, 9.99, TRUE, 1, 1, ':/images/skyscraper.png');
INSERT INTO Multimedia(id, durata, studio)
VALUES (14, 102, 'Legendary Pictures');
INSERT INTO Film(id, regista, attoreprotagonista, visto)
VALUES (14, 'Rawson Marshall Thurber', 'Dwayne Johnson', TRUE);

-- ID 15: CD
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (15, 'CD', 'The dark side of the moon', 'Rock', 1973, 8.99, TRUE, 5, 5, ':/images/dark_side.png');
INSERT INTO Multimedia(id, durata, studio)
VALUES (15, 43, 'Harvest');
INSERT INTO CD(id, artista, ascoltato, ntracce)
VALUES (15, 'Pink Floyd', FALSE, 10);

-- ID 16: Riviste
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (16, 'Riviste', 'Vanity fair', 'Moda', 2003, 3.99, TRUE, 4, 5, ':/images/vanity_fair.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (16, 'Simone Marchetti', 'Condé Nast', FALSE);
INSERT INTO Riviste(id, diffusion)
VALUES (16, 'Nazionale');

-- ID 17: Riviste (duplicato)
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (17, 'Riviste', 'Vanity fair', 'Moda', 2003, 3.99, TRUE, 5, 5, ':/images/vanity_fair2.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (17, 'Simone Marchetti', 'Condé Nast', FALSE);
INSERT INTO Riviste(id, diffusion)
VALUES (17, 'Nazionale');

-- ID 18: Manga
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (18, 'Manga', 'Vinland Saga', 'Seinen', 2005, 5.20, TRUE, 3, 3, ':/images/vinland_saga2.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (18, 'Makoto Yukimura', 'Kodansha', TRUE);
INSERT INTO Libri(id, linguaoriginale, nvolumi)
VALUES (18, 'Giapponese', 28);
INSERT INTO Manga(id, concluso)
VALUES (18, TRUE);

-- ID 19: Manga 
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (19, 'Manga', 'Vinland Saga', 'Seinen', 2006, 5.20, TRUE, 3, 3, ':/images/vinland_saga.png');
INSERT INTO Cartaceo(id, autore, editore, letto)
VALUES (19, 'Makoto Yukimura', 'Kodansha', TRUE);
INSERT INTO Libri(id, linguaoriginale, nvolumi)
VALUES (19, 'Giapponese', 28);
INSERT INTO Manga(id, concluso)
VALUES (19, TRUE);

-- ID 20: CD
INSERT INTO Biblioteca(id, classe, titolo, genere, anno, costo, disponibile, copie_disponibili, nprestiti, immagine)
VALUES (20, 'CD', 'È finita la pace', 'Rap', 2024, 8.99, TRUE, 3, 5, ':/images/e_finita_la_pace.png');
INSERT INTO Multimedia(id, durata, studio)
VALUES (20, 46, 'Island');
INSERT INTO CD(id, artista, ascoltato, ntracce)
VALUES (20, 'Marracash', TRUE, 13);

SELECT setval('biblioteca_id_seq', (SELECT MAX(id) FROM biblioteca));