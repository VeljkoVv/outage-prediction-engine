BEGIN TRANSACTION;
DROP TABLE IF EXISTS "lv_feeder";
CREATE TABLE "lv_feeder" (
	"feeder_id"	NUMBER(10) NOT NULL,
	"feeder_name"	VARCHAR2(100) NOT NULL,
	CONSTRAINT "pk_lv_feeder" PRIMARY KEY("feeder_id"),
	CONSTRAINT "uq_lv_feeder_name" UNIQUE("feeder_name")
);
DROP TABLE IF EXISTS "lv_transformer";
CREATE TABLE lv_transformer (
    transformer_id   NUMBER(10)     NOT NULL,
    transformer_name VARCHAR2(100)  NOT NULL,
    CONSTRAINT pk_lv_transformer      PRIMARY KEY (transformer_id),
    CONSTRAINT uq_lv_transformer_name UNIQUE (transformer_name)
);
DROP TABLE IF EXISTS "outage";
CREATE TABLE "outage" (
	"outage_id"	INTEGER,
	"type"	TEXT NOT NULL,
	"service_point_id"	INTEGER,
	"feeder_id"	INTEGER,
	"transformer_id"	INTEGER,
	"status"	TEXT NOT NULL DEFAULT 'Predicted',
	"started_at"	INTEGER NOT NULL,
	"ended_at"	INTEGER,
	"merged_id"	INTEGER,
	PRIMARY KEY("outage_id"),
	FOREIGN KEY("feeder_id") REFERENCES "lv_feeder"("feeder_id"),
	FOREIGN KEY("merged_id") REFERENCES "outage"("outage_id") DEFERRABLE INITIALLY DEFERRED,
	FOREIGN KEY("service_point_id") REFERENCES "service_point"("point_id"),
	FOREIGN KEY("transformer_id") REFERENCES "lv_transformer"("transformer_id"),
	CONSTRAINT "outage_type_ck" CHECK("type" IN ('ServicePoint', 'Feeder', 'Transformer')),
	CONSTRAINT "outage_status_ck" CHECK("status" IN ('Predicted', 'Confirmed', 'Restored', 'Merged', 'Cancelled')),
	CONSTRAINT "outage_element_ck" CHECK(("type" = 'ServicePoint' AND "service_point_id" IS NOT NULL AND "feeder_id" IS NULL AND "transformer_id" IS NULL) OR ("type" = 'Feeder' AND "service_point_id" IS NULL AND "feeder_id" IS NOT NULL AND "transformer_id" IS NULL) OR ("type" = 'Transformer' AND "service_point_id" IS NULL AND "feeder_id" IS NULL AND "transformer_id" IS NOT NULL))
);
DROP TABLE IF EXISTS "service_delivery_point";
CREATE TABLE "service_delivery_point" (
	"delivery_id"	NUMBER(10) NOT NULL,
	"point_id"	NUMBER(10) NOT NULL,
	"delivery_name"	VARCHAR2(100) NOT NULL,
	"delivery_adress"	VARCHAR2(100) NOT NULL,
	CONSTRAINT "pk_lv_service_delivery_point" PRIMARY KEY("delivery_id"),
	CONSTRAINT "uq_lv_service_delivery_point_name" UNIQUE("delivery_name"),
	FOREIGN KEY("point_id") REFERENCES "service_point"("point_id")
);
DROP TABLE IF EXISTS "service_point";
CREATE TABLE "service_point" (
	"point_id"	NUMBER(10) NOT NULL,
	"feeder_id"	NUMBER(10) NOT NULL,
	"point_name"	VARCHAR2(100) NOT NULL,
	CONSTRAINT "pk_lv_service_point" PRIMARY KEY("point_id"),
	CONSTRAINT "uq_lv_service_point_name" UNIQUE("point_name"),
	CONSTRAINT "fk_lv_service_point" FOREIGN KEY("feeder_id") REFERENCES "lv_feeder"("feeder_id")
);
DROP TABLE IF EXISTS "trouble_call";
CREATE TABLE "trouble_call" (
	"call_id"	INTEGER,
	"point_id"	INTEGER NOT NULL,
	"outage_id"	INTEGER,
	"type"	TEXT NOT NULL,
	"received_at"	INTEGER NOT NULL,
	"call_status"	TEXT NOT NULL DEFAULT 'Open',
	PRIMARY KEY("call_id"),
	FOREIGN KEY("outage_id") REFERENCES "outage"("outage_id"),
	FOREIGN KEY("point_id") REFERENCES "service_delivery_point"("delivery_id"),
	CONSTRAINT "trouble_call_type_ck" CHECK("type" IN ('NoPower', 'PartialPower', 'Flickering', 'Hazard')),
	CONSTRAINT "trouble_call_status_ck" CHECK("call_status" IN ('Open', 'Assigned', 'Closed', 'Cancelled'))
);
INSERT INTO "lv_feeder" ("feeder_id","feeder_name") VALUES (10,'Feeder 1 - Rumenacka'),
 (20,'Feeder 2 - Janka Cmelika'),
 (30,'Feeder 3 - Kornelija Stankovica'),
 (40,'Feeder 4 - Bulevar Jase Tomica');
