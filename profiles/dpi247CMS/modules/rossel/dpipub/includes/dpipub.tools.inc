<?php 

/**
 * Return an entry from the zz_pub_UMI
 * @param string $delta
 *   The block delta croorepsponding to the position column
 *
 * @return
 *   Return the database entry as an array or FALSE
 *
 */
function pub_load_block($delta = ''){
  if($delta == ''){ return null; }
  $query = db_select('zz_pub_UMI' , 'p');
  $val_return = $query->condition('position', $delta, '=')->fields('p', array('builder'))->execute();
  return $val_return->fetchField();
}

/**
 * Return the parents hierarchy of a taxonomic term according to the vocabulary structure.
 * Example: if given the tid of the term "Belgium", returns "News/World/Belgium".
 *
 * @param string $term
 *   A taxonomy term ID or name. If it is a name, the chosen term will
 * be the one matching the $vid, if given, or the first in the results
 * list.
 * @param string $vid
 *   The vocabulary ID.
 *
 * @return
 *   A string containing the parents hierarchy, separated by "/".
 */
function pub_taxonomy_get_path_by_tid_or_term($term, $vid = '', $add_voc = FALSE) {
  $return = '';
  if (!is_numeric($term)) {
    $terms_list = taxonomy_get_term_by_name($term);
    if ($vid != '') {
      foreach ($terms_list as $term_elem) {
        if ($term_elem->vid == $vid) {
          $tid = $term_elem->tid;
          break;
        }
      }
    } else {
       $val = array_values($terms_list);
       $object = $val[0];
       $tid = $object->tid;
    }
  } else {
    $tid = $term;
  }
  
  $tax_list = taxonomy_get_parents_all($tid);
  foreach (array_reverse($tax_list) as $tax_elem) {
    $return .= $tax_elem->name.'/';
  }
  if ($add_voc) {
    $voc = taxonomy_vocabulary_load($tax_list[0]->vid);
    $return = $voc->name.'/'.$return;
  }

  return substr($return, 0, strlen($return)-1);
}


?>