-- phpMyAdmin SQL Dump
-- version 4.7.7
-- https://www.phpmyadmin.net/
--
-- Host: localhost:8889
-- Generation Time: Dec 03, 2018 at 03:33 PM
-- Server version: 5.6.38
-- PHP Version: 7.2.1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `update_lekaren_is`
--

-- --------------------------------------------------------

--
-- Table structure for table `drug`
--

CREATE TABLE `drug` (
  `id` int(10) UNSIGNED NOT NULL,
  `name` varchar(50) CHARACTER SET utf8 NOT NULL,
  `count` int(10) UNSIGNED NOT NULL,
  `producer` int(11) NOT NULL,
  `price` int(10) UNSIGNED NOT NULL,
  `description` text CHARACTER SET utf8 NOT NULL,
  `image` varchar(100) CHARACTER SET utf8 NOT NULL DEFAULT '/image/product/default.png',
  `Time_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `state` tinyint(1) NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=ascii;

-- --------------------------------------------------------

--
-- Table structure for table `drug_insurer`
--

CREATE TABLE `drug_insurer` (
  `id` int(11) NOT NULL,
  `price` int(10) UNSIGNED NOT NULL,
  `drug_id` int(10) UNSIGNED NOT NULL,
  `insurer_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=ascii;

-- --------------------------------------------------------

--
-- Table structure for table `insurer`
--

CREATE TABLE `insurer` (
  `id` int(11) NOT NULL,
  `name` varchar(50) CHARACTER SET utf8 NOT NULL,
  `state` tinyint(1) NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=ascii;

-- --------------------------------------------------------

--
-- Table structure for table `order_drug`
--

CREATE TABLE `order_drug` (
  `id` int(10) UNSIGNED NOT NULL,
  `count` int(10) UNSIGNED NOT NULL DEFAULT '1',
  `drug_id` int(10) UNSIGNED NOT NULL,
  `order_id` int(10) UNSIGNED NOT NULL,
  `price` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=ascii;

-- --------------------------------------------------------

--
-- Table structure for table `order_product`
--

CREATE TABLE `order_product` (
  `id` int(10) UNSIGNED NOT NULL,
  `status` varchar(20) CHARACTER SET utf8 NOT NULL,
  `city` varchar(20) CHARACTER SET utf8 DEFAULT NULL,
  `zip` varchar(6) CHARACTER SET utf8 DEFAULT NULL,
  `address` varchar(50) CHARACTER SET utf8 DEFAULT NULL,
  `user_id` int(10) NOT NULL,
  `Time_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `state` tinyint(1) NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=ascii;

-- --------------------------------------------------------

--
-- Table structure for table `producer`
--

CREATE TABLE `producer` (
  `id` int(11) NOT NULL,
  `name` varchar(50) CHARACTER SET utf8 NOT NULL,
  `time_delivery` int(3) UNSIGNED NOT NULL,
  `state` tinyint(1) NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=ascii;

-- --------------------------------------------------------

--
-- Table structure for table `user`
--

CREATE TABLE `user` (
  `id` int(11) NOT NULL,
  `name` varchar(20) CHARACTER SET utf8 NOT NULL,
  `surname` varchar(50) CHARACTER SET utf8 NOT NULL,
  `email` varchar(50) CHARACTER SET utf8 NOT NULL,
  `password` varchar(256) CHARACTER SET utf8 NOT NULL,
  `role` varchar(11) CHARACTER SET utf8 DEFAULT 'user',
  `city` varchar(20) CHARACTER SET utf8 NOT NULL,
  `address` varchar(50) CHARACTER SET utf8 NOT NULL,
  `zip` varchar(6) CHARACTER SET utf8 NOT NULL,
  `country` varchar(50) CHARACTER SET utf8 NOT NULL,
  `insurer_id` int(11) DEFAULT NULL,
  `avatar` varchar(100) CHARACTER SET utf8 NOT NULL DEFAULT '/image/avatar/default.png',
  `Time_created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `state` tinyint(1) NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=ascii;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `drug`
--
ALTER TABLE `drug`
  ADD PRIMARY KEY (`id`),
  ADD KEY `producer` (`producer`);

--
-- Indexes for table `drug_insurer`
--
ALTER TABLE `drug_insurer`
  ADD PRIMARY KEY (`id`),
  ADD KEY `drug_id` (`drug_id`),
  ADD KEY `insurer_id` (`insurer_id`);

--
-- Indexes for table `insurer`
--
ALTER TABLE `insurer`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `order_drug`
--
ALTER TABLE `order_drug`
  ADD PRIMARY KEY (`id`),
  ADD KEY `drug_id` (`drug_id`),
  ADD KEY `order_id` (`order_id`);

--
-- Indexes for table `order_product`
--
ALTER TABLE `order_product`
  ADD PRIMARY KEY (`id`),
  ADD KEY `id_user` (`user_id`);

--
-- Indexes for table `producer`
--
ALTER TABLE `producer`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `user`
--
ALTER TABLE `user`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `email` (`email`),
  ADD KEY `insurer_id` (`insurer_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `drug`
--
ALTER TABLE `drug`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `drug_insurer`
--
ALTER TABLE `drug_insurer`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `insurer`
--
ALTER TABLE `insurer`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `order_drug`
--
ALTER TABLE `order_drug`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `order_product`
--
ALTER TABLE `order_product`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `producer`
--
ALTER TABLE `producer`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `user`
--
ALTER TABLE `user`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `drug`
--
ALTER TABLE `drug`
  ADD CONSTRAINT `drug_ibfk_1` FOREIGN KEY (`producer`) REFERENCES `producer` (`id`) ON DELETE CASCADE;

--
-- Constraints for table `drug_insurer`
--
ALTER TABLE `drug_insurer`
  ADD CONSTRAINT `drug_insurer_ibfk_1` FOREIGN KEY (`drug_id`) REFERENCES `drug` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `drug_insurer_ibfk_2` FOREIGN KEY (`insurer_id`) REFERENCES `insurer` (`id`) ON DELETE CASCADE;

--
-- Constraints for table `order_drug`
--
ALTER TABLE `order_drug`
  ADD CONSTRAINT `order_drug_ibfk_1` FOREIGN KEY (`drug_id`) REFERENCES `drug` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `order_drug_ibfk_2` FOREIGN KEY (`order_id`) REFERENCES `order_product` (`id`) ON DELETE CASCADE;

--
-- Constraints for table `order_product`
--
ALTER TABLE `order_product`
  ADD CONSTRAINT `id_user` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`);

--
-- Constraints for table `user`
--
ALTER TABLE `user`
  ADD CONSTRAINT `user_ibfk_1` FOREIGN KEY (`insurer_id`) REFERENCES `insurer` (`id`) ON DELETE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
