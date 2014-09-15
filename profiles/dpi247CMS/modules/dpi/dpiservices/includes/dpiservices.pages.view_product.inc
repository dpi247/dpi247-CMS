<?php

function dpiservices_pages_form_view_product_form($form, &$form_state){
  $form=array();
  $form['info']=array(
    '#markup'=>'Attention, All required fields must be completed !!! This is the settings for using correctly the varnish & ESI. You need to complete all fields if you want to don\'t have problem with other module',
  );
  return $form;
}