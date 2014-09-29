<?php 


function dpissologinmanagerserver_api_check_email_unicity($mail){
  
  $query = db_select('dpissologinmanager_users', 'd')
    ->condition('d.mail', $mail, '=');
   $count=$query->countQuery()->execute()->fetchField();
   if($count==0)
    return TRUE;
   
   return FALSE;
}
function dpissologinmanagerserver_api_check_loginid_unicity($loginId){
  $query = db_select('dpissologinmanager_users', 'd')
  ->condition('d.loginId', $loginId, '=');
  $count=$query->countQuery()->execute()->fetchField();
  if($count==0)
    return TRUE;
   
  return FALSE;
}