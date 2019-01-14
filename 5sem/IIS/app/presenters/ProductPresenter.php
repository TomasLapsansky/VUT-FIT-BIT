<?php

namespace App\Presenters;

use Nette\Application\UI;

final class ProductPresenter extends BasePresenter
{

    public function renderDefault() {

        $this->template->products = $this->productService->getAllActive();
    }

    public function renderDetail($id) {

        $this->template->id = $id;
        $this->template->product = $product = $this->productService->getByIDActive($id);

        if($product) {
            $this->template->producer = $this->producerService->getByID($product->producer);

            $this['cartCount']->setDefaults([
                'id' => $id,
                'count' => 1
            ]);
        }
    }

    public function cartCountSucceeded(UI\Form $form, $values) {

        if(!$this->user->isLoggedIn()) {
            $this->flashMessage("Musite byt prihlaseny");
            $this->redirect("Login:");
            return;
        }

        if($values->count > 0) {

            if (!isset($_SESSION['cart'])) {
                $_SESSION['cart'] = array();
            }

            /* duplicity check */
            foreach ($_SESSION['cart'] as $product) {
                if($product['productId'] == $values->id) {
                    return;
                }
            }

            $bag = array(
                "productId" => $values->id,
                "quantity"  => $values->count
            );
            $_SESSION['cart'][] = $bag;

            $this->flashMessage("Produkt bol uspesne pridany do kosika", "info");

        } else {
            $this->flashMessage("Mnozstvo musi byt aspon 1", "warning");
            return;
        }
    }

    protected function createComponentCartCount() {

        $form = new UI\Form();
        $form->addInteger("count", "Počet:")->setRequired();
        $form->addHidden("id");
        $form->addSubmit("addToCart", "Pridať do košíku");
        $form->onSuccess[] = [$this, "cartCountSucceeded"];

        return $form;
    }
}