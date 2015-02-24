<?php
/*
 * Reorder process
 * 
 * Technical documentation: <a href="/node/193">link text</a> 
 * 
 * 
 */

function dpidestinations_page_targetview_reorder($term,$target){

  $targetblock=NULL;
  $targetblocks=dpidestinations_api_get_all_targetblocks_from_target_for_destination($target->machine_name,$term->tid);
  
  
  // We can take the first targetblock because his node_count_for_targetblock 
  // is synchronised between all targetblock_instance for this target and destiantion 
  //
  // in case of splitted targetblock we always sum the node_count_for_targetblockinstance 
  // to deduce the node_count_for_targetblock)
  $targetblock=current($targetblocks);
  
  //We jute need to fake the node_count_for_targetblockinstance and the offset
  $targetblock->node_count_for_targetblockinstance=$targetblock->node_count_for_targetblock;
  $targetblock->offset=0;

  $targetview=_dpidestinations_targetviewreorder_initialize_view($targetblock,$term);
  $targetview->pre_execute();
  $targetview->execute();
  
  $nodes=$targetview->result;
  
  return drupal_get_form('dpidestinations_page_targetview_reorder_form',$nodes,$term->tid ,$targetblock->target);
  
}

function dpidestinations_page_targetview_reorder_form($form, &$form_state, $nodes, $sectiontid, $destination_target){
  $form['#tree'] = TRUE;
  
  $weight_delta = round(count($nodes) / 2);
  $weight_delta =count($nodes);
  $original_weight=0;
  //dsm($nodes,nodes);
  foreach($nodes as $node){
    $form['nodes'][$node->nid]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => 0,
      '#delta' => $weight_delta,
      '#title_display' => 'invisible',
      '#title' => t('Weight for @node ', array('@node' => $node->node_title)),
      '#attributes'=> array('class' => array('node-weight')),
    );
    $form['nodes'][$node->nid]['original_weight'] = array(
      '#type' => 'value',
      '#value' => $original_weight,
    );
    $form['nodes'][$node->nid]['nid'] = array(
      '#type' => 'value',
      '#value' => $node->nid,
    );

    $original_weight++;
    $form['nodes'][$node->nid]['title'] = array(
      '#markup' => $node->node_title,
    );
    
    $str_author = "";
    foreach($node->field_field_authors as $author){
      $str_author .= '<a href="/'.$author["rendered"]["#href"].'">'.$author["rendered"]["#title"].'</a> ';
    }
    $form['nodes'][$node->nid]['field_field_authors'] = array(
      '#markup' => (!empty($str_author))? $str_author : t("Undefined"),
    );
    
    $str_date = "";
    foreach($node->field_field_editorialpublicationdate as $date){
      $str_date .= $date["rendered"]["#markup"];
    }
    $form['nodes'][$node->nid]['field_field_editorialpublicationdate'] = array(
      '#markup' => $str_date,
    );
    
    $form['nodes'][$node->nid]['edit'] = array(
      '#markup' => '<a href="/node/'.$node->nid.'/edit">Edit</a>',
    );
  }
  
  $form['sectiontid'] = array(
    '#type' => 'value',
    '#value' =>  $sectiontid,
  );
  $form['destination_targetname'] = array(
    '#type' => 'value',
    '#value' => $destination_target,
  );
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  return $form;
  
}


function dpidestinations_page_targetview_reorder_form_submit(& $form, &$form_state){
  
  $old_values=array();
  foreach($form_state["values"]['nodes'] as $nid=>$t){
    $new_values[$nid]=$t['weight'];
    $old_values[$nid]=$t['original_weight'];
  }

    
  $order_by_weight = array();
  foreach($form_state['values']['nodes'] as $key => $data) {
    $order_by_weight[$data['weight']] = $data;
  }
  ksort($order_by_weight);
  foreach( $order_by_weight as $weight=>$data){
    $order_by_new_rank[]=$data;
  }
  
  
  /*
   * les changements dans le reorder se traduisent par le fait que la somme des
   * old_rank et des new_rank ne sont pas égale à boundaries.
   * 
   * Selon qu'on a descendu un noeud ou elevé un noeud on peut avoir un comportement différent 
   * et il est préferable (d'un point de vue performance) de ne considérer un des deux cas suivant
   * uniquement les noeuds dont la somme est inférieur à boundary
   * uniquement les noeuds dont la somme est supérieur à boundary.
   * 
   * On va donc d'abord analyser le nombre de noeuds impliqués dans chaque cas
   * et en fonction on choisira laquel met à jour le moins de noeud...
   * 
   * 
   */
  
  /*
   * 
    THIS IS PURE BULLSHIT

  $boundary=count($form_state["values"]['nodes']);
  foreach($order_by_new_rank as $rank=>$data){
    $sum=abs($data['weight'])+abs($data['original_weight']);
    if($sum>$boundary){
      $smaller_than_boundary[$rank]=$data;
    }
    if($sum<$boundary){
      $greater_than_boundary[$rank]=$data;
      
    }
    
  }
  
  if(count($greater_than_boundary)<count($smaller_than_boundary)){
    $nodes_to_process=$greater_than_boundary;
  }else{
    $nodes_to_process=$smaller_than_boundary;
  }
  
  $nodes_to_process=array_merge($smaller_than_boundary,$greater_than_boundary);
  */
  
$boundary=count($form_state["values"]['nodes']);
  foreach($order_by_new_rank as $rank=>$data){
    if(!($data['weight']==$data['original_weight'] and  $data['original_weight']==0)){
      $sum=abs($data['weight'])+abs($data['original_weight']);
      if($sum>$boundary){
        $nodes_to_process[$rank]=$data;
      }
      if($sum<$boundary){
        $nodes_to_process[$rank]=$data;

      }
    }
  }
  if(count($nodes_to_process)>0){

    $term_id= $form_state["values"]['sectiontid'];
    $target_name= $form_state["values"]['destination_targetname'];
    foreach($nodes_to_process as $new_rank=>$data){
      $position=$new_rank+1;
      $nid=$data['nid'];
      $virtual_rank=_dpidestinations_virtualrank_give_virtual_rank_for_position($term_id,$target_name,$position);
      $destinations_to_update[$nid]=$virtual_rank;

    }
    foreach($destinations_to_update as $nid=>$virtual_rank){
      //@todo: LE faire en une seule querry avec l'invalidation du cache une seule fois par nid
      _dpidestinations_virtualrank_update_virtualrank($nid, $term_id, $target_name, $virtual_rank,$nid);

    }
    $destination['tid']=$form_state['values']['sectiontid'];
    $destination['target']=$form_state['values']['destination_targetname'];
    $category = dpidestination_api_get_category ( $destination );
    dpicache_api_manage_regeneration_all_cache_entry($destination ['tid'], $category);
  }
  else{
    drupal_set_message(t("Nothing to reorder"));
  }
  
}





