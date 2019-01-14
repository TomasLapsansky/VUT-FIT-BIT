<?php

namespace Models;


class User extends DBtable {

    public static $tableName = "user";

    public function getByEmail($email) {
        return $this->getTable()->where('email', $email);
    }

}