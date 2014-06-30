<?php 

/**
 * Return the list of all available functions which return a statistic's parameter.
 */
function dpistat_available_params_function() {
  return array(
    'dpistat_get_url_argument' => t('One of the URL arguments'),
    'dpistat_get_node_field' => t('The value of one of the node fields'),
    'dpistat_get_eval' => t('PHP code'),
  );
}

/**
 * Return an URL argument
 */
function dpistat_get_url_argument($node, $i) {
  return arg($i);
}

/**
 * Return the form element(s) for this callback
 */
function dpistat_get_url_argument_form() {
  $form = array();

  $form[] = array(
    '#type'        => 'select',
    '#title'       => t('Argument ID in URL'),
    '#options'     => array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10),
  );

  return $form;
}

/**
 * Return the value of a node field
 */
function dpistat_get_node_field($node, $field) {
  $node_wrapper = entity_metadata_wrapper('node', $node);
  if (isset($node_wrapper->{$field}) && $field_value = $node_wrapper->{$field}->value()) {
    return $field_value['value'];
  }
  return '';
}

/**
 * Return the form element(s) for this callback
 */
function dpistat_get_node_field_form() {
  $form = array();

  $form[] = array(
    '#type'        => 'textfield',
    '#title'       => t('Field name'),
    '#size'        => 80,
    '#maxlength'   => 512,
  );

  return $form;
}

/**
 * Return the evaluated code
 */
function dpistat_get_eval($node, $code) {
  return eval($code);
}

/**
 * Return the form element(s) for this callback
 */
function dpistat_get_eval_form() {
  $form = array();

  $form[] = array(
    '#type'        => 'textarea',
    '#title'       => t('PHP Code'),
    '#description' => t('The $node variable is available.<br>
        <b style="color:red;">Use this with caution! This code will be executed every time a node is viewed.</b>'),
    '#rows'        => 5,
  );

  return $form;
}

/**
 * Add the stat AJAX updater callback to node pages
 */
function dpistat_add_updater_callback(&$vars) {
  $node = $vars['node'];
  $node_wrapper = entity_metadata_wrapper('node', $node);
  $current_id = $node->nid;
  $title = addslashes(htmlspecialchars_decode($node->title));

  $current_terms = '';
  if (isset($node_wrapper->field_destinations)) {
    foreach ($node_wrapper->field_destinations->value() as $dest) {
      $current_terms .= $dest['tid'].'+';
    }
  }
  $db_terms = db_select('taxonomy_index', 't')
  ->fields('t', array('tid'))
  ->condition('t.nid', $current_id)
  ->execute();
  foreach ($db_terms as $row) {
    $current_terms .= $row->tid.'+';
  }

  $params_array = array();
  for ($i = 1; $i <= 10; $i++) {
    $params_callback = variable_get('dpistat_param_'.$i, array());
    $current_callback = isset($params_callback['current_callback']) ? $params_callback['current_callback'] : 'none';

    unset($params_callback[$current_callback]['name']);
    if ($current_callback != 'none') {
      $param_value = call_user_func_array($current_callback, array($vars['node']) + $params_callback[$current_callback]);
      $param_value = rawurldecode($param_value);
      $params_array[] = ($i == 10) ? 'param'.$i.'='.$param_value : 'param0'.$i.'='.$param_value;
    } else {
      $params_array[] = ($i == 10) ? 'param'.$i.'=' : 'param0'.$i.'=';
    }
  }

  $current_terms = trim($current_terms, '+');
  drupal_add_js("
      (function($) {
        function updateStats(){
          $.ajax({
            url: \""."/".drupal_get_path('module', 'dpistat')."/updater.php?nid=".$current_id."&title=".$title."&terms=".$current_terms."&".implode('&', $params_array)."\",
            cache: false,
            async: false
          });
        }

        Drupal.behaviors.dpiatom_reference = {
          attach: function(context) {
            updateStats();
          }
        }
      })(jQuery);", "inline");
}