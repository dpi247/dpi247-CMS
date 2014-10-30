<?php

/**
 * Wrapper function to avoid dependancy of dpicache
 */
function _dpidestinations_variable_get($name, $default_value) {
  if(function_exists('dpi_variable_get')) {
    return dpi_variable_get($name, $default_value);
  }
  else {
    return variable_get($name, $default_value);
  }
}

/**
 * Wrapper function to avoid dependancy of dpicache
 */
function _dpidestinations_variable_set($name, $default_value) {
  if(function_exists('dpi_variable_set')) {
    return dpi_variable_set($name, $default_value);
  }
  else {
    return variable_set($name, $default_value);
  }
}

/**
 * Wrapper function to avoid dependancy of dpicache
 */
function _dpidestinations_variable_del($name) {
  if(function_exists('dpi_variable_del')) {
    return dpi_variable_del($name);
  }
  else {
    return variable_del($name);
  }
}

// @todo : Interet de cette fonction par rapport à taxonomy_get_tree ?
function _dpidestinations_api_get_all_sections_terms_from_db($reset_cache = FALSE) {
  $vid = dpidestinations_api_get_sections_vocabulary_vid();
  $query = db_select('taxonomy_term_data', 't');
  $result = $query
    //->addTag('translatable')
    ->fields('t')
    ->condition('t.vid', $vid)
    ->orderBy('t.weight')
    ->orderBy('t.name')
    ->execute();

  $terms = array();
  foreach ($result as $term) {
    $terms[$term->tid] = $term;
  }
  return $terms;
}

function _dpidestinations_target_machine_name_exists($value) {
  $result = db_select('dpidestinations_target', 't')
    ->fields('t')
    ->condition('machine_name', $value,'=')
    ->execute()
    ->fetchAssoc();
  if(is_array($result)) {
    return TRUE;
  }
  return FALSE;
}

function _dpidestinations_get_all_targetviews() {
  //@todo: Peut être interessant de mettre un cache statique ici pour performance (DDU)
  // Il faudrait alors implémenter le hook_views_invalidate_cache()
  $targetviews = array();
  $views = views_get_all_views();
  foreach($views as $view_name => $view) {
    foreach($view->display as $display_name => $display) {
      if($display->display_plugin == 'targetview'){
        $targetviews[$view_name]['label'] = $view->human_name;
        $targetviews[$view_name]['displays'][] = array('label' => $display->display_title, 'display_name' => $display_name);
      }
    }
  }

  return $targetviews;
}

function _dpidestinations_get_panels_displayid_for_sectionterm($term_id) {
  $handler = _dpidestinations_get_panels_handler_for_sectionterm($term_id);
  return $handler->did;
}

function _dpidestinations_get_panels_display_for_sectionterm($term_id) {
  $displays = &drupal_static(__FUNCTION__);
  if (!isset($displays[$term_id])) {
    ctools_include('context');
    ctools_include('context-task-handler');
    ctools_include('plugins');

    $handler = _dpidestinations_get_panels_handler_for_sectionterm($term_id);
    ctools_plugin_load_function('page_manager', 'task_handlers', $handler->handler, 'render');

    $display = panels_panel_context_get_display($handler);

    $displays[$term_id] = $display;
  }
  return $displays[$term_id];
}

/**
 * Return the name form the page_manager_handlers table for a display_id
 */
function _dpidestinations_get_pagemanager_handler_name_for_displayid($did) {
  $db_names = db_select('page_manager_handlers', 'p')
    ->fields('p', array('name'))
    ->condition('p.did', $did)
    ->execute();
  $name = FALSE;
  foreach ($db_names as $db_name) {
    $name = $db_name;
    break;
  }
  return $name;
}

function _dpidestinations_get_panels_handler_for_sectionterm($term_id) {
  $handlers = &drupal_static(__FUNCTION__);
  if (!isset($handlers[$term_id])) {
    ctools_include('context');
    ctools_include('context-task-handler');
    ctools_include('plugins');
     
    $data['tid'] = $term_id;

    $section = sectionterm_load($term_id);

    $task= page_manager_get_task('sections');
    $contexts = ctools_context_handler_get_task_contexts($task, '', array($section));

    $sorted_handlers = page_manager_load_sorted_handlers($task, '', TRUE);
    $id = ctools_context_handler_get_render_handler($task, NULL, $sorted_handlers,$contexts, array());

    $handler = $sorted_handlers[$id];
    $handlers[$term_id] = $handler;
  }
  return $handlers[$term_id];
}

