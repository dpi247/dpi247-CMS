<?php
/**
 * @file
 * Configuration file of the Google Tag Manager
 * 
 * @author  Maximilien Richard <mri@audaxis.com>
 * @package Google Tag Manager
 * @version  1.0
 */

/**
 * Implementation of hook_admin_settings()
 * for configuring the module (form).
 */
function googletagmanager_admin_settings_form($form, &$form_state){
  $form = array();

  $form['account'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('General settings'),
    '#collapsible' => FALSE 
  );

  $form['account']['googletagmanager_account'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Web Property ID'),
    '#default_value' => variable_get('googletagmanager_account', 'GTM-'),
    '#size'          => 15,
    '#maxlength'     => 20,
    '#required'      => TRUE,
    '#description'   => t('This ID is unique to each site you want to track separately, and is in the form of GTM-XXXX. To get a Web Property ID, <a href="@tagmanager">register your site with Google Tag Manager</a>, or if you already have registered your site, go to your Google Tag Manager Settings page to see the ID next to every site profile. <a href="@webpropertyid">Find more information in the documentation</a>.', array(
        '@tagmanager'    => 'http://www.google.com/tagmanager/',
        '@webpropertyid' => url(
          'https://developers.google.com/tag-manager/quickstart', 
          array('fragment' => 'webProperty')
        ) 
      )
    )
  );

  $form['page_vis_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Page specific tracking settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $options = array(
    t('Add to every page except the listed pages.'),
    t('Add to the listed pages only.')
  );
  $visibility = variable_get('googletagmanager_visibility', 0);
  $form['page_vis_settings']['googletagmanager_visibility'] = array(
    '#type'          => 'radios',
    '#title'         => t('Add tracking to specific pages'),
    '#options'       => $options,
    '#default_value' => $visibility,
  );

  $pages        = variable_get('googletagmanager_pages', GOOGLEANALYTICS_PAGES);
  $description  = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.",
    array(
      '%blog'          => 'blog',
      '%blog-wildcard' => 'blog/*', 
      '%front'         => '<front>'
    )
  );
  $form['page_vis_settings']['googletagmanager_pages'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Pages'),
    '#default_value' => $pages,
    '#description'   => $description,
    '#wysiwyg'       => FALSE,
    '#rows'          => 10,
  );



  $token_enabled                = module_exists('token');
  $googletagmanager_custom_vars = variable_get('googletagmanager_custom_var', array());
  $nbLignes                     = 1;
  $nbLinesUsed                  = 1;
  $nbMin                        = 11;

  // Custom variables
  $form['googletagmanager_custom_var'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Custom variables'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
    '#description'  => t('You can add Google Tag Manager <a href="@custom_var_documentation">Custom Variables</a> here. These will be added to every page that Google Tag Manager code appears on.', array(
        '@custom_var_documentation' => 'https://developers.google.com/tag-manager/devguide#datalayer' 
      )
    ),
    '#theme' => 'googletagmanager_admin_custom_var_table',
    '#tree'  => TRUE,

    // Set up the wrapper so that AJAX will be able to replace the fieldset.
    '#prefix' => '<div id="wrapper-gtm-custom-vars">',
    '#suffix' => '</div>',
  );

  if ($token_enabled) {
    $types = array('user','node','taxonomy','vocabulary');
    drupal_alter('gtm_tokentypes', $types );
  }

  // Build the fieldset with the proper number of names. We'll use
  // $form_state['nb_lines'] to determine the number of lines to build.
  if (empty($form_state['nb_lines'])) {
    $form_state['nb_lines'] = (!empty($googletagmanager_custom_vars['slots'])) ? count($googletagmanager_custom_vars['slots']) : 1;
  }
  // dsm($googletagmanager_custom_vars);
  // dsm($form_state);

  for ($i = 1; $i <= $form_state['nb_lines']; $i++) {
    $form['googletagmanager_custom_var']['slots'][$i]['slot'] = array(
      '#default_value' => $i,
      '#description'   => t('Number'),
      '#disabled'      => TRUE,
      '#size'          => 1,
      '#type'          => 'textfield',
      '#value'         => $i 
    );

    $form['googletagmanager_custom_var']['slots'][$i]['name'] = array(
      '#default_value' => (!empty($googletagmanager_custom_vars['slots'][$i]['name'])) ? $googletagmanager_custom_vars['slots'][$i]['name'] : '',
      '#description'   => t('The custom variable name.'),
      '#maxlength'     => 255,
      '#size'          => 20,
      '#type'          => 'textfield' 
    );

    $form['googletagmanager_custom_var']['slots'][$i]['value'] = array(
      '#default_value' => (!empty($googletagmanager_custom_vars['slots'][$i]['value'])) ? $googletagmanager_custom_vars['slots'][$i]['value'] : '',
      '#description'   => t('The custom variable value.'),
      '#maxlength'     => 255,
      '#type'          => 'textfield' 
    );

    $form['googletagmanager_custom_var']['slots'][$i]['delete'] = array(
      '#type' => 'checkbox',
      '#title' => 'Delete',
    );

    if ($token_enabled) {
      $form['googletagmanager_custom_var']['slots'][$i]['name']['#element_validate'][]  = 'token_element_validate';
      $form['googletagmanager_custom_var']['slots'][$i]['value']['#element_validate'][] = 'token_element_validate';
      
      foreach ($types as $type) {
        $form['googletagmanager_custom_var']['slots'][$i]['name']['#token_types'][]  = $type;
        $form['googletagmanager_custom_var']['slots'][$i]['value']['#token_types'][] = $type;
      }
    }
  }

  $form['googletagmanager_custom_var']['add_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add more'),
    '#submit' => array('_gtm_ajax_one'),
    '#ajax' => array(
      'callback' => '_gtm_ajax_callback',
      'wrapper' => 'wrapper-gtm-custom-vars',
      'method'   => 'replace',
      'effect'   => 'fade'
    ),
  );


  $form['googletagmanager_custom_var']['token_fieldset'] = array(
   '#type' => 'fieldset',
   '#title' => t('Available tokens'),
   '#collapsible' => TRUE,
   '#collapsed' => TRUE,
  );

  if ($token_enabled) {
    $form['googletagmanager_custom_var']['token_fieldset']['googletagmanager_custom_var_token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => $types, // The token types that have specific context. Can be multiple token types like 'term' and/or 'user'
      '#global_types' => TRUE, // A boolean TRUE or FALSE whether to include 'global' context tokens like [current-user:*] or [site:*]. Defaults to TRUE.
      '#click_insert' => TRUE, // A boolean whether to include the 'Click this token to insert in into the the focused textfield' JavaScript functionality. Defaults to TRUE.
    );
  } else {
    $form ['googletagmanager_custom_var']['token_fieldset']['googletagmanager_custom_var_token_tree'] = array(
        '#type' => 'item',
        '#description' => t( 'Install / Enable the <a href="@custom_var_documentation">Token Module</a> in order to use the tokens.', array(
          '@custom_var_documentation' => 'https://drupal.org/project/token' 
        )
      ) 
    );
  }

  
  return system_settings_form($form);
}

