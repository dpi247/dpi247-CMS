<?php

function ccidynamicfeedback_page_render($ext_ref) {
  $GLOBALS['conf']['cache'] = FALSE;
  $options = dpi_variable_get('ccidynamicfeedback_display_options', array());

  $node = NULL;
  if (is_numeric($ext_ref)) {
    // If it's a numeric value it's probably a nid
    $nid = $ext_ref;
    $node = node_load($nid);
  } else {
    $node = dpicontenttypes_api_getnodebyexternalreference($ext_ref);
    $nid = $node->nid;
  }

  $datas = array();
  if ($node) {
    $datas['options'] = array();

    foreach ($options as $option) {
      if ($option) {
        $tmp_datas = module_invoke_all('ccidynamicfeedback_options', 'content', $option, $node);
        if (!empty($tmp_datas)) {
          $datas['options'][$option] = $tmp_datas;
        }
      }
    }

    $datas['title'] = $node->title;
    $datas['nid'] = $nid;
    $datas['base_url'] = $_SERVER['HTTP_HOST'];
  }

  if (empty($datas)) {
    $datas = array('nid' => FALSE);
  }

  print theme('ccidynamicfeedback_page_render', array('datas' => $datas));
}