function _dpidestinations_get_available_targetlayouts() {
  $entites_info = entity_get_info('node');
  $available_targetlayouts = $entites_info['view modes'];
  unset($available_targetlayouts['rss']);
  unset($available_targetlayouts['search_index']);
  unset($available_targetlayouts['search_result']);
  $return = array();
  foreach ($available_targetlayouts as $key => $value) {
    $return[$key] = $value['label'];
  }
  return $return;
}

function _dpidestinations_get_targets_for_sectionterm($term_id) {
  $targets = &drupal_static(__FUNCTION__);
  if (!isset($targets[$term_id])) {
    $display = _dpidestinations_get_panels_display_for_sectionterm($term_id);
    $temp = array();
    foreach($display->content as $content) {
      if($content->type == 'targetblock') {
        $temp[$content->subtype] = target_load($content->subtype);
      }
    }
    $targets[$term_id] = $temp;
  }
  return $targets[$term_id];
}

function _target_metadata_getter($object, $options, $property_name) {
  if($property_name == 'available_targetlayouts') {
    foreach(unserialize($object->available_targetlayouts) as $key => $layout) {
      if($layout != FALSE) {
        $layouts[$key] = $layout;
      }
    }
    return $layouts;
  }

  if($property_name == 'available_templateoverrides') {
    $result = array();
    return $result;
    if($object->available_templateoverrides != '' && $object->available_templateoverrides != '\n') {
      foreach(explode('\n', $object->available_templateoverrides) as $line) {
        list($key, $value) = explode('|', $line);
        $result[$key] = $value;
      }
    }
    return $object->available_templateoverrides = $result;
  }

  if($property_name == 'targetview') {
    $view_name = $object->view_name;
    $view_display = $object->view_display;
    if($view = views_get_view($view_name)) {
      $view->set_display($view_display);
      return $view;
    }
    else {
      return NULL;
    }
  }
}

function _target_metadata_setter($object, $property_name, $wrapper) {
  if($property_name == 'available_targetlayouts') {
    return $object->available_targetlayouts = serialize($wrapper);
  }

  if($property_name == 'available_templateoverrides') {
    $db_value = '';
    foreach($wrapper as $key => $value) {
      $db_value .= $key.'|'.$value.'\n';
    }
    return $object->available_templateoverrides = $db_value;
  }
}

function _targetblock_metadata_getter($object, $options, $property_name) {
  if($property_name == 'target') {
    return $object->target;
  }

  if($property_name == 'target_object') {
    return $object->target;
  }

  if($property_name == 'other_splitted_targetblocks') {
    $targetblockController = new TargetBlockController('targetblock');

    $other_splitted_targetblocks = $targetblockController->load_targetblocks_by_did_and_target($object->did, $object->target, $object->delta);

    // We remove the current targetblock from the set
    unset($other_splitted_targetblocks[$object->targetblockid]);
    return $other_splitted_targetblocks;
  }
}

/**
 * Update the virtualtrank of each items of a node
 */
function _dpidestinations_virtualrank_update_items(&$items) {
  foreach($items as &$item) {
    _dpidestinations_virtualrank_update_item($item);
  }
}

/**
 * Update the virtualrank of an item
 */
function _dpidestinations_virtualrank_update_item(&$item) {
  if($item['rank'] == 'default') {
    $controller = new TargetBlockController('targetblock');
    $targetblock = $controller->load_targetblock_by_termid_and_target($item['tid'], $item['target']);

    if($targetblock) {
      $wrapper = entity_metadata_wrapper('targetblock', $targetblock);
      $rank_max_value = $wrapper->rank_max_value->value();
      $item['rank'] = $rank_max_value + 1;
    }
    else {
      //We need a fallback if the targetblock is not found
      watchdog("dpidestinations", t("No targetblock found for tid %tid, target %target", array('%tid' => $item['tid'], '%target' => $item['target'])));
      drupal_set_message(t("An error occured during rank calculation for tid %tid and target %target", array('%tid' => $item['tid'], '%target' => $item['target'])));

      if($item['rank'] == 'default') {
        $item['rank'] = 1;
      }
      elseif($item['rank'] == 'DNC') {
        $item['rank'] = 0;
      }
    }
  }

  if($item['rank'] != 'DNC') {
    //Caclulate virtual_rank
    $item['virtual_rank'] = _dpidestinations_virtualrank_give_virtual_rank_for_position($item['tid'], $item['target'], $item['rank']);
  }
  else {
    $item['rank'] = 0;
  }
}

