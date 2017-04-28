-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema pankkiautomaatti
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema pankkiautomaatti
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `pankkiautomaatti` DEFAULT CHARACTER SET utf8 ;
USE `pankkiautomaatti` ;

-- -----------------------------------------------------
-- Table `pankkiautomaatti`.`Asiakas`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `pankkiautomaatti`.`Asiakas` ;

CREATE TABLE IF NOT EXISTS `pankkiautomaatti`.`Asiakas` (
  `idAsiakas` INT NOT NULL AUTO_INCREMENT,
  `pankkikortinNumero` VARCHAR(45) NOT NULL,
  `tunnusluku` INT(4) NOT NULL,
  `SOTU` VARCHAR(45) NOT NULL,
  `etunimi` VARCHAR(45) NOT NULL,
  `sukunimi` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`idAsiakas`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `pankkiautomaatti`.`Tili`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `pankkiautomaatti`.`Tili` ;

CREATE TABLE IF NOT EXISTS `pankkiautomaatti`.`Tili` (
  `idTili` INT NOT NULL AUTO_INCREMENT,
  `tilinumero` VARCHAR(45) NOT NULL,
  `saldo` FLOAT NOT NULL,
  `idAsiakas` INT NOT NULL,
  PRIMARY KEY (`idTili`, `idAsiakas`),
  INDEX `idAsiakas1_idx` (`idAsiakas` ASC),
  CONSTRAINT `idAsiakas1`
    FOREIGN KEY (`idAsiakas`)
    REFERENCES `pankkiautomaatti`.`Asiakas` (`idAsiakas`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `pankkiautomaatti`.`Tilitapahtumat`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `pankkiautomaatti`.`Tilitapahtumat` ;

CREATE TABLE IF NOT EXISTS `pankkiautomaatti`.`Tilitapahtumat` (
  `idTapahtuma` INT NOT NULL AUTO_INCREMENT,
  `pvm` DATE NOT NULL,
  `tapahtuma` VARCHAR(45) NOT NULL,
  `tapahtumanKuvaus` VARCHAR(45) NOT NULL,
  `summa` VARCHAR(45) NULL,
  `idTili` INT NOT NULL,
  PRIMARY KEY (`idTapahtuma`, `idTili`),
  INDEX `idTili1_idx` (`idTili` ASC),
  CONSTRAINT `idTili1`
    FOREIGN KEY (`idTili`)
    REFERENCES `pankkiautomaatti`.`Tili` (`idTili`)
    ON DELETE RESTRICT
    ON UPDATE CASCADE)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

-- -----------------------------------------------------
-- Data for table `pankkiautomaatti`.`Asiakas`
-- -----------------------------------------------------
START TRANSACTION;
USE `pankkiautomaatti`;
INSERT INTO `pankkiautomaatti`.`Asiakas` (`idAsiakas`, `pankkikortinNumero`, `tunnusluku`, `SOTU`, `etunimi`, `sukunimi`) VALUES (DEFAULT, '1234', 1234, '1234', 'Matti', 'Meikalainen');
INSERT INTO `pankkiautomaatti`.`Asiakas` (`idAsiakas`, `pankkikortinNumero`, `tunnusluku`, `SOTU`, `etunimi`, `sukunimi`) VALUES (DEFAULT, '4321', 4321, '4321', 'Aku', 'Ankka');

COMMIT;


-- -----------------------------------------------------
-- Data for table `pankkiautomaatti`.`Tili`
-- -----------------------------------------------------
START TRANSACTION;
USE `pankkiautomaatti`;
INSERT INTO `pankkiautomaatti`.`Tili` (`idTili`, `tilinumero`, `saldo`, `idAsiakas`) VALUES (DEFAULT, '1234', 100, 1);
INSERT INTO `pankkiautomaatti`.`Tili` (`idTili`, `tilinumero`, `saldo`, `idAsiakas`) VALUES (DEFAULT, '4321', 1000, 2);

COMMIT;

