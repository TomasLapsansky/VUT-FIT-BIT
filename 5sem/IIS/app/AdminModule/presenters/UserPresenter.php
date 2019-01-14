<?php

namespace App\AdminModule\Presenters;


use Nette\Application\UI;
use Nette\Security\Passwords;

final class UserPresenter extends AdminBasePresenter {

    public function actionDefault() {
        if ($this->userService->count() != 0) {
            $this->template->users = $this->userService->getAllActive();
        }
    }

    public function actionDetail($id) {
        $this->template->id = $id;
        $this->template->sys_user = $sys_user = $this->userService->getByIDActive($id);
        if($sys_user) {
            $this->template->insurer = $this->insurerService->getByID($sys_user->insurer_id);
        }

    }

    public function actionAdd() {
        
    }

    public function actionEdit($id) {
        $sys_user = $this->userService->getByIDActive($id);
        $this->template->sys_user = $sys_user;

        $this['editForm']->setDefaults([
            'name' => $sys_user->name,
            'surname' => $sys_user->surname,
            'email' => $sys_user->email,
            //'role' => $sys_user->role,
            'city' => $sys_user->city,
            'address' => $sys_user->address,
            'zip' => $sys_user->zip,
            'country' => $sys_user->country,
            'insurer' => $sys_user->insurer_id,
            'id' => $sys_user->id
        ]);

        if($sys_user->role == 'user') {
            $this['editForm']->setDefaults(['role' => 0]);
        } elseif($sys_user->role == 'admin') {
            $this['editForm']->setDefaults(['role' => 1]);
        }
    }

    protected function createComponentAddForm()
    {
        $insurers = $this->insurerService->getAllActive();

        $form = new UI\Form();
        $form->addText('name', 'Meno:')->setRequired();
        $form->addText('surname', 'Priezvisko:')->setRequired();
        $form->addEmail('email', 'Email:')->setRequired();
        $form->addPassword('password', 'Heslo:')->setRequired();
        $form->addSelect('role', 'Rola:', [
            'user',
            'admin'
        ])->setRequired();
        $form->addText('city', 'Mesto:')->setRequired();
        $form->addText('address', 'Adresa:')->setRequired();
        $form->addText('zip', 'PSČ:')->setRequired();
        $form->addText('country', 'Štát:')->setRequired();
        $form->addSelect('insurer', 'Poisťovňa:', $insurers->fetchPairs('id', 'name'))->setRequired();
        $form->addSubmit('add', 'Pridať');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        return $form;
    }

    public function addFormSucceeded(UI\Form $form, $values)
    {
        if($values->role == 'user') {
            $role = 'user';
        } elseif($values->role == 'admin') {
            $role = 'admin';
        }

        $this->userService->insert([
            'name' => $values->name,
            'surname' => $values->surname,
            'email' => $values->email,
            'password' => $values->password,
            'role' => $role,
            'city' => $values->city,
            'address' => $values->address,
            'zip' => $values->zip,
            'country' => $values->country,
            'insurer_id' => $values->insurer
        ]);

        $this->redirect('User:');
    }

    protected function createComponentEditForm()
    {
        $insurers = $this->insurerService->getAll();

        $form = new UI\Form();
        $form->addText('name', 'Meno:')->setRequired();
        $form->addText('surname', 'Priezvisko:')->setRequired();
        $form->addEmail('email', 'Email:')->setRequired();
        $form->addPassword('password', 'Heslo:');
        $form->addSelect('role', 'Role:', [
            'user',
            'admin'
        ])->setRequired();
        $form->addText('city', 'Mesto:')->setRequired();
        $form->addText('address', 'Adresa:')->setRequired();
        $form->addText('zip', 'PSČ:')->setRequired();
        $form->addText('country', 'Štát:')->setRequired();
        $form->addSelect('insurer', 'Poistovna:', $insurers->fetchPairs('id', 'name'))->setRequired();
        $form->addSubmit('edit', 'Upraviť');
        $form->addHidden('id');
        $form->onSuccess[] = [$this, 'editFormSucceeded'];
        return $form;
    }

    public function editFormSucceeded(UI\Form $form, $values)
    {
        $sys_user = $this->userService->getByID($values->id);

        $sys_user->update([
            'name' => $values->name,
            'surname' => $values->surname,
            'email' => $values->email,
            'city' => $values->city,
            'address' => $values->address,
            'zip' => $values->zip,
            'country' => $values->country,
            'insurer_id' => $values->insurer
        ]);

        if($values->role == 'user') {
            $sys_user->update(['role' => 'user']);
        } elseif($values->role == 'admin') {
            $sys_user->update(['role' => 'admin']);
        }

        if($values->password) {
            $sys_user->update(['password' => Passwords::hash($values->password)]);
        }

        $this->redirect('User:');
    }

    public function handleDelete($id)
    {
        $this->userService->getByID($id)->update([
            'state' => 0
        ]);

        $this->redirect("User:");
    }

}