/**
 * Update the virtualrank of an item
 */
function _dpidestinations_virtualrank_give_virtual_rank_for_position($tid, $target, $position) {
  //@todo: avoid hard coding of the table...
  $destination_table = 'field_data_field_destinations';

  // USING OFFSET for efficency
  if($position == 0 || $position == 1 || $position == 2) {
    $offset = 0;
  } else {
    $offset = $position - 2;
  }

  $query = db_select($destination_table, 'd');
  //$query->join('node', 'n', 'n.nid = d.entity_id and n.vid=d.revision_id');
  $query->condition('d.field_destinations_tid', $tid, '=');
  $query->condition('d.field_destinations_target', $target, '=');
  $query->fields('d', array('field_destinations_virtual_rank'));
  $query->orderBy('d.field_destinations_virtual_rank', 'DESC');
  $query->range($offset, 2); // Offset is the first argument, second argument is
  $results = $query->execute();
  $array_results = array();
  foreach($results as$result) {
    $array_results[] = $result;
  }

  if(count($array_results) == 0) {
    //this is the first package in the destination
    //set the value regarding the number of reserved position in the destiantion
    $controller=new TargetBlockController('targetblock');
    $targetblock=$controller->load_targetblock_by_termid_and_target($tid, $target);
    $wrapper = entity_metadata_wrapper('targetblock', $targetblock);
    $rank_max_value = $wrapper->rank_max_value->value();

    $virtual_rank = (int)_dpidestinations_variable_get('dpidestinations_virtualrank_delta', 1000) * ($rank_max_value + 1);
  } else {
    $virtual_rank = 0;
    if($position == 1) {
      $virtual_rank = $array_results[0]->field_destinations_virtual_rank + _dpidestinations_variable_get('dpidestinations_virtualrank_delta', 1000);
    }
    if($position > 1) {
      if(count($array_results) > 1) {
        // we do (1+A+B)/2 to be sure that if there is a saturation the saturation is for the upper element
        $virtual_rank = (1 + $array_results[0]->field_destinations_virtual_rank + $array_results[1]->field_destinations_virtual_rank )/2;
      } else{
        $virtual_rank = $array_results[0]->field_destinations_virtual_rank/2;
      }
    }
    $virtual_rank = (int)$virtual_rank;

    if($virtual_rank == $array_results[0]->field_destinations_virtual_rank) {
      watchdog('dpidestinations', 'Virtual_rank saturation for term %tid and target %target at position %position', array('%tid' => $tid, '%target' => $target, '%position' => $position));
      drupal_set_message(t('Virtual_rank saturation for term %tid and target %target at position %position this can lead to ordering issue for this targetblock... if the problem perssist please contact your support team', array('%tid' => $tid, '%target' => $target, '%position' => $position)));

      //We are gonna update the virtual rank directly in DB .... if we have trouble with that we should use node_save...

      $query2 = db_select($destination_table, 'd');
      //$query->join('node', 'n', 'n.nid = d.entity_id and n.vid=d.revision_id');
      $query2->condition('d.field_destinations_tid', $tid, '=');
      $query2->condition('d.field_destinations_target', $target, '=');
      $query2->fields('d', array('field_destinations_virtual_rank','entity_id','delta'));
      $query2->orderBy('d.field_destinations_virtual_rank', 'DESC');
      $query2->range(0,$position -1); //offset is the first argument, second argument is
      $results2 = $query2->execute();

      $nids = array();
      foreach($results2 as $index => $row_result) {
        $new_virtual_rank = $virtual_rank+((count($results2)-$index+1)*_dpidestinations_variable_get('dpidestinations_virtualrank_delta', 1000));
        $nids[] = array('nid' => $row_result->entity_id, 'virtual_rank' => $new_virtual_rank);
        $keyed_nids[$index] = $row_result->entity_id;
        $keyed_virtualranks[$index] = $new_virtual_rank;

        db_update($destination_table) // Table name no longer needs {}
          ->fields(array('field_destinations_virtual_rank' => $new_virtual_rank))
          ->condition('entity_id', $row_result->entity_id)
          ->condition('field_destinations_tid', $tid)
          ->condition('field_destinations_target', $target)
          ->condition('delta',$row_result->delta)
          ->execute();
      }

      entity_get_controller('node')->resetCache($keyed_nids);

      module_invoke_all('dpidestinations_virtualrank_dispersion', $tid, $target, $keyed_nids);
    }
  }

  return $virtual_rank;
}

