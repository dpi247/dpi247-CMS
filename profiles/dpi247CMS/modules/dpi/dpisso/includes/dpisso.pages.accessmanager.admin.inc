<?php

function dpisso_accessmanager_admin_settings_page_form(){
  $form=array();
  
  $form['dpisso_accessmanager_uri']=array(
      '#type'=>'textfield',
      '#title'=>t('Access Manager URI'),
      '#description'=>t(" Do not indicate the scheme such as http:// neither the trailling slash. DPI will inform the Access Manager of any changes in the form bellow."),
      '#default_value'=>dpi_variable_get('dpisso_accessmanager_uri',''),
  );
  
  $form['dpisso_accessmanager_whitelist']=array(
      '#type'=>'textarea',
      '#title'=>t('Access Manager Whitelist'),
      '#description'=>t(" One url (or patern) per line. All theses urls will <strong>always</strong> be on free access."),
      '#default_value'=>dpi_variable_get('dpisso_accessmanager_whitelist',""),
  );  

  $form['dpisso_accessmanager_blacklist']=array(
      '#type'=>'textarea',
      '#title'=>t('Access Manager Blacklist'),
      '#description'=>t(" One url (or patern) per line. All theses urls will <strong>never</strong> be on free access."),
      '#default_value'=>dpi_variable_get('dpisso_accessmanager_blacklist',""),
  );
  
  return dpi_api_system_settings_form($form);
 
}