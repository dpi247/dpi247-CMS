<?php

/**
 * Load a soap client
 *
 * @param string $wsdl_uri
 * @return SoapClient|boolean
 */
function cciadmin_api_load_soapclient($wsdl_uri) {
 // if ($wsdl_uri && is_string($wsdl_uri) && valid_url($wsdl_uri, TRUE)) {
  if ($wsdl_uri && is_string($wsdl_uri)) {
    try {
      $client = new SoapClient($wsdl_uri, array('features' => SOAP_SINGLE_ELEMENT_ARRAYS, 'trace' => TRUE));
      return $client;
    } catch (SoapFault $exception) {
      cciadmin_api_output_webservice_error('WebService error while creating SOAP cleint', $exception->faultstring, $exception->faultcode);
    }
  }
  return FALSE;
}

/**
 * Output a CCI WebService error message
 */
function cciadmin_api_output_webservice_error($human_message, $error_message, $error_code) {
  drupal_set_message(t($human_message).'<br />'.
    t('Error message : @faultstring', array('@faultstring' => $error_message)).'<br />'.
    t('Error code : @faultcode', array('@faultcode' => $error_code)), 'error');
}
