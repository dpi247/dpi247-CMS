<?php
/**
 * Implement the hook_form
 */
function dpisso_loginmanager_admin_settings_page_form(){
  $form=array();

  /**
   * The unique identifier of the dpi instance in the all SSO processes & modules
   */
  $form['dpisso_unitid']=array(
    '#type'=>'textfield',
    '#title'=>t('Unit ID'),
    '#description'=>t("The unique identifier of the dpi instance"),
    '#default_value'=> dpisso_authentication_api_get_unitid(),
  );

  return dpi_api_system_settings_form($form);
}