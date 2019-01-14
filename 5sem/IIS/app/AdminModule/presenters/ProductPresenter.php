<?php

namespace App\AdminModule\Presenters;

use Nette\Application\UI;

final class ProductPresenter extends AdminBasePresenter {

    public function actionDefault() {
        if ($this->productService->getAllActive()->count() != 0) {
            $this->template->products = $this->productService->getAllActive();
        }
        $this->template->producers = $this->producerService;
    }

    public function actionDetail($id) {
        $this->template->id = $id;
        $this->template->product = $product = $this->productService->getByIDActive($id);
        if($product) {
            $this->template->producer = $this->producerService->getByID($product->producer)->name;
        }
    }

    public function actionAdd() {

    }

    public function actionEdit($id) {
        $product = $this->productService->getByIDActive($id);
        $this->template->product = $product;

        if($product) {
            $this['editForm']->setDefaults([
                'name' => $product->name,
                'count' => $product->count,
                'producer' => $product->producer,
                'price' => $product->price,
                'description' => $product->description,
                'id' => $product->id
            ]);
        }
    }

    protected function createComponentAddForm()
    {
        $producers = $this->producerService->getAll();

        $form = new UI\Form();
        $form->addText('name', 'Meno:')->setRequired();
        $form->addText('count', 'Počet:')->setRequired();
        $form->addSelect('producer', 'Výrobca:', $producers->fetchPairs('id', 'name'))->setRequired();
        $form->addText('price', 'Cena:')->setRequired();
        $form->addTextArea('description', 'Popis:')->setRequired();
        $form->addSubmit('add', 'Pridať');
        $form->onSuccess[] = [$this, 'addFormSucceeded'];
        return $form;
    }

    public function addFormSucceeded(UI\Form $form, $values)
    {
        $this->productService->insert([
            'name' => $values->name,
            'count' => $values->count,
            'producer' => $values->producer,
            'price' => $values->price,
            'description' => $values->description
        ]);

        $this->redirect('Product:');
    }

    protected function createComponentEditForm()
    {
        $producers = $this->producerService->getAll();

        $form = new UI\Form();
        $form->addText('name', 'Meno:')->setRequired();
        $form->addText('count', 'Počet:')->setRequired();
        $form->addSelect('producer', 'Výrobca:', $producers->fetchPairs('id', 'name'))->setRequired();        
        $form->addText('price', 'Cena:')->setRequired();
        $form->addTextArea('description', 'Popis:')->setRequired();
        $form->addSubmit('edit', 'Upraviť');
        $form->addHidden('id');
        $form->onSuccess[] = [$this, 'editFormSucceeded'];
        return $form;
    }

    public function editFormSucceeded(UI\Form $form, $values)
    {
        $product = $this->productService->getByID($values->id);

        $product->update([
            'name' => $values->name,
            'count' => $values->count,
            'producer' => $values->producer,
            'price' => $values->price,
            'description' => $values->description
        ]);

        $this->redirect('Product:');
    }

    public function handleDelete($id)
    {
        $this->productService->getByID($id)->update([
            'state' => 0
        ]);
    }

    protected function createComponentUploadImage()
    {
        $id = $this->getParameter('id');

        $form = new \Nette\Application\UI\Form;
        $form->addUpload('file', 'Image:');
        $form->addSubmit('Upload');
        $form->addHidden('id')->setDefaultValue($id);
        $form->onSuccess[] = function(\Nette\Application\UI\Form $form) {
            $values = $form->getValues();
            $path = "image/product/".$values->id;
            $values->file->move($path);
            $this->productService->getByID($values->id)->update([
                'image' => '/'.$path
            ]);
            $this->redirect('Product:');
        };

        return $form;
    }

}