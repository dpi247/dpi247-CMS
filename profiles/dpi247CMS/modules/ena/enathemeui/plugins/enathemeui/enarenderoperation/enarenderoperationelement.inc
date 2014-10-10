<?php
$plugin = array(
  'handler' => array(
    'class' => 'EnaRenderOperationElement',
  ),

  'active_hooks'=>array(
    'hook_form_alter',
//    'preprocess_node'

    ),
);




class EnaRenderOperationElement extends EnaRenderOperation {
  
  public function operation($a,$b){
    return $a-$b;
  }
  
}

