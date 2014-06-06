<?php

function ccimapping_form_packagetype_editor_form($form, $form_state, $dpi_type = NULL) {
  if ($dpi_type) {
    module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');

    $form['xml'] = array(
      '#type' => 'textarea',
      '#title' => t('XML Mapping'),
      '#default_value' => dpicommons_format_xml_string(ccimapping_get_specific_type(ccimapping_get_packagetype_fromWS(), $dpi_type)),
      '#disabled' => !user_access('edit synchronization datas'),
    );

    $form['type'] = array(
      '#type' => 'value',
      '#value' => $dpi_type,
    );

    $form['send'] = array(
      '#type' => 'submit',
      '#name' => 'send',
      '#value' => t('Save & Send to CCI'),
    );
  }
  else {
    $form['type_selector'] = array(
      '#markup' => theme('ccimapping_typeselector', array('type' => 'packagetype')),
    );
  }

  return $form;
}

function ccimapping_form_packagetype_editor_form_submit($form, $form_state) {
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
      drupal_set_message(t('Product name not configured, please configure it <a href="/'.DPI_ADMIN_PATH.'/general-settings">here</a>.'), 'error');
      $return = TRUE;
    }
    if ($return) {
      return;
    }

    $dom = new DomDocument('1.0', 'UTF-8');
    $dom->loadXML($form_state['values']['xml']);

    module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');
    if (dpicommons_validate_xml($dom, drupal_get_path('module', 'ccimapping').'/definitions/CCIPackageTypeXML.xsd')) {
      $xml = $dom->saveXML();

      if ($client = cciadmin_api_load_soapclient($wsdl_uri)) {
        $parameters_array = array('newsroom' => $newsroom, 'productname' => $product_name, 'xml' => $xml);

        try {
          $result = $client->setPackageType($parameters_array);
          drupal_set_message(t('The packageType has been sent to CCI'));
        }
        catch (SoapFault $exception) {
          cciadmin_api_output_webservice_error('WebService error while sending the Package Type XML', $exception->faultstring, $exception->faultcode);
          dsm($exception);
        }
      }
    }
    else {
      drupal_set_message(t('XML not validated'));
    }
  }
}

/**
 * Call the CCI WS and ask for the XML definition of a content type
 *
 * @param: $type
 *    the content type's machine name -not used for the moment-
 *
 * @return the result of the call from the  WS: an XML structure in string format wich describe the packagetype
 */
function ccimapping_get_packagetype_fromWS() {
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
    drupal_set_message(t('Product name not configured, please configure it <a href="/'.DPI_ADMIN_PATH.'/general-settings">here</a>.'), 'error');
    $return = TRUE;
  }
  if ($return) {
    return '';
  }

  if ($client = cciadmin_api_load_soapclient($wsdl_uri)) {
    try {
      $result = $client->getPackageType(array('newsroom' => $newsroom, 'productname' => $product_name));
      return $result->return;
    }
    catch (SoapFault $exception) {
      cciadmin_api_output_webservice_error('WebService error while getting the Package Type XML', $exception->faultstring, $exception->faultcode);
      dsm($exception);
    }
  }

  return '';
}

function ccimapping_get_specific_type($xml, $dpi_type) {
  if (!empty($xml)) {
    $cci_type = _ccimapping_type_converter($dpi_type);
    $dom = new DOMDocument('1.0', 'UTF-8');
    $dom->loadXML($xml);
    $packagedefs = $dom->getElementsByTagName('packagedefs');
    foreach ($packagedefs as $packagedef) {
      // We want to find the packagedef wit packagetype->name == $title .... but DomDocument API ins'nt easy as Jquery selector

      $packagetype = $packagedef->getElementsByTagName('packagetype');
      $packagetype = $packagetype->item(0);
      $name = $packagetype->getElementsByTagName('name');
      $name = $name->item(0);
      if ($name->nodeValue == $cci_type) {
        $newdoc = new DOMDocument('1.0', 'UTF-8');
        $newdoc->formatOutput = true;
        $node = $newdoc->importNode($packagedef, true);
        $newdoc->appendChild($node);
        return $newdoc->saveXML();
      }
    }
  }
  else {
    return '';
  }
}
