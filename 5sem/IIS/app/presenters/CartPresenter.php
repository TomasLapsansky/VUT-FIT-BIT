<?php

namespace App\Presenters;

use Nette\Application\UI;


final class CartPresenter extends BasePresenter
{

    protected function startup()
    {
        parent::startup();

        if(!$this->user->isLoggedIn()) {
            $this->flashMessage("Musite byt prihlaseny");
            $this->redirect("Login:");
            return;
        }
    }

    public function renderDefault()
    {
        if (isset($_SESSION['cart'])) {
            if(count($_SESSION['cart']) == 0) {
                unset($this->template->products);
            } else {
                $cart_products = $_SESSION['cart'];

                $template_products = [];

                foreach ($cart_products as $product) {
                    $temp_product = $this->productService->getByIDActive($product['productId']);
                    if ($temp_product) {
                        $template_products[$product['productId']] = array($temp_product, $product['quantity']);
                    }
                }

                $this->template->products = $template_products;
            }
        }

    }

    public function actionSummary()
    {
        if(isset($_SESSION['cart'])) {

            $cart_products = $_SESSION['cart'];

            $template_products = [];

            foreach ($cart_products as $product) {
                $temp_product = $this->productService->getByIDActive($product['productId']);
                if ($temp_product) {
                    $template_products[$product['productId']] = array($temp_product, $product['quantity']);
                }
            }

            $this->template->products = $template_products;
            $this->template->sys_user = $this->userService->getByID($this->user->getId());
        }
    }

    protected function createComponentSummaryForm()
    {

        $form = new UI\Form();
        $form->addSubmit("summary", "Pokračovať");
        $form->onSuccess[] = [$this, "sendToSummarySuccedeed"];

        return $form;
    }

    public function sendToSummarySuccedeed(UI\Form $form, $values)
    {
        foreach ($_SESSION['cart'] as $key => $cart_product) {
            $product = $this->productService->getByIDActive($cart_product['productId']);
            if($product) {
                $prod_count = $form->getHttpData($form::DATA_TEXT, "{$product->id}");
                if($prod_count <= 0) {
                    $this->flashMessage("Mnozstvo produktov musi byt aspon 1", "error");
                    return;
                }
                $_SESSION['cart'][$key]['quantity'] = $prod_count;

            }
        }

        $this->redirect("Cart:summary");
    }

    public function handleDelete($id) {
        foreach ($_SESSION['cart'] as $key => $cart_product) {
            if($_SESSION['cart'][$key]['productId'] == $id) {
                unset($_SESSION['cart'][$key]);
                $this->redirect("Cart:");
                $this->flashMessage("Produkt bol vyhodeny z kosika", "info");
                return;
            }
        }
    }

    public function handleBuy() {
        if(isset($_SESSION['cart'])) {

            $cart_products = $_SESSION['cart'];

            /* Capacity check and sub */
            foreach ($cart_products as $cart_product) {
                $product = $this->productService->getByID($cart_product['productId']);
                if($product->count < $cart_product['quantity']) {
                    $this->flashMessage("V kosiku je viac produktov ako mame naskladnenych, prepacte", "error");
                    return;
                } else {
                    $product->update([
                        'count' => $product->count - $cart_product['quantity']
                    ]);
                }
            }

            $sys_user = $this->userService->getByID($this->user->getId());
            $new_order = $this->orderService->insert([
                'city' => $sys_user->city,
                'zip' => $sys_user->zip,
                'address' => $sys_user->address,
                'user_id' => $sys_user->id,
                'status' => 'waiting'
            ]);

            foreach ($cart_products as $product) {
                $this->orderDrugService->insert([
                    'count' => $product['quantity'],
                    'price' => $this->productService->getByID($product['productId'])->price,
                    'drug_id' => $product['productId'],
                    'order_id' => $new_order->id
                ]);
            }

            $this->flashMessage("Vasa objednavka bola uspesne spracovana", "info");
            unset($_SESSION['cart']);
            $this->redirect("Order:");
        }
    }

}