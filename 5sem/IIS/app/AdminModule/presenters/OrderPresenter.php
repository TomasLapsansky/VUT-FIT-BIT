<?php

namespace App\AdminModule\Presenters;

use Nette\Application\UI;

final class OrderPresenter extends AdminBasePresenter {

    public function actionDefault() {
        if ($this->orderService->getAllActive()->count() != 0) {
            $this->template->orders = $this->orderService->getAllActive();
        }
    }

    public function actionDetail($id) {
        $this->template->id = $id;
        $this->template->order = $order = $this->orderService->getByIDActive($id);
        if($order) {
            $this->template->sys_user = $this->userService->getByID($order->user_id);

            $this->template->productsOrder = $this->orderDrugService->getAll()->where('order_id', $id);
            $this->template->products = $this->productService;
        }
    }

    public function actionAdd() {
        $this->redirect("Order:");
    }

    public function actionEdit($id) {
        $order = $this->orderService->getByID($id);
        $this->template->order = $order;

        $this['editForm']->setDefaults([
            'status' => $order->status,
            'city' => $order->city,
            'zip' => $order->zip,
            'address' => $order->address,
            'id' => $order->id
        ]);

    }

    protected function createComponentAddForm()
    {
        $form = new UI\Form();
        $form->addText('user_id', 'Uzivatel ID:')->setRequired();
        $form->addText('city', 'Mesto:')->setRequired();
        $form->addText('zip', 'PSČ:')->setRequired();
        $form->addText('address', 'Adresa:')->setRequired();
        $form->addSubmit('add', 'Pridať');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        return $form;
    }

    public function addFormSucceeded(UI\Form $form, $values)
    {
        $sys_user = $this->userService->getByID($values->user_id);

        if($sys_user) {

            $this->userService->insert([
                'user_id' => $values->user_id,
                'status' => 'created',
                'city' => $values->city,
                'zip' => $values->zip,
                'address' => $values->address
            ]);

            $this->redirect('User:');

        } else {
            $this->flashMessage("Nespravne uzivatelske ID", "error");
        }
    }

    protected function createComponentEditForm()
    {
        //$insurers = $this->insurerService->getAll();

        $form = new UI\Form();
        $form->addText('status', 'Status:')->setRequired();
        $form->addText('city', 'Mesto:')->setRequired();
        $form->addText('zip', 'PSČ:')->setRequired();
        $form->addText('address', 'Adresa:')->setRequired();
        //$form->addSelect('insurer', 'Poistovna:', $insurers->fetchPairs('id', 'name'))->setRequired();
        $form->addSubmit('edit', 'Zmeniť');
        $form->addHidden('id');
        $form->onSuccess[] = [$this, 'editFormSucceeded'];
        return $form;
    }

    public function editFormSucceeded(UI\Form $form, $values)
    {
        $sys_user = $this->orderService->getByID($values->id);

        $sys_user->update([
            'status' => $values->status,
            'city' => $values->city,
            'address' => $values->address,
            'zip' => $values->zip,
        ]);

        $this->redirect('Order:');
    }

    public function handleDelete($id)
    {
        $this->orderService->getByID($id)->update([
            'state' => 0
        ]);

        $this->redirect("Order:");
    }
}