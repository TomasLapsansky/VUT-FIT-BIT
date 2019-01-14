<?php

namespace App\Presenters;

final class HomepagePresenter extends BasePresenter
{

    public function renderDefault() {
        $this->template->products = $this->productService->sortByTimeNewest();
    }

}
