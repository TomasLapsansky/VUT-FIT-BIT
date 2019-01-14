<?php

namespace Models;


class Drug extends DBtable {

    public static $tableName = "drug";

    public function sortByTimeNewest() {
        return $this->getAllActive()->order('Time_created DESC')->limit(4);
    }

}