<?php

namespace App\Presenters;


use Joseki\Application\Responses\PdfResponse;

final class OrderPresenter extends BasePresenter
{

    protected function startup()
    {
        parent::startup();

        if (!$this->user->isLoggedIn()) {
            $this->redirect("Login:");
        }
    }

    public function renderDefault()
    {
        $this->template->orders = $this->orderService->getAllActive()->where('user_id', $this->user->getId());
        $this->template->orderDrugService = $this->orderDrugService;
    }

    public function renderDetail($id)
    {
        $this->template->order = $order = $this->orderService->getByID($id);

        if ($this->user->getId() != $order->user_id) {
            $this->redirect("Order:");
            return;
        }

        $orderDrugs = $this->orderDrugService->getAll()->where('order_id', $id);
        $template_products = [];

        foreach ($orderDrugs as $orderDrug) {
            $temp_product = $this->productService->getByID($orderDrug->drug_id);
            $template_products[$orderDrug->drug_id] = array($temp_product, $orderDrug);
        }

        $this->template->products = $template_products;
        $this->template->userService = $this->userService;
    }

    public function handleCreatePDF($id)
    {
        $template = $this->createTemplate();
        $template->setFile(__DIR__ . "/../templates/Order/pdf.latte");

        /* PDF values */
        $template->order = $order = $this->orderService->getByID($id);
        $orderDrugs = $this->orderDrugService->getAll()->where('order_id', $id);
        $template_products = [];

        foreach ($orderDrugs as $orderDrug) {
            $temp_product = $this->productService->getByID($orderDrug->drug_id);
            $template_products[$orderDrug->drug_id] = array($temp_product, $orderDrug);
        }

        $template->products = $template_products;
        $template->userService = $this->userService;
        /* end PDF values */

        $pdf = new PdfResponse($template);

        // optional
        $pdf->documentTitle = date("Y-m-d") . " order {$order->id}"; // creates filename 2012-06-30-my-super-title.pdf
        $pdf->pageFormat = "A4"; // wide format
        $pdf->getMPDF()->setFooter("|© Lekaren is|"); // footer

        $pdf->setSaveMode(PdfResponse::DOWNLOAD);
        // do something with $pdf
        $this->sendResponse($pdf);
    }

}