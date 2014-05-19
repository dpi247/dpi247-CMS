<?php



function dpicache_pages_form_cache_clear_form(&$form,&$form_state){
  $form=array();
  dsm(ok);
  $form['info']=array(
    '#markup'=>'hello'
  );

  $form['name'] = array(
    '#title' => t('Your full name'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );

  return $form;

}