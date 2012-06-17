--
-- Table structure for table 'addresses'
--

CREATE TABLE addresses (
  first varchar(30) default NULL,
  last varchar(40) default NULL,
  street varchar(80) default NULL,
  city varchar(30) default NULL,
  zip varchar(10) default NULL,
  country varchar(20) default NULL,
  phone varchar(20) default NULL
) TYPE=MyISAM;

--
-- Dumping data for table 'addresses'
--


INSERT INTO addresses VALUES ('John','Doe','13 Mulholland Drive','LA','1234','USA',NULL);
INSERT INTO addresses VALUES ('Avacon','AG','Schöninger Str. 10','Helmstedt','38350','Germany','0180-3282266');
INSERT INTO addresses VALUES ('Jose','Velarde','Malacañang','Metro Manila','1000','Philippines','N.N.');
INSERT INTO addresses VALUES ('Carbonos de Portugal','S.A.','Apartado 215','Sines','7520-903','Portugal','+351-26-9630180');