/**
 * Implementation of hook_admin_settings_form_validate()
 * for validating the form
 */
function googletagmanager_admin_settings_form_validate($form, &$form_state){
  // Custom variables validation.
  foreach($form_state['values']['googletagmanager_custom_var']['slots'] as $custom_var ) {

    $form_state['values']['googletagmanager_custom_var']['slots'][$custom_var['slot']]['name']  = trim($custom_var['name']);
    $form_state['values']['googletagmanager_custom_var']['slots'][$custom_var['slot']]['value'] = trim($custom_var['value']);
    
    // Validate empty names/values.
    if(empty($custom_var['name']) && !empty($custom_var['value'])) {
      form_set_error (
        "googletagmanager_custom_var][slots][" . $custom_var ['slot'] . "][name", 
        t(
          'The custom variable @slot-number requires a <em>Name</em> if a <em>Value</em> has been provided.',
          array('@slot-number' => $custom_var['slot'])
        )
      );
    } elseif(!empty($custom_var['name']) && empty($custom_var['value'])) {
      form_set_error(
        "googletagmanager_custom_var][slots][" . $custom_var['slot'] . "][value", 
        t(
          'The custom variable @slot-number requires a <em>Value</em> if a <em>Name</em> has been provided.',
          array('@slot-number' => $custom_var ['slot'])
        )
      );
    }
  }// endforeach
  
  // Get rid of any superfluous whitespaces at the beginning and/or end of the gtm account field.
  $form_state['values']['googletagmanager_account'] = trim($form_state['values']['googletagmanager_account']);
  
  if(! preg_match('/^GTM-\w{4,}$/', $form_state['values']['googletagmanager_account'])) {
    form_set_error(
      'googletagmanager_account',
      t('A valid Google Tag Manager ID is case sensitive and formatted like GTM-XXXX.')
    );
  }

  // Deleting all checked custom vars
  $elements = $form_state['values']['googletagmanager_custom_var']['slots'];
  $new_elements = array();
  $count = 1;
  foreach($elements as $k => $v){
    if(!empty($v['name']) && !empty($v['value']) && $v['delete'] != 1){
      $v['slot'] = $count;
      unset($v['delete']);
      $new_elements[$count] = $v;
      $count++;
    }
  }
  
  if(!empty($new_elements)){
    $form_state['values']['googletagmanager_custom_var']['slots'] = $new_elements;
  } else {
    // Si le tableau a complétement été vidé, on supprime la variable.
    unset($form_state['values']['googletagmanager_custom_var']);
    variable_del('googletagmanager_custom_var');
  }

}

/**
 * effect of the submit button "add one"
 */
function _gtm_ajax_one($form, &$form_state){
  $form_state['nb_lines']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Ajax Callback for button "add one"
 */
function _gtm_ajax_callback($form, $form_state){
  return $form['googletagmanager_custom_var'];
}

/**
 * Layout for the custom variables table in the admin settings form.
 */
function theme_googletagmanager_admin_custom_var_table(&$vars) {
  $header = array(
    array('data' => t('Slot')),
    array('data' => t('Name')),
    array('data' => t('Value')),
    array('data' => t('Delete')),
  );

  $rows = array();
  $form = current($vars);
  $elements = $form['slots'];
  foreach($elements as $key => $field ) {
    if(is_int($key)) {
      $cols = array();
      $cols[] = drupal_render($field['slot']);
      $cols[] = drupal_render($field['name']);
      $cols[] = drupal_render($field['value']);
      $cols[] = drupal_render($field['delete']);

      $rows[] = array(
        'data' => $cols,
      );
    }
  }

  $output  = theme('table', array("header" => $header, "rows" => $rows));
  $output .= drupal_render($form['add_more']);
  $output .= drupal_render($form['token_fieldset']);

  return $output;
}