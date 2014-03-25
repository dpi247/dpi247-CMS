<?php 

class Prenode {
  
  protected $id;
  protected $node; 
  
  function Prenode($id = NULL){
    if ($id == NULL){
      $this->id = uniqid();
    } else {
      $this->id = $id;
      $this->getcachednode();
    } 
  }
  
  /**
   * Get node from cache
   */
  protected function getcachednode(){
    $result = db_query("SELECT data FROM {dpicontenttypes_prenode_object_cache} WHERE id = :id", array(':id' => $this->id));
    foreach ($result as $record){
      $this->node = unserialize($record->data);
      break;
    }
  }
  
  /**
   * 
   * Add node to prenode
   * @param Object $node
   */
  public function setNode($node){
    $this->node = $node;
  }
  
  /**
   * 
   * Get node from prenode
   */
  public function getNode(){
    return $this->node;
  }
  
  /**
   * Save prenode in db
   */
  public function save(){

    //Save prenode id and title in prenode table
    db_merge('dpicontenttypes_prenode')
    ->key(array('id' => $this->id))
    ->fields(array(
      'updated' => time(),
      'title' => $this->node->title,
    ))
    ->execute();
    
    //Save prenode id and prenode object in prenode cache table
    db_merge('dpicontenttypes_prenode_object_cache')
    ->key(array('id' => $this->id))
    ->fields(array(
      'updated' => time(),
      'data' => serialize($this->node),
    ))
    ->execute();
  }
  
  /**
   * 
   * Produce the render of the prenode and save it to the database
   */
  public function nodeRender(){
    //generate node html
    $menu_rooter = menu_get_router();
    $callback = $menu_rooter['node/%']['page callback'];
    
    //trouver comment faire le require once correctement
    if ($menu_rooter['node/%']['include file'] != NULL){
      require_once DRUPAL_ROOT . '/' . $menu_rooter['node/%']['include file'];
    }
    $node = $this->node;
    
    //to avoid fatal error during the insertion in node history
    $node->nid = 0;
    $page_callback_result = call_user_func_array($callback, array($node));
    $render = drupal_render($page_callback_result);
    
    $this->saveRender($render);
  }
  
  /**
   * 
   * Save the render in the database
   * @param String $render
   */
  protected function saveRender($render){   
    //Save prenode id and prenode object in prenode cache table
    db_merge('dpicontenttypes_prenode_html_cache')
    ->key(array('id' => $this->id))
    ->fields(array(
          'updated' => time(),
          'data' => $render
    ))
    ->execute();
  }
  
  /**
   * 
   * Produce de render of a destination of the prenode and save it to the database
   * @param Integer $destination
   */
  public function destinationRender($destination){
    $task = page_manager_get_task('destinations');

    // Load the term into a context.
    ctools_include('context');
    ctools_include('context-task-handler');
    $contexts = ctools_context_handler_get_task_contexts($task, '', array($destination));
    
    $preview_context = new stdClass();
    $preview_context->wallyedit_preview = array();
    $preview_context->wallyedit_preview['key'] = $destination;
    $preview_context->wallyedit_preview['cache name'] = $this->id;
    $preview_context->wallyedit_preview['node'] = $this->getNode();
    $contexts['wallyedit_preview'] = $preview_context;
    
    
    $output = ctools_context_handler_render($task, '', $contexts, array(array($destination), 0, 'page'));
    // Hide messages before return
    //drupal_get_messages();
    //generate destinations html*/
  }
}