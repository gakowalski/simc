CREATE TABLE `teryt_simc` (
	`WOJ` CHAR(2) NOT NULL,
	`POW` CHAR(2) NOT NULL,
	`GMI` CHAR(2) NOT NULL,
	`RODZ_GMI` CHAR(1) NOT NULL,
	`RM` CHAR(2) NOT NULL,
	`MZ` CHAR(1) NOT NULL,
	`NAZWA` TINYTEXT NOT NULL,
	`SYM` CHAR(7) NOT NULL,
	`SYMPOD` CHAR(7) NOT NULL,
	`STAN_NA` DATE NOT NULL,
	PRIMARY KEY (`SYM`)
)
COLLATE='utf8_unicode_520_ci'
;
