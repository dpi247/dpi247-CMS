<?php

/**
 * Generate a DAM search form
 */
function _dpidamwidgets_generate_search_form(&$form, $profile, $conf=array()) {
  if ($profile['settings']['advanced']) {
    _dpidamwidgets_generate_advanced_search_form($form, $profile);
  } else {
    _dpidamwidgets_generate_simple_search_form($form, $profile);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  // do we have to show results?
  if ($conf["display_results"]) {
    if (isset($_GET['profile']) && !empty($_GET['profile']) && $profile['pid'] == $_GET['profile']) {
      $form['#suffix'] = _dpidamwidgets_perform_result_and_display($profile);
    }
  }
}

/**
 * Generate an advanced DAM search form
 */
function _dpidamwidgets_generate_advanced_search_form(&$form, $profile) {
  $form['words'] = array(
    '#type' => 'textfield',
    '#title' => t('Searched words'),
    '#description' => t('Use commas to perform the search on several words'),
    '#default_value' => isset($_GET['words']) ? filter_xss($_GET['words']) : '',
  );

  $form['excludes'] = array(
    '#type' => 'textfield',
    '#title' => t('Excluded words'),
    '#description' => t('Use commas to exclude several words'),
    '#default_value' => isset($_GET['excludes']) ? filter_xss($_GET['excludes']) : '',
  );
}

/**
 * Generate a simple DAM search form
 */
function _dpidamwidgets_generate_simple_search_form(&$form, $profile) {
  $sort_options = array();

  $form['word'] = array(
    '#type' => 'textfield',
    '#title' => '',
    '#default_value' => isset($_GET['word']) ? filter_xss($_GET['word']) : '',
  );

  if(isset($profile["settings"]["sort"]["weight"])) {
    $sort_options["weight"]=t('Weight');
  }

  if(isset($profile["settings"]["sort"]["date"])) {
    $sort_options["date asc"]=t('Date Asceding');
    $sort_options["date desc"]=t('Date Descending');
  }

  if (count($sort_options)) {
    $form['sort'] = array(
      '#type' => 'select',
      '#title' => t('Sort Options'),
      '#options' => $sort_options,
      '#default_value' => isset($_GET['sort']) ? filter_xss($_GET['sort']) : '',
    );
  }

  $date_option = $profile["settings"]["datefilter"];
  foreach ($date_option as $k=>$v) {
    $date_option[$k] = t($v);
  }

  if(isset($profile["settings"]["datefilter"])) {
    if (!isset($profile["settings"]["datefilter"]["none"])) {
      $form['datefilter'] = array(
        '#type' => 'select',
        '#title' => t('Date Filter'),
        '#options' => $date_option,
        '#default_value' => isset($_GET['datefilter']) ? filter_xss($_GET['datefilter']) : '',
      );
    }
  }
}

/**
 * Perform a search and display results
 */
function _dpidamwidgets_perform_result_and_display($profile) {
  global $dam_results;

  if (!isset($dam_results[$profile["pid"]])) {
    if ($profile['settings']['advanced']) {
      $dam_results[$profile["pid"]] = _dpidamwidgets_perform_advanced_search($profile);
    } else {
      $dam_results[$profile["pid"]] = _dpidamwidgets_perform_simple_search($profile);
    }
  }

  if (!empty($dam_results[$profile["pid"]]['results'])) {
    $return = _dpidamwidgets_render_search_results($dam_results[$profile["pid"]], $profile);
  } else {
    $messages = array();
    foreach ($dam_results[$profile["pid"]]['errors'] as $error) {
      $message = _dpidamwidgets_damsearch_interpret_message_type($error);
      if (!empty($message)) {
        $messages[] = $message;
      }
    }
    if (empty($messages)) {
      $messages[] = t('Your research yields to no results.');
    }
    $return = _dpidamwidgets_render_empty_results($profile, $messages);
  }

  return $return;
}

/**
 * Get pagination for a search results and display the result
 */
function _dpidamwidgets_perform_pagination_and_display($profile) {
  global $dam_results;

  if (!isset($dam_results[$profile["pid"]])) {
    if ($profile['settings']['advanced']) {
      $dam_results[$profile["pid"]] = _dpidamwidgets_perform_advanced_search($profile);
    } else {
      $dam_results[$profile["pid"]] = _dpidamwidgets_perform_simple_search($profile);
    }
  }

  if (!empty($dam_results[$profile["pid"]]['results'])) {
    $return = _dpidamwidgets_render_pagination_results($dam_results[$profile["pid"]]["pagination"], $profile);
  } else {
    $return = "<!-- no results -->";
  }

  return $return;
}

/**
 * Get the facets for a search results and display the result
 */
function _dpidamwidgets_perform_facets_and_display($profile) {
  global $dam_results;

  if (!isset($dam_results[$profile["pid"]])) {
    if ($profile['settings']['advanced']) {
      $dam_results[$profile["pid"]] = _dpidamwidgets_perform_advanced_search($profile);
    } else {
      $dam_results[$profile["pid"]]= _dpidamwidgets_perform_simple_search($profile);
    }
  }

  if (!empty($dam_results[$profile["pid"]]['results'])) {
    $return = _dpidamwidgets_render_facets_results($dam_results[$profile["pid"]]["facets"], $profile);
  } else {
    $return = "<!-- no results -->";
  }

  return $return;
}

/**
 * Perform an advanced search
 */
function _dpidamwidgets_perform_advanced_search($profile) {
  $results = array();
  $words = explode(',', $_GET['words']);
  $excludes = explode(',', $_GET['excludes']);
  $facets = _dpidamwidgets_extract_facets(check_plain($_GET['facets']));
  if (isset($_GET['sort'])) {
    $sort = array($_GET['sort']);
  }
  if (isset($_GET['datefilter'])) {
    $datefilter = array($_GET['datefilter']);
  }

  $tmp_results = _dpidamwidgets_get_search_results($words, $excludes, $facets, $profile, $sort, $datefilter);
  if (is_array($tmp_results)) {
    $results = $tmp_results;
  }

  return $results;
}

/**
 * Perform a simple search
 */
function _dpidamwidgets_perform_simple_search($profile) {
  $results = array();

  if (isset($_GET['word'])) {
    $words = array($_GET['word']);
    $facets = _dpidamwidgets_extract_facets(check_plain($_GET['facets']));
    if (isset($_GET['sort'])) {
      $sort = array(filter_xss(urldecode($_GET['sort'])));
    }
    if (isset($_GET['datefilter'])) {
      $datefilter = array(filter_xss(urldecode($_GET['datefilter'])));
    }

    $tmp_results = _dpidamwidgets_get_search_results($words, array(), $facets, $profile, $sort, $datefilter);
    if (is_array($tmp_results)) {
      $results = $tmp_results;
    }
  }

  return $results;
}

/**
 * Extract facets from a search result
 */
function _dpidamwidgets_extract_facets($facets) {
  $facets_array = array();
  $facets = explode("::", $facets);
  foreach ($facets as $facet) {
    $facet_array = explode(":", $facet);
    $facet_array[1] = explode(",", $facet_array[1]);
    $facets_array[$facet_array[0]] = $facet_array[1];
  }
  return $facets_array;
}

/**
 * Recursively filter XSS code from an array.
 * Any string value is xss tested.
 */
function _filter_xss_array($array){
  foreach ($array as $k=>$value) {
    if (is_string($value)) {
      $array[$k] = filter_xss($value);
    } else {
      if (is_array($value)) {
        $array[$k] = _filter_xss_array($value);
      }
    }
  }
  return $array;
}

/**
 * Build the pagination URL
 */
function _dpidamwidgets_compute_pagination_url(&$pagination){
  $p = array();
  $last = array();
  $first= array();
  $prev = array();
  $next= array();

  $argtemporigin = _filter_xss_array($_GET);

  foreach($pagination[0]->pages as $kpage=>$page){
    $p[$kpage]["number"]=$page;
    $p[$kpage]["offset"]=($page-1)*$pagination[0]->rowsPerPage;
    $argtemp = $argtemporigin;
    unset($argtemp["q"]);
    $argtemp["start"]= $p[$kpage]["offset"];
    $p[$kpage]["url"] = "?". urldecode(http_build_query($argtemp,"","&"));
  }
  $pagination[0]->pages = $p;

  $last["offset"]=($pagination[0]->last-1)*$pagination[0]->rowsPerPage;
  $last["number"]=($pagination[0]->last);
  $argtemp = $argtemporigin;
  unset($argtemp["q"]);
  $argtemp["start"]= $last["offset"];
  $last["url"] = "?". urldecode(http_build_query($argtemp,"","&"));
  $pagination[0]->last = $last;

  $first["offset"]=($pagination[0]->first-1)*$pagination[0]->rowsPerPage;
  $first["number"]=($pagination[0]->first);
  $argtemp = $argtemporigin;
  unset($argtemp["q"]);
  $argtemp["start"]= $first["offset"];
  $first["url"] = "?". urldecode(http_build_query($argtemp,"","&"));
  $pagination[0]->first = $first;

  if($pagination[0]->current<$last["number"]) {
    $next["offset"]=($pagination[0]->current)*$pagination[0]->rowsPerPage;
    $next["number"]=($pagination[0]->current+1);
    $argtemp = $argtemporigin;
    unset($argtemp["q"]);
    $argtemp["start"]= $next["offset"];
    $next["url"] = "?". urldecode(http_build_query($argtemp,"","&"));
    $pagination[0]->next = $next;
  }	else {
    unset($pagination[0]->last);
  }

  if($pagination[0]->current>$first["number"]) {
    $prev["offset"]=($pagination[0]->current-2)*$pagination[0]->rowsPerPage;
    $prev["number"]=($pagination[0]->current-1);
    $argtemp = $argtemporigin;
    unset($argtemp["q"]);
    $argtemp["start"]= $prev["offset"];
    $prev["url"] = "?". urldecode(http_build_query($argtemp,"","&"));
    $pagination[0]->prev = $prev;
  }	else {
    unset($pagination[0]->first);
  }
}

/**
 * Build the facets URLs
 */
function _dpidamwidgets_compute_facet_url(&$facets) {
  $argtemporigin = _filter_xss_array($_GET);

  foreach ($facets as $fkey=>&$facet) {

    /* Urls of Terms - ADD or REMOVE */
    foreach($facet->terms as $kterm=>&$term) {
      $urifacets = _dpidamwidgets_extract_facets(check_plain($_GET['facets']));
       
      if (!in_array($term->id,$urifacets[$facet->thCode])) {
        // Do not exist, we add the term.
        $urifacets[$facet->thCode][]=$term->id;
      } else {
        // Exist, we delete the term.
        unset( $urifacets[$facet->thCode][array_search($term->id,$urifacets[$facet->thCode])] );
      }
       
      // Remove empty facets (no terms)
      if (sizeof($urifacets[$facet->thCode])==0) {
        unset($urifacets[$facet->thCode]);
      }
       
      $urlarray = array();
      foreach($urifacets as $kurifacet=>$urifacet) {
        $urlarray[] =$kurifacet.":".implode(",",$urifacet);
      }
      $term->url["arg"] = trim(implode("::",$urlarray),"::");
      $argtemp = $argtemporigin;

      unset($argtemp["q"]);
      unset($argtemp["start"]);
      $argtemp["facets" ]= $term->url["arg"];
      $term->url["url"] = check_url("?". urldecode(http_build_query($argtemp,"","&")));
    }

    /* Urls of Facets - ALL or NONE */
    $urifacets = _dpidamwidgets_extract_facets(check_plain($_GET['facets']));
    unset($urifacets[$facet->thCode]);

    $urlarray = array();
    foreach($urifacets as $kurifacet=>$urifacet) {
      $urlarray[] =$kurifacet.":".implode(",",$urifacet);
    }

    $facet->url["arg"] = trim(implode("::",$urlarray),"::");
    $argtemp = $argtemporigin;
    unset($argtemp["q"]);
    unset($argtemp["start"]);
     
    $argtemp["facets"]= $facet->url["arg"];
    $facet->url["url"] = "?". urldecode(http_build_query($argtemp,"","&"));
  }
}

/**
 * Compute DAM search results
 *
 * @param $results array
 * 	returned array of data
 * @param $rawResults object
 * 	results returned by the dam
 *
 * @return array
 * 	Search results
 */
function _dpidamwidgets_compute_search_results(&$results, $rawResults) {
  $results['results'] = $rawResults->results;
  $results['facets'] = $rawResults->facets;

  $rawResults->pagination->numFound=$rawResults->numFound;
  $results['pagination'][] = $rawResults->pagination;

  $query = $rawResults->query;
  $querymap = $query->map;

  foreach($results['facets'] as &$facet) {

    $thCode = $facet->thCode;
    if (isset($querymap->$thCode)) {

      # La facet est dans le Query.
      foreach($querymap->$thCode as $term) {
        foreach($facet->terms as &$facet_term) {
          # On recherche les terms du Query
          if ($facet_term->id == $term->code) {
            $facet_term->active = TRUE;
          }
        }
      }
      $facet->active = TRUE;

    }
  }
  // Add all "facets" usefull URL to the results.
  _dpidamwidgets_compute_facet_url($results['facets']);
  // Add all "paginations" usefull URL to the results.
  _dpidamwidgets_compute_pagination_url($results['pagination']);
}

/**
 * Perform a DAM search and get results
 *
 * @param array $keys
 *   Searched words
 * @param array $conf
 *   Settings of the content type CTools
 *
 * @return array
 *   Search results
 */
function _dpidamwidgets_get_search_results($words, $excludes, $facets, $profile, $sort = array(), $datefilter = array()) {
  $publishedFrom_values = array();
  $publishedFrom_values["today"] = date("d/m/y",  strtotime("now"));
  $publishedFrom_values["yesterday"] = date("d/m/y", strtotime("-1 day"));
  $publishedFrom_values["lastweek"] = date("d/m/y", strtotime("-1 week"));
  $publishedFrom_values["lastmonth"] = date("d/m/y", strtotime("-1 month"));
  $publishedFrom_values["last6month"] = date("d/m/y", strtotime("-6 month"));
  $publishedFrom_values["lastyear"] = date("d/m/y", strtotime("-1 year"));
  $publishedFrom_values["last5year"] = date("d/m/y", strtotime("-5 year"));
  $publishedFrom_values["last10year"] = date("d/m/y", strtotime("-10 year"));

  module_load_include('inc', 'dpidam', 'includes/DAMSearch');
  $search = new DAMSearch($profile['pid']);
  $search_query = $search->getSearchQuery();

  foreach ($words as $word) {
    $word = check_plain(trim($word));
    $search_query->addReqWord($word);
  }
  foreach ($excludes as $exclude) {
    $exclude = check_plain(trim($exclude));
    $search_query->excludeReqWord($word);
  }

  if (isset($publishedFrom_values[$datefilter[0]])) {
    $search_query->setQueryParameter("publishedFrom", $publishedFrom_values[$datefilter[0]]);
    $search_query->setQueryParameter("publishedTo", date("d/m/y",  strtotime("now")));
  }

  foreach ($facets as $facetID=>$facetterms) {
    $search_query->setQueryParameter($facetID, $facetterms);
  }
  if (isset($_GET['start'])) {
    $search_query->setOption("start", check_plain($_GET['start']));
  }

  $search_query->setOption("pagination", TRUE);
  $search_query->setOption("highlight", TRUE);

  if (count($sort)) {
    if ($sort[0] != "weight") {
      $search_query->setOption("sort", check_plain($sort[0]));
    }
  }

  $search->executeQuery();

  $errorLogs = $search->getErrorLogs();
  $results = array(
    'results' => array(),
    'errors' => array(),
  );

  if (!$errorLogs) {
    $rawResults = $search->getRawResults();
    _dpidamwidgets_compute_search_results($results, $rawResults);
  } else {
    foreach ($errorLogs as $errorLog) {
      $results['errors'][] = $errorLog['location'];
    }
  }

  return $results;
}

/**
 * Render search results
 */
function _dpidamwidgets_render_search_results($response, $profile) {
  $output = "";
  $output .= theme('damsearch_render_results', $response, $profile);

  return $output;
}

/**
 * Render facets
 */
function _dpidamwidgets_render_facets_results($facets, $profile) {
  $output = theme('damsearch_render_facets', $facets, $profile);
  return $output;
}

/**
 * Render the pagination
 */
function _dpidamwidgets_render_pagination_results($pagination, $profile) {
  $output = theme('damsearch_render_pagination', $pagination, $profile);
  return $output;
}

/**
 * Render empty results
 */
function _dpidamwidgets_render_empty_results($profile, $messages) {
  $output = theme('damsearch_empty_results', $profile, $messages);
  return $output;
}

/**
 * Interpret an error message
 */
function _dpidamwidgets_damsearch_interpret_message_type($error) {
  $message = '';

  switch ($error) {
    case 'validating query':
    case 'set advanced':
    case 'exclude request word':
    case 'building':
    case 'add request word':
    case 'init profile':
    case 'execute':
    case 'query':
      $message = t('A technical issue occurred, please try again later.');
      break;

    case 'results':
      $message = t('Your research yields to no results.');
      break;

    default:
      break;
  }

  return $message;
}