function _dpidestinations_virtualrank_update_virtualrank($nid, $term_id, $target_name, $new_virtual_rank){
  //@todo: avoid hard coding of the table...
  $destination_table='field_data_field_destinations';

  db_update($destination_table) // Table name no longer needs {}
    ->fields(array('field_destinations_virtual_rank' => $new_virtual_rank))
    ->condition('entity_id',$nid)
    ->condition('field_destinations_tid',$term_id)
    ->condition('field_destinations_target',$target_name)
    ->execute();

  entity_get_controller('node')->resetCache(array($nid));
}

function _dpidestinatons_targetblock_calculate_node_count_for_targetblock(& $targetblock){
  if($targetblock->use_split and $targetblock->total_count_do_not_propagate!=TRUE){
    $targetblock_wrapper=entity_metadata_wrapper('targetblock', $targetblock);
    $targetblockController=new TargetBlockController("targetblock");

    $node_count_for_targetblock=0;
    $all_splitted_targetblocks=$targetblockController->load_targetblocks_by_did_and_target($targetblock->did, $targetblock->target);
    foreach($all_splitted_targetblocks as $splitted){
      $node_count_for_targetblock+=$splitted->node_count_for_targetblockinstance;
    }
    $targetblock->node_count_for_targetblock=$node_count_for_targetblock;

  }
  elseif($targetblock->use_split==FALSE){
    $targetblock->node_count_for_targetblock=$targetblock->node_count_for_targetblockinstance;
  }
}

function _dpidestinatons_targetblock_sync_node_count_for_targetblock(& $targetblock){
  if($targetblock->use_split and $targetblock->total_count_do_not_propagate!=TRUE){
    $targetblock_wrapper=entity_metadata_wrapper('targetblock', $targetblock);
    $targetblockController=new TargetBlockController("targetblock");

    $node_count_for_targetblock=0;
    $other_splitted_targetblocks=$targetblockController->load_targetblocks_by_did_and_target($targetblock->did, $targetblock->target);
    foreach($other_splitted_targetblocks as $splitted){
      $node_count_for_targetblock+=$splitted->node_count_for_targetblockinstance;
    }

    $other_splitted_targetblocks=$targetblock_wrapper->other_splitted_targetblocks->value();
    foreach($other_splitted_targetblocks as $other_splitted_targetblock){
      //Set total_count_do_not_propagate to avoid recursive calls
      $other_splitted_targetblock->total_count_do_not_propagate=TRUE;
      $other_splitted_targetblock->node_count_for_targetblock=$node_count_for_targetblock;
      targetblock_save( $other_splitted_targetblock);
    }
  }
}

/**
 * Initialize a reorder targetview
 *
 * You still need to execute
 * $targetview->pre_execute();
 * $targetview->execute();
 */
function _dpidestinations_targetviewreorder_initialize_view($targetblock,$term){
  $targetblock_wrapper = entity_metadata_wrapper('targetblock', $targetblock);

  $target=$targetblock_wrapper->target->value();
  list($targetview_reorder_view_name,$targetview_reorder_display)=_dpidestinations_variable_get("dpidestinations_targetview_reorder_default",array("targetview_reorder_default","targetview1"));
  $target->view_name=$targetview_reorder_view_name;
  $target->view_display=$targetview_reorder_display;

  $target_wrapper = entity_metadata_wrapper('target', $target);

  //We use the targetview_reorder view to ensure we got the proper information.
  $targetview=$target_wrapper->targetview->value();

  //We double the number of rows and force the use of pager
  $targetblock->node_count_for_targetblockinstance=2*$targetblock->node_count_for_targetblockinstance;
  $targetblock->use_pager=TRUE;

  $term_value=$term->tid;
  $target_value=$target->machine_name;

  
  //add title

  $title_view_field['id'] = 'title';
  $title_view_field['table'] = 'node';
  $title_view_field['field'] = 'title';
  
  $title_view_field['label'] = '';
  $title_view_field['alter']['word_boundary'] = FALSE;
  $title_view_field['alter']['ellipsis'] = FALSE;
  
  $targetview->set_item($targetview->current_display , 'field', 'title', $title_view_field);
  
  _dpidestinations_targetview_initialize_view($targetview, $term_value, $target_value, $targetblock);
  
  
  return $targetview;
}

