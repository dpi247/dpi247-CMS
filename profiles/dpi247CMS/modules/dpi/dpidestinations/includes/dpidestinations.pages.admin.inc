<?php

/**
 * @file
 * Group membersihp type editing UI.
 */

/**
 * UI controller.
 */
class DpidestinationsTargetUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    //Since we use the same controller
    $items[$this->path]['description'] = 'Manage group memberships type, including fields.';
    if(isset($items['admin/dpi247/dpidestinations/target'])){
      $items['admin/dpi247/dpidestinations/target']['type']=MENU_LOCAL_TASK;
      $items['admin/dpi247/dpidestinations/target']['weight']=0;
    }
    if(isset($items['admin/dpi247/dpidestinations/targetblock'])){
      $items['admin/dpi247/dpidestinations/targetblock']['type']=MENU_LOCAL_TASK;
      $items['admin/dpi247/dpidestinations/targetblock']['weight']=9;

    }
    return $items;
  }

  /**
   * Builds the entity overview form.
   */
  public function overviewForm($form, &$form_state) {
    $form = parent::overviewForm($form, $form_state);
    return $form;
  }
}

/**
 * @file
 * Group membersihp type editing UI.
 */

/**
 * UI controller.
 */
class DpidestinationsTargetblockUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    //Since we use the same controller
    $items[$this->path]['description'] = 'Manage group memberships type, including fields.';
    if(isset($items['admin/dpi247/dpidestinations/targetblock'])){
      $items['admin/dpi247/dpidestinations/targetblock']['type']=MENU_LOCAL_TASK;
      $items['admin/dpi247/dpidestinations/targetblock']['weight']=9;

    }
    unset($items['admin/dpi247/dpidestinations/targetblock/add']);
    return $items;
  }
  
  /**
   * Builds the entity overview form.
   */
  public function overviewForm($form, &$form_state) {
    //FAKE the exportable behavior cause it is confusing...
    $this->entityInfo['exportable']=FALSE;
    
    if ($filters = $this->overviewFilters()) {
      $form['filters'] = $this->buildFiltersForm($filters);
    }
    // By default just show a simple overview for all entities.
    $form['table'] = $this->overviewTable();
    
    /* Suppression de l'édition */
    foreach($form['table']['#rows'] as $k => $row){
      unset($form['table']['#rows'][$k][7]);
    }
    
    $form['pager'] = array('#theme' => 'pager');       
    return $form;
  }
  
  /**
   * Generates the render array for the overview table for arbitrary entities
   * matching the given conditions.
   *
   * @param $conditions
   *   An array of conditions as needed by entity_load().
   *
   * @return array
   *   A renderable array.
   */
  public function overviewTable($conditions = array()) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->entityType);

    // Add all conditions to query.
    foreach ($conditions as $key => $value) {
      $query->propertyCondition($key, $value);
    }

    if ($this->overviewPagerLimit) {
      $query->pager($this->overviewPagerLimit);
    }
    //$query->tableSort($headers);

    // Add filters if there are any set.
    $filter_data = isset($_SESSION[$this->entityType . '_overview_filter']) ? $_SESSION[$this->entityType . '_overview_filter'] : array();
    foreach ($filter_data as $filter) {
      list($key, $value) = $filter;
      $query->propertyCondition($key, $value);
    }

    $results = $query->execute();

    $ids = isset($results[$this->entityType]) ? array_keys($results[$this->entityType]) : array();
    $entities = $ids ? entity_load($this->entityType, $ids) : array();
    ksort($entities);

    ctools_include('export');
    $handlers = ctools_export_load_object('page_manager_handlers');
    $rows = array();
    $additional_rows = array(
      t('Variant') => '',
      t('Target') => '',
      t('# nodes') => 0,
      t('Use Pager') => 'No',
      t('# nodes in split') => 0,
      t('Rank max value') => 0,
    );
    foreach ($entities as $entity) {
      $page_manager_handler_name=_dpidestinations_get_pagemanager_handler_name_for_displayid($entity->did);
      $title = t('undefined');
      if(isset($page_manager_handler_name->name) && isset($handlers[$page_manager_handler_name->name])) {
        $title_text = $handlers[$page_manager_handler_name->name]->conf['title'];
        $title_link = "admin/structure/pages/nojs/operation/sections/handlers/".$page_manager_handler_name->name."/content";
        $title = l($title_text, $title_link);
      }

      $target = $entity->target;
      $nodes_in_split = '-';
      if($entity->use_split) {
        $split = $entity->split_delta + 1;
        $target .= " ( split ".$split .")";
        $nodes_in_split = $entity->node_count_for_targetblockinstance;
      }

      $additional_rows = array(
        t('Variant') => $title,
        t('Target') => $target,
        t('# nodes') => $entity->node_count_for_targetblock,
        t('Use Pager') => ($entity->use_pager) ? t('No') : $entity->pager_type,
        t('# nodes in split') => $nodes_in_split,
        t('Rank max value') => $entity->rank_max_value,
      );

      $row = $this->overviewTableRow($conditions, entity_id($this->entityType, $entity), $entity, $additional_rows);
      //We unset the label cause it has no sence here.
      unset($row[0]);
      $rows[] = $row;
    }

    $header = $this->overviewTableHeaders($conditions, $rows, array_keys($additional_rows));
    //We unset the label cause it has no sence here.
    unset($header[0]);

    $render = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('None.'),
    );
    return $render;
  }

  /**
   * Create the form used for applying a set of filters to the overview list.
   */
  public function buildFiltersForm($filters) {
    $session = isset($_SESSION[$this->entityType . '_overview_filter']) ? $_SESSION[$this->entityType . '_overview_filter'] : array();

    $form = array(
      '#type' => 'fieldset',
      '#title' => $this->filtersLabel(),
      '#theme' => 'exposed_filters__entityui',
    );

    $i = 0;
    foreach ($session as $filter) {
      list($type, $value) = $filter;
      $value = $filters[$type]['options'][$value];

      $t_args = array('%property' => $filters[$type]['title'], '%value' => $value);
      if ($i++) {
        $form['current'][] = array('#markup' => t('and where %property is %value', $t_args));
      }
      else {
        $form['current'][] = array('#markup' => t('where %property is %value', $t_args));
      }

      if (in_array($type, array('type', 'status'))) {
        // Remove the option if it is already being filtered on.
        unset($filters[$type]);
      }
    }

    $form['status'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('clearfix')),
      '#prefix' => ($i ? '<div class="additional-filters">' . t('and where') . '</div>' : ''),
    );

    $form['status']['filters'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('filters')),
    );

    foreach ($filters as $key => $filter) {
      if(!$this->isFilterAlreadySet($key)){
        $form['status']['filters'][$key] = array(
          '#type' => 'select',
          '#options' => $filter['options'],
          '#title' => $filter['title'],
          '#default_value' => '[any]',
        );
      }
    }

    $form['status']['actions'] = array(
      '#type' => 'actions',
      '#attributes' => array('class' => array('container-inline')),
    );

    $form['status']['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => count($session) ? t('Refine') : t('Filter'),
      '#submit' => array('entityadminui_filter_form_submit'),
    );

    if (count($session)) {
      $form['status']['actions']['undo'] = array(
        '#type' => 'submit',
        '#value' => t('Undo'),
        '#submit' => array('entityamdinui_filter_form_submit'),
      );

      $form['status']['actions']['reset'] = array(
        '#type' => 'submit',
        '#value' => t('Reset'),
        '#submit' => array('entityadminui_filter_form_submit'),
      );
    }

    drupal_add_js('misc/form.js');

    return $form;
  }

  /**
   * Label to display above filters form on overview page.
   *
   * @return string
   */
  public function filtersLabel() {
    return t('Show only @type where', array('@type' => $this->entityInfo['label']));
  }

  /**
   * Get a list of filters that can be used to narrow the overview list.
   *
   * Currently this will only work with properties and not fields.
   *
   * @return array
   *   An array of filter options keyed by property name with the following
   *   sub array.
   *    'title': Label to be used on the form field.
   *    'options': An array of options for use with a #type = select list.
   */
  public function overviewFilters() {
    $filters = $this->overviewPropertyList();
    $filters = array_filter($filters, 'is_array');
    return $filters;
  }

  public function overviewPropertyList() {
    $session = isset($_SESSION[$this->entityType . '_overview_filter']) ? $_SESSION[$this->entityType . '_overview_filter'] : array();

    $info = array();
    // Add the label field.
    $info[$this->entityInfo['entity keys']['label']] = NULL;
    // Add the ID field.
    $info[$this->entityInfo['entity keys']['id']] = NULL;

    if (isset($this->entityInfo['bundle keys']['bundle']) && isset($this->entityInfo['bundles'])) {
      $bundles = array();
      foreach ($this->entityInfo['bundles'] as $key => $value) {
        $bundles[$key] = $value['label'];
      }
      $info[$this->entityInfo['entity keys']['bundle']] = array(
        'title' => t('Bundle'),
        'options' => array(
          '[any]' => t('any'),
        ) + $bundles,
      );
    }

    $targets=dpidestinations_api_get_all_targets();
    $targets_options = array();
    foreach($targets as $target){
      $targets_options[$target->machine_name]=$target->label;
    }

    $info["target"]=array(
      'title' => t('Target'),
      'options' => array(
        '[any]' => t('any'),
      )+$targets_options,
    );

    ctools_include('export');
    $handlers = ctools_export_load_object('page_manager_handlers');

    $targetblocks = dpidestinations_api_get_all_targetblocks();
    $variants_options = array();
    foreach($targetblocks as $targetblock) {
      $page_manager_handler_name=_dpidestinations_get_pagemanager_handler_name_for_displayid($targetblock->did);
      if (isset($page_manager_handler_name->name)){
        $title_text=$handlers[$page_manager_handler_name->name]->conf['title'];
      }else{
        $title_text = "Undefined";
      }
      $variants_options[$targetblock->did] = $title_text;
    }

    $info["did"] = array(
      'title' => t('Variant'),
      'options' => array(
        '[any]' => t('any'),
      ) + $variants_options,
    );
    return $info;
  }

  /**
   * Check if a filter is already set in the session
   *
   * Return true if filter already set, FALSE otherwise
   */
  public function isFilterAlreadySet($filter_name) {
    $session = isset($_SESSION[$this->entityType . '_overview_filter']) ? $_SESSION[$this->entityType . '_overview_filter'] : array();

    foreach($session as $filter){
      if($filter[0]==$filter_name){
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Handle submission of the filters form.
   *
   * Save submitted data into a session for use later in filtering.
   */
  public function filtersFormSubmit($form, &$form_state) {
    $filters = $this->overviewFilters();
    switch ($form_state['values']['op']) {
      case t('Filter'):
      case t('Refine'):
        // Apply every filter that has a choice selected other than 'any'.
        foreach ($filters as $filter => $options) {
          if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
            // Flatten the options array to accommodate hierarchical/nested options.
            $flat_options = form_options_flatten($filters[$filter]['options']);
            // Only accept valid selections offered on the dropdown, block bad input.
            if (isset($flat_options[$form_state['values'][$filter]])) {
              $_SESSION[$this->entityType . '_overview_filter'][] = array($filter, $form_state['values'][$filter]);
            }
          }
        }
        break;

      case t('Undo'):
        array_pop($_SESSION[$this->entityType . '_overview_filter']);
        break;

      case t('Reset'):
        $_SESSION[$this->entityType . '_overview_filter'] = array();
        break;
    }
  }
}

function dpidestinations_page_administration_targets(){
  return t('You can add/manage field for the target entity here. Do that only if you exactly know what you are doing :)');
}

function dpidestinations_page_administration_targetblocks(){
  return t('You can add/manage field for the targetblock entity here. Do that only if you exactly know what you are doing :)');
}

function dpidestinations_page_administration_destinations(){
  module_load_include("inc", "taxonomy","taxonomy.admin");
  $sections_vid=dpidestinations_api_get_sections_vocabulary_vid();
  $sections=entity_load_unchanged('taxonomy_vocabulary',$sections_vid);
  return drupal_get_form('taxonomy_overview_terms',$sections);
}

function dpidestinations_page_administration_configuration(){
  // @todo : quel est l'intérêt des 4 premières lignes?
  $virtual_rank =  _dpidestinations_variable_get('dpidestinations_virtualrank_delta',1000)* ($rank_max_value+1);
  list($targetview_reorder_view_name,$targetview_reorder_display)=_dpidestinations_variable_get("dpidestinations_targetview_reorder_default",array("targetview_reorder_default","targetview1"));
  _dpidestinations_variable_get('dpidestinations_destination_vid',2);
  _dpidestinations_variable_get('dpidestination_max_reserved_position',10);

  return drupal_get_form('dpidestinations_form_configuration_form');
}

function dpidestinations_form_configuration_form($form,& $form_state){
  $form=array();

  $form["config_form"]=array(
    '#type'=>'vertical_tabs',
  );

  $form["default"]=array(
    '#type'=>'fieldset',
    '#title' => t('Default'),
    '#collapsible' => TRUE,
    '#description' => t('Default config for destinations.'),
    '#group' => 'config_form'
  );

  $form["advanced"]=array(
    '#type'=>'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#description' => t('Advanced config for destinations.<br/> Edit thoses settings with care'),
    '#group' => 'config_form'
  );

  $vocabularies=taxonomy_get_vocabularies();
  foreach($vocabularies as $vocabulary){
    $options[$vocabulary->vid]=$vocabulary->name;
  }
  $form["default"]['dpidestinations_section_vid']=array(
    "#type"=>'select',
    '#title' => t('Section vocabulary'),
    '#description' => t('We do not recommend to change this settings in production.'),
    '#options'=>$options,
    '#default_value'=>_dpidestinations_variable_get('dpidestinations_section_vid',2),
  );

  $form["default"]['dpidestination_max_reserved_position']=array(
    "#type"=>'textfield',
    "#required"=>TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
    '#title' => t('Absoulte rank\'s max value'),
    '#description' => t('The rank mak value can be defined in each targetblock but cannot be bigger than the absolute rank max value'),
    '#default_value'=>_dpidestinations_variable_get('dpidestination_max_reserved_position',10),
  );

  $help="This value is used as a multiplicator in the timerank calculation when a package is placed on the first position of a destination<br/>";

  $form["advanced"]['dpidestinations_virtualrank_delta']=array(
    "#type"=>'textfield',
    "#required"=>TRUE,
    '#title' => t('Virtual rank delta'),
    '#description' => t($help),
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value'=>_dpidestinations_variable_get('dpidestinations_virtualrank_delta',1000),
  );

  list($targetview_reorder_view_name,$targetview_reorder_display)=_dpidestinations_variable_get("dpidestinations_targetview_reorder_default",array("targetview_reorder_default","targetview1"));

  $view_name_options = __dpidestinations_dependant_target_viewname_and_display_options_for_view_name();

  $selected_view_name = isset($form_state['values']['view_name']) ? $form_state['values']['view_name'] : $targetview_reorder_view_name;
  if ($selected_view_name == "" or $selected_view_name == NULL){
    $selected_view_name = key($view_name_options);
  }
  $selected_view_display = isset($form_state['values']['view_display']) ? $form_state['values']['view_display'] : $targetview_reorder_display;
  $form["advanced"]['reorder_view']=array(
    '#type'=>'fieldset',
    '#title' => t('Reorder view'),
  );

  $form["advanced"]['reorder_view']['view_name']=array(
    '#type' => 'select',
    '#title' => t('View Name'),
    '#options' => $view_name_options,
    '#default_value' => $selected_view_name,
    '#required' => TRUE,
    '#ajax' => array(
      // When 'event' occurs, Drupal will perform an ajax request in the
      // background. Usually the default value is sufficient (eg. change for
      // select elements), but valid values include any jQuery event,
      // most notably 'mousedown', 'blur', and 'submit'.
      // 'event' => 'change',
      'callback' => '_dpidestinations_ajaxcallback_viewname_viewdisplay_config',
      'wrapper' => 'dropdown-view-display',
    ),
  );

  $form["advanced"]['reorder_view']['view_display'] = array(
    '#type' => 'select',
    '#title' => t('View Display'),
    '#options' => __dpidestinations_dependant_target_viewname_and_display_options_for_view_display($selected_view_name),
    '#default_value' => $selected_view_display,
    '#required' => TRUE,
    '#prefix' => '<div id="dropdown-view-display">',
    '#suffix' => '</div>',
  );

  $form["submit"] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function dpidestinations_form_configuration_form_submit($form, &$form_state){

  $values=$form_state['values'];

  _dpidestinations_variable_set('dpidestinations_section_vid',$values['dpidestinations_section_vid']);
  _dpidestinations_variable_set('dpidestinations_virtualrank_delta',$values['dpidestinations_virtualrank_delta']);
  _dpidestinations_variable_set('dpidestination_max_reserved_position',$values['dpidestination_max_reserved_position']);
  _dpidestinations_variable_set('dpidestinations_targetview_reorder_default',array($values['view_name'],$values['view_display']));
}

function _dpidestinations_ajaxcallback_viewname_viewdisplay_config( $form, $form_state){
  return $form["advanced"]['reorder_view']['view_display'];
}
