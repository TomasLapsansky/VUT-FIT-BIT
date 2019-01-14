<?php

namespace App\Presenters;
use Nette\Application\UI;
use Nette\Forms\Form;
use Nette\Security\Passwords;


final class SettingsPresenter extends BasePresenter {

    protected function startUp() {

        parent::startup();

        if(!$this->user->isLoggedIn()) {
            $this->flashMessage("Je potrebne sa prihlasit", "warn");
            $this->redirect(":Login:");
        }

    }

    public function actionDefault() {

        $this->template->sys_user = $sys_user = $this->userService->getByID($this->user->getIdentity()->getId());

        $this['setUser']->setDefaults([
            'name' => $sys_user->name,
            'surname' => $sys_user->surname,
            'city' => $sys_user->city,
            'address' => $sys_user->address,
            'zip' => str_pad($sys_user->zip, 5, "0", STR_PAD_LEFT),
            'country' => $sys_user->country,
            'insurer' => $sys_user->insurer_id,
            'id' => $sys_user->id
        ]);

    }

    protected function createComponentSetUser()
    {
        $insurers = $this->insurerService->getAll();

        $form = new UI\Form();
        $form->addText('name', 'Meno:')->setRequired()
            ->addRule(Form::PATTERN, 'Meno moze obsahovat iba znaky', '[a-zA-Z]+');
        $form->addText('surname', 'Priezvisko:')->setRequired()
            ->addRule(Form::PATTERN, 'Priezvisko moze obsahovat iba znaky', '[a-zA-Z]+');
        $form->addPassword('password', 'Heslo:')->setRequired(false)
            ->addRule(Form::MIN_LENGTH, 'Heslo musi mat aspon 5 znakov', 5);
        $form->addText('city', 'City:')->setRequired()
            ->addRule(Form::PATTERN, 'Mesto moze obsahovat iba znaky', '[a-zA-Z]+');
        $form->addText('address', 'Adresa:')->setRequired();
        $form->addText('zip', 'PSČ:')->setRequired()
            ->addRule(Form::PATTERN, 'ZIP musi mat aspon 5 cisel', '([0-9]\s*){5}');
        $form->addText('country', 'Štát:')->setRequired()
            ->addRule(Form::PATTERN, 'Krajina moze obsahovat iba znaky', '[a-zA-Z]+');
        $form->addHidden('id');
        $form->addSelect('insurer', 'Poistovna:', $insurers->fetchPairs('id', 'name'))->setRequired();
        $form->addSubmit('edit', 'Uložiť');
        $form->onSuccess[] = [$this, 'setUserSucceeded'];
        return $form;
    }

    public function setUserSucceeded(UI\Form $form, $values) {

        $sys_user = $this->userService->getByID($values->id);

        $sys_user->update([
            'name' => $values->name,
            'surname' => $values->surname,
            'city' => $values->city,
            'address' => $values->address,
            'zip' => $values->zip,
            'country' => $values->country,
            'insurer_id' => $values->insurer
        ]);

        if($values->password) {
            $sys_user->update(['password' => Passwords::hash($values->password)]);
        }

        $this->flashMessage("Vas profil bol upraveny", "success");
    }

    protected function createComponentUploadAvatar()
    {
        $form = new \Nette\Application\UI\Form;
        $form->addUpload('file', 'Avatar:');
        $form->addSubmit('upload', 'Upload');
        $form->onSuccess[] = function(\Nette\Application\UI\Form $form) {
            $values = $form->getValues();
            $path = "image/avatar/".$this->user->getId();
            $values->file->move($path);
            $this->userService->getByID($this->user->getId())->update([
                'avatar' => '/'.$path
            ]);
            $this->redirect('Homepage:default');
        };

        return $form;
    }

}