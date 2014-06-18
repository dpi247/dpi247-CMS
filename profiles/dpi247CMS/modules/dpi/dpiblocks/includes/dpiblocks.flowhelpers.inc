<?php

function dpiblocks_flowhelpers_init_array($feed_infos = array()) {
  $array_packages = array();
  $array_packages['Packages']['Package'] = array();
  $array_packages['Packages']['__log'] = array();
  $array_packages['Packages']['__log']['success'] = TRUE;
  $array_packages['Packages']['__log']['msg'] = '';
  $array_packages['Packages']['__settings'] = $feed_infos;
  return $array_packages;
}
