<?php

function _dpijsonexporter_jsonproduct_machine_name_exists($value){
  $result = db_select('dpijsonexporter_jsonproduct', 't')
  ->fields('t')
  ->condition('machine_name', $value,'=')
  ->execute()
  ->fetchAssoc();
  if(is_array($result)){
    return TRUE;
  }
  return FALSE;
}
