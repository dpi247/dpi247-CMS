<?php 

/**
 * Page callback for the administration of filters
 */
function dpipressfilters_page_admin(){
  $content = '';
  
  $filters = dpipressfilters_getfilters();    
  
  if ($filters != NULL){
    $header = array(t('Name'), t('Description'), t('Operations'));
    foreach ($filters as $filter){
      $rows[] = array(
        $filter->name,
        $filter->description,
        '<a href = "/admin/config/content/dpipressfilters/settings/edit/'.$filter->fid.'">'.t('Edit').'</a>, <a href = "/admin/config/content/dpipressfilters/settings/delete/'.$filter->fid.'">'.t('Delete').'</a>',
      );
    }
    $content .= theme('table', array('header' => $header, 'rows' => $rows));
  } else {
    $content .= t('There is no filter. To add one click on "Create filter".');
  }
  
  return $content;
}

/**
 * Form to create/edit filter
 */
function dpipressfilters_form_addfilter_form($form, $form_state, $fid = NULL){
  $form['dpipressfilters']['#tree'] = TRUE;

  $form['dpipressfilters']['name'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Name'),
    '#description' => t('The unique Name of the filter.'),
    '#size'        => 80,
    '#maxlength'   => 100,
    '#required'    => TRUE,
  );
  
  $form['dpipressfilters']['description'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Description'),
    '#description' => t('A description of the filter'),
    '#size'        => 80,
    '#maxlength'   => 1000,
  );

  $form['dpipressfilters']['tags'] = array(
    '#type'          => 'textarea',
    '#title'         => t('List of the tags'),
    '#description'   => t('The tags who will be filtered. Enter one value per line, without the "&lt;", "&gt;".<br>
        You can also add a label, following schema "<i>tag|label</i>".'),
    '#default_value' => '',
    '#cols'          => 10,
    '#rows'          => 30,
    '#resizable'     => TRUE,
  );
  
  /*
  $form['dpipressfilters']['markers'] = array(
    '#type'          => 'textarea',
    '#title'         => t('List of the markers'),
    '#description'   => t('The markers who will be filtered. Enter one value per line, without the "&lt;", "&gt;".<br>
        You can also add a label, following schema "<i>marker|label</i>".'),
    '#default_value' => '',
    '#cols'          => 10,
    '#rows'          => 15,
    '#resizable'     => TRUE,
  );
  */

  if ($fid != NULL){
    $filter = dpipressfilters_getfilterbyid($fid);
    $form['dpipressfilters']['fid'] = array('#type' => 'value', '#value' => $filter->fid);
    $tags = dpipressfilters_gettagsfromfilter($filter->name);
    $form['dpipressfilters']['name']['#default_value'] = $filter->name;
    $form['dpipressfilters']['description']['#default_value'] = $filter->description;

    $tags_default_value = $markers_default_value = $attr_rel_tags_default_value = '';
    $tag_space = $marker_space = $attr_rel_tags_space = '';
    foreach ($tags as $tag) {
    //  switch ($tag->type) {
    //    case 'tag':
          $tags_default_value .= $tag_space.$tag->tag.'|'.$tag->description;
          $tag_space = "\n";
    //      break;
          
    /*    case 'marker':
          $markers_default_value .= $marker_space.$tag->tag.'|'.$tag->description;
          $marker_space = "\n";
          break;*/
    //  }
    }
    $form['dpipressfilters']['tags']['#default_value'] = $tags_default_value;
    //$form['dpipressfilters']['markers']['#default_value'] = $markers_default_value;
  }

  $form['sumbit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Validation of the dpipresfilters_form_addfilter_form
 */
function dpipressfilters_form_addfilter_form_validate($form, &$form_state){
  $form_state['values']['dpipressfilters']['name'] = strtolower(check_plain($form_state['values']['dpipressfilters']['name']));
  $form_state['values']['dpipressfilters']['description'] = check_plain($form_state['values']['dpipressfilters']['description']);
  
  // If it's edition
  if (isset($form_state['values']['dpipressfilters']['fid'])){
    $form_state['values']['update'] = array('fid');
  } else {
    // Check if the name is correct
    if (preg_match('/[^a-z0-9_]/', $form_state['values']['dpipressfilters']['name'])) {
      form_set_error('dpipressfilters][name', t('Filter name must be alphanumeric or underscores only.'));
    } 
    // Check if the name already exists
    elseif (dpipressfilters_getfilterbyname($form_state['values']['dpipressfilters']['name']) != NULL){
      form_set_error('dpipressfilters][name', t('A filter with this name already exists.'));
    }
    $form_state['values']['update'] = array();
  }
  
  $form_state['values']['dpipresstags'] = array();

  $weight = 0;
  $tags = explode("\r\n", $form_state['values']['dpipressfilters']['tags']);
  foreach ($tags as $tmp_tag) {
    list($tag, $description) = explode('|', $tmp_tag.'|');

    // Check if the tag is correct
    if ($tag == '') {
      continue;
    }
    
    // Check is the tag exists two times
    if (array_key_exists($tag, $form_state['values']['dpipresstags'])) {
      form_set_error('dpipressfilters][tags', t('There can not be two identical tags.'));
    } else {
      $update = array();
      $form_state['values']['dpipresstags'][$tag] = array(
        'tag' => $tag,
        'description' => $description,
        'filter' => $form_state['values']['dpipressfilters']['name'],
        'type' => 'tag',
        'update' => $update,
        'weight' => $weight++,
      );
    }
  }
  /*
  $markers = explode("\r\n", $form_state['values']['dpipressfilters']['markers']);
  foreach ($markers as $tmp_marker) {
    list($marker, $description) = explode('|', $tmp_marker);
    
    // Check if the marker is correct
    if ($marker == '') {
      ;
    }
    // Check is the marker exists two times
    elseif (array_key_exists($marker, $form_state['values']['dpipresstags'])) {
      form_set_error('dpipressfilters][markers', t('There can not be two identical markers.'));
    } else {
      $update = array();
      $form_state['values']['dpipresstags'][$marker] = array(
        'tag' => $marker,
        'description' => $description ? $description : '',
        'filter' => $form_state['values']['dpipressfilters']['name'],
        'type' => 'marker',
        'update' => $update,
      );
    }
  }
  */
}

/**
 * Submit of the dpipresfilters_form_addfilter_form
 */
function dpipressfilters_form_addfilter_form_submit($form, &$form_state){
  // Save filter
  $result = drupal_write_record('dpipressfilters', $form_state['values']['dpipressfilters'], $form_state['values']['update']);
  $message = '';
  if ($result) {
    $message = "The press filter as been created/updated. Don't forget ".l("to configure tags behaviour for all text formats", 'admin/config/content/formats').".";
    
    foreach ($form_state['values']['dpipresstags'] as $tag){
      // Save tags
      if ($old_tags = dpipressfilters_gettagsbynameandfilter($tag['tag'], $form_state['values']['dpipressfilters']['name'])) {
        $update = array('tid');
        foreach ($old_tags as $old_tag) {
          $new_tag = array_merge((array)$old_tag, $tag);
          $result = drupal_write_record('dpipressfilters_tags', $new_tag, $update);
        }
        $action = 'updated';
      } else {
        $tag['settings'] = _dpipressfilters_settings_defaults();
        $result = drupal_write_record('dpipressfilters_tags', $tag);
        $action = 'created';
      }
      if ($result) {
        $message .= '<br>'.t('The !tag_type "@tag" has been !action', array('!tag_type' => $tag['type'], '@tag' => $tag['tag'], '!action' => $action));
      } else {
        $message .= '<br>'.t('The !tag_type "@tag" has not been !action', array('!tag_type' => $tag['type'], '@tag' => $tag['tag'], '!action' => $action));
      }
    }

    // Delete tags and markers
    foreach (dpipressfilters_gettagsfromfilter($form_state['values']['dpipressfilters']['name']) as $filter_tags) {
      if (!array_key_exists($filter_tags->tag, $form_state['values']['dpipresstags'])) {
        db_delete('dpipressfilters_tags')
          ->condition('filter',$filter_tags->filter)
          ->condition('type',$filter_tags->type)
          ->condition('tag',$filter_tags->tag)
          ->execeute();
        $message .= '<br>'.t('The !tag_type "@tag" has been deleted', array('!tag_type' => $filter_tags->type, '@tag' => $filter_tags->tag));
      }
    }
  } else {
    $message = t('The filter as not been created/udpated');
  }
  drupal_set_message($message);
  drupal_goto('admin/config/content/dpipressfilters/settings/general');
}

/**
 * Form to delete a filter
 */
function dpipressfilters_form_deletefilter_form($form, $form_state, $fid){
  $filter = dpipressfilters_getfilterbyid($fid);
  $question = t('Are you sure you want to delete "@filter_name" filter?', array('@filter_name' => $filter->name));
  
  $add_form = array();
  $add_form['fid'] = array(
    '#type' => 'value',
    '#value' => $fid,
  );
  
  $form = confirm_form($add_form, $question, 'admin/config/content/dpipressfilters/settings');
  $form_state['redirect'] = 'admin/config/content/dpipressfilters/settings';
  return $form;
}

/**
 * Submit of the dpipressfilters_form_deletefilter_form
 */
function dpipressfilters_form_deletefilter_form_submit($form, &$form_state){
  $filter = dpipressfilters_getfilterbyid($form_state['values']['fid']);
  $result = db_delete('dpipressfilters')->condition('fid',$filter->fid)->execute();
  $result2 = db_delete('dpipressfilters_tags')->condition('filter',$filter->name)->execute();
  if ($result && $result2){
    drupal_set_message(t('The filter as correctly been deleted'));
  } else {
    drupal_set_message(t('The filter couldn\'t be correctly deleted'));
  }
}
