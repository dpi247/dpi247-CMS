<?php
$plugin = array(
  'handler' => array(
    'class' => 'EnaRenderOperationWrapper',
  ),
  //Sert aussi a documenter la structure a laquelle s'attend le enarenderapi plugin
  'default_values'=>array(
    'wrapper_html_tag'=>'h2',
    'wrapper_html_tag_custom_class'=>'TATA',
    )
  ,
 
);



class EnaRenderOperationWrapper extends EnaRenderOperation {
  
  
  public $default_values;
  public $unique_id;
  
  
  public function settings_form(){
    
    
  $default_value = $this->get_values();
    
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
  
  
  public function render_operation($render_array){
    
    $default_values=$this->get_values($plugin_name, $unique_id);
   
    return array(
     '#value' =>array($render_array,$render_array,$render_array,$render_array,$render_array,$render_array,$render_array),
      '#tag' => $default_values['wrapper_html_tag'],
      '#attributes' => array(
      'class' => $default_values['wrapper_html_tag_custom_class'],
      ),
      '#theme_wrappers' => array(0 => 'ena_render_wrapper'),
    );
    
  }
  
}


