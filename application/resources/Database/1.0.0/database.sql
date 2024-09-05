PRAGMA foreign_keys=ON;

------------------------------------------------------------------------------------------------------------------------;

CREATE TABLE IF NOT EXISTS "version" (
    "major" INTEGER,
    "minor" INTEGER,
    "micro" INTEGER,
    "description" TEXT
);

------------------------------------------------------------------------------------------------------------------------;

CREATE TABLE IF NOT EXISTS "quotes" (
    "asset_type" INTEGER,
    "resolution" INTEGER,
    "timestamp" TEXT,
    "price" REAL,
    "capacity" price
);

------------------------------------------------------------------------------------------------------------------------;

CREATE TABLE IF NOT EXISTS "history" (
    "asset_type" INTEGER,
    "hash" TEXT UNIQUE NOT NULL PRIMARY KEY,
    "note" TEXT
);

------------------------------------------------------------------------------------------------------------------------;

INSERT INTO "version" ("major", "minor", "micro", "description") VALUES (1, 0, 0, 'initial');