/**
 * initialize a targetview
 *
 * Set tid and target filter and define
 *  - offest
 *  - items_per_page
 *  - the use of a pager
 *
 * @param unknown $targetview
 * @param unknown $term_value
 * @param unknown $target_value
 * @param unknown $targetblock
 */
function _dpidestinations_targetview_initialize_view(&$targetview, $term_value, $target_value, $targetblock) {
  //@todo: documenter ces arguments dans la doc technique
  $targetview->targetview_tid=$term_value;
  $targetview->targetview_target=$target_value;  

  $filter_term=_dpidestinations_targetview_initialize_view_get_term_filter($term_value);
  $filter_target=_dpidestinations_targetview_initialize_view_get_target_filter($target_value);

  if($targetblock->use_pager){
    $pager =  $targetview->display_handler->get_option('pager');
    $pager['type'] = $targetblock->pager_type;
    $pager['options']['items_per_page']=$targetblock->node_count_for_targetblock;
    //We dont need offset since we execute the view only once
    //$pager['options']['offset']=$targetblock->offset;
    $targetview->display_handler->set_option('pager', $pager);
  }else{
    $pager =  $targetview->display_handler->get_option('pager');
    $pager['type'] = 'some';
    $pager['options']['items_per_page']=$targetblock->node_count_for_targetblock;
    //We dont need offset sinc we execute the view only once
    //$pager['options']['offset']=$targetblock->offset;
    $targetview->display_handler->set_option('pager', $pager);

    $targetview->set_items_per_page($targetblock->node_count_for_targetblock);
    //$targetview->set_offset($targetblock->offset);
  }
  $targetview->dpidestinations['destination'] = new stdClass();
  $targetview->dpidestinations['destination']->tid=$term_value;
  $targetview->dpidestinations['target'] = new stdClass();
  $targetview->dpidestinations['target']->machine_name=$target_value;
  
  $targetview->set_item($targetview->current_display , 'filter', 'dpidestination_field_destinations_target', $filter_target);
  $targetview->set_item($targetview->current_display , 'filter', 'dpidestination_field_destinations_tid', $filter_term);
}

/**
 * Return the structure of the filter to filter results within a targetview for a specific destination
 * This return the filter structure of the target
 *
 * @param String $target_value
 * the target machine_name
 *
 * @return multitype:string boolean multitype:string boolean number multitype:  multitype:string unknown  multitype:boolean string multitype:number
 */
function   _dpidestinations_targetview_initialize_view_get_target_filter($target_value){
  return array (
    'id' => 'field_destinations_target',
    'table' => 'field_data_field_destinations',
    'field' => 'field_destinations_target',
    'relationship' => 'none',
    'group_type' => 'group',
    'ui_name' => '',
    'operator' => '=',
    'value' => $target_value,
    'group' => '1',
    'exposed' => false,
    'expose' => array (
      'operator_id' => false,
      'label' => '',
      'description' => '',
      'use_operator' => false,
      'operator' => '',
      'identifier' => '',
      'required' => false,
      'remember' => false,
      'multiple' => false,
      'remember_roles' => array (
        2 => 2,
      ),
    ),
    'is_grouped' => false,
    'group_info' => array (
      'label' => '',
      'description' => '',
      'identifier' => '',
      'optional' => true,
      'widget' => 'select',
      'multiple' => false,
      'remember' => 0,
      'default_group' => 'All',
      'default_group_multiple' => array (),
      'group_items' => array (),
    ),
  );
}

/**
 * Return the structure of the filter to filter results within a targetview for a specific destination
 *
 * @param Integer $term_value
 * the tid of the destination term
 * @return multitype:string boolean multitype:string boolean number multitype:  multitype:string unknown  multitype:boolean string multitype:number
 */
