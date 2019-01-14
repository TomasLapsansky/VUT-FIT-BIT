<?php

namespace App\Presenters;


use Nette\Application\UI;
use Nette\Security\Passwords;

final class RegisterPresenter extends BasePresenter {

    protected function startup() {

        parent::startup();

        if($this->user->isLoggedIn()) {
            $this->flashMessage("Ste prihlaseny", "warn");
            $this->redirect("Homepage:");
        }
    }

    protected function createComponentRegisterForm() {

        $insurers = $this->insurerService->getAll();

        $form = new UI\Form();
        $form->addText("name", "Meno:")->setRequired();
        $form->addText("surname", "Priezvisko:")->setRequired();
        $form->addEmail("email", "Email:")->setRequired();
        $form->addPassword("password", "Heslo:")->setRequired();
        $form->addText("city", "Mesto:")->setRequired();
        $form->addText("address", "Adresa:")->setRequired();
        $form->addText("zip", "ZIP:")->setRequired();
        $form->addText("country", "Krajina:")->setRequired();
        $form->addSelect("insurer", "Poistovna:", $insurers->fetchPairs('id', 'name'));
        $form->addSubmit("register", "Registrovat");
        $form->onSuccess[] = [$this, "registerFormSuccedeed"];

        return $form;
    }

    public function registerFormSuccedeed(UI\Form $form, $values) {

        $this->userService->insert([
            'name' => $values->name,
            'surname' => $values->surname,
            'email' => $values->email,
            'password' => Passwords::hash($values->password),
            'city' => $values->city,
            'address' => $values->address,
            'zip' => $values->zip,
            'country' => $values->country,
            'insurer_id' => $values->insurer
        ]);

        $this->flashMessage("Boli ste uspesne zaregistrovany, mozete sa prihlasit", "info");
        $this->redirect("Login:");
    }

}