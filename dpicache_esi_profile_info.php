<?php
define('DRUPAL_ROOT', getcwd());
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
global $user;


print "Hello ";
print $user->name;
$role_string=urlencode(implode("--", $user->roles));

$username=$user->uid?$user->name:"anonymous";
drupal_add_http_header('X-Dpicache-userid', $user->uid);
drupal_add_http_header('X-Dpicache-username', $username);
drupal_add_http_header('X-Dpicache-roles', $role_string);