function   _dpidestinations_targetview_initialize_view_get_term_filter($term_value){
  return array (
    'id' => 'field_destinations_tid',
    'table' => 'field_data_field_destinations',
    'field' => 'field_destinations_tid',
    'relationship' => 'none',
    'group_type' => 'group',
    'ui_name' => '',
    'operator' => '=',
    'value' => array (
      'min' => '',
      'max' => '',
      'value' => $term_value,
    ),
    'group' => '1',
    'exposed' => false,
    'expose' => array (
      'operator_id' => false,
      'label' => '',
      'description' => '',
      'use_operator' => false,
      'operator' => '',
      'identifier' => '',
      'required' => false,
      'remember' => false,
      'multiple' => false,
      'remember_roles' => array (
        2 => 2,
      ),
    ),
    'is_grouped' => false,
    'group_info' => array (
      'label' => '',
      'description' => '',
      'identifier' => '',
      'optional' => true,
      'widget' => 'select',
      'multiple' => false,
      'remember' => 0,
      'default_group' => 'All',
      'default_group_multiple' => array (),
      'group_items' => array (),
    ),
  );
}

function __dpidestinations_get_selectoption_get_targets_for_sectionterm($term_id){
  if($term_id==''){
    return array(''=>'');
  }

  $options=array();
  $targets= _dpidestinations_get_targets_for_sectionterm($term_id);
  foreach($targets as $subtype=>$target){
    $options[$subtype]=$target->label;
  }

  return $options;
}

/**
 * Helpers function to build select options from availlable target views
 */
function __dpidestinations_dependant_target_viewname_and_display_options_for_view_name($include_blank=TRUE){
  $target_views=_dpidestinations_get_all_targetviews();

  $options=array();
  foreach($target_views as $target_view_name=> $target_view ){
    $options[$target_view_name]=$target_view['label'];
  }

  if($include_blank){
    $options=array_merge(array(NULL=>t('Choose a targetview')),$options);
  }

  return $options;
}

/**
 * Helpers function to build select options from availlable target display for a view
 */
function __dpidestinations_dependant_target_viewname_and_display_options_for_view_display($view_name=NULL){
  $target_views=_dpidestinations_get_all_targetviews();

  $options=array();

  if($view_name==NULL or $view_name=="" or !isset($target_views[$view_name])){
    $options['']=t('Choose a view name first');
    return $options;
  }
  else{
    foreach($target_views[$view_name]['displays'] as $display){
      $options[$display['display_name']]=$display['label'];
    }
  }

  return $options;
}

function __dpidestinations_api_get_all_layouts_available_for_sectiontid_and_target($target_id,$target_name){
  if($target_id='' || $target_name==NULL) {
    return array(""=>"");
  }

  $controller=new TargetController('target');
  $target=$controller->load_target_by_name($target_name);
  $wrapper = entity_metadata_wrapper('target', $target);

  return $wrapper->available_targetlayouts->value();
}

function __dpidestinations_api_get_all_ranks_available_for_sectiontid_and_target($term_id, $target_name, $is_update = FALSE) {
  if($term_id == "") {
    return array('' => '');
  }

  $rank_options = array();

  if($is_update) {
    $rank_options['DNC'] = t('Dont change the rank');
  }
  $rank_options['default'] = t('Default behavior');

  $options = dpidestinations_api_get_all_ranks_available_for_sectiontid_and_target($term_id, $target_name, $is_update);
  return $rank_options + $options;
}

/**
 *  Return the info for destination field
 *
 *  This is used when we need to add destinations 's features to a bundle.
 *
 *  @see dpidestination_install()
 *  @see _dpidestination_get_field_info_for_destination()
 *  @see _dpidestination_get_instance_field_info_for_destination()
 */
function _dpidestination_attach_field_destination_to_bundle($entity_type,$bundle){
  $field_info_instance=_dpidestination_get_instance_field_info_for_destination($entity_type,$bundle);
  field_create_instance($field_info_instance);
}

/**
 *  Return the info for destination field
 *
 *  This is used when we need to add destinations 's features to a bundle.
 *
 *  @see dpidestination_install()
 *  @see _dpidestination_get_field_info_for_destination()
 *  @see _dpidestination_get_instance_field_info_for_destination()
 */
