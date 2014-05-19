<?php

function ccimapping_form_productinfo_editor_form($form, $form_state) {
  module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');

  $generated_dom = ccimapping_build_productinfo();

  $form['xml'] = array(
    '#type' => 'textarea',
    '#title' => t('XML Mapping'),
    '#default_value' => dpicommons_format_xml_string($generated_dom->saveXML()),
    '#disabled' => !user_access('edit synchronization datas'),
  );

  $form['actual_xml_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Current version of the XML from CCI-Newsgate'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['actual_xml_fieldset']['actual_xml'] = array(
    '#markup' => '<pre>'.htmlspecialchars(dpicommons_format_xml_string(ccimapping_get_productinfo_fromWS())).'</pre>',
  );

  $form['xml_to_be_sent_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('XML that will be sent to CCI'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $context = 'before_send';
  drupal_alter('ccimapping_productinfo', $generated_dom, $context);
  $form['xml_to_be_sent_fieldset']['xml_to_be_sent'] = array(
    '#markup' => '<pre>'.htmlspecialchars(dpicommons_format_xml_string($generated_dom->saveXML())).'</pre>',
  );

  $form['send'] = array(
    '#type' => 'submit',
    '#name' => 'send',
    '#value' => t('Save & Send to CCI'),
  );

  return $form;
}

function ccimapping_form_productinfo_editor_form_submit($form, $form_state) {
  if ($form_state['clicked_button']['#name'] == 'send') {
    $wsdl_uri = dpi_variable_get('ccimapping_ws_wsdluri', '');
    $newsroom = dpi_variable_get('ccimapping_newsroom', '');
    $product_name = dpi_variable_get('dpicommons_product', '');

    $return = FALSE;
    if (empty($wsdl_uri)) {
      drupal_set_message(t('WSDL URI not configured, please configure it <a href="/'.CCI_ADMIN_PATH.'/ccimapping/configuration">here</a>.'), 'error');
      $return = TRUE;
    }
    if (empty($newsroom)) {
      drupal_set_message(t('NewsRoom not configured, please configure it <a href="/'.CCI_ADMIN_PATH.'/ccimapping/configuration">here</a>.'), 'error');
      $return = TRUE;
    }
    if (empty($product_name)) {
      drupal_set_message(t('Product name not configured, please configure it <a href="/admin/config/dpi247/settings">here</a>.'), 'error');
      $return = TRUE;
    }
    if ($return) {
      return;
    }

    $dom = new DomDocument('1.0', 'UTF-8');
    $dom->loadXML($form_state['values']['xml']);
    $context = 'before_send';
    drupal_alter('ccimapping_productinfo', $dom, $context);

    //if ($dom->schemaValidate(drupal_get_path('module', 'ccimapping').'/definitions/CCIPackageTypeXML.xsd')){
      $xml = $dom->saveXML();

      if ($client = cciadmin_api_load_soapclient($wsdl_uri)) {
        $parameters_array = array('newsroom' => $newsroom, 'productname' => $product_name, 'xml' => $xml);

        try {
          $result = $client->setProductTemplate($parameters_array);
          drupal_set_message(t('The productinfo has been sent to CCI'));
        } catch (SoapFault $exception) {
          cciadmin_api_output_webservice_error('WebService Error when sending the Product Info XML', $exception->faultstring, $exception->faultcode);
          dsm($exception);
        }
      }
    /*} else {
      drupal_set_message(t('XML not validated'));
    }*/
  }
}

/**
 * Call the CCI WS and ask for the XML definition of  the site structure (product info : destinations, target, layout)
 *
 * @return the result of the call from the  WS: an XML structure in string format wich describe the packagetype
 */
function ccimapping_get_productinfo_fromWS() {
  $wsdl_uri = dpi_variable_get('ccimapping_ws_wsdluri', '');
  $newsroom = dpi_variable_get('ccimapping_newsroom', '');
  $product_name = dpi_variable_get('dpicommons_product', '');

  $return = FALSE;
  if (empty($wsdl_uri)) {
    drupal_set_message(t('WSDL URI not configured, please configure it <a href="/'.CCI_ADMIN_PATH.'/ccimapping/configuration">here</a>.'), 'error');
    $return = TRUE;
  }
  if (empty($newsroom)) {
    drupal_set_message(t('NewsRoom not configured, please configure it <a href="/'.CCI_ADMIN_PATH.'/ccimapping/configuration">here</a>.'), 'error');
    $return = TRUE;
  }
  if (empty($product_name)) {
    drupal_set_message(t('Product name not configured, please configure it <a href="/admin/config/dpi247/settings">here</a>.'), 'error');
    $return = TRUE;
  }
  if ($return) {
    return '';
  }

  if ($client = cciadmin_api_load_soapclient($wsdl_uri)) {
    try {
      $result = $client->getProductTemplate(array('newsroom' => $newsroom, 'productname' => $product_name));
      return $result->return;
    } catch (SoapFault $exception) {
      cciadmin_api_output_webservice_error('WebService Error when getting the Product Info XML', $exception->faultstring, $exception->faultcode);
      dsm($exception);
    }
  }

  return '';
}

/**
 * Build an XML file corresponding the actual structure of the site. this XML should Be sent to the CCI WS
 *
 * @return an XML in string format
 */
function ccimapping_build_productinfo() {
  $dom = new DomDocument('1.0', 'UTF-8');
  return $dom;

  $hierarchies_array = wallyctools_get_hierarchies();
  $taxonomy_tree = taxonomy_get_tree(variable_get('wallymport_destinationpath', 0));
  $nested_taxonomy_tree = wallytoolbox_convert_taxonomy_flat_to_tree($taxonomy_tree);
  $dom = new DomDocument('1.0', 'UTF-8');
  ccimapping_build_productinfo_xml($dom, $hierarchies_array, $nested_taxonomy_tree);
  $context = 'generate';
  drupal_alter('ccimapping_productinfo', $dom, $context);
  return $dom;
}

function ccimapping_build_productinfo_xml(&$dom, $hierarchies_array, $nested_taxonomy_tree) {
  $dom->standalone = FALSE;
  $exportproductinformation = $dom->appendChild($dom->createElement('exportproductinformation'));
  $exportproductinformation->setAttribute('newsroom', wally_variable_get('ccimapping_newsroom', ''));
  $exportproductinformation->setAttribute('publicationchannel', wally_variable_get('wallyadmin_product', ''));
  $exportproductinformation->setAttribute('subtype', 'Online');
  $pubdatestring = $exportproductinformation->appendChild($dom->createElement('pubdatestring'));
  $pubdatestring->setAttribute('name', '%d-%m-%Y');
  $product = $exportproductinformation->appendChild($dom->createElement('product', wally_variable_get('wallyadmin_product', '')));

  $baseinfo = $exportproductinformation->appendChild($dom->createElement('baseinfo'));
  $leafname = $baseinfo->appendChild($dom->createElement('leafname'));
  $leafname->setAttribute('name', 'Target');
  $mainpriorities = $baseinfo->appendChild($dom->createElement('mainpriorities'));
  $rank_limit = wally_variable_get('cckdestinations_rank_limit', 10);
  $mainpriority = $mainpriorities->appendChild($dom->createElement('mainpriority'));
  $mainpriority->setAttribute('name', 'Rank 0');
  $mainpriority->setAttribute('value', '0');
  $mainpriority->setAttribute('default', 'yes');
  for ($i = 1; $i <= $rank_limit; $i++) {
    $mainpriority = $mainpriorities->appendChild($dom->createElement('mainpriority'));
    $mainpriority->setAttribute('name', 'Rank '.$i);
    $mainpriority->setAttribute('value', $i);
  }
  
  $budgettemplate = $exportproductinformation->appendChild($dom->createElement('budgettemplate'));
  ccimapping_build_productinfo_xml_hierarchy_items($dom, $budgettemplate, $hierarchies_array, $nested_taxonomy_tree);
}

function ccimapping_build_productinfo_xml_hierarchy_items($dom, &$dom_element, $hierarchies, $nested_taxonomy_tree) {
  foreach ($nested_taxonomy_tree as $key => $values) {
    $term = $values['term'];
    $level = $term->depth + 1;
    $dom_level = $dom_element->appendChild($dom->createElement('level'.$level));
    $dom_level->setAttribute('name', $hierarchies[$term->tid]['term_name']);

    unset($hierarchies[$term->tid]['redacblocks']['none']);
    foreach ($hierarchies[$term->tid]['redacblocks'] as $redack_machine_name => $redacblock) {
      $leaf = $dom_level->appendChild($dom->createElement('leaf'));
      $leaf->setAttribute('name', $redack_machine_name);
    }
    ccimapping_build_productinfo_xml_hierarchy_items($dom, $dom_level, $hierarchies, $values['childrens']);
  }
}