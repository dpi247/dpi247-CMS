<?php

function ccimapping_cropsync_form_cropings_editor_form($form, $form_state) {
  module_load_include('inc', 'dpicommons', 'includes/dpicommons.helpers');

  $form['xml'] = array(
    '#type' => 'textarea',
    '#title' => t('XML Cropings'),
    '#default_value' => dpicommons_format_xml_string(ccimapping_cropsync_get_cropings_xml()),
    '#disabled' => !user_access('edit synchronization datas'),
  );

  $form['send'] = array(
    '#type' => 'submit',
    '#name' => 'send',
    '#value' => t('Save & Send to CCI'),
  );

  $form['fyi'] = array(
    '#type' => 'fieldset',
    '#title' => t('FYI, these are the current croping values'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 999,
  );

  $form['fyi']['markup'] = array(
    '#markup' => '<pre>'.htmlentities(ccimapping_cropsync_get_cropings_fromWS()).'</pre>',
  );

  return $form;
}

function ccimapping_cropsync_form_cropings_editor_form_submit($form, $form_state) {
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

  $parameters_array = array(
    'newsroom' => $newsroom,
    'productname' => $product_name,
    'xml' => $form_state['values']['xml'],
  );

  if ($client = cciadmin_api_load_soapclient($wsdl_uri)) {
    try {
      $result = $client->setImageSettings($parameters_array);
      drupal_set_message(t('The cropings have been sent to CCI'));
    } catch (SoapFault $exception) {
      cciadmin_api_output_webservice_error('WebService error while sending the Cropings XML', $exception->faultstring, $exception->faultcode);
    }
  }
}

/**
 * Call the CCI WS and ask for the XML definition of the cropings
 *
 * @return the result of the call from the  WS: an XML structure in string format wich describe the cropings
 */
function ccimapping_cropsync_get_cropings_fromWS() {
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

  $parameters_array = array(
    'newsroom' => $newsroom,
    'productname' => $product_name,
  );

  if ($client = cciadmin_api_load_soapclient($wsdl_uri)) {
    try {
      $result = $client->getImageSettings($parameters_array);
      return $result->return;
    } catch (SoapFault $exception) {
      cciadmin_api_output_webservice_error('WebService error while getting the Cropings XML', $exception->faultstring, $exception->faultcode);    
    }
  }

  return '';
}

function ccimapping_cropsync_get_cropings_xml() {
  $dom = new DOMDocument('1.0', 'UTF-8');
  $imageSettings = $dom->appendChild($dom->createElement('imageSettings'));
  $imageSettings->setAttribute('xmlns:cci', 'http://cci/extension/integration');

  $crops = ccimapping_cropsync_get_crops();

  foreach ($crops as $crop) {
    $cropSetting = $imageSettings->appendChild($dom->createElement('cropSetting'));
    $cropSetting->setAttribute('id', $crop['id']);
    $cropSetting->setAttribute('validate', $crop['validate']);
    $cropSetting->appendChild($dom->createElement('label', $crop['label']));
    $dimension = $cropSetting->appendChild($dom->createElement('dimension'));
    $dimension->setAttribute('width', $crop['width']);
    $dimension->setAttribute('height', $crop['height']);
    $dimension->setAttribute('type', $crop['type']);

    $gallerySetting = $imageSettings->appendChild($dom->createElement('gallerySetting'));
    $gallerySetting->setAttribute('id', $crop['id']);
    $gallerySetting->setAttribute('validate', $crop['validate']);
    $gallerySetting->appendChild($dom->createElement('label', $crop['label']));
    $gDimension = $gallerySetting->appendChild($dom->createElement('dimension'));
    $gDimension->setAttribute('width', $crop['width']);
    $gDimension->setAttribute('height', $crop['height']);
    $gDimension->setAttribute('type', $crop['type']);
  }

  return $dom->saveXML();
}

function ccimapping_cropsync_get_crops() {
  $crops = array();
  $styles = image_styles();
  foreach ($styles as $style_id => $style) {
    if (isset($style['effects'])) {
      $found_dpicrop = FALSE;
      $datas = array();
      foreach ($style['effects'] as $effect) {
        if ($effect['name'] == 'dpicontenttypes_dpicrop') {
          $found_dpicrop = TRUE;
        } elseif (($effect['name'] == 'image_scale' || $effect['name'] == 'image_scale_and_crop') && $found_dpicrop) {
          $crops[$style_id] = array(
            'id' => $style['name'],
            'validate' => 'none',
            'label' => $style['label'],
            'width' => $effect['data']['width'],
            'height' => $effect['data']['height'],
            'type' => 'pixel',
          );
        }
      }
    }
  }

  return $crops;
}
