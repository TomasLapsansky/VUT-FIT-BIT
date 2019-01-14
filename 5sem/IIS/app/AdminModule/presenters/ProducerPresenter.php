<?php

namespace App\AdminModule\Presenters;

use App\Presenters\BasePresenter;
use Models\Producer;
use Nette;
use Nette\Application\UI;

final class ProducerPresenter extends AdminBasePresenter {
    
    /** @var Producer @inject */
    public $producerService;

    public function actionDefault() {
        if ($this->producerService->getAllActive()->count() != 0) {
            $this->template->producers = $this->producerService->getAllActive();
        }
    }

    public function actionDetail($id) {
        $this->template->id = $id;
        $this->template->producer = $producer = $this->producerService->getByIDActive($id);
        if($producer) {
            $this->template->products = $this->productService->getAllActive()->where('producer', $id);
        }
    }

    public function actionEdit($id) {
        $producer = $this->producerService->getByIDActive($id);
        $this->template->producer = $producer;

        $this['editForm']->setDefaults([
            'name' => $producer->name,
            'time' => $producer->time_delivery,
            'id' => $producer->id
        ]);

    }

    protected function createComponentEditForm()
    {
        $form = new UI\Form();
        $form->addHidden("id");        
        $form->addText('name', 'Názov výrobcu:')->setRequired();
        $form->addInteger('time', 'Čas dodania:')->setRequired();
        $form->addSubmit("edit", "Uložiť");
        $form->onSuccess[] = [$this, 'editFormSucceeded'];
        return $form;
    }

    public function editFormSucceeded(UI\Form $form, $values)
    {
        $producer = $this->producerService->getByID($values->id);

        $producer->update([
            'name' => $values->name,
            'time_delivery' => $values->time
        ]);

        $this->redirect('Producer:');
    }

    protected function createComponentAddForm()
    {
        $form = new UI\Form(); 
        $form->addText('name', 'Názov výrobcu:')->setRequired();
        $form->addInteger('time', 'Čas dodania:')->setRequired();
        $form->addSubmit("add", "Pridať nový");
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        return $form;
    }

    public function addFormSucceeded(UI\Form $form, $values)
    {
        $this->producerService->insert([
            'name' => $values->name,
            'time_delivery' => $values->time
        ]);

        $this->redirect('Producer:');
    }

    public function handleDelete($id)
    {
        $this->producerService->getByID($id)->update([
            'state' => 0
        ]);
    }

}