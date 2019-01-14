<?php

namespace App\AdminModule\Presenters;

use App\Presenters\BasePresenter;
use Nette;

class AdminBasePresenter extends BasePresenter
{
    protected function startUp() {

        parent::startup();

        if(!$this->user->isInRole('admin')) {
            $this->redirect(":Login:");
        }

    }
}