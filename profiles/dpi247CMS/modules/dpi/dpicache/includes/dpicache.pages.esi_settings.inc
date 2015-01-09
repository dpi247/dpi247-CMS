<?php
function dpicache_pages_form_esi_settings_form($form, &$form_state){
  $form=array();

  $form['debug'] = array(
    '#title' => t('Debug'),
    '#type' => 'fieldset',
  );
  $form['debug']['dpicache_disable_esi'] = array(
    '#title' => t('Disable Esi block for developpement'),
    '#type' => 'checkbox',
    '#default_value'=>variable_get('dpicache_disable_esi', FALSE),
  );

  $form['esi_ttl'] = array(
    '#title' => t('Esi TTL'),
    '#type' => 'fieldset',
  );

  $form['esi_ttl']['dpicache_esi_default_tll_block'] = array(
    '#title' => t('Esi default ttl for block'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value'=>variable_get('dpicache_esi_default_tll_block', FALSE),
  );

  $form['esi_ttl']['dpicache_esi_default_tll_creditentials'] = array(
    '#title' => t('Esi default ttl for creditentials of dpicache_.php'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value'=>variable_get('dpicache_esi_default_tll_creditentials', FALSE),
  );


  $form['varnish_purge'] = array(
    '#title' => t('Varnish purge method'),
    '#type' => 'fieldset',
  );
  $form['varnish_purge']['dpicache_varnish_purge_method'] = array(
    '#title' => t('Varnish url'),
    '#type' => 'radios',
    "#options"=>array(
      'direct'=>'Direct',
      'accumulator'=>'Acumulator',
    ),
    '#default_value'=>variable_get('dpicache_varnish_purge_method', array()),
    '#required' => TRUE,
  );
  $form['varnish_purge']['varnish_bantype'] = array(
    '#type' => 'select',
    '#title' => t('Varnish ban type'),
    '#default_value' => variable_get('varnish_bantype', VARNISH_DEFAULT_BANTYPE),
    '#description' => t('Select the type of varnish ban you wish to use. Ban lurker support requires you to add beresp.http.x-url and beresp.http.x-host entries to the response in vcl_fetch. THIS PARAMETER IS USELESS IF YOU USE dpi7.vcl BECAUSE WE FORCE PURGE TO BE BAN LURKER FRIENDLY'),
    '#options' => array(
      VARNISH_BANTYPE_NORMAL => 'Normal',
      VARNISH_BANTYPE_BANLURKER => 'Ban Lurker',
    ),
  );

  $form['varnish_config'] = array(
    '#title' => t('Varnish server connection'),
    '#type' => 'fieldset',
    '#collapsible'=>TRUE,
    '#collapsed'=>TRUE,
  );
  $form['varnish_config']['varnish_control_terminal'] = array(
    '#title' => t('Varnish url'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value'=>variable_get('varnish_control_terminal', '127.0.0.1:6082'),
  );
  $form['varnish_config']['varnish_control_key'] = array(
    '#title' => t('Varnish secret key'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value'=>variable_get('varnish_control_key', FALSE),
  );

  $form['varnish_config']['varnish_version'] = array(
    '#type' => 'select',
    '#title' => t('Varnish version'),
    '#default_value' => variable_get('varnish_version', 3),
    '#description' => t('Select your varnish version.'),
    '#options' => array(
      '2' => '2.0.x',
      '2.1' => '2.1.x',
      '3' => '3.x',
    ),
  );
  $form['varnish_config']['varnish_socket_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Varnish connection timeout (milliseconds)'),
    '#default_value' => variable_get('varnish_socket_timeout', VARNISH_DEFAULT_TIMEOUT),
    '#description' => t('If Varnish is running on a different server, you may need to increase this value.'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
   '#value'=>'Submit'
  );

  return $form;

}

function dpicache_pages_form_esi_settings_form_submit($form, & $form_state){
  //Check if we have a modification in use of esi
  if($form_state['values']['dpicache_disable_esi']!=variable_get('dpicache_disable_esi', FALSE)){
    drupal_set_message("We have to sync Esi roles and Drupal Roles");
    
    //if yes then sync permission between drupal roles and esi visibility
    if($form_state['values']['dpicache_disable_esi']){
      _dpicache_esi_sync_esi_visibility_to_drupal_roles();
    }
    else{
      _dpicache_esi_sync_drupal_roles_to_esi_visibility();
    }
  }  
  system_settings_form_submit($form, $form_state);
}