INSERT INTO "lv_transformer" ("transformer_id","transformer_name") VALUES (1,'TS Detelinara 2');
INSERT INTO "service_delivery_point" ("delivery_id","point_id","delivery_name","delivery_adress") VALUES (1001,101,'Marko Jovanovic','Rumenacka 12'),
 (1002,102,'Jelena Petrovic','Rumenacka 14'),
 (1003,103,'Dragan Nikolic','Rumenacka 18'),
 (1004,103,'Dragan Nikolic - garage','Rumenacka 18'),
 (1005,104,'Milica Djordjevic','Cirpanova 5'),
 (2001,201,'Apartment 1 - Stefan Ilic','Janka Cmelika 40/1'),
 (2002,201,'Apartment 2 - Ana Markovic','Janka Cmelika 40/2'),
 (2003,201,'Apartment 3 - Nikola Stojanovic','Janka Cmelika 40/3'),
 (2004,201,'Apartment 4 - Ivana Pavlovic','Janka Cmelika 40/4'),
 (2005,201,'Apartment 5 - Luka Zivkovic','Janka Cmelika 40/5'),
 (2006,201,'Apartment 6 - Sara Kovacevic','Janka Cmelika 40/6'),
 (2011,202,'Apartment 1 - Milos Popovic','Janka Cmelika 44/1'),
 (2012,202,'Apartment 2 - Tamara Savic','Janka Cmelika 44/2'),
 (2013,202,'Apartment 3 - Vladimir Lazic','Janka Cmelika 44/3'),
 (2014,202,'Apartment 4 - Katarina Mitic','Janka Cmelika 44/4'),
 (2021,203,'Apartment 1 - Djordje Radovic','Janka Veselinovica 5/1'),
 (2022,203,'Apartment 2 - Maja Todorovic','Janka Veselinovica 5/2'),
 (2023,203,'Apartment 3 - Aleksandar Saric','Janka Veselinovica 5/3'),
 (3001,301,'Petar Milosevic','Kornelija Stankovica 7'),
 (3002,302,'Pekara Zlatni klas','Kornelija Stankovica 9'),
 (3003,303,'Zoran Vukovic','Ilije Bircanina 3'),
 (3011,304,'Apartment 1 - Nevena Colic','Bate Brkica 15/1'),
 (3012,304,'Apartment 2 - Bojan Ristic','Bate Brkica 15/2'),
 (3013,304,'Apartment 3 - Jovana Zarkovic','Bate Brkica 15/3'),
 (3014,304,'Apartment 4 - Uros Bogdanovic','Bate Brkica 15/4'),
 (4001,401,'Apoteka Detelinara','Bulevar Jase Tomica 1'),
 (4002,401,'Kancelarija Novitas d.o.o.','Bulevar Jase Tomica 1'),
 (4003,401,'Frizerski salon Lana','Bulevar Jase Tomica 1'),
 (4004,401,'Common areas','Bulevar Jase Tomica 1'),
 (4011,402,'Market Detelinara','Bulevar Jase Tomica 3'),
 (4012,402,'Market Detelinara - cold storage','Bulevar Jase Tomica 3');
INSERT INTO "service_point" ("point_id","feeder_id","point_name") VALUES (101,10,'House - Rumenacka 12'),
 (102,10,'House - Rumenacka 14'),
 (103,10,'House - Rumenacka 18'),
 (104,10,'House - Cirpanova 5'),
 (201,20,'Building - Janka Cmelika 40'),
 (202,20,'Building - Janka Cmelika 44'),
 (203,20,'Building - Janka Veselinovica 5'),
 (301,30,'House - Kornelija Stankovica 7'),
 (302,30,'Shop - Kornelija Stankovica 9'),
 (303,30,'House - Ilije Bircanina 3'),
 (304,30,'Building - Bate Brkica 15'),
 (401,40,'Commercial building - Bulevar Jase Tomica 1'),
 (402,40,'Shop - Bulevar Jase Tomica 3'),
 (403,40,'Building under construction - Bulevar Jase Tomica 7');
DROP INDEX IF EXISTS "idx_call_outage";
CREATE INDEX idx_call_outage ON trouble_call(outage_id);
COMMIT;
