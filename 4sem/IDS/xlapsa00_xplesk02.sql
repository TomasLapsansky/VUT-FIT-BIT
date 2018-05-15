DROP TABLE Zakazka CASCADE CONSTRAINTS;
DROP TABLE Nabytok CASCADE CONSTRAINTS;
DROP TABLE Material CASCADE CONSTRAINTS;
DROP TABLE Dodavatel CASCADE CONSTRAINTS;
DROP TABLE Zakaznik CASCADE CONSTRAINTS;
DROP TABLE Zamestnanec CASCADE CONSTRAINTS;
DROP TABLE Externista CASCADE CONSTRAINTS;
DROP TABLE Internista CASCADE CONSTRAINTS;

DROP TABLE Doda CASCADE CONSTRAINTS;
DROP TABLE PouzijeSa CASCADE CONSTRAINTS;
DROP TABLE Pracuje CASCADE CONSTRAINTS;
DROP TABLE Vyroba CASCADE CONSTRAINTS;

DROP SEQUENCE nID;

CREATE TABLE Zakazka (
ZakazkaID INT NOT NULL,
ZakaznikID INT NOT NULL,
ZamestnanecID INT NOT NULL,
Datum DATE DEFAULT SYSDATE,
Stav VARCHAR2(30) NOT NULL,
Cena INT,
Hodiny INT DEFAULT 0,

PRIMARY KEY (ZakazkaID),

CHECK (ZakazkaID < 1000000),
CHECK (ZakazkaID > 0),
CHECK (Cena >= 0)
);

CREATE TABLE Nabytok (
NabytokID INT NOT NULL,
Nazov VARCHAR2(30) NOT NULL,
Farba VARCHAR2(30) NOT NULL,
Sirka INT NOT NULL,
Vyska INT NOT NULL,
Hlbka INT NOT NULL,
Prislusenstvo VARCHAR2(30),

PRIMARY KEY (NabytokID),

CHECK (NabytokID < 1000000),
CHECK (NabytokID > 0)
);

CREATE TABLE Material (
MaterialID INT NOT NULL,
Typ VARCHAR2(30) NOT NULL,
Cena INT NOT NULL,

PRIMARY KEY (MaterialID),

CHECK (MaterialID < 1000000),
CHECK (MaterialID > 0),
CHECK (CENA >= 0)
);

CREATE TABLE Dodavatel (
DodavatelID INT NOT NULL,
Meno VARCHAR2(30) NOT NULL,
Kontakt INT NOT NULL,
Email VARCHAR2(100),
IBAN VARCHAR(34) NOT NULL,

PRIMARY KEY (DodavatelID),

CHECK (DodavatelID < 1000000),
CHECK (DodavatelID > 0)
--CHECK (regexp_like(Meno,'^[A-Za-z]+$'))
--CHECK (regexp_like(Email,'^(([^<>()\[\]\\.,;:\s@"]+(\.[^<>()\[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$'))
);

CREATE TABLE Zakaznik (
ZakaznikID INT NOT NULL,
Meno VARCHAR2(30) NOT NULL,
Priezvisko VARCHAR2(30) NOT NULL,
Ulica VARCHAR2(30) NOT NULL,
Mesto VARCHAR2(30) NOT NULL,
PSC INT NOT NULL,
Kontakt INT NOT NULL,
Email VARCHAR2(100),

PRIMARY KEY (ZakaznikID),

CHECK (ZakaznikID < 1000000),
CHECK (ZakaznikID > 0),
CHECK (regexp_like(Priezvisko,'^[A-Za-z]+$')),
CHECK (regexp_like(Meno,'^[A-Za-z]+$'))
);

CREATE TABLE Zamestnanec (
ZamestnanecID INT,
Meno VARCHAR2(30) NOT NULL,
Priezvisko VARCHAR2(30) NOT NULL,
Specializacia VARCHAR2(30) NOT NULL,
Mzda INT NOT NULL,

PRIMARY KEY (ZamestnanecID),

CHECK (ZamestnanecID < 1000000),
CHECK (ZamestnanecID > 0),
CHECK (regexp_like(Priezvisko,'^[A-Za-z]+$')),
CHECK (regexp_like(Meno,'^[A-Za-z]+$')),
CHECK (Mzda >= 0)
);

CREATE TABLE Externista (
ZamestnanecID INT NOT NULL PRIMARY KEY,
Firma VARCHAR2(30),
FOREIGN KEY (ZamestnanecID) REFERENCES Zamestnanec
);

CREATE TABLE Internista (
ZamestnanecID INT NOT NULL PRIMARY KEY,
FOREIGN KEY (ZamestnanecID) REFERENCES Zamestnanec
);

