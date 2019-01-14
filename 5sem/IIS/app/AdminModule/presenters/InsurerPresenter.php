<?php

namespace App\AdminModule\Presenters;

use App\Presenters\BasePresenter;
use Models\Insurer;
use Nette;
use Nette\Application\UI;

final class InsurerPresenter extends AdminBasePresenter {

    /** @var Insurer @inject */
    public $insurerService;

    public function actionDefault() {
        if ($this->insurerService->getAllActive()->count() != 0) {
            $this->template->insurers = $this->insurerService->getAllActive();
        }
    }

    public function actionDetail($id) {
        $this->template->id = $id;
        $this->template->insurer = $this->insurerService->getByIDActive($id);

        $this->template->insuredProducts = $this->drugInsurerService->getAll()->where('insurer_id', $id);
        $this->template->products = $this->productService;
    }

    public function actionAddDrug($id) {
        $this->template->id = $id;
    }

    public function actionEdit($id) {
        $insurer = $this->insurerService->getByID($id);
        $this->template->insurer = $insurer;

        $this['editForm']->setDefaults([
            'name' => $insurer->name,
            'id' => $insurer->id
        ]);

    }

    protected function createComponentEditForm()
    {
        $form = new UI\Form();
        $form->addHidden("id");        
        $form->addText('name', 'Názov poisťovne:')->setRequired();
        $form->addSubmit("edit", "Uložiť");
        $form->onSuccess[] = [$this, 'editFormSucceeded'];
        return $form;
    }

    public function editFormSucceeded(UI\Form $form, $values)
    {
        $insurer = $this->insurerService->getByID($values->id);

        $insurer->update([
            'name' => $values->name,
        ]);

        $this->redirect('Insurer:');
    }

    protected function createComponentAddForm()
    {
        $form = new UI\Form();    
        $form->addText('name', 'Názov poisťovne:')->setRequired();
        $form->addSubmit("add", "Pridať");
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        return $form;
    }

    protected function createComponentAddDrugForm()
    {
        $id = $this->getParameter('id');
        $drug = $this->productService->getAllActive();

        $form = new UI\Form();
        $form->addSelect('drug_id', 'Liek:', $drug->fetchPairs('id', 'name'))->setRequired();
        $form->addText('price', "Cena:")->setRequired()->setDefaultValue(0)
            ->addRule(Nette\Forms\Form::INTEGER, "Zľava musí byť celé číslo")->addRule(Nette\Forms\Form::MIN, "Zľava musí byť minimálne 1", 1);
        $form->addSubmit("add", "Pridať");
        $form->addHidden('insurer_id')->setDefaultValue($id);
        $form->onSuccess[] = [$this, 'addDrugFormSucceeded'];
        return $form;
    }

    public function addDrugFormSucceeded(UI\Form $form, $values) {

        $this->drugInsurerService->insert([
            'insurer_id' => $values->insurer_id,
            'drug_id' => $values->drug_id,
            'price' => $values->price
        ]);

        $this->redirect("Insurer:");

    }

    public function addFormSucceeded(UI\Form $form, $values)
    {
        $this->insurerService->insert([
            'name' => $values->name,
        ]);

        $this->redirect('Insurer:');
    }

    public function handleDelete($id)
    {
        $this->insurerService->getByID($id)->update([
            'state' => 0
        ]);
    }

}