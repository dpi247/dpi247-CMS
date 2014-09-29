<?php

function dpisso_loginmanager_admin_settings_page_form(){
  $form=array();

  $form['dpisso_loginmanager_domain']=array(
    '#type'=>'textfield',
    '#title'=>t('Login Manager Domain'),
    '#description'=>t(" Do not indicate the scheme such as http:// neither the trailling slash"),
    '#default_value'=>dpi_variable_get('dpisso_loginmanager_domain'),
  );
  $form['dpisso_unitid']=array(
    '#type'=>'textfield',
    '#title'=>t('Unit ID'),
    '#description'=>t("The unique identifier of the dpi instance"),
    '#default_value'=> dpisso_authentication_api_get_unitid(),
  );

  return dpi_api_system_settings_form($form);
}