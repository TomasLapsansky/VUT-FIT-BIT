<?php

namespace App\Presenters;

use Models\Drug;
use Models\DrugInsurer;
use Models\Insurer;
use Models\Order;
use Models\OrderDrug;
use Models\Producer;
use Models\User;
use Nette;

class BasePresenter extends Nette\Application\UI\Presenter
{
    /** @var Nette\Database\Context @inject */
    public $database;

    /** @var User @inject */
    public $userService;

    /** @var Drug @inject */
    public $productService;

    /** @var Producer @inject */
    public $producerService;

    /** @var Order @inject */
    public $orderService;

    /** @var Insurer @inject */
    public $insurerService;

    /** @var OrderDrug @inject */
    public $orderDrugService;

    /** @var DrugInsurer @inject */
    public $drugInsurerService;

    protected function startup() {

        parent::startup();

        $this->template->user = $this->user;
    }

    public function handleLogOut() {
        $this->user->logout();
        $this->flashMessage("Boli ste uspesne odhlaseny", "info");
        $this->redirect("Homepage:");
    }
}