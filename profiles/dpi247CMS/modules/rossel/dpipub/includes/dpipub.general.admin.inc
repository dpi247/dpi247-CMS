<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Settings form.
 */
function pub_page_general_admin_form($form_state) {
  $form = array();

  $form['pub_website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website name'),
    '#description' => t('The website name used to the ads requests.'),
    '#default_value' => variable_get('pub_website', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['pub_webserv'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the webservice returning the ads'),
    '#default_value' => variable_get('pub_webserv', 'http://172.21.132.180:8080/Wally-Tags/rest/wally/script/detail'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['pub_header'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the webservice returning the header javascript content'),
    '#default_value' => variable_get('pub_header', ''),
    '#size' => 80,
    '#maxlength' => 512,
  );
  
  return system_settings_form($form);
}