-----------------------------------------

CREATE TABLE Pracuje (
ZamestnanecID INT NOT NULL,
ZakazkaID INT NOT NULL,
FOREIGN KEY (ZamestnanecID) REFERENCES Zamestnanec,
FOREIGN KEY (ZakazkaID) REFERENCES Zakazka
);

CREATE TABLE Vyroba (
Mnozstvo INT NOT NULL,
ZakazkaID INT NOT NULL,
NabytokID INT NOT NULL,
FOREIGN KEY (ZakazkaID) REFERENCES Zakazka,
FOREIGN KEY (NabytokID) REFERENCES Nabytok
);

CREATE TABLE PouzijeSa (
Mnozstvo INT NOT NULL,
NabytokID INT NOT NULL,
MaterialID INT NOT NULL,
FOREIGN KEY (NabytokID) REFERENCES Nabytok,
FOREIGN KEY (MaterialID) REFERENCES Material
);

CREATE TABLE Doda (
MaterialID INT NOT NULL,
DodavatelID INT NOT NULL,
FOREIGN KEY (MaterialID) REFERENCES Material,
FOREIGN KEY (DodavatelID) REFERENCES Dodavatel
);

-----------------------------------------

ALTER TABLE Zakazka ADD CONSTRAINT ZakaznikID FOREIGN KEY (ZakaznikID) REFERENCES Zakaznik;          -- Ma
ALTER TABLE Zakazka ADD CONSTRAINT ZamestnanecRiadiID FOREIGN KEY (ZamestnanecID) REFERENCES Zamestnanec;  -- Riadi

-----------------------------------------
-- IBAN kontrola TRIGGER
-----------------------------------------

CREATE OR REPLACE TRIGGER IBANChecker
BEFORE INSERT ON Dodavatel
FOR EACH ROW
DECLARE
Ucet Dodavatel.IBAN%TYPE;
Krajina varchar2(2);
Cislo varchar2(32);
BEGIN
Ucet := :NEW.IBAN;
Krajina := substr(Ucet, 1, 2);
Cislo := substr(Ucet, 3, 22);