function _dpidestination_get_field_info_for_destination(){
  return array(
    'translatable' => '0',
    'entity_types' => array (
    ),
    'settings' => array (
    ),
    'storage' => array (
      'type' => 'field_sql_storage',
      'settings' => array (
      ),
      'module' => 'field_sql_storage',
      'active' => '1',
      'details' => array (
        'sql' => array (
          'FIELD_LOAD_CURRENT' => array (
            'field_data_field_destinations' => array (
              'tid' => 'field_destinations_tid',
              'target' => 'field_destinations_target',
              'layout' => 'field_destinations_layout',
              'rank' => 'field_destinations_rank',
              'virtual_rank' => 'field_destinations_virtual_rank',
            ),
          ),
          'FIELD_LOAD_REVISION' => array (
            'field_revision_field_destinations' => array (
              'tid' => 'field_destinations_tid',
              'target' => 'field_destinations_target',
              'layout' => 'field_destinations_layout',
              'rank' => 'field_destinations_rank',
              'virtual_rank' => 'field_destinations_virtual_rank',
            ),
          ),
        ),
      ),
    ),
    'foreign keys' => array (
    ),
    'indexes' => array (
      'tid' => array (
        0 => 'tid',
      ),
      'virtual_rank' => array (
        0 => 'virtual_rank',
      ),
    ),
    'field_name' => 'field_destinations',
    'type' => 'destination',
    'module' => 'dpidestinations',
    'active' => '1',
    'locked' => '0',
    'cardinality' => '-1',
    'deleted' => '0',
    'columns' => array (
      'tid' => array (
        'type' => 'int',
        'size' => 'medium',
        'not null' => true,
      ),
      'target' => array (
        'type' => 'varchar',
        'length' => 254,
        'not null' => false,
      ),
      'layout' => array (
        'type' => 'varchar',
        'length' => 254,
        'not null' => false,
      ),
      'rank' => array (
        'type' => 'int',
        'size' => 'medium',
        'not null' => false,
      ),
      'virtual_rank' => array (
        'type' => 'int',
        'size' => 'big',
        'not null' => false,
      ),
    ),
    'bundles' => array (
      'node' => array (
        0 => 'package',
      ),
    ),
  );
}

/**
 *  Return the info for destination field
 *
 *  This is used when we need to add destinations 's features to a bundle.
 *
 *  @see dpidestination_install()
 *  @see _dpidestination_get_field_info_for_destination()
 *  @see _dpidestination_get_instance_field_info_for_destination()
 */
function _dpidestination_get_instance_field_info_for_destination($entity_type,$bundle){
  return array('label' => 'Destinations',
    'widget' => array (
      'weight' => '28',
      'type' => 'destination_default_widget',
      'module' => 'dpidestinations',
      'active' => 0,
      'settings' => array (),
    ),
    'settings' => array (
      'user_register_form' => false,
    ),
    'display' => array (
      'default' => array (
        'label' => 'above',
        'type' => 'destination_default_formatter',
        'settings' => array (),
        'module' => 'dpidestinations',
        'weight' => 35,
      ),
      'teaser' => array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array (),
        'weight' => 0,
      ),
      'targetblock_small' => array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array (),
        'weight' => 0,
      ),
      'targetblock_medium' => array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array (),
        'weight' => 0,
      ),
      'targetblock_big' => array (
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array (),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_destinations',
    'entity_type' => $entity_type,
    'bundle' => $bundle,
    'deleted' => '0',
  );
}

/**
 * Get the view mode of a node in a destination from a views row result
 *
 * @param object $view
 *   The view
 * @param object $row
 *   The views row
 *
 * @return string | bool
 *   The view mode or FALSE in case of failure
 */
function _dpidestinations_get_view_mode_from_views_row($view, $entity, $entity_type) {
  $view_mode = FALSE;

  list($eid, $revision, $bundle) = entity_extract_ids($entity_type, $entity);

  // Get destinations from entity
  static $static_destinations = array();
  if (!isset($static_destinations[$eid])) {
    $destinations = dpicommons_get_field_from_entity($entity, $entity_type, 'field_destinations');
    $static_destinations[$eid] = $destinations;
  } else {
    $destinations = $static_destinations[$eid];
  }

  if ($destinations) {
    if (isset($view->targetview_tid) && isset($view->targetview_target)) {
      $tid = $view->targetview_tid;
      $target = $view->targetview_target;

      // Get view mode from destination and add suggestions
      foreach ($destinations as $destination) {
        if ($destination['tid'] == $tid && $destination['target'] == $target) {
          $view_mode = $destination['layout'];
          break;
        }
      }
    }

    return $view_mode;
  }
  
}


