<?php
$plugin = array(
  'handler' => array(
    'class' => 'EnaRenderApiElement',
  ),

  'active_hooks'=>array(
    'hook_form_alter',
//    'preprocess_node'

    ),
);




class EnaRenderApiElement extends EnaRenderApi {
  
  public function operation($a,$b){
    return $a-$b;
  }
  
}

