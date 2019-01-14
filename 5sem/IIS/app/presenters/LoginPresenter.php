<?php

namespace App\Presenters;


use Nette\Application\UI;
use Nette\Security\AuthenticationException;

final class LoginPresenter extends BasePresenter {

    protected function startup() {

        parent::startup();

        if($this->user->isLoggedIn()) {
            $this->redirect("Homepage:");
        }
    }

    protected function createComponentLoginForm() {

        $form = new UI\Form;
        $form->addEmail("email", "Email:")->setRequired();
        $form->addPassword("password", "Heslo:")->setRequired();
        $form->addSubmit("login", "Prihlásiť");
        $form->onSuccess[] = [$this, "loginFormSucceeded"];

        return $form;
    }

    public function loginFormSucceeded(UI\Form $form, $values) {

        try {
            $this->user->login($values->email, $values->password);

            $this->flashMessage("Boli ste uspesne prihlaseny", "info");
            $this->redirect("Homepage:");

        } catch (AuthenticationException $e) {
            $this->flashMessage('Nespravne prihlasovacie meno alebo heslo', 'warning');
        }

    }

}