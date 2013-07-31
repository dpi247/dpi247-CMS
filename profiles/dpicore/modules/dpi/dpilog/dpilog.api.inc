<?php

function hook_dpilog_register(){
  
  return array(
    "flowmix_extract"=>array(
        "actions"=>array("fetch","build","cache_set","cache_clear"),
      ),
    "flowmix_setup"=>array(
        "actions"=>array("create","update","delete"),
      ),
    
    );
}

