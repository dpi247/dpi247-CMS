<?php

function _dummycontenttypes_form_render_form($form,&$form_state){
  $conf=$form_state["build_info"]['args'][0]['conf'];




  foreach($conf['options'] as $key=>$item){
    if($item)
      $availlable_options[$key]=$key;
  }

  $form['#attached']['library'] = array(
    drupal_get_path('module', 'pipo') . '/plugins/ctools/content_types/dummycontenttype/dummycontenttype.inc',
  );


  $form['select']=array(
    '#type'=>'select',
    "#title"=>'Mode',
    "#options"=>$availlable_options,
    '#ajax' => array(
      'callback' => 'dummycontenttypes_ajax_replace_form_markup_callback',
      'wrapper' => 'dummycontent_markup_content',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );


  $form["markup"]=array(
    '#markup'=>'toto',
    '#prefix' => '<div id="dummycontent_markup_content">',
    '#suffix' => '</div>',
  );

  //if we have submitted the form with ajax
  if(isset($form_state['values']['select'])){

    $form["markup"]["#markup"]=$form_state['values']['select'];

  }



  return $form;
}



function dummycontenttypes_ajax_replace_form_markup_callback($form, $form_state) {

  return $form['markup'];

}
