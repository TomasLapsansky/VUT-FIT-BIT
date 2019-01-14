<?php

namespace Models;


class OrderDrug extends DBtable {

    public static $tableName = "order_drug";

    public function getPrice($order_id) {

        $orderProducts = $this->getAll()->where('order_id', $order_id);

        $price = 0;

        foreach ($orderProducts as $orderProduct) {
            $price += $orderProduct->count * $orderProduct->price;
        }

        return $price;
    }

}