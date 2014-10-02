<?php
$plugin = array(
  'handler' => array(
    'class' => 'EnaRenderApiWrapper',
  ),
  'active_hooks'=>array(
    'hook_form_alter',
//    'preprocess_node'

    ),
);




class EnaRenderApiWrapper extends EnaRenderApi {
  
  
  public $default_values;
  public $unique_id;
  
  public function  get_uniqueid($form, $form_state){
    
    $menu_name = str_replace('_','-',$form['menu_name']['#default_value']);
    return $menu_name;
    
  }
  public function operation($a,$b){
    return $a+$b;
  }
  
  
  public function settings_form(){
    
    
    $form['wrapper_html'] =$this->_settings_form_details();

    return $form;
    
  }
  
  
  
  function _settings_form_details(){
    $default_value = $this->default_values;
    dsm($default_value,dv);
    
    $element = array(
      '#type' => 'fieldset',
      '#title' => t('Wrapper\'s HTML tag'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE
    );
    
    $element['wrapper_html_tag'] = array(
      '#type' => 'select',
      '#title' => t('Wrapper\'s Tag'),
      '#description' => t('The element(s)\'s wrapper HTML tag'),
      '#default_value' => $default_value['wrapper_html_tag'],
      '#options' => ena_theme_ui_ressources_tag_html(),
    );
    
    $element['wrapper_html_tag_bootstrap_class'] = array(
      '#type' => 'select',
      '#title' => t('Add a boostrap class to the wrapper\'s HTML tag'),
      '#description' => t('The class assigned to the element\'s wrapper HTML tag'),
      '#default_value' => $default_value['wrapper_html_tag_bootstrap_class'],
      '#options' => ena_theme_ui_ressources_wrapper_bootstrap_class(),
      '#states' => array(
        'invisible' => array(
          ':input[name="wrapper_html_tag"]' => array('value' => '- None -'),
        ),
      ),
    );
    
    $element['wrapper_html_tag_custom_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Add custom class(es) to the wrapper\'s HTML tag'),
      '#description' => t('The class assigned to the element\'s wrapper HTML tag'),
      '#default_value' => $default_value['wrapper_html_tag_custom_class'],
      '#states' => array(
        'invisible' => array(
          ':input[name="wrapper_html_tag"]' => array('value' => '- None -'),
        ),
      ),
    );

    return $element;
  }
  
}


