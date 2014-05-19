<?php

/**
 * Load a soap client
 *
 * @param string $wsdl_uri
 * @return SoapClient|boolean
 */
function cciadmin_load_soapclient($wsdl_uri) {
  if ($wsdl_uri && is_string($wsdl_uri) && valid_url($wsdl_uri, TRUE)) {
    try {
      $client = new SoapClient($wsdl_uri, array('features' => SOAP_SINGLE_ELEMENT_ARRAYS, 'trace' => TRUE));
      return $client;
    } catch (SoapFault $exception) {
      drupal_set_message(t('WebService error while creating SOAP client: \n FAULTSTRING: @faultstring\n FAULTCODE: @faultcode', array('@faultstring' => $exception->faultstring, '@faultcode' => $exception->faultcode)), 'error');
    }
  }
  return FALSE;
}