IF (LENGTH(TRIM(TRANSLATE(Krajina, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', ' '))) IS NOT NULL) THEN
RAISE_APPLICATION_ERROR(-20200, 'Bad IBAN');
END IF;
IF (LENGTH(TRIM(TRANSLATE(Cislo, '0123456789', ' '))) IS NOT NULL) THEN
RAISE_APPLICATION_ERROR(-20200, 'Bad IBAN');
END IF;
IF (LENGTH(Ucet) < 24) THEN
RAISE_APPLICATION_ERROR(-20200, 'Bad IBAN');
END IF;
END;

-----------------------------------------
-- ID TRIGGER
-----------------------------------------

CREATE SEQUENCE nID;
CREATE OR REPLACE TRIGGER incIDZamestnanec
BEFORE INSERT ON Zamestnanec
FOR EACH ROW
BEGIN
:NEW.ZamestnanecID := nID.NEXTVAL;
END;

CREATE OR REPLACE TRIGGER incIDZakaznik
BEFORE INSERT ON Zakaznik
FOR EACH ROW
BEGIN
:NEW.ZakaznikID := nID.NEXTVAL;
END;

CREATE OR REPLACE TRIGGER incIDZakazka
BEFORE INSERT ON Zakazka
FOR EACH ROW
BEGIN
:NEW.ZakazkaID := nID.NEXTVAL;
END;

CREATE OR REPLACE TRIGGER incIDNabytok
BEFORE INSERT ON Nabytok
FOR EACH ROW
BEGIN
:NEW.NabytokID := nID.NEXTVAL;
END;

CREATE OR REPLACE TRIGGER incIDMaterial
BEFORE INSERT ON Material
FOR EACH ROW
BEGIN
:NEW.MaterialID := nID.NEXTVAL;
END;

CREATE OR REPLACE TRIGGER incIDDodavatel
BEFORE INSERT ON Dodavatel
FOR EACH ROW
BEGIN
:NEW.DodavatelID := nID.NEXTVAL;
END;

-----------------------------------------
-- Zodpovedny zamestnanec TRIGGER
-----------------------------------------

CREATE OR REPLACE TRIGGER defaultPracuje
AFTER INSERT ON Zakazka
FOR EACH ROW
BEGIN
INSERT INTO Pracuje (ZakazkaID, ZamestnanecID) VALUES (:NEW.ZakazkaID, :NEW.ZamestnanecID);
END;

-----------------------------------------
-- PROCEDURA
-----------------------------------------

SET serveroutput ON;
CREATE OR REPLACE PROCEDURE profityZaRok(rok IN INTEGER)
IS
cursor obsah IS SELECT * FROM ZAKAZKA WHERE ZAKAZKA.Datum >= to_date('1.1.' || rok, 'DD.MM.YYYY') AND
ZAKAZKA.Datum < to_date('1.1.' || (rok + 1), 'DD.MM.YYYY');
cena INTEGER;
act_row ZAKAZKA%ROWTYPE;
BEGIN
cena := 0;
OPEN obsah;
loop
fetch obsah INTO act_row;
exit WHEN obsah%NOTFOUND;
cena := cena + act_row.cena;
END loop;
DBMS_OUTPUT.PUT_LINE('Obrat za rok ' || rok || ' je ' || cena);
END;

SET serveroutput ON;
CREATE OR REPLACE PROCEDURE priemernaMzda
IS
cursor pracovnik IS SELECT * FROM Zamestnanec;
act_row Zamestnanec%ROWTYPE;
plat INTEGER;
pocet INTEGER;
BEGIN
plat := 0;
pocet := 0;
OPEN pracovnik;
loop
fetch pracovnik INTO act_row;
exit WHEN pracovnik%NOTFOUND;
plat := plat + act_row.mzda;
pocet := pocet + 1;
END loop;
DBMS_OUTPUT.PUT_LINE('Priemerny plat zamestnanca vo firme je ' || ROUND(plat/pocet));
EXCEPTION
WHEN ZERO_DIVIDE THEN
DBMS_OUTPUT.PUT_LINE('Firma nema ziadnych zamestnancov');
END;

-----------------------------------------
--INSERT
-----------------------------------------
INSERT INTO Zamestnanec (Meno, Priezvisko, Specializacia, Mzda) VALUES ('Tomas', 'Lapsansky', 'IT', 5000000000);
INSERT INTO Zamestnanec (Meno, Priezvisko, Specializacia, Mzda) VALUES ('Filip', 'Plesko', 'IT-helper', 500000);
INSERT INTO Zamestnanec (Meno, Priezvisko, Specializacia, Mzda) VALUES ('Fero', 'Radiator', 'Tester', 7623);
INSERT INTO Zamestnanec (Meno, Priezvisko, Specializacia, Mzda) VALUES ('Juro', 'Kovac', 'Manazer', 8562);
INSERT INTO Zamestnanec (Meno, Priezvisko, Specializacia, Mzda) VALUES ('Marek', 'Kladivo', 'Skladnik', 53452);
INSERT INTO Zamestnanec (Meno, Priezvisko, Specializacia, Mzda) VALUES ('David', 'Stolar', 'Servisak', 23400);

INSERT INTO Internista(ZamestnanecID) VALUES (1);
INSERT INTO Internista(ZamestnanecID) VALUES (2);
INSERT INTO Internista(ZamestnanecID) VALUES (4);
INSERT INTO Externista(ZamestnanecID, Firma) VALUES (3,'Matros s.r.o');
INSERT INTO Externista(ZamestnanecID, Firma) VALUES (5,'Matros s.r.o');
INSERT INTO Externista(ZamestnanecID, Firma) VALUES (6,'Drevo s.r.o');

INSERT INTO Nabytok (Nazov, Farba, Sirka, Vyska, Hlbka, Prislusenstvo) VALUES ('Skrina' ,'Biela', '55', '40', '50', '3 policky');
INSERT INTO Nabytok (Nazov, Farba, Sirka, Vyska, Hlbka, Prislusenstvo) VALUES ('Ina Skrina' ,'Cierna', '65', '80', '30', '5 policiek');
INSERT INTO Nabytok (Nazov, Farba, Sirka, Vyska, Hlbka, Prislusenstvo) VALUES ('Postel' ,'Zlta', '100', '60', '200', 'Perina');
INSERT INTO Nabytok (Nazov, Farba, Sirka, Vyska, Hlbka, Prislusenstvo) VALUES ('Stolicka' ,'Hneda', '56', '47', '100', 'Operadlo');

INSERT INTO Material (Typ, Cena) VALUES ('Buk', 5);
INSERT INTO Material (Typ, Cena) VALUES ('Smrek', 3);
INSERT INTO Material (Typ, Cena) VALUES ('Javor', 8);
INSERT INTO Material (Typ, Cena) VALUES ('Borovica', 2);

INSERT INTO Dodavatel (Meno, Kontakt, Email, IBAN) VALUES ('Matros s.r.o', '00421485038593', 'matros@eml.com', 'SK7511000000002912560981');
INSERT INTO Dodavatel (Meno, Kontakt, Email, IBAN) VALUES ('Drevo s.r.o', '00421638593847', 'drevo@eml.com', 'SK7511000000003458124704');
INSERT INTO Dodavatel (Meno, Kontakt, Email, IBAN) VALUES ('Rezbar s.r.o', '00421485348593', 'rezbar@eml.com', 'SK7511000000002628740740');
INSERT INTO Dodavatel (Meno, Kontakt, Email, IBAN) VALUES ('Stolar s.r.o', '00421638504847', 'stolar@eml.com', 'SK7511000000002741980736');

INSERT INTO Doda  (MaterialID, DodavatelID) VALUES (11, 15);
INSERT INTO Doda  (MaterialID, DodavatelID) VALUES (11, 16);
INSERT INTO Doda  (MaterialID, DodavatelID) VALUES (12, 17);
INSERT INTO Doda  (MaterialID, DodavatelID) VALUES (12, 18);
INSERT INTO Doda  (MaterialID, DodavatelID) VALUES (13, 15);
INSERT INTO Doda  (MaterialID, DodavatelID) VALUES (13, 16);
INSERT INTO Doda  (MaterialID, DodavatelID) VALUES (14, 17);

INSERT INTO PouzijeSa (Mnozstvo, NabytokID, MaterialID) VALUES (3, 7, 11);
INSERT INTO PouzijeSa (Mnozstvo, NabytokID, MaterialID) VALUES (4, 8, 12);
INSERT INTO PouzijeSa (Mnozstvo, NabytokID, MaterialID) VALUES (5, 9, 13);
INSERT INTO PouzijeSa (Mnozstvo, NabytokID, MaterialID) VALUES (1, 10, 14);


INSERT INTO Zakaznik (Meno, Priezvisko, Ulica, Mesto, PSC, Email, Kontakt) VALUES ('Jurko', 'Mrkvicka', 'Hlinena', 'Betakaroten', 93472, 'jozko@eml.com', '00231254332');
INSERT INTO Zakaznik (Meno, Priezvisko, Ulica, Mesto, PSC, Kontakt) VALUES ('Samo', 'Vrah', 'Vezenska', 'Nadvorovo', '14762', 12354333);
INSERT INTO Zakaznik (Meno, Priezvisko, Ulica, Mesto, PSC, Email, Kontakt) VALUES ('Jano', 'Paradajka', 'Sklarska', 'Brno', 93572, 'jano@eml.com', '00237254332');
INSERT INTO Zakaznik (Meno, Priezvisko, Ulica, Mesto, PSC, Kontakt) VALUES ('Marcel', 'Vnatka', 'Skacelova', 'Praha', '14862', 12354339);
INSERT INTO Zakaznik (Meno, Priezvisko, Ulica, Mesto, PSC, Email, Kontakt) VALUES ('Michal', 'Stolicka', 'Kolejni', 'Presov', 93772, 'michal@eml.com', '00234254332');
INSERT INTO Zakaznik (Meno, Priezvisko, Ulica, Mesto, PSC, Kontakt) VALUES ('Dusan', 'Hranaty', 'Podhajska', 'Martin', '14262', 12354336);

INSERT INTO Zakazka (ZakaznikID, ZamestnanecID, Stav, Cena) VALUES (19, 1, 'Vyroba', 55);
INSERT INTO Zakazka (ZakaznikID, ZamestnanecID, Stav, Cena) VALUES (20, 1, 'Distribucia', 250);
INSERT INTO Zakazka (ZakaznikID, ZamestnanecID, Stav, Cena) VALUES (21, 2, 'Distribucia', 400);
INSERT INTO Zakazka (ZakaznikID, ZamestnanecID, Stav, Cena) VALUES (21, 2, 'Distribucia', 500);
INSERT INTO Zakazka (ZakaznikID, ZamestnanecID, Stav, Cena) VALUES (19, 2, 'Distribucia', 400);

INSERT INTO Vyroba (Mnozstvo, ZakazkaID, NabytokID) VALUES ('3', 25, 7);
INSERT INTO Vyroba (Mnozstvo, ZakazkaID, NabytokID) VALUES ('5', 26, 7);
INSERT INTO Vyroba (Mnozstvo, ZakazkaID, NabytokID) VALUES ('15', 27, 8);
INSERT INTO Vyroba (Mnozstvo, ZakazkaID, NabytokID) VALUES ('4', 28, 9);
INSERT INTO Vyroba (Mnozstvo, ZakazkaID, NabytokID) VALUES ('8', 29, 10);

-- Zistuje ktory zakaznik ma ktoru zakazku
SELECT Z.Meno, Z.Priezvisko, P.ZakazkaID
FROM ZAKAZNIK Z, ZAKAZKA P
WHERE P.ZakaznikID = Z.ZakaznikID;

-- Zistuje ktory zamestnanec je externista a z ktorej firmy pochadza
SELECT Z.Meno, Z.Priezvisko, E.ZamestnanecID, E.Firma
FROM ZAMESTNANEC Z, EXTERNISTA E
WHERE Z.ZamestnanecID = E.ZamestnanecID;

-- Zistuje aky material sa pouziva na vyrobu urciteho nabytku a v akom mnozstve
SELECT N.Nazov, M.Typ, P.Mnozstvo
FROM NABYTOK N, POUZIJESA P, MATERIAL M
WHERE N.NabytokID = P.NabytokID AND M.MaterialID = P.MaterialID;

-- Zisti kolko nabytkov je urcitej farby
SELECT Farba, COUNT(Farba) AS pocet
FROM NABYTOK
GROUP BY Farba;

-- Zisti kolko zakaznikov pochadza z Brna
SELECT Mesto, COUNT(Mesto) AS zije_v_meste
FROM ZAKAZNIK
WHERE Mesto = 'Brno'
GROUP BY Mesto;

-- Vyberie nabytok ktory nie je vo vyrobe

SELECT N.Nazov
FROM NABYTOK N
WHERE NOT EXISTS(
SELECT *
FROM VYROBA V
WHERE V.NabytokID = N.NabytokID
);


-- Zisti ktory nabytok je z materialu lacnejsieho ako 5
SELECT N.*
FROM MATERIAL, NABYTOK N, POUZIJESA
WHERE Cena IN
(SELECT Cena
FROM MATERIAL
WHERE Cena < 5 AND N.NabytokID = PouzijeSa.NabytokID AND PouzijeSa.MaterialID = Material.MaterialID);

-- VOLANIE PROCEDUR
BEGIN
profityZaRok(2018);
priemernaMzda();
END;

------------------------------
-- EXPLAIN  PLAN
------------------------------
DROP INDEX plan_index;
EXPLAIN PLAN FOR
SELECT Priezvisko, COUNT(*)
FROM ZAKAZKA NATURAL JOIN ZAKAZNIK
GROUP BY Priezvisko;
SELECT * FROM TABLE(DBMS_XPLAN.display);

CREATE INDEX plan_index ON ZAKAZKA(ZakaznikID);

EXPLAIN PLAN FOR
SELECT Priezvisko, COUNT(*)
FROM ZAKAZKA NATURAL JOIN ZAKAZNIK
GROUP BY Priezvisko;
SELECT * FROM TABLE(DBMS_XPLAN.display);

------------------------------
-- RIGHTS FOR OTHER USER
------------------------------

GRANT ALL ON ZAKAZKA TO XPLESK02;
GRANT ALL ON NABYTOK TO XPLESK02;
GRANT ALL ON MATERIAL TO XPLESK02;
GRANT ALL ON DODAVATEL TO XPLESK02;
GRANT ALL ON ZAKAZNIK TO XPLESK02;
GRANT ALL ON ZAMESTNANEC TO XPLESK02;
GRANT ALL ON EXTERNISTA TO XPLESK02;
GRANT ALL ON INTERNISTA TO XPLESK02;
GRANT ALL ON DODA TO XPLESK02;
GRANT ALL ON POUZIJESA TO XPLESK02;
GRANT ALL ON PRACUJE TO XPLESK02;
GRANT ALL ON VYROBA TO XPLESK02;

------------------------------
-- MATERIALIZED VIEW
------------------------------
DROP MATERIALIZED VIEW stavZakazok;

CREATE MATERIALIZED VIEW LOG ON Zakazka WITH PRIMARY KEY, ROWID(Stav) INCLUDING NEW VALUES;

CREATE MATERIALIZED VIEW stavZakazok
NOLOGGING
CACHE
BUILD IMMEDIATE
REFRESH FAST ON COMMIT
ENABLE QUERY REWRITE
AS
SELECT Zakazka.Stav, count(Zakazka.Stav) as Pocet
FROM Zakazka
GROUP BY Zakazka.Stav;

GRANT ALL ON stavZakazok TO XPLESK02;

SELECT * FROM stavZakazok;

INSERT INTO Zakazka(ZakaznikID, ZamestnanecID, Stav, Cena, Hodiny) VALUES (19, 1, 'Vyroba', 500, 0);
COMMIT;

SELECT * FROM stavZakazok;
