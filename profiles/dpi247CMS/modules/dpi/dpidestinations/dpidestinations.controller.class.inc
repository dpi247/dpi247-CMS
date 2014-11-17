<?php

class TargetController extends EntityAPIControllerExportable {

  public function create(array $values = array()) {
    $values += array (
      'targetid' => '',
      'machine_name' => '',
      'label' => '',
      'description' => '',
      'view_name' => '',
      'view_display' => '',
      'available_targetlayouts' => serialize ( array () ),
      'available_templateoverrides' => '' 
    );
    return parent::create ( $values );
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper ( 'target', $entity );
    
    // Make Description and Status themed like default fields.
    $content ['description'] = array (
      '#theme' => 'field',
      '#weight' => 0,
      '#title' => t ( 'Description' ),
      '#access' => TRUE,
      '#label_display' => 'above',
      '#view_mode' => 'full',
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#entity_type' => 'target',
      '#bundle' => 'target',
      '#items' => array (
        array (
          'value' => $entity->description 
        ) 
      ),
      '#formatter' => 'text_default',
      0 => array (
        '#markup' => check_plain ( $entity->description ) 
      ) 
    );
    
    return parent::buildContent ( $entity, $view_mode, $langcode, $content );
  }

  public function load_target_by_name($target_name) {
    $conditions = array (
      'machine_name' => $target_name 
    );
    $result = target_load_multiple ( FALSE, $conditions );
    if (! $result) {
      $result = NULL;
    }
    return current ( $result );
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * @param $transaction Optionally
   *          a DatabaseTransaction object to use. Allows overrides to pass
   *          in their transaction object.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    // Delete related targetblocks
    foreach ( $ids as $id ) {
      $targetblock_controller = entity_get_controller ( 'targetblock' );
      $targetblocks = $targetblock_controller->load_targetblocks_by_target ( $id );
      $targetblocks_ids = array ();
      foreach ( $targetblocks as $targetblock ) {
        $targetblocks_ids [] = $targetblock->targetblockid;
      }
      $targetblock_controller->delete ( $targetblocks_ids );
    }
    
    return parent::delete ( $ids, $transaction );
  }
}

class TargetBlockController extends EntityAPIControllerExportable {

  public function create(array $values = array()) {
    global $user;
    $values += array (
      'use_pager' => FALSE,
      'use_split' => FALSE,
      'split_delta' => 0,
      'offset' => 0 
    );
    
    return parent::create ( $values );
  }
  
  // return an array keyed with tid of targetblocks that belongs to a display id
  public function load_targetblocks_by_panels_display($did) {
    $conditions = array (
      'did' => $did 
    );
    return targetblock_load_multiple ( FALSE, $conditions );
  }

  public function load_targetblock_by_panels_panelpid($pid) {
    $conditions = array (
      'pid' => $pid 
    );
    $result = current ( targetblock_load_multiple ( FALSE, $conditions ) );
    if (! $result) {
      $result = FALSE;
      // $result=new TargetBlockEntity(array(),'TargetBlock');
    }
    return $result;
  }

  public function load_targetblock_by_did_and_target($did, $target_name) {
    $results = $this->load_targetblocks_by_did_and_target ( $did, $target_name );
    return current ( $results );
  }

  public function load_targetblock_by_did_target_and_splitdelta($did, $target_name, $split_delta) {
    $conditions = array (
      'did' => $did,
      ' target' => $target_name,
      'split_delta' => $split_delta 
    );
    $results = targetblock_load_multiple ( FALSE, $conditions );
    $result = current ( $results );
    if (! $result) {
      $result = NULL;
    }
    return $result;
  }

  public function load_targetblock_by_termid_and_target($term_id, $target_name) {
    
    // @todo: WARNING this function does not handle Ctols exportable ...
    // so we are stuck right after install process :/
    $did = dpidestinations_api_get_panelsdisplayid_for_sectionterm ( $term_id );
    return $this->load_targetblock_by_did_and_target ( $did, $target_name );
  }

  public function load_targetblocks_by_target($target_name) {
    $conditions = array (
      'target' => $target_name 
    );
    $results = targetblock_load_multiple ( FALSE, $conditions );
    if (! $results) {
      $results = array ();
    }
    return $results;
  }

  public function load_targetblocks_by_did_and_target($did, $target_name) {
    $conditions = array (
      'did' => $did,
      'target' => $target_name 
    );
    $results = targetblock_load_multiple ( FALSE, $conditions );
    if (! $results) {
      $results = array ();
    }
    return $results;
  }

  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $this->syncrohize_global_settings_of_targetblocks ( $ids, 'delete' );
    
    $targetblocks_entities = entity_load ( 'targetblock', $ids );
    // Delete related targetblocks instances
    foreach ( $targetblocks_entities as $targetblocks_entity ) {
      db_delete ( 'panels_pane' )->condition ( 'pid', $targetblocks_entity->pid )->execute ();
    }
    
    $result = parent::delete ( $ids, $transaction );
    // _dpidestinatons_targetblock_sync_node_count_for_targetblock($entity);
    return $result;
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $this->syncrohize_global_settings_of_targetblocks ( $entity, 'save' );
    
    $result = parent::save ( $entity );
    return $result;
  }

  public function syncrohize_global_settings_of_targetblocks($entity, $operation) {
    if ($entity && is_object ( $entity ) && ! isset ( $entity->prevent_sync_recurssion )) {
      $original_entity_id = isset ( $entity->targetblockid ) ? $entity->targetblockid : NULL;
      
      if (! isset ( $entity->targetblockid ) || ! $entity->targetblockid) {
        $entity->targetblockid = "new";
      }
      
      if (isset ( $entity->did ) && isset ( $entity->target )) {
        $targetblocks = $this->load_targetblocks_by_did_and_target ( $entity->did, $entity->target );
        
        if ($operation == 'save') {
          // We replace the existing $entity in DB by the new one
          unset ( $targetblocks [$entity->targetblockid] );
          $targetblocks [$entity->targetblockid] = $entity;
        } elseif ($operation == "delete") {
          unset ( $targetblocks [$entity->targetblockid] );
        }
        
        if (count ( $targetblocks ) > 1) {
          $total_node_count = 0;
          foreach ( $targetblocks as $targetblok ) {
            $total_node_count += $targetblok->node_count_for_targetblockinstance;
            $order [$targetblok->targetblockid] = $targetblok->split_delta;
          }
          asort ( $order );
          $split_delta = 0;
          $offset = 0;
          foreach ( $order as $targetblockid => $order ) {
            $targetblocks [$targetblockid]->node_count_for_targetblock = $total_node_count;
            $targetblocks [$targetblockid]->use_split = TRUE;
            $targetblocks [$targetblockid]->split_delta = $split_delta;
            $targetblocks [$targetblockid]->offset = $offset;
            $targetblocks [$targetblockid]->prevent_sync_recurssion = TRUE;
            $split_delta = $split_delta + 1;
            $offset += $targetblocks [$targetblockid]->node_count_for_targetblockinstance;
            
            // no need to save the acutal entity object cause il will be saved right after the if method
            if ($targetblockid != $original_entity_id) {
              targetblock_save ( $targetblocks [$targetblockid] );
            }
          }
        } else {
          $entity->use_split = FALSE;
          $entity->node_count_for_targetblock = $entity->node_count_for_targetblockinstance;
          $entity->split_delta = 0;
          $entity->offset = 0;
        }
        
        if ($entity->targetblockid == "new") {
          unset ( $entity->targetblockid );
        }
      }
    }
  }
